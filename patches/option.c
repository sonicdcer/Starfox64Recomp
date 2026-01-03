#include "patches.h"

#if 1

extern f32 D_menu_801B91F4;
extern f32 D_menu_801B91F8;
extern s32 sVsMenuCursor;
extern s32 sRankInPos;
extern s32 D_menu_801B9134;
extern s32 D_menu_801B913C;
extern f32 sArwingLightXrot;
extern f32 sArwingLightYrot;
extern s32 sArwingLightColR;
extern s32 sArwingLightColG;
extern s32 sArwingLightColB;
extern s32 sArwingLightAmbR;
extern s32 sArwingLightAmbG;
extern s32 sArwingLightAmbB;
extern f32 sRightArwingCursorYrot;
extern f32 sLeftArwingCursorYrot;
extern f32 sOkConfirmTimer;
extern bool sVsSubMenuFromCancel;
extern int gEndingBorderFrame;

s32 Title_GetRankTotalHits(void);
s32 Option_RankInPos_Check(void);

void Option_OrdinalNumbers_Draw(s32, s32, s32);
void Option_RankingName_Draw(s32, s32, s32);
void Option_RankingTotalHits_Draw(s32, s32, s32);
void Map_RemainingLives_Draw(s32, s32, s32);
void Option_RankingTeamAlive_Draw(s32, s32, s32);
void Option_RankingRoute_Draw(s32, f32, f32);
void Option_Menu_Push(void);
void Option_Color_FlashRed(f32*);

typedef enum OptionId {
    /* 1000 */ OPTION_MAIN_MENU = 1000,
    /* 2000 */ OPTION_VERSUS_STAGE = 2000,
    /*    0 */ OPTION_MAP = 0,
    /*    1 */ OPTION_TRAINING,
    /*    2 */ OPTION_VERSUS,
    /*    3 */ OPTION_RANKING,
    /*    4 */ OPTION_SOUND,
#ifdef VERSION_EU
    /*    5 */ OPTION_LANGUAGE,
    /*    6 */ OPTION_DATA,
    /*    7 */ OPTION_EXPERT_SOUND,
#else
    /*    5 */ OPTION_DATA,
    /*    6 */ OPTION_EXPERT_SOUND,
#endif
    /*   10 */ OPTION_POINT_MATCH = 10, // Point Match
    /*   20 */ OPTION_BR_MATCH = 20,    // Battle Royal Match
    /*   30 */ OPTION_TT_MATCH = 30,    // Time Trial Match
    /*  200 */ OPTION_NAME_ENTRY = 200,
    /*  300 */ OPTION_SCORE = 300,
    /*  400 */ OPTION_INVOICE = 400,
    /*  401 */ OPTION_MAX
} OptionId;

// @recomp: Fix ranking display fade bars positions
RECOMP_PATCH void Option_80197914(void) {
    Vec3f* vec1;
    Vec3f* vec2;
    s32 i;

    RCP_SetupDL(&gMasterDisp, SETUPDL_0);

    for (i = 0, vec1 = D_menu_801AF100, vec2 = D_menu_801AF118; i < 2; i++, vec1++, vec2++) {
        Matrix_Push(&gGfxMatrix);
        Matrix_Translate(gGfxMatrix, vec1->x, vec1->y, -500.0f, MTXF_APPLY);

        // @recomp: Increase the scale by 2.5f to compensate for missing borders
        Matrix_Scale(gGfxMatrix, vec2->x * 4, vec2->y + 2.5f, 1.0f, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, D_menu_801AEF30);
        Matrix_Pop(&gGfxMatrix);
    }
}

RECOMP_PATCH void Option_Setup(void) {
    bool enableExpertModes;
    bool playedExpertMode;
    s32 i;

#if MODS_LEVEL_SELECT == 1
    for (i = 0; i < ARRAY_COUNT(gSaveFile.save.data.planet); i++) {
        gSaveFile.save.data.planet[i].expertClear = 1;
        gSaveFile.save.data.planet[i].normalClear = 1;
        gSaveFile.save.data.planet[i].normalMedal = 1;
        gSaveFile.save.data.planet[i].expertMedal = 1;
        gSaveFile.save.data.planet[i].played = 1;
    }
    Save_Write();
#endif
    gVIsPerFrame = 2;

    sOptionCardList[OPTION_MAP].tex.texture = aMainGameCardTex;
    sOptionCardList[OPTION_MAP].tex.width = 72;
    sOptionCardList[OPTION_MAP].tex.height = 12;
    sOptionCardList[OPTION_SOUND].tex.texture = aSoundCardTex;
    sOptionCardList[OPTION_SOUND].tex.width = 64;
    sOptionCardList[OPTION_SOUND].tex.height = 10;

    for (i = 0; i < ARRAY_COUNT(sOptionCardList); i++) {
        sOptionCardCurTextPosX[i] = sOptionCardTextPosX[i];
        sOptionCardCurTextPosY[i] = sOptionCardTextPosY[i];
    }

    enableExpertModes = true;
    for (i = 0; i < ARRAY_COUNT(gSaveFile.save.data.planet); i++) {
        if (i == SAVE_SLOT_VENOM_1) {
            continue;
        }
        if (!(gSaveFile.save.data.planet[i].normalMedal & 1)) {
#if MODS_LEVEL_SELECT == 1
            enableExpertModes = true;
#elif MODS_SFX_JUKEBOX == 1
            enableExpertModes = true;
#else
            enableExpertModes = false;
#endif
            break;
        }
    }

    sExpertModesEnabled = enableExpertModes;

    gFillScreenRed = 0;
    gFillScreenGreen = 0;
    gFillScreenBlue = 0;
    gFillScreenAlpha = 0;

    gBlurAlpha = 255;
    gBgColor = 0;

    sMainMenuCursor = OPTION_MAP;
    if ((gLastGameState == GSTATE_PLAY) && (gCurrentLevel == LEVEL_TRAINING)) {
        gLastGameState = GSTATE_NONE;
        sMainMenuCursor = OPTION_TRAINING;
    }

    sVsMenuCursor = 0;
    sExpertSoundCursor = 0;
    sExpertModeCursor = 0;
    playedExpertMode = gExpertMode;
    gExpertMode = false;

    gGameFrameCount = 0;
    sWipeHeight = 0;
    D_menu_801B9178 = 0;
    D_menu_801B917C = 0;

    sMenuEntryState = OPTION_MAIN_MENU;

    if ((gLastGameState == GSTATE_GAME_OVER) || (gLastGameState == GSTATE_ENDING)) {
        if (gLastGameState == GSTATE_ENDING) {

            // @recomp: reset ending borders
            gEndingBorderFrame = 0; // Reset frame when not in ending state
            gMissionPlanet[gMissionNumber] = PLANET_VENOM;
            if (gLeveLClearStatus[LEVEL_VENOM_ANDROSS] == 1) {
                gSaveFile.save.data.planet[SAVE_SLOT_VENOM_1].played = 1;
                if (playedExpertMode) {
                    gSaveFile.save.data.planet[SAVE_SLOT_VENOM_1].expertClear = 1;
                } else {
                    gSaveFile.save.data.planet[SAVE_SLOT_VENOM_1].normalClear = 1;
                }
                Save_Write();
            } else if (gLeveLClearStatus[LEVEL_VENOM_ANDROSS] == 2) {
                gMissionMedal[gMissionNumber] = 1;
                gSaveFile.save.data.planet[SAVE_SLOT_VENOM_2].played = 1;
                if (playedExpertMode) {
                    gSaveFile.save.data.planet[SAVE_SLOT_VENOM_2].expertClear = 1;
                    gSaveFile.save.data.planet[SAVE_SLOT_VENOM_2].expertMedal = 1;
                } else {
                    gSaveFile.save.data.planet[SAVE_SLOT_VENOM_2].normalClear = 1;
                    gSaveFile.save.data.planet[SAVE_SLOT_VENOM_2].normalMedal = 1;
                }
                Save_Write();
            }
        }

        Title_GetRankTotalHits();

        sRankInPos = Option_RankInPos_Check();

        if (sRankInPos != -1) {
            sMenuEntryState = OPTION_NAME_ENTRY;
        } else {
            sMenuEntryState = OPTION_SCORE;
        }
    } else if (D_game_800D2870) {
        sMainMenuCursor = OPTION_VERSUS;
        sVsMenuCursor = gVsMatchType;
        for (i = 0; i < ARRAY_COUNT(sOptionCardList); i++) {
            if (i == OPTION_VERSUS) {
                continue;
            }
            sOptionCardList[i].frame.xScale = 0.01f;
            sOptionCardList[i].frame.yScale = 0.21f;
            sOptionCardList[i].frame.y = sOptionCardPosY[i];
            sOptionCardList[i].frame.x = 0.0f;
            sOptionCardList[i].frame.z = 0.0f;
            sOptionCardList[i].drawFrame = false;

            sOptionCardList[i].tex.xScale = 0.01f;
            sOptionCardList[i].tex.yScale = 1.0f;
            sOptionCardList[i].tex.xPos = 158.0f;
            sOptionCardList[i].tex.yPos = sOptionCardCurTextPosY[i];
            sOptionCardList[i].drawTex = false;
        }
        sOptionCardList[OPTION_VERSUS].frame.x = 0.0f;
        sOptionCardList[OPTION_VERSUS].frame.z = 0.0f;
        sOptionCardList[OPTION_VERSUS].frame.y = 90.0f;
        sOptionCardList[OPTION_VERSUS].frame.xScale = 0.4f;
        sOptionCardList[OPTION_VERSUS].frame.yScale = 0.21f;
        sOptionCardList[OPTION_VERSUS].drawFrame = true;
        sOptionCardList[OPTION_VERSUS].tex.xScale = 1.0f;
        sOptionCardList[OPTION_VERSUS].tex.yScale = 1.0f;
        sOptionCardList[OPTION_VERSUS].tex.xPos = 145.0f;
        sOptionCardList[OPTION_VERSUS].tex.yPos = 25.0f;
        sOptionCardList[OPTION_VERSUS].drawTex = true;

        sMenuEntryState = OPTION_VERSUS;
    } else {
        sMenuEntryState = OPTION_MAIN_MENU;
    }

    sMainMenuState = 0;
    D_menu_801B9134 = 0;
    D_menu_801B913C = 0;

    gOptionMenuStatus = OPTION_UPDATE;

    Memory_FreeAll();

    gStarfieldX = SCREEN_WIDTH;
    gStarfieldY = SCREEN_HEIGHT;

    Play_GenerateStarfield();

    gCsCamEyeX = 0.0f;
    gCsCamEyeY = 0.0f;
    gCsCamEyeZ = 10.0f;
    gCsCamAtX = 0.0f;
    gCsCamAtY = 0.0f;
    gCsCamAtZ = 0.0f;

    sArwingLightXrot = -80.0f;
    sArwingLightYrot = 0.0f;

    sArwingLightColR = 225;
    sArwingLightColG = 225;
    sArwingLightColB = 225;
    sArwingLightAmbR = 30;
    sArwingLightAmbG = 30;
    sArwingLightAmbB = 30;

    D_menu_801B91F8 = 0.1f;
    D_menu_801B91F4 = -130.0f;
    sRightArwingCursorYrot = 0.0f;
    sLeftArwingCursorYrot = 0.0f;

    sOkConfirmTimer = 0;
    sMainMenuFromCancel = false;
    sVsSubMenuFromCancel = false;
    AUDIO_PLAY_BGM(NA_BGM_SELECT);
}

RECOMP_PATCH void Option_RankingMenu2_Draw(void) {
    f32 var_fs0;
    f32 var_fs1;
    s32 colorGB;
    s32 i;
    static f32 D_menu_801AF0F4 = 130.0f;
    static f32 D_menu_801AF0F8[] = { 48.0f, 175.0f };

    var_fs0 = D_menu_801B91D4;
    var_fs1 = D_menu_801B91D8;

    for (i = 0; i < RANKING_MAX; i++) {
        if ((var_fs0 > 69.0f) && (var_fs0 < 230.0f)) {
            Option_OrdinalNumbers_Draw(i, 41, var_fs0);
            Option_RankingName_Draw(i, 75, var_fs0 - 25.0f);
            Option_RankingTotalHits_Draw(i, 130, (var_fs0 - 26.0f));
            Map_RemainingLives_Draw(210, (var_fs0 - 24.0f), gSaveFile.save.data.rankingLives[i]);
            Option_RankingTeamAlive_Draw(i, 258, var_fs0 - 25.0f);
        }
        Option_RankingRoute_Draw(i, var_fs0, var_fs1);
        var_fs0 += D_menu_801AF0F4;
        var_fs1 -= D_menu_801AF0F4;
    }

    Option_Menu_Push();
    Option_80197914();

    Matrix_Pop(&gGfxMatrix);

    RCP_SetupDL(&gMasterDisp, SETUPDL_76);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

    Lib_TextureRect_IA8(&gMasterDisp, D_OPT_800D170, 8, 16, 0.0f, D_menu_801AF0F8[0], 40.0f, 1.5f);
    Lib_TextureRect_IA8_MirY(&gMasterDisp, D_OPT_800D170, 8, 16, 0.0f, D_menu_801AF0F8[1], 40.0f, 1.5f);

    // @recomp:
    // Background_DrawPartialStarfield(0, 70);
    // Background_DrawPartialStarfield(170, 239);
    Background_DrawStarfield();

    RCP_SetupDL(&gMasterDisp, SETUPDL_83);

    if (D_menu_801B91D4 >= 114.0f) {
        gDPSetPrimColor(gMasterDisp++, 0, 0, 32, 32, 32, 255);
    } else {
        Option_Color_FlashRed(&D_menu_801B93F4);
        colorGB = D_menu_801B93F4;
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, colorGB, colorGB, 255);
    }

    Lib_TextureRect_IA8(&gMasterDisp, D_OPT_800D070, 16, 16, 150.0f, 44.0f, 1.0f, 1.0f);

    if (D_menu_801B91D4 <= -1055.0f) {
        gDPSetPrimColor(gMasterDisp++, 0, 0, 32, 32, 32, 255);
    } else {
        Option_Color_FlashRed(&D_menu_801B93F8);
        colorGB = D_menu_801B93F8;
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, colorGB, colorGB, 255);
    }

    Lib_TextureRect_IA8_MirY(&gMasterDisp, D_OPT_800D070, 16, 16, 150.0f, 200.0f, 1.0f, 1.0f);
}

#endif