#ifndef DEBUG_H
#define DEBUG_H


/**
 * Quick Boot:
 * Define this variable to a game state to boot into that
 * state. Two presets (map and main menu) are provided.
 * For the full list of game states, see sf64thread.h.
 */
// #define DEBUG_BOOT_STATE 3  // main menu
// #define DEBUG_BOOT_STATE 4 // map

/**
 * Level Select:
 * Use the D-Pad to select a level. Press L to start in
 * an advanced level phase (warp zone or Andross fight).
 * Useful for debugging and speedrunning training.
 */
#define DEBUG_LEVEL_SELECT 1

/**
 * Sound Effects Jukebox:
 * Ability to play sound effects inside the expert sound menu
 * Use L to switch between jukebox and soundtrack
 * Use D-PAD to move the cursor
 * Use C buttons to edit values
 */
#define DEBUG_SFX_JUKEBOX 0

/**
 * Spawner:
 * Spawn Scenery, Actors, Bosses, Sprites, Items, Effects and even Event Actors.
 * 
 * Controls:
 * D-Pad left and right to set the object Id.
 * C-Right to change between spawn modes.
 * Analog stick sets the spawn position.
 * L-Trigger to spawn the object.
 * D-Pad UP to kill all objects.
 * D-Pad DOWN to freeze/unfreeze the ship speed.
 * WARNING: Spawning an object that's not loaded in memory will likely result in a crash.
 */
#define DEBUG_SPAWNER 0

/**
 * Cheats:
 * Max Bombs and Max Laser upgrade
 */
#define DEBUG_CHEATS 0

#define DEBUG_SKIP_BRIEFING 0
#define DEBUG_L_TO_ALL_RANGE 1
#define DEBUG_L_TO_LEVEL_COMPLETE 0
#define DEBUG_Z_R_START_TO_RESET 0
#define DEBUG_Z_R_START_TO_MAP 1
#define DEBUG_L_TO_WARP_ZONE 0
#define DEBUG_SPEED_CONTROL 0
#define DEBUG_FULL_SAVE 1
#define DEBUG_NO_COLLISION 0
#define DEBUG_BOSS_KILLER 0
#define DEBUG_ENDING 0
#define DEBUG_VERSUS 0

// AUDIO
#define DEBUG_AUDIO_LOCALIZATION 0
#define DEBUG_US_AUDIO 0
#define DEBUG_JP_AUDIO 0
#define DEBUG_EU_AUDIO 0

// Settings
#define ENDING_SKIP_INTERPOLATION 1
#define ENDING_BORDERS 1
#endif
