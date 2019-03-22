#ifndef MPN_GAMESTATE_H
#define MPN_GAMESTATE_H

#include <stdint.h>
#include <string>
#include "Core/Config/GraphicsSettings.h"
#include "Core/Core.h"
#include "Core/HW/CPU.h"
#include "Core/HW/Memmap.h"
#include "Core/HW/Wiimote.h"
#include "Core/HW/WiimoteEmu/WiimoteEmu.h"
#include "InputCommon/ControllerEmu/ControlGroup/ControlGroup.h"
#include "InputCommon/ControllerEmu/Setting/BooleanSetting.h"
#include "InputCommon/InputConfig.h"
#include "VideoCommon/VideoConfig.h"

#define MPN_GAMEID_MP4 0x474D5045
#define MPN_GAMEID_MP5 0x47503545
#define MPN_GAMEID_MP6 0x47503645
#define MPN_GAMEID_MP7 0x47503745
#define MPN_GAMEID_MP8 0x524D3845
#define MPN_GAMEID_MP9 0x53535145

#define MPN_NEEDS_SAFE_TEX_CACHE   (1 << 0)
#define MPN_NEEDS_NATIVE_RES       (1 << 1)
#define MPN_NEEDS_EFB_TO_TEXTURE   (1 << 2)
#define MPN_NEEDS_SIDEWAYS_WIIMOTE (1 << 3)
#define MPN_NEEDS_NOTHING          0xFF
#define NAPP                       -1

#undef MPN_USE_LEADERBOARDS
#define MPN_USE_OSD

#include "Discord.h"

#ifdef MPN_USE_LEADERBOARDS
#include "Core/MarioPartyNetplay/Leaderboards.h"
#endif

#ifdef MPN_USE_OSD
#include "VideoCommon/OnScreenDisplay.h"
#define MPN_OSD_COLOR 0xFF4A4A94
#endif

typedef struct mpn_gamestate_t
{
  int16_t MiniGameId;
  int16_t SceneId;
  std::string Name;
  uint8_t Needs;
} mpn_gamestate_t;

typedef struct mpn_game_metadata_t
{
  /*
  uint32_t BlueSpaces;
  uint32_t RedSpaces;
  uint32_t HappeningSpaces;
  uint32_t ChanceSpaces;
  uint32_t BowserSpaces;
  uint32_t BattleSpaces;
  uint32_t ItemSpaces;
  uint32_t SpringSpaces;
  */
  uint32_t Board;
  uint32_t CurrentTurn;
  uint32_t TotalTurns;
  uint32_t MinigameIdAddress;
  uint32_t SceneIdAddress;
} mpn_game_metadata_t;

typedef struct mpn_player_t
{
  uint8_t BlueSpaces;
  uint16_t Coins;
  uint16_t CoinStar;
  uint16_t GameStar;
  uint8_t Stars;
} mpn_player_t;

extern const mpn_gamestate_t* CurrentState;
extern uint16_t CurrentSceneId;
extern uint16_t PreviousSceneId;
extern const mpn_game_metadata_t* GameMetadata;
extern const mpn_gamestate_t* CurrentStateArray;

uint8_t  mpn_get_needs(uint16_t StateId, bool IsSceneId = false);
void     mpn_per_frame();
uint32_t mpn_read_value(uint32_t Address, uint8_t Size);
bool     mpn_update_state();

const mpn_game_metadata_t MP4_ADDRESSES = {/*0x0018FC4C, 0x0018FC4D, 0x0018FC4E,
                                           0x0018FC4F,*/
                                           0x004301C6, 0x0018FCFC, 0x0018FCFD, 0x0018FD2C,
                                           0x001D3CE2};

const mpn_gamestate_t MP4_GAMESTATES[] = {{0x00, 0x09, {"Manta Rings"}, 0},
                                          {0x01, 0x0A, {"Slime Time"}, 0},
                                          {0x02, 0x0B, {"Booksquirm"}, 0},
                                          {0x03, 0x0C, {"Trace Race"}, MPN_NEEDS_SAFE_TEX_CACHE},
                                          {0x04, 0x0D, {"Mario Medley"}, 0},
                                          {0x05, 0x0E, {"Avalanche!"}, 0},
                                          {0x06, 0x0F, {"Domination"}, 0},
                                          {0x07, 0x10, {"Paratrooper Plunge"}, 0},
                                          {0x08, 0x11, {"Toad's Quick Draw"}, 0},
                                          {0x09, 0x12, {"Three Throw"}, 0},
                                          {0x0A, 0x13, {"Photo Finish"}, 0},
                                          {0x0B, 0x14, {"Mr. Blizzard's Brigade"}, 0},
                                          {0x0C, 0x15, {"Bob-omb Breakers"}, 0},
                                          {0x0D, 0x16, {"Long Claw of the Law"}, 0},
                                          {0x0E, 0x17, {"Stamp Out!"}, 0},
                                          {0x0F, 0x18, {"Candlelight Fright"}, 0},
                                          {0x10, 0x19, {"Makin' Waves"}, 0},
                                          {0x11, 0x1A, {"Hide and Go BOOM!"}, 0},
                                          {0x12, 0x1B, {"Tree Stomp"}, 0},
                                          {0x13, 0x1C, {"Fish n' Drips"}, 0},
                                          {0x14, 0x1D, {"Hop or Pop"}, 0},
                                          {0x15, 0x1E, {"Money Belts"}, 0},
                                          {0x16, 0x1F, {"GOOOOOOOAL!!"}, 0},
                                          {0x17, 0x20, {"Blame it on the Crane"}, 0},
                                          {0x18, 0x21, {"The Great Deflate"}, 0},
                                          {0x19, 0x22, {"Revers-a-Bomb"}, 0},
                                          {0x1A, 0x23, {"Right Oar Left?"}, 0},
                                          {0x1B, 0x24, {"Cliffhangers"}, 0},
                                          {0x1C, 0x25, {"Team Treasure Trek"}, 0},
                                          {0x1D, 0x26, {"Pair-a-sailing"}, 0},
                                          {0x1E, 0x27, {"Order Up"}, 0},
                                          {0x1F, 0x28, {"Dungeon Duos"}, 0},
                                          {0x20, 0x29, {"Beach Volley Folley"}, 0},
                                          {0x21, 0x2A, {"Cheep Cheep Sweep"}, 0},
                                          {0x22, 0x2B, {"Darts of Doom"}, 0},
                                          {0x23, 0x2C, {"Fruits of Doom"}, 0},
                                          {0x24, 0x2D, {"Balloon of Doom"}, 0},
                                          {0x25, 0x2E, {"Chain Chomp Fever"}, 0},
                                          {0x26, 0x2F, {"Paths of Peril"}, 0},
                                          {0x27, 0x30, {"Bowser's Bigger Blast"}, 0},
                                          {0x28, 0x31, {"Butterfly Blitz"}, 0},
                                          {0x29, 0x32, {"Barrel Baron"}, 0},
                                          {0x2A, 0x33, {"Mario Speedwagons"}, 0}};

const mpn_gamestate_t MP5_GAMESTATES[] = {
    {0x04, 0x00, {"Pushy Penguins"}, MPN_NEEDS_EFB_TO_TEXTURE}};

const mpn_gamestate_t MP6_GAMESTATES[] = {{0x04, 0x00, {"Pushy Penguins"}, 0}};

const mpn_gamestate_t MP7_GAMESTATES[] = {{0x04, 0x00, {"Pushy Penguins"}, 0}};

const mpn_gamestate_t MP8_GAMESTATES[] = {
    {0x00, 0x17, {"Speedy Graffiti"}, 0},
    {0x01, 0x18, {"Swing Kings"}, 0},
    {0x02, 0x19, {"Water Ski Spree"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x03, 0x1A, {"Punch-a-Bunch"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x04, 0x1B, {"Crank to Rank"}, 0},
    {0x05, 0x1C, {"At the Chomp Wash"}, 0},
    {0x06, 0x1D, {"Mosh-Pit Playroom"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x07, 0x1E, {"Mario Matrix"}, 0},
    {0x08, 0x1F, {"??? - Hammer de Pokari"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x09, 0x20, {"Grabby Giridion"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x0A, 0x21, {"Lava or Leave 'Em"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x0B, 0x22, {"Kartastrophe"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x0C, 0x23, {"??? - Ribbon Game"}, 0},
    {0x0D, 0x24, {"Aim of the Game"}, 0},
    {0x0E, 0x25, {"Rudder Madness"}, 0},
    {0x0F, 0x26, {"Gun the Runner"}, MPN_NEEDS_SIDEWAYS_WIIMOTE}, //1P is sideways
    {0x10, 0x27, {"Grabbin' Gold"}, 0},
    {0x11, 0x28, {"Power Trip"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x12, 0x29, {"Bob-ombs Away"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x13, 0x2A, {"Swervin' Skies"}, 0},
    {0x14, 0x2B, {"Picture Perfect"}, 0},
    {0x15, 0x2C, {"Snow Way Out"}, MPN_NEEDS_SIDEWAYS_WIIMOTE}, //3P are sideways
    {0x16, 0x2D, {"Thrash 'n' Crash"}, 0},
    {0x17, 0x2E, {"Chump Rope"}, 0},
    {0x18, 0x2F, {"Sick and Twisted"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x19, 0x30, {"Bumper Balloons"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x1A, 0x31, {"Rowed to Victory"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x1B, 0x32, {"Winner or Dinner"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x1C, 0x33, {"Paint Misbehavin'"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x1D, 0x34, {"Sugar Rush"}, 0},
    {0x1E, 0x35, {"King of the Thrill"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x1F, 0x36, {"Shake It Up"}, 0},
    {0x20, 0x37, {"Lean, Mean Ravine"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x21, 0x38, {"Boo-ting Gallery"}, 0},
    {0x22, 0x39, {"Crops 'n' Robbers"}, 0},
    {0x23, 0x3A, {"In the Nick of Time"}, 0},
    {0x24, 0x3B, {"Cut from the Team"}, 0},
    {0x25, 0x3C, {"Snipe for the Picking"}, 0},
    {0x26, 0x3D, {"Saucer Swarm"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x27, 0x3E, {"Glacial Meltdown"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x28, 0x3F, {"Attention Grabber"}, 0},
    {0x29, 0x40, {"Blazing Lassos"}, 0},
    {0x2A, 0x41, {"Wing and a Scare"}, 0},
    {0x2B, 0x42, {"Lob to Rob"}, 0},
    {0x2C, 0x43, {"Pumper Cars"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x2D, 0x44, {"Cosmic Slalom"}, 0},
    {0x2E, 0x45, {"Lava Lobbers"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x2F, 0x46, {"Loco Motives"}, 0},
    {0x30, 0x47, {"Specter Inspector"}, 0},
    {0x31, 0x48, {"Frozen Assets"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x32, 0x49, {"Breakneck Building"}, MPN_NEEDS_NATIVE_RES},
    {0x33, 0x4A, {"Surf's Way Up"}, 0},
    {0x34, 0x4B, {"??? - Bull Riding"}, 0},
    {0x35, 0x4C, {"Balancing Act"}, 0},
    {0x36, 0x4D, {"Ion the Prize"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x37, 0x4E, {"You're the Bob-omb"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x38, 0x4F, {"Scooter Pursuit"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x39, 0x50, {"Cardiators"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x3A, 0x51, {"Rotation Station"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x3B, 0x52, {"Eyebrawl"}, 0},
    {0x3C, 0x53, {"Table Menace"}, 0},
    {0x3D, 0x54, {"Flagging Rights"}, 0},
    {0x3E, 0x55, {"Trial by Tile"}, 0},
    {0x3F, 0x56, {"Star Carnival Bowling"}, 0},
    {0x40, 0x57, {"Puzzle Pillars"}, 0},
    {0x41, 0x58, {"Canyon Cruisers"}, 0},
    {0x42, 0x59, {"??? - CRASH"}, 0},
    {0x43, 0x5A, {"Settle It in Court"}, 0},
    {0x44, 0x5B, {"Moped Mayhem"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {0x45, 0x5C, {"Flip the Chimp"}, 0},
    {0x46, 0x5D, {"Pour to Score"}, 0},
    {0x47, 0x5E, {"Fruit Picker"}, 0},
    {0x48, 0x5F, {"Stampede"}, 0},
    {0x49, 0x60, {"Superstar Showdown"}, 0},
    {0x4A, 0x61, {"Alpine Assault"}, 0},
    {0x4B, 0x62, {"Treacherous Tightrope"}, MPN_NEEDS_SIDEWAYS_WIIMOTE},
    {NAPP, 0x0A, {"Mini-Game Free Play"}, 0},
    {NAPP, 0x16, {"Mini-Game Explanation"}, 0},
    {NAPP, 0x04, {"Main Menu"}, 0}
};

#endif