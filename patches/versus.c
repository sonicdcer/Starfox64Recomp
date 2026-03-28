#include "patches.h"

#if DEBUG_DISABLE_VERSUS == 1

bool Option_Input_MoveCursor_Y(s32* arg0, s32 arg1, bool arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6,
                               s32 controllerNum, StickInputOption* stickY);
void Option_MainMenu_Setup(void);
void Option_Entry_Setup(void);

// @recomp: Disable versus mode for now.
RECOMP_PATCH void Option_MainMenu_Update(void) {
    s32 i;
    f32 var_fs0;
    f32 var_fs1;

    switch (sMainMenuState) {
        case 0:
            Option_MainMenu_Setup();
            if (!sMainMenuFromCancel) {
                sMainMenuState++;
            } else {
                sOptionCardCurTextPosX[0] = D_menu_801AE5E8[sExpertModeCursor];
                sOptionCardCurTextPosY[0] = D_menu_801AE5F0[sExpertModeCursor];
                sOptionCardCurTextPosX[4] = D_menu_801AE5F8[sExpertSoundCursor];
                sOptionCardCurTextPosY[4] = D_menu_801AE600[sExpertSoundCursor];
                sMainMenuState = 1000;
            }
            break;

        case 1000:
            for (i = 0; i < 6; i++) {
                if (sOptionCardList[i].frame.xScale < 0.4f) {
                    sOptionCardList[i].frame.xScale += 0.04f;
                    if (sOptionCardList[i].frame.xScale > 0.4f) {
                        sOptionCardList[i].frame.xScale = 0.4f;
                    }
                }
                if (sOptionCardList[i].tex.xScale < 1.0f) {
                    sOptionCardList[i].tex.xScale += 0.1f;
                    if (sOptionCardList[i].tex.xScale > 1.0f) {
                        sOptionCardList[i].tex.xScale = 1.0f;
                    }
                }

                Math_SmoothStepToF(&sOptionCardList[i].tex.yPos, sOptionCardCurTextPosY[i], 0.15f, 100.0f, 0.1f);
                Math_SmoothStepToF(&sOptionCardList[i].frame.y, sOptionCardPosY[i], 0.15f, 100.0f, 0.1f);
                Math_SmoothStepToF(&sOptionCardList[i].tex.xPos, sOptionCardCurTextPosX[i], 0.2f, D_menu_801AE620[i],
                                   0.1f);
            }

            gBlurAlpha += 4;

            if (sOptionCardList[sMainMenuCursor].frame.y == sOptionCardPosY[sMainMenuCursor]) {
                for (i = 0; i < ARRAY_COUNT(sOptionCardList); i++) {
                    sOptionCardList[i].tex.xScale = 1.0f;
                    sOptionCardList[i].tex.xPos = sOptionCardCurTextPosX[i];
                    sOptionCardList[i].tex.yPos = sOptionCardCurTextPosY[i];
                }
                sDrawCursor = true;
                gBlurAlpha = 255;
                sMainMenuState = 1;
            }
            break;

        case 1:
            // clang-format off
            if (Option_Input_MoveCursor_Y(&sMainMenuCursor, 6 - 1, 1, 0, 20, 5, 4, gMainController, &D_menu_801B9180)) { \
                AUDIO_PLAY_SFX(NA_SE_ARWING_CURSOR, gDefaultSfxSource, 4);
            }
            // clang-format on

            if ((sMainMenuCursor == 0) && sExpertModesEnabled &&
                Option_Input_MoveCursor_Y(&sExpertModeCursor, 1, 0, 0, 20, 5, 4, gMainController, &D_menu_801B9190)) {
                AUDIO_PLAY_SFX(NA_SE_ARWING_CURSOR, gDefaultSfxSource, 4);
                if (sExpertModeCursor) {
                    sOptionCardList[0].tex.texture = aExpertCardTex;
                    sOptionCardList[0].tex.width = 80;
                    sOptionCardList[0].tex.height = 12;
                    sOptionCardList[0].tex.xPos = 125.0f;
                    sOptionCardList[0].tex.yPos = 55.0f;
                } else {
                    sOptionCardList[0].tex.texture = aMainGameCardTex;
                    sOptionCardList[0].tex.width = 72;
                    sOptionCardList[0].tex.height = 12;
                    sOptionCardList[0].tex.xPos = 124.0f;
                    sOptionCardList[0].tex.yPos = 54.0f;
                }
                gExpertMode = sExpertModeCursor;
            }

            if ((sMainMenuCursor == 4) && sExpertModesEnabled &&
                Option_Input_MoveCursor_Y(&sExpertSoundCursor, 1, 0, 0, 20, 5, 4, gMainController, &D_menu_801B9198)) {
                AUDIO_PLAY_SFX(NA_SE_ARWING_CURSOR, gDefaultSfxSource, 4);
                if (sExpertSoundCursor) {
                    sOptionCardList[4].tex.texture = aExpertCardTex;
                    sOptionCardList[4].tex.width = 80;
                    sOptionCardList[4].tex.height = 12;
                    sOptionCardList[4].tex.xPos = 125.0f;
                    sOptionCardList[4].tex.yPos = 151.0f;
                } else {
                    sOptionCardList[4].tex.texture = aSoundCardTex;
                    sOptionCardList[4].tex.width = 64;
                    sOptionCardList[4].tex.height = 10;
                    sOptionCardList[4].tex.xPos = 133.0f;
                    sOptionCardList[4].tex.yPos = 151.0f;
                }
            }

            if (gControllerPress[gMainController].button & (A_BUTTON | START_BUTTON)) {

                // @recomp: Disable Versus mode
                if (sMainMenuCursor == 2) {
                    break;
                }

                AUDIO_PLAY_SFX(NA_SE_ARWING_DECIDE, gDefaultSfxSource, 4);
                sLightningYpos = sOptionCardPosY[sMainMenuCursor];
                sDrawCursor = false;
                sMainMenuState = 11;
            }
            if (gControllerPress[gMainController].button & B_BUTTON) {
                AUDIO_PLAY_SFX(NA_SE_ARWING_CANCEL, gDefaultSfxSource, 4);
                sWipeHeight = 0;
                sLevelStartState = true;
                sMainMenuState = 100;
            }
            break;

        case 11:
            gBlurAlpha = 16;
            D_menu_801B9178 = 3;
            sOptionCardList[sMainMenuCursor].frame.xScale -= 0.01f;
            sOptionCardList[sMainMenuCursor].frame.yScale -= 0.01f;
            sOptionCardList[sMainMenuCursor].tex.xScale -= 0.1f;
            sOptionCardList[sMainMenuCursor].tex.yScale -= 0.1f;
            sOptionCardList[sMainMenuCursor].tex.xPos += 4.0f;
            D_menu_801B917C = 3;
            sMainMenuState++;
            break;

        case 12:
            if (D_menu_801B9178 == 1) {
                sOptionCardList[sMainMenuCursor].frame.xScale += 0.01f;
                sOptionCardList[sMainMenuCursor].frame.yScale += 0.01f;
                sOptionCardList[sMainMenuCursor].tex.xScale += 0.1f;
                sOptionCardList[sMainMenuCursor].tex.yScale += 0.1f;
                sOptionCardList[sMainMenuCursor].tex.xPos -= 4.0f;
                D_menu_801B9214 = 1;
            }

            for (i = 0; i < ARRAY_COUNT(sOptionCardList); i++) {
                if (i == sMainMenuCursor) {
                    continue;
                }

                if (sOptionCardList[i].frame.xScale > 0.01f) {
                    sOptionCardList[i].frame.xScale -= 0.05f;
                    sOptionCardList[i].tex.xScale -= 0.19f;
                    Math_SmoothStepToF(&sOptionCardList[i].tex.xPos, 158.0f, 0.2f, D_menu_801AE608[i], 0.1f);
                } else {
                    sOptionCardList[i].drawFrame = false;
                    sOptionCardList[i].drawTex = false;
                }
            }

            if (D_menu_801B917C == 0) {
                var_fs0 = 25.0f;
                var_fs1 = 90.0f;

                if ((sMainMenuCursor == 0) || (sMainMenuCursor == 1)) {
                    var_fs0 = 0.0f;
                    var_fs1 = 120.0f;
                }

                Math_SmoothStepToF(&sOptionCardList[sMainMenuCursor].tex.yPos, var_fs0, 0.25f, 100.0f, 0.1f);
                Math_SmoothStepToF(&sOptionCardList[sMainMenuCursor].frame.y, var_fs1, 0.25f, 100.0f, 0.1f);

                gBlurAlpha += 4;

                if ((sOptionCardList[sMainMenuCursor].tex.yPos == var_fs0) &&
                    (sOptionCardList[sMainMenuCursor].frame.y == var_fs1)) {
                    gBlurAlpha += 255;
                    sMenuEntryState = sMainMenuCursor;

                    if ((sMainMenuCursor == 4) && sExpertSoundCursor) {
                        sMenuEntryState = 6;
                    }
                    Option_Entry_Setup();
                }
            }
            break;

        case 100:
            if (sWipeHeight < 120) {
                sWipeHeight += 18;
            } else {
                gStarCount = 0;
                gGameState = GSTATE_TITLE;
                gNextGameStateTimer = 2;
                gTitleState = 0;
                gGoToTitle = true;
                gDrawMode = DRAW_NONE;
                sLevelStartState = false;
                sWipeHeight = 0;
                gControllerLock = 3;
            }
            break;
    }
}
#endif