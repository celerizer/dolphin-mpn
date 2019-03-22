#ifndef MPN_DISCORD_CPP
#define MPN_DISCORD_CPP

#include "Discord.h"

bool mpn_update_discord()
{
  if (!Memory::IsInitialized())
    return false;

  DiscordRichPresence RichPresence = {};

  RichPresence.largeImageKey = "box-mp4";
  RichPresence.largeImageText = "Mario Party 4";
  
  if (CurrentState != NULL)
    RichPresence.state = CurrentState->Name.c_str();

  if (GameMetadata != NULL)
  {
    char Details[128] = "dummy";

    snprintf(Details, sizeof(Details), "Players: %d/4 Turn: %d/%d",
      1,
      mpn_read_value(GameMetadata->CurrentTurn, 1),
      mpn_read_value(GameMetadata->TotalTurns, 1));
    RichPresence.details = Details;

    switch (mpn_read_value(GameMetadata->Board, 1))
    {
    case 0:
      RichPresence.smallImageKey = "mp4-toad";
      RichPresence.smallImageText = "Toad's Midway Madness";
      break;
    case 1:
      RichPresence.smallImageKey = "mp4-goomba";
      RichPresence.smallImageText = "Goomba's Greedy Gala";
      break;
    case 2:
      RichPresence.smallImageKey = "mp4-shyguy";
      RichPresence.smallImageText = "Shy Guy's Jungle Jam";
      break;
    case 3:
      RichPresence.smallImageKey = "mp4-boo";
      RichPresence.smallImageText = "Boo's Haunted Bash";
      break;
    case 4:
      RichPresence.smallImageKey = "mp4-koopa";
      RichPresence.smallImageText = "Koopa's Seaside Soiree";
      break;
    case 5:
      RichPresence.smallImageKey = "mp4-bowser";
      RichPresence.smallImageText = "Bowser's Gnarly Party";
      break;
    default:
      RichPresence.smallImageKey = "mp4-bowser";
      RichPresence.smallImageText = "Bowser's Gnarly Party";
      break;
    }
  }
  Discord_UpdatePresence(&RichPresence);

  return true;
}

#endif
