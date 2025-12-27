/**
 * Used to reproduce recordings made from real N64 hardware
 * to accurately play cutscenes at the correct speed.
 * These recordings adjust gVisPerFrame during runtime to produce
 * the same behaviour as the original game.
 */

#include "patches.h"
#include "functions.h"
#include "i1.h"
#include "i2.h"
#include "i3.h"
#include "i4.h"
#include "i5.h"
#include "i6.h"

void Cutscene_WarpZoneComplete(Player* player);
void Cutscene_FortunaComplete(Player* player);
void Cutscene_CoComplete2(Player* player);

typedef struct Record {
    u8 vis;
    u16 frame;
} Record;

// clang-format off

// Carrier destroy cutscene timings recorded from a real N64
Record gCarrierCutsceneRecord[] = {
    { 2, 0 },
    { 3, 2 },
    { 4, 16 },
    { 3, 50 },
    { 4, 58 },
    { 5, 67 },
    { 4, 68 },
    { 5, 71 },
    { 4, 73 },
    { 5, 74 },
    { 4, 106 },
    { 3, 146 },
    { 2, 194 },
};

// Granga destroy cutscene timings recorded from a real N64
Record gGrangaCutsceneRecord[] = {
    { 2, 0 },
    { 3, 1 },
    { 2, 5 },
    { 3, 7 },
    { 2, 52 },
    { 3, 78 },
    { 4, 103 },
    { 3, 125 },
    { 2, 153 },
    { 3, 155 },
    { 2, 157 },
    { 3, 158 },
    { 2, 160 },
};

// MeCrusher destroy cutscene timings recorded from a real N64
Record gMeCrusherCutsceneRecord[] = {
    { 2, 0 },
    { 3, 200 },
    { 2, 224 },
};

// Train cutscene timings recorded from a real N64
Record gMacbethCutsceneRecord[] = {
    // Train breaking barriers
    { 2, 0 },
    { 3, 2 },
    { 2, 31 },
    { 3, 400 },
    { 2, 418 },
    { 3, 433 },
    { 4, 435 },
    { 3, 444 },
    { 2, 509 },

    // Not worth it and very noticeable
    // { 3, 559 },
    // { 2, 581 },
    // { 3, 587 },

    // Explosions
    { 2, 589 },
    { 3, 714 },
    { 4, 821 },
    { 5, 849 },
    { 2, 942 },
};

// SyRobot destroy cutscene timings recorded from a real N64
Record gSyRobotCutsceneRecord[] = {
    { 2, 0 },
    { 3, 129 },
    { 2, 230 },
};

// After defeating Andross
/*
03 00 00 00 02 00 00 08 03 00 00 13 02 00 00 23 03 00 00 9C 02 00 00 A6 03 00 00 C1 04 00 00 C6 03 00 00 C7 04 00 00 C9
03 00 00 CD 04 00 00 CF 03 00 00 D7 04 00 00 EA 03 00 00 EC 04 00 00 ED 03 00 00 EF 04 00 00 F4 03 00 00 F5 04 00 00 F6
03 00 01 10 04 00 01 12 03 00 01 22 04 00 01 24 03 00 01 27 04 00 01 2D 03 00 01 2E 04 00 01 30 03 00 01 31 04 00 01 33
03 00 01 4C 04 00 01 4E 03 00 01 4F
*/
Record gAndrossRobotKillCutscene1[] = {
    { 3, 0 },
    { 2, 8 },
    { 3, 19 },
    { 2, 35 },
    { 3, 156 },
    { 2, 166 },
    { 3, 193 },
    { 4, 198 },
    { 3, 199 },
    { 4, 201 },
    { 3, 205 },
    { 4, 207 },
    { 3, 215 },
    { 4, 234 },
    // { 3, 236 },
    { 4, 237 },
    { 3, 239 },
    { 4, 244 },
    // { 3, 245 },
    { 4, 246 },
    // { 3, 272 },
    { 4, 274 },
    // { 3, 290 },
    { 4, 292 },
    // { 3, 295 }, // too much?
    { 4, 301 },
    // { 3, 302 },
    { 4, 304 },
    // { 3, 305 },
    { 4, 307 },
    // { 3, 332 },
    { 4, 334 },
    // { 3, 335 },
    { 2, 339 },
};

/*
04 00 00 00 02 00 00 01 05 00 00 02 02 00 00 03 03 00 00 2A 02 00 00 2B 03 00 00 39 02 00 00 3A 03 00 00 3F 02 00 00 45
03 00 00 9E 04 00 00 A1 05 00 00 A3 04 00 00 B6 03 00 00 B9 04 00 00 BD 05 00 00 C1 04 00 00 CD 03 00 00 CF 02 00 00 D3
*/
Record gAndrossRobotKillCutscene2[] = {
    { 4, 0 },
    { 2, 1 },
    { 5, 2 },
    { 2, 3 },
    { 3, 42 },
    { 2, 43 },
    { 3, 57 },
    { 2, 58 },
    { 3, 63 },
    { 2, 69 },
    { 3, 158 },
    { 4, 161 },
    { 5, 163 },
    { 4, 182 },
    { 3, 185 },
    { 4, 189 },
    { 5, 193 },
    { 4, 205 },
    { 3, 207 },
    { 2, 211 },
};
// clang-format on

void UpdateVisPerFrameFromRecording(Record* record, s32 maxFrames) {
    int i;

    if (gCsFrameCount > record[maxFrames - 1].frame) {
        return;
    }

    for (i = 0; i < maxFrames; i++) {
        if (gCsFrameCount == record[i].frame) {
            gVIsPerFrame = record[i].vis;
        }
    }
}

RECOMP_PATCH void Cutscene_LevelComplete(Player* player) {
    s32 sp24;
    s32 sp20;
    s32 btn;

    gCsFrameCount++;

    switch (player->form) {
        case FORM_ARWING:
            if ((gCurrentLevel == LEVEL_VENOM_ANDROSS) || ((gCurrentLevel == LEVEL_VENOM_2) && (gLevelPhase == 1))) {

                // @recomp: Update VisPerFrame with N64 Recording
                if (player->csState < 3) {
                    UpdateVisPerFrameFromRecording(gAndrossRobotKillCutscene1, ARRAY_COUNT(gAndrossRobotKillCutscene1));
                } else if ((player->csState > 2) && player->csState < 6) {
                    UpdateVisPerFrameFromRecording(gAndrossRobotKillCutscene2, ARRAY_COUNT(gAndrossRobotKillCutscene2));
                }

                Andross_LevelComplete(player);
            } else if (gCurrentLevel == LEVEL_SECTOR_X) {
                if (gLevelPhase == 0) {
                    SectorX_LevelComplete(player);
                } else {
                    Cutscene_WarpZoneComplete(player);
                }
            } else if (gCurrentLevel == LEVEL_AREA_6) {
                Area6_LevelComplete(player);
            } else if (gCurrentLevel == LEVEL_FORTUNA) {
                Cutscene_FortunaComplete(player);
                Player_FloorCheck(player);
            } else if (gCurrentLevel == LEVEL_BOLSE) {
                Bolse_LevelComplete(player);
                Player_FloorCheck(player);
            } else if (gCurrentLevel == LEVEL_SECTOR_Z) {
                SectorZ_LevelComplete(player);
            } else if (gCurrentLevel == LEVEL_KATINA) {
                Katina_LevelComplete(player);
                Player_FloorCheck(player);
            } else if (gCurrentLevel == LEVEL_SECTOR_Y) {
                // @recomp: Update VisPerFrame with N64 Recording
                UpdateVisPerFrameFromRecording(gSyRobotCutsceneRecord, ARRAY_COUNT(gSyRobotCutsceneRecord));
                SectorY_LevelComplete(player);
                Player_FloorCheck(player);
            } else if (gCurrentLevel == LEVEL_SOLAR) {
                Solar_LevelComplete(player);
                Player_FloorCheck(player);
            } else if (gCurrentLevel == LEVEL_ZONESS) {
                Zoness_LevelComplete(player);
                Player_FloorCheck(player);
            } else if (gCurrentLevel == LEVEL_VENOM_2) {
                Venom2_LevelComplete(player);
                Player_FloorCheck(player);
            } else if (gCurrentLevel == LEVEL_METEO) {
                if (gLevelPhase == 0) {
                    // @recomp: Update VisPerFrame with N64 Recording
                    UpdateVisPerFrameFromRecording(gMeCrusherCutsceneRecord, ARRAY_COUNT(gMeCrusherCutsceneRecord));
                    Meteo_LevelComplete(player);
                } else {
                    Cutscene_WarpZoneComplete(player);
                }
            } else if ((gCurrentLevel == LEVEL_CORNERIA) && (gLevelMode == LEVELMODE_ALL_RANGE)) {
                // @recomp: Update VisPerFrame with N64 Recording
                UpdateVisPerFrameFromRecording(gGrangaCutsceneRecord, ARRAY_COUNT(gGrangaCutsceneRecord));
                Corneria_LevelComplete1(player);
                Player_FloorCheck(player);
            } else {
                if (gCsFrameCount == 170) {
                    AUDIO_PLAY_BGM(NA_BGM_COURSE_CLEAR);
                }
                // @recomp: Update VisPerFrame with N64 Recording
                UpdateVisPerFrameFromRecording(gCarrierCutsceneRecord, ARRAY_COUNT(gCarrierCutsceneRecord));
                Cutscene_CoComplete2(player);
                Player_FloorCheck(player);
            }
            Player_UpdateArwingRoll(player);
            break;

        case FORM_LANDMASTER:
            sp20 = gInputPress->button;
            sp24 = gInputHold->button;
            gInputPress->button = 0;
            btn = gInputPress->button;
            gInputHold->button = gInputPress->button;
            gInputPress->stick_y = btn;
            gInputPress->stick_x = btn;

            if (gCurrentLevel == LEVEL_TITANIA) {
                Titania_LevelComplete(player);
            } else if (gMissionStatus != MISSION_COMPLETE) {
                // @recomp: Update VisPerFrame with N64 Recording
                UpdateVisPerFrameFromRecording(gMacbethCutsceneRecord, ARRAY_COUNT(gMacbethCutsceneRecord));
                Macbeth_LevelComplete2(player);
            } else {
                Macbeth_LevelComplete1(player);
            }

            func_tank_80046358(player);
            gInputPress->button = sp20;
            gInputHold->button = sp24;
            break;

        case FORM_BLUE_MARINE:
            if (gCurrentLevel == LEVEL_AQUAS) {
                Aquas_CsLevelComplete(player);
            }
            break;
    }
}
