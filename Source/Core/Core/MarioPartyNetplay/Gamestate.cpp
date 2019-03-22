#ifndef MPN_GAMESTATE_CPP
#define MPN_GAMESTATE_CPP

#include "Gamestate.h"

uint16_t CurrentSceneId = 0;
uint16_t PreviousSceneId = 0;

const mpn_gamestate_t* CurrentState = NULL;
const mpn_gamestate_t* CurrentStateArray = NULL;
const mpn_game_metadata_t* GameMetadata = NULL;

bool initializeCurrentStateArray()
{
  if (!Memory::IsInitialized())
    return false;

  switch (mpn_read_value(0x00000000, 4))
  {
  case MPN_GAMEID_MP4:
    CurrentStateArray = MP4_GAMESTATES;
    GameMetadata = &MP4_ADDRESSES;
    break;
  case MPN_GAMEID_MP5:
    CurrentStateArray = MP5_GAMESTATES;
    break;
  case MPN_GAMEID_MP6:
    CurrentStateArray = MP6_GAMESTATES;
    break;
  case MPN_GAMEID_MP7:
    CurrentStateArray = MP7_GAMESTATES;
    break;
  case MPN_GAMEID_MP8:
    CurrentStateArray = MP8_GAMESTATES;
    break;
  case MPN_GAMEID_MP9:
    CurrentStateArray = NULL;
    break;
  default:
    CurrentStateArray = NULL;
  }

  return CurrentStateArray == NULL;
}

uint8_t mpn_get_needs(uint16_t StateId, bool IsSceneId)
{
  uint16_t i = 0;

  if (CurrentStateArray == NULL)
    return MPN_NEEDS_NOTHING;

  if (CurrentSceneId != PreviousSceneId)
  {
    for (i = 0; i < 4; i++)
    {
      if ((IsSceneId && StateId == CurrentStateArray[i].SceneId) ||
          (StateId == CurrentStateArray[i].MiniGameId))
        return CurrentStateArray[i].Needs;
    }
  }

  return MPN_NEEDS_NOTHING;
}

void mpn_push_osd_message(const std::string& Message)
{
#ifdef MPN_USE_OSD
  OSD::AddMessage(Message, OSD::Duration::SHORT, MPN_OSD_COLOR);
#endif
}

bool mpn_update_state()
{
  if (CurrentStateArray == NULL && !initializeCurrentStateArray())
    return false;
  if (!Memory::IsInitialized())
    return false;

  PreviousSceneId = CurrentSceneId;
  CurrentSceneId = mpn_read_value(GameMetadata->SceneIdAddress, 2);

  for (uint16_t i = 0; i < 50; i++)
  {
    if (CurrentSceneId == CurrentStateArray[i].SceneId)
    {
      CurrentState = &CurrentStateArray[i];
      return true;
    }
  }
  CurrentState = NULL;

  return false;
}

void mpn_per_frame()
{
  uint8_t Needs = 0;

  if (!mpn_update_state() || PreviousSceneId == CurrentSceneId)
    return;

  mpn_update_discord();

  Needs = mpn_get_needs(mpn_read_value(GameMetadata->MinigameIdAddress, 2));

  if (Needs != MPN_NEEDS_NOTHING)
  {
    if (Needs & MPN_NEEDS_SAFE_TEX_CACHE)
    {
      mpn_push_osd_message("Adjusting GFX_SAFE_TEXTURE_CACHE_COLOR_SAMPLES");
      Config::SetCurrent(Config::GFX_SAFE_TEXTURE_CACHE_COLOR_SAMPLES, 0);
    }
    else
      Config::SetCurrent(Config::GFX_SAFE_TEXTURE_CACHE_COLOR_SAMPLES, 128);

    if (Needs & MPN_NEEDS_NATIVE_RES)
    {
      mpn_push_osd_message("Adjusting GFX_EFB_SCALE");
      Config::SetCurrent(Config::GFX_EFB_SCALE, 1);
    }
    else
      Config::SetCurrent(Config::GFX_EFB_SCALE, Config::GetBase(Config::GFX_EFB_SCALE));

    if (Needs & MPN_NEEDS_EFB_TO_TEXTURE)
    {
      mpn_push_osd_message("Adjusting GFX_HACK_SKIP_EFB_COPY_TO_RAM");
      Config::SetCurrent(Config::GFX_HACK_SKIP_EFB_COPY_TO_RAM, false);
    }
    else
      Config::SetCurrent(Config::GFX_HACK_SKIP_EFB_COPY_TO_RAM, true);

    /* TODO: Special casing for the two 1v3 minis that use split orientation */
    if (Needs & MPN_NEEDS_SIDEWAYS_WIIMOTE)
    {
      mpn_push_osd_message("Adjusting Wii Remote orientation");
      for (uint8_t i = 0; i < 4; i++)
      {
        Wiimote::GetWiimoteGroup(i, WiimoteEmu::WiimoteGroup::Options)
            ->boolean_settings[2]
            ->SetValue(true);
        Wiimote::GetWiimoteGroup(i, WiimoteEmu::WiimoteGroup::Options)
            ->boolean_settings[1]
            ->SetValue(false);
      }
    }
    else
    {
      for (uint8_t i = 0; i < 4; i++)
      {
        Wiimote::GetWiimoteGroup(i, WiimoteEmu::WiimoteGroup::Options)
            ->boolean_settings[2]
            ->SetValue(false);
        Wiimote::GetWiimoteGroup(i, WiimoteEmu::WiimoteGroup::Options)
            ->boolean_settings[1]
            ->SetValue(true);
      }
    }

    UpdateActiveConfig();
  }
}

uint32_t mpn_read_value(uint32_t Address, uint8_t Size)
{
  uint32_t Value = 0;

  for (int8_t i = 0; i < Size; i++)
    Value += Memory::m_pRAM[Address + i] * pow(256, Size - i - 1);

  return Value;
}

#endif
