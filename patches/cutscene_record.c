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
#include "sf64_record.h"

void Cutscene_WarpZoneComplete(Player* player);
void Cutscene_FortunaComplete(Player* player);
void Cutscene_CoComplete2(Player* player);
void Cutscene_PathTexScroll(Player* player);
void Ending_8018A8FC(void);
void Ending_8018B3D8(void);
void Ending_8018ABE8(void);
void Ending_8018A570(void);
bool Ending_8018BCB0(void);

long gWarpzoneCsFrameCount = 0;
long gA6GorgonCsFrameCount = 0;

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

Record gWarpzoneCsRecord[19] = {
    { 2, 1 },
    { 3, 3 },
    { 4, 4 },
    { 3, 8 },
    { 2, 9 },
    { 3, 52 },
    { 2, 54 },
    { 3, 69 },
    { 2, 70 },
    { 3, 75 },
    { 2, 76 },
    { 3, 79 },
    { 2, 117 },
    { 3, 118 },
    { 2, 120 },
    { 3, 145 },
    { 2, 215 },
    { 3, 216 },
    { 2, 230 },
};

Record gEndingCsRecord[] = {
    { 2,    0 },
    { 5,    1 },
    { 2,    2 },
    { 3,  277 },
    { 2,  278 },
    { 3,  340 },
    { 2,  397 },
    { 3,  537 },
    { 2,  538 },
    { 3,  548 },
    { 2,  549 },
    { 3,  568 },
    { 2,  569 },
    { 3,  620 },
    { 2,  623 },
    { 3,  803 },
    { 2,  806 },
    { 3,  814 },
    { 2,  823 },
    { 3, 1282 },
    { 2, 1284 },
    { 5, 3703 },
    { 2, 3704 },
    { 4, 3904 },
    { 2, 3905 },
    { 3, 4782 },
    { 2, 4783 },
    { 3, 4785 },
    { 2, 4820 },
    { 3, 4986 },
    { 2, 6779 },
    { 5, 6780 },
    { 2, 6781 },
    { 3, 6785 },
    { 4, 6786 },
    { 3, 6793 },
    { 2, 6799 },
};

Record gA6GorgonCsRecord[12] = {
    { 3, 0 },  { 4, 3 },  { 5, 6 },  { 4, 37 }, { 5, 38 }, { 4, 44 },
    { 3, 53 }, { 4, 54 }, { 3, 57 }, { 4, 58 }, { 3, 59 }, { 2, 170 },
};

Record gSolarIntroCsRecord[16] = {
    { 2, 0   },
    { 3, 94  },
    { 2, 105 },
    { 3, 362 },
    { 4, 377 },
    { 3, 379 },
    { 4, 381 },
    { 2, 382 },
    { 3, 443 },
    { 2, 444 },
    { 3, 456 },
    { 2, 492 },
    { 3, 494 },
    { 2, 499 },
    { 3, 552 },
    { 2, 629 },
};

// clang-format on

void UpdateVisPerFrameFromRecording(Record* record, s32 maxFrames, long* frameCounter) {
    int i;

    if (*frameCounter > record[maxFrames - 1].frame) {
        return;
    }

    for (i = 0; i < maxFrames; i++) {
        if (*frameCounter == record[i].frame) {
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
                    UpdateVisPerFrameFromRecording(gAndrossRobotKillCutscene1, ARRAY_COUNT(gAndrossRobotKillCutscene1),
                                                   &gCsFrameCount);
                } else if ((player->csState > 2) && player->csState < 6) {
                    UpdateVisPerFrameFromRecording(gAndrossRobotKillCutscene2, ARRAY_COUNT(gAndrossRobotKillCutscene2),
                                                   &gCsFrameCount);
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
                UpdateVisPerFrameFromRecording(gSyRobotCutsceneRecord, ARRAY_COUNT(gSyRobotCutsceneRecord),
                                               &gCsFrameCount);
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
                    UpdateVisPerFrameFromRecording(gMeCrusherCutsceneRecord, ARRAY_COUNT(gMeCrusherCutsceneRecord),
                                                   &gCsFrameCount);
                    Meteo_LevelComplete(player);
                } else {
                    Cutscene_WarpZoneComplete(player);
                }
            } else if ((gCurrentLevel == LEVEL_CORNERIA) && (gLevelMode == LEVELMODE_ALL_RANGE)) {
                // @recomp: Update VisPerFrame with N64 Recording
                UpdateVisPerFrameFromRecording(gGrangaCutsceneRecord, ARRAY_COUNT(gGrangaCutsceneRecord),
                                               &gCsFrameCount);
                Corneria_LevelComplete1(player);
                Player_FloorCheck(player);
            } else {
                if (gCsFrameCount == 170) {
                    AUDIO_PLAY_BGM(NA_BGM_COURSE_CLEAR);
                }
                // @recomp: Update VisPerFrame with N64 Recording
                UpdateVisPerFrameFromRecording(gCarrierCutsceneRecord, ARRAY_COUNT(gCarrierCutsceneRecord),
                                               &gCsFrameCount);
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
                UpdateVisPerFrameFromRecording(gMacbethCutsceneRecord, ARRAY_COUNT(gMacbethCutsceneRecord),
                                               &gCsFrameCount);
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

RECOMP_PATCH void Cutscene_LevelStart(Player* player) {
    gCsFrameCount++;
    if (gLevelMode == LEVELMODE_ON_RAILS) {
        switch (gCurrentLevel) {
            case LEVEL_CORNERIA:
                Corneria_LevelStart(player);
                Player_FloorCheck(player);
                break;

            case LEVEL_METEO:
                Meteo_LevelStart(player);
                break;

            case LEVEL_SECTOR_X:
                SectorX_LevelStart(player);
                break;

            case LEVEL_TITANIA:
                Titania_LevelStart(player);
                Player_FloorCheck(player);
                break;

            case LEVEL_ZONESS:
                Zoness_LevelStart(player);
                break;

            case LEVEL_MACBETH:
                Macbeth_LevelStart(player);
                break;

            case LEVEL_SECTOR_Y:
                SectorY_LevelStart(player);
                break;

            case LEVEL_SOLAR:
                // @recomp: Update VisPerFrame with N64 Recording
                UpdateVisPerFrameFromRecording(gSolarIntroCsRecord, ARRAY_COUNT(gSolarIntroCsRecord), &gCsFrameCount);
                Solar_LevelStart(player);
                break;

            case LEVEL_VENOM_1:
                Venom1_LevelStart(player);
                Player_FloorCheck(player);
                break;

            case LEVEL_AQUAS:
                Aquas_CsLevelStart(player);
                break;

            case LEVEL_AREA_6:
                Area6_LevelStart(player);
                break;
        }
        Cutscene_PathTexScroll(player);
    } else {
        switch (gCurrentLevel) {
            case LEVEL_FORTUNA:
                AllRange_FortunaIntro(player);
                Player_FloorCheck(player);
                break;

            case LEVEL_VENOM_2:
                Venom2_LevelStart(player);
                Player_FloorCheck(player);
                break;

            case LEVEL_BOLSE:
                Bolse_LevelStart(player);
                Player_FloorCheck(player);
                break;

            case LEVEL_KATINA:
                Katina_LevelStart(player);
                Player_FloorCheck(player);
                break;

            case LEVEL_SECTOR_Z:
                SectorZ_LevelStart(player);

            default:
                break;
        }
    }
}

RECOMP_PATCH void Ending_Main(void) {
    gCsFrameCount++;
    gGameFrameCount++;

    switch (D_ending_80196D00) {
        case 0:
            gRadioState = 0;
            gGameFrameCount = 0;
            gSceneSetup = 0;
            gCsCamEyeX = gCsCamEyeY = gCsCamEyeZ = 0.0f;
            gCsCamAtX = gCsCamAtY = 0.0f;
            gCsCamAtZ = -100.0f;
            D_ending_80196D00 = 1;
            break;

        case 1:
            if (D_ending_80192E70 < 2800) {
                break;
            }
        case 2:
            D_ending_80196D00 = 3;
            gCsFrameCount = 0;

        case 3:
            if (Ending_8018BCB0() == 0) {
                break;
            }
        case 4:
            gSceneSetup = 2;
            D_ending_80196D00 = 5;
            break;

        case 5:
            D_ending_80196F8C = 0;
            D_ending_80196D00 = 6;

        case 6:
            Ending_8018A570();
            break;

        case 7:
            break;
    }
    Ending_8018A8FC();
    Ending_8018B3D8();
    Ending_8018ABE8();

    // @recomp: Update VisPerFrame with N64 Recording
    UpdateVisPerFrameFromRecording(gEndingCsRecord, ARRAY_COUNT(gEndingCsRecord), &gGameFrameCount);
}

RECOMP_PATCH void Ending_8018D250(u32 arg0, AssetInfo* asset) {
    gSceneSetup = asset->unk_08;
    // @recomp: avoid updating gVIsPerFrame since we're using a recording
    //  gVIsPerFrame = asset->unk_70;
    gStarCount = asset->unk_14;
}

RECOMP_PATCH void Area6_A6Gorgon_Init(A6Gorgon* this) {
    Hitbox* hitbox;
    s32 i;

    gBossActive = true;
    gProjectFar = 25000.0f;
    gBossFrameCount = 0;

    // @recomp: Vi recording
    gA6GorgonCsFrameCount = 1;

    this->health = 780;

    this->fwork[2] = this->fwork[34] = 2.0f;
    this->fwork[35] = 1.2f;

    this->swork[25] = 3;
    this->swork[15 + 0] = this->swork[15 + 1] = this->swork[15 + 2] = 40;
    this->swork[18 + 0] = this->swork[18 + 1] = this->swork[18 + 2] = 3;

    for (i = 0; i < 40; i++) {
        D_i3_801C2250[i] = 0;
    }

    D_i3_801C22F0.unk_24 = D_i3_801C22F0.unk_28[0] = D_i3_801C22F0.unk_28[2] = D_i3_801C22F0.unk_28[1] = 255.0f;
    D_i3_801C22F0.unk_34 = 0.0f;

    this->swork[27 + 0] = this->swork[27 + 1] = this->swork[27 + 2] = 0;
    this->swork[22] = 32;
    this->swork[23] = 32;
    this->swork[24] = 255;

    this->fwork[29] = 255.0f;
    this->fwork[3] = -1700.0f;
    this->fwork[5] = 30.0f;
    this->fwork[32] = 1.0f;

    this->timer_050 = 500;

    A6_HIT_1(this, 0)->z.size = A6_HIT_1(this, 1)->z.size = A6_HIT_1(this, 2)->z.size = 195.0f;
    A6_HIT_1(this, 0)->y.size = A6_HIT_1(this, 1)->y.size = A6_HIT_1(this, 2)->y.size = 147.0f;
    A6_HIT_1(this, 0)->x.size = A6_HIT_1(this, 1)->x.size = A6_HIT_1(this, 2)->x.size = 153.0f;

    SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM, 40);
    SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_FANFARE, 40);

    AUDIO_PLAY_SFX(NA_SE_EN_SHIELD_ROLL_LEVEL, this->sfxSource, 4);
}
