#ifndef MPN_8PLAYER_CPP
#define MPN_8PLAYER_CPP

#include "8Player.h"

mpn_team_t* Teams = NULL;

bool mpn_8p_active()
{
  return Teams == NULL;
}

GCPadStatus mpn_8p_combined_input(uint8_t Port)
{
  GCPadStatus CombinedPad;

  Port &= 0x03;
  /* Left half */
  CombinedPad.stickX = Teams[Port].LeftPad.stickX;
  CombinedPad.stickY = Teams[Port].LeftPad.stickY;
  CombinedPad.triggerLeft = Teams[Port].LeftPad.triggerLeft;
  /* Right half */
  CombinedPad.substickX = Teams[Port].RightPad.substickX;
  CombinedPad.substickY = Teams[Port].RightPad.substickY;
  CombinedPad.triggerRight = Teams[Port].LeftPad.triggerRight;
  /* Etc */
  CombinedPad.button = Teams[Port].LeftPad.button | Teams[Port].RightPad.button;
  CombinedPad.analogA = 0;
  CombinedPad.analogB = 0;
  CombinedPad.isConnected = true;

  return CombinedPad;
}

void mpn_8p_free()
{
  if (mpn_8p_active())
  {
    free(Teams);
    Teams = NULL;
  }
}

void mpn_8p_init()
{
  Teams = (mpn_team_t*)calloc(4, sizeof(mpn_team_t));
}

bool mpn_8p_port_ready(uint8_t Port)
{
  Port &= 0x03;

  if (!mpn_8p_active() || !(Teams[Port].Flags & MPN_TEAM_ACTIVE))
    return true;
  else if (Teams[Port].Flags & MPN_TEAM_L_READY & MPN_TEAM_R_READY)
  {
    Teams[Port].Flags = MPN_TEAM_ACTIVE;
    return true;
  }
  else
    return false;
}

void mpn_8p_push_back_input(GCPadStatus* Pad, uint8_t Port)
{
  bool Left = Port > 3;

  Port &= 0x03;
  if (!Teams[Port].Flags & MPN_TEAM_ACTIVE)
    return;
  if (Left)
  {
    Teams[Port].LeftPad = *Pad;
    Teams[Port].Flags |= MPN_TEAM_L_READY;
  }
  else
  {
    Teams[Port].RightPad = *Pad;
    Teams[Port].Flags |= MPN_TEAM_R_READY;
  }
}

void mpn_8p_set_port_active(uint8_t Port, bool Active)
{
  if (mpn_8p_active())
    Teams[Port & 3].Flags = Active ? MPN_TEAM_ACTIVE : 0;
}

#endif
