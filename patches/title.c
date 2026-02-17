#include "patches.h"

#if 1 // Global

void Title_SetLightRot(f32 xRot, f32 yRot, f32 zSrc, f32* dirX, f32* dirY, f32* dirZ);
void Title_SetCamUp2(f32 arg0, f32 arg1, f32 arg2, f32* arg3, f32* arg4, f32* arg5, f32 arg6, f32* arg7, f32* arg8,
                     f32* arg9, f32 argA, f32 argB, f32 argC);
void Title_Cutscene_SetCamera(CameraPoint* arg0, s32 arg1, f32 arg2);
void Title_SetCamUp(f32 xRot, f32 yRot);
void Title_GetCamRot(f32* xRot, f32* yRot);
void Title_CsTakeOff_Setup(void);
void Title_GreatFoxDeckPlatform_Draw(void);
bool should_interpolate_perspective(Vec3f* eye, Vec3f* at);

extern s32 F_80177D80;

typedef enum TitleCsStates {
    /* 0 */ TITLE_SCREEN,              // In the Title Screen.
    /* 1 */ TITLE_GREAT_FOX_TRAVELING, // Close up to the Great Fox while it's traveling to corneria.
    /* 2 */ TITLE_CS_TEAM_RUNNING,     // Team is running in Great Fox's the passage way before take-off.
    /* 3 */ TITLE_GREAT_FOX_CLOSE_UP,  // Closing up to the Great Fox before take-off.
    /* 4 */ TITLE_TAKE_OFF,            // Take-off cutscene.
    /* 5 */ TITLE_TAKE_OFF_SPACE,      // Arwings coming out of the Great Fox out to space.
    /* 7 */ TITLE_RANKING = 7          // Show ranking
} TitleCsStates;

void Title_Ranking_Draw(void);
void Title_Screen_Draw(void);
void Title_StarfoxLogo_Draw(void);
void Title_CopyrightSymbol_Draw(void);
void Title_Copyright_Draw(void);
void Title_PressStart_Draw(void);
void Title_64Logo_Draw(void);
void Title_CsTeamRunning_Draw(void);
void Title_Logos_Draw(void);
void Title_SunGlare_Draw(void);
void Title_CsGreatFoxTraveling_Draw(void);
void Title_CsTakeOff_Draw(void);
void Title_CsGreatFoxCloseUp_Draw(void);
void Title_CsTakeOffSpace_Draw(void);
void Title_Matrix_Push(void);
void Title_TeamName_Draw(void);
void Title_TitleCard_Draw(void);

extern bool D_menu_801B8348;
extern bool sDrawTeamName;
extern s32 D_menu_801B8298;
bool sSkipInterpolation = 0;

#if 1
RECOMP_PATCH void Title_Draw(void) {
    static s32 camSkipTimes;

    if (sSkipInterpolation) {
        // Skip interpolation for this frame.
        gEXMatrixGroupSimple(gMasterDisp++, TAG_CAMERA_1, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);
        recomp_printf("CAMERA 1 SKIPED: %d\n", camSkipTimes++);
        gCamera1Skipped = true;
        sSkipInterpolation = 0;
    } else {
        // Simple interpolation works much better for cameras because they orbit around a focus.
        gEXMatrixGroupSimple(gMasterDisp++, TAG_CAMERA_1, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP);
        gCamera1Skipped = false;
    }

    switch (sCutsceneState) {
        case TITLE_RANKING:
            Title_Ranking_Draw();
            break;

        case TITLE_SCREEN:
            Title_Matrix_Push();
            Title_Screen_Draw();

            Matrix_Pop(&gGfxMatrix);

            Title_StarfoxLogo_Draw();
            Title_CopyrightSymbol_Draw();
            Title_Copyright_Draw();
            Title_PressStart_Draw();
            Title_Matrix_Push();
            Title_64Logo_Draw();

            Matrix_Pop(&gGfxMatrix);
            break;

        case TITLE_GREAT_FOX_TRAVELING:
            if (D_menu_801B8348) {
                Title_Matrix_Push();
                Title_CsGreatFoxTraveling_Draw();

                Matrix_Pop(&gGfxMatrix);

                Radio_Draw();
                Title_TitleCard_Draw();
            }
            Title_SunGlare_Draw();
            Title_Logos_Draw();
            break;

        case TITLE_CS_TEAM_RUNNING:
            Title_Matrix_Push();
            Title_CsTeamRunning_Draw();

            Matrix_Pop(&gGfxMatrix);

            if (sDrawTeamName) {
                Title_TeamName_Draw();
            }
            break;

        case TITLE_GREAT_FOX_CLOSE_UP:
            Title_Matrix_Push();

            Title_CsGreatFoxCloseUp_Draw();

            Matrix_Pop(&gGfxMatrix);
            break;

        case TITLE_TAKE_OFF:
            Title_Matrix_Push();
            Title_CsTakeOff_Draw();

            Matrix_Pop(&gGfxMatrix);

            Title_TitleCard_Draw();
            break;

        case TITLE_TAKE_OFF_SPACE:
            Title_Matrix_Push();

            Title_CsTakeOffSpace_Draw();

            Matrix_Pop(&gGfxMatrix);
            break;
    }

    if (sWipeHeight != 0) {
        gFillScreenAlpha = 0;
        gFillScreenRed = 0;
        gFillScreenGreen = 0;
        gFillScreenBlue = 0;
        Wipe_Draw(WIPE_VERTICAL, sWipeHeight);
    }

// Debug prints for camera skipping
#if 0
    // Matrix_LookAt(gGfxMatrix, gCsCamEyeX, gCsCamEyeY, gCsCamEyeZ, gCsCamAtX, gCsCamAtY, gCsCamAtZ, sTitleCamUpX,
    //               sTitleCamUpY, sTitleCamUpZ, MTXF_APPLY);

    RCP_SetupDL(&gMasterDisp, SETUPDL_83);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplaySmallText(10, 10, 1.0f, 1.0f, "CSEYEX:");
    Graphics_DisplaySmallNumber(80, 10, (int) ABS(gCsCamEyeX));
    if (gCsCamEyeX < 0.0f)
        Graphics_DisplaySmallText(70, 10, 1.0f, 1.0f, "-");

    Graphics_DisplaySmallText(10, 20, 1.0f, 1.0f, "CSEYEY:");
    Graphics_DisplaySmallNumber(80, 20, (int) ABS(gCsCamEyeY));
    if (gCsCamEyeY < 0.0f)
        Graphics_DisplaySmallText(70, 20, 1.0f, 1.0f, "-");

    Graphics_DisplaySmallText(10, 30, 1.0f, 1.0f, "CSEYEZ:");
    Graphics_DisplaySmallNumber(80, 30, (int) ABS(gCsCamEyeZ));
    if (gCsCamEyeZ < 0.0f)
        Graphics_DisplaySmallText(70, 30, 1.0f, 1.0f, "-");

    Graphics_DisplaySmallText(10, 40, 1.0f, 1.0f, "CSATX:");
    Graphics_DisplaySmallNumber(80, 40, (int) ABS(gCsCamAtX));
    if (gCsCamAtX < 0.0f)
        Graphics_DisplaySmallText(70, 40, 1.0f, 1.0f, "-");

    Graphics_DisplaySmallText(10, 50, 1.0f, 1.0f, "CSATY:");
    Graphics_DisplaySmallNumber(80, 50, (int) ABS(gCsCamAtY));
    if (gCsCamAtY < 0.0f)
        Graphics_DisplaySmallText(70, 50, 1.0f, 1.0f, "-");

    Graphics_DisplaySmallText(10, 60, 1.0f, 1.0f, "CSATZ:");
    Graphics_DisplaySmallNumber(80, 60, (int) ABS(gCsCamAtZ));
    if (gCsCamAtZ < 0.0f)
        Graphics_DisplaySmallText(70, 60, 1.0f, 1.0f, "-");

    Graphics_DisplaySmallText(10, 70, 1.0f, 1.0f, "SCNSTATE:");
    Graphics_DisplaySmallNumber(80, 70, (int) ABS(sSceneState));

    Graphics_DisplaySmallText(10, 80, 1.0f, 1.0f, "CSSTATE:");
    Graphics_DisplaySmallNumber(80, 80, (int) ABS(sCutsceneState));
#endif
}
#endif

#if 1 // Hangar Widescreen fix
RECOMP_PATCH void Title_GreatFoxDeck_Draw(void) {
    F_80177D80++;

    // Set up lights and display list
    Lights_SetOneLight(&gMasterDisp, D_menu_801B82E0, D_menu_801B82E4, D_menu_801B82E8, 0, 0, 0, gAmbientR, gAmbientG,
                       gAmbientB);
    RCP_SetupDL(&gMasterDisp, SETUPDL_23);

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_GREATFOXDECK, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GREATFOXDECK, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    }

    Matrix_Push(&gGfxMatrix);
    Matrix_Translate(gGfxMatrix, 0.0f, D_menu_801B9048, D_menu_801B904C, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 0.4f, 0.4f, 0.4f, MTXF_APPLY);

    Matrix_SetGfxMtx(&gMasterDisp);

    // Draw the original display list
    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckDL);

    // Apply scaling for mirroring
    Matrix_Push(&gGfxMatrix);                                // Push current matrix state
    Matrix_Scale(gGfxMatrix, -1.0f, 1.0f, 1.0f, MTXF_APPLY); // Mirror the display list horizontally
    Matrix_Translate(gGfxMatrix, -800.0f, 0.0f, 0.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);

    // Invert the culling mode for the mirrored geometry
    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Clear normal backface culling
    gSPSetGeometryMode(gMasterDisp++, G_CULL_FRONT);  // Enable front-face culling (inverted culling)

    if (F_80177D80 > 50) {
        // Draw the mirrored display list
        gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckDL);
    }
    // Restore culling mode
    gSPClearGeometryMode(gMasterDisp++, G_CULL_FRONT); // Clear front-face culling (inverted)
    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);    // Re-enable normal backface culling

    Matrix_Pop(&gGfxMatrix); // Restore original matrix state

    Title_GreatFoxDeckPlatform_Draw();

    Matrix_Pop(&gGfxMatrix); // Restore original transformation

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}
#endif

RECOMP_PATCH void Title_GreatFoxDeckPlatform_Draw(void) {
    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_GREATFOXDECK_PLATFORM, G_EX_PUSH, G_MTX_MODELVIEW,
                                        G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GREATFOXDECK_PLATFORM, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);
    }

    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
    Matrix_Translate(gGfxMatrix, 400.0f, -250.0f, 0.0f, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 1.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckPlatformDL);
    Matrix_Translate(gGfxMatrix, 400.0f - 400.0f, -250.0f + 250.0f, 500.0f, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 1.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckPlatformDL);
    Matrix_Translate(gGfxMatrix, 400.0f - 400.0f, -250.0f + 250.0f, -1000.0f, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 1.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckPlatformDL);
    Matrix_Translate(gGfxMatrix, 400.0f - 400.0f, -250.0f + 250.0f, -500.0f, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 1.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckPlatformDL);

    // reuse same DLs to complete the other side of the platform

    Matrix_Translate(gGfxMatrix, -100.0f, -110.0f, 0.0f, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f, -1.0f, 1.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckPlatformDL);

    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 500.0f, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 1.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckPlatformDL);

    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 500.0f, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 1.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckPlatformDL);

    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 500.0f, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 1.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckPlatformDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

#if 1
RECOMP_PATCH void Title_CorneriaExplosions_Draw(void) {
    f32 scale;
    s32 i;

    for (i = 0; i < sMaxExplosions; i++) {
        switch (D_menu_801B7CA0[i]) {
            case 0:
                D_menu_801B7C48[i] += 0.15f;
                if (D_menu_801B7C48[i] >= D_menu_801B7C70[i]) {
                    D_menu_801B7C48[i] = D_menu_801B7C70[i];
                    D_menu_801B7CA0[i] = 10;
                }
                break;

            case 10:
                D_menu_801B7D18[i]--;
                if (D_menu_801B7D18[i] <= 0) {
                    D_menu_801B7CA0[i] = 1;
                }
                break;

            case 1:
                D_menu_801B7CC8[i] -= 16;
                if (D_menu_801B7CC8[i] < 0) {
                    D_menu_801B7CC8[i] = 0;
                }
                D_menu_801B7CF0[i] -= 16;
                if (D_menu_801B7CF0[i] < 0) {
                    D_menu_801B7CF0[i] = 0;
                }
                if (D_menu_801B7CC8[i] == 0 && D_menu_801B7CF0[i] == 0) {
                    D_menu_801B7CA0[i] = 255;
                }
                break;

            case 255:
                D_menu_801B7BF8[i] = 10.0f + RAND_FLOAT(10.0f);
                D_menu_801B7C20[i] = -10.0f + RAND_FLOAT(-60.0f);
                D_menu_801B7C48[i] = 0.0f;
                D_menu_801B7C70[i] = 0.1f + RAND_FLOAT(0.3f);
                D_menu_801B7D18[i] = 5 + RAND_INT(5.0f);
                D_menu_801B7CC8[i] = 255;
                D_menu_801B7CF0[i] = 128;
                D_menu_801B7CA0[i] = 0;
                break;
        }
    }

    RCP_SetupDL(&gMasterDisp, SETUPDL_49);

    for (i = 0; i < sMaxExplosions; i++) {
        // @recomp Tag the transform.
        // gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_CORNERIA_EXPLOSIONS + i, G_EX_PUSH, G_MTX_MODELVIEW,
        //                                 G_EX_EDIT_ALLOW);
        gEXMatrixGroupDecomposed(gMasterDisp++, TAG_CORNERIA_EXPLOSIONS + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_SKIP);

        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 200, 200, D_menu_801B7CC8[i]);
        gDPSetEnvColor(gMasterDisp++, 255, 0, 0, D_menu_801B7CF0[i]);

        scale = D_menu_801B7C48[i] / sTitleCorneria.scale;

        Matrix_Push(&gGfxMatrix);

        Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 5.0f, MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, D_menu_801B7C20[i] * M_DTOR, MTXF_APPLY);
        Matrix_Translate(gGfxMatrix, 0.0f, D_menu_801B7BF8[i], 0.0f, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, scale, scale, scale, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);

        gSPDisplayList(gMasterDisp++, aTitleArwingEngineGlowDL);

        Matrix_Pop(&gGfxMatrix);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}
#endif

RECOMP_PATCH void Title_GreatFoxDeckLauncher_Draw(TitleTeam teamIdx) {
    Lights_SetOneLight(&gMasterDisp, D_menu_801B82E0, D_menu_801B82E4, D_menu_801B82E8, gLight1R, gLight1G, gLight1B,
                       gAmbientR, gAmbientG, gAmbientB);
    RCP_SetupDL(&gMasterDisp, SETUPDL_23);

    Matrix_Push(&gGfxMatrix);
    Matrix_Translate(gGfxMatrix, sTitleArwing[teamIdx].pos.x, -12.8f, sTitleDeckLauncherZpos, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 0.8f, 0.8f, 0.8f, MTXF_APPLY);

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_GREATFOXDECK_LAUNCHER + teamIdx, G_EX_PUSH, G_MTX_MODELVIEW,
                                        G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GREATFOXDECK_LAUNCHER + teamIdx, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);
    }

    Matrix_SetGfxMtx(&gMasterDisp);

    gSPDisplayList(gMasterDisp++, aTitleGreatFoxDeckLauncherDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    Matrix_Pop(&gGfxMatrix);
}

RECOMP_PATCH void Title_Arwing_DrawEngineGlow(TitleTeam teamIdx) {
    f32 engineGlowScale;
    f32 sp40;
    f32 sp3C;
    f32 var_fv1;
    f32 var_fa0;
    f32 temp;

    RCP_SetupDL(&gMasterDisp, SETUPDL_67);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

    if (sTitleArwing[teamIdx].unk_40 == 1) {
        var_fa0 = -30.0f;
        var_fv1 = 0.02f;
        gDPSetEnvColor(gMasterDisp++, 128, 128, 255, 255);
    } else {
        var_fa0 = -65.0f;
        var_fv1 = 0.2f;
        gDPSetEnvColor(gMasterDisp++, 0, 0, 255, 255);
    }

    engineGlowScale = sTitleArwing[teamIdx].unk_44;

    if ((gGameFrameCount % 2) != 0) {
        engineGlowScale += var_fv1;
    }

    Matrix_Push(&gGfxMatrix);
    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, var_fa0, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, engineGlowScale, engineGlowScale * 0.7f, engineGlowScale, MTXF_APPLY);

    Matrix_RotateZ(gGfxMatrix, -sTitleArwing[teamIdx].zRot * M_DTOR, MTXF_APPLY);
    Matrix_RotateX(gGfxMatrix, -sTitleArwing[teamIdx].xRot * M_DTOR, MTXF_APPLY);
    Matrix_RotateY(gGfxMatrix, -sTitleArwing[teamIdx].yRot * M_DTOR, MTXF_APPLY);

    sp3C = -Math_Atan2F(gCsCamEyeX - sTitleArwing[teamIdx].pos.x, gCsCamEyeZ - sTitleArwing[teamIdx].pos.z);
    temp = sqrtf(SQ(gCsCamEyeZ - sTitleArwing[teamIdx].pos.z) + SQ(gCsCamEyeX - sTitleArwing[teamIdx].pos.x));
    sp40 = Math_Atan2F(gCsCamEyeY - sTitleArwing[teamIdx].pos.y, temp);

    Matrix_RotateY(gGfxMatrix, -sp3C, MTXF_APPLY);
    Matrix_RotateX(gGfxMatrix, -sp40, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);

    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_ARWING_ENGINE_GLOW + teamIdx, G_EX_PUSH, G_MTX_MODELVIEW,
                                        G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ARWING_ENGINE_GLOW + teamIdx, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);
    }

    gSPDisplayList(gMasterDisp++, aTitleArwingEngineGlowDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);

    Matrix_Pop(&gGfxMatrix);
}

RECOMP_PATCH void Title_EngineGlowParticles_Draw(TitleTeam teamIdx) {
    s32 i;
    f32 sp70;
    f32 sp6C;
    f32 temp;

    for (i = 0; i < D_menu_801B8100; i++) {
        switch (D_menu_801B8108[i]) {
            case 0:
                Math_SmoothStepToF(&D_menu_801B7FC0[i], D_menu_801B8060[i], 0.05f, 100.0f, 0.01f);

                if (D_menu_801B7FC0[i] >= D_menu_801B8060[i]) {
                    D_menu_801B7FC0[i] = D_menu_801B8060[i];
                    D_menu_801B8108[i] = 1;
                }
                break;

            case 1:
                Math_SmoothStepToF(&D_menu_801B7D40[i], 0.0f, D_menu_801B7F20[i], 100.0f, 0.01f);
                Math_SmoothStepToF(&D_menu_801B7DE0[i], 0.0f, D_menu_801B7F20[i], 100.0f, 0.01f);
                Math_SmoothStepToF(&D_menu_801B7E80[i], -25.0f, D_menu_801B7F20[i], 100.0f, 0.01f);

                D_menu_801B7FC0[i] -= 0.002f;
                if (D_menu_801B7FC0[i] < 0.0f) {
                    D_menu_801B7FC0[i] = 0.0f;
                    D_menu_801B8108[i] = 255;
                }
                break;

            case 255:
                D_menu_801B7D40[i] = 10.0f - RAND_FLOAT(20.0f);
                D_menu_801B7DE0[i] = 3.0f - RAND_FLOAT(7.0f);
                D_menu_801B7E80[i] = -30.0f - RAND_FLOAT(15.0f);
                D_menu_801B7FC0[i] = 0.0f;
                D_menu_801B8060[i] = RAND_FLOAT(0.02f);
                D_menu_801B7F20[i] = 0.02f + RAND_FLOAT(0.08f);
                D_menu_801B8108[i] = 0;
                break;
        }
    }

    sp6C = -Math_Atan2F(gCsCamEyeX - sTitleArwing[teamIdx].pos.x, gCsCamEyeZ - sTitleArwing[teamIdx].pos.z);
    temp = sqrtf(SQ(gCsCamEyeZ - sTitleArwing[teamIdx].pos.z) + SQ(gCsCamEyeX - sTitleArwing[teamIdx].pos.x));
    sp70 = Math_Atan2F(gCsCamEyeY - sTitleArwing[teamIdx].pos.y, temp);

    RCP_SetupDL(&gMasterDisp, SETUPDL_49);

    gDPSetPrimColor(gMasterDisp++, 0, 0, 253, 253, 255, 255);
    gDPSetEnvColor(gMasterDisp++, 251, 251, 255, 255);

    for (i = 0; i < D_menu_801B8100; i++) {

        if (D_menu_801B7FC0[i] == 0.0f) {
            continue;
        }

        Matrix_Push(&gGfxMatrix);

        Matrix_Translate(gGfxMatrix, D_menu_801B7D40[i], D_menu_801B7DE0[i], D_menu_801B7E80[i], MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, D_menu_801B7FC0[i], D_menu_801B7FC0[i], D_menu_801B7FC0[i], MTXF_APPLY);

        Matrix_RotateZ(gGfxMatrix, -sTitleArwing[teamIdx].zRot * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, -sTitleArwing[teamIdx].xRot * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, -sTitleArwing[teamIdx].yRot * M_DTOR, MTXF_APPLY);

        Matrix_RotateY(gGfxMatrix, -sp6C, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, -sp70, MTXF_APPLY);

        Matrix_SetGfxMtx(&gMasterDisp);

        if (gCamera1Skipped) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_ARWING_ENGINE_GLOW_PARTICLES, G_EX_PUSH, G_MTX_MODELVIEW,
                                            G_EX_EDIT_NONE);
        } else {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ARWING_ENGINE_GLOW_PARTICLES + (teamIdx << 8) + i,
                                           G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        }

        gSPDisplayList(gMasterDisp++, aTitleArwingEngineGlowDL);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        Matrix_Pop(&gGfxMatrix);
    }
}

RECOMP_PATCH void Title_ArwingShadow_Draw(s32 arg0) {
    Matrix_Push(&gGfxMatrix);
    Matrix_Translate(gGfxMatrix, 0.0f,
                     (D_menu_801B9050 - sTitleArwing[arg0].pos.y * 2.05f) + (D_menu_801B9048 - 84.0f) * 1.99f, 0.0f,
                     MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 1.0f, MTXF_APPLY);
    Matrix_RotateY(gGfxMatrix, M_PI, MTXF_APPLY);

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_ARWING_SHADOW + arg0, G_EX_PUSH, G_MTX_MODELVIEW,
                                        G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ARWING_SHADOW + arg0, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);
    }

    Matrix_SetGfxMtx(&gMasterDisp);

    RCP_SetupDL_64();

    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 240);
    gSPDisplayList(gMasterDisp++, aTitleArwingShadowDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    Matrix_Pop(&gGfxMatrix);
}

RECOMP_PATCH void Title_Passage_Draw(void) {
    Lights_SetOneLight(&gMasterDisp, 0, 0, 0, 0, 0, 0, gAmbientR, gAmbientG, gAmbientB);

    sPassageWayZoffset += 70.0f;
    sPassageWayZoffset = Math_ModF(sPassageWayZoffset, 4101.6f);

    RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, 255, 995, 1000);

    Matrix_Push(&gGfxMatrix);

    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 3079.2002f - sPassageWayZoffset, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 0.6f, 0.6f, 0.6f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);

    gEXMatrixGroupDecomposed(gMasterDisp++, TAG_TITLE_PASSAGE, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_AUTO,
                             G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);

    gSPDisplayList(gMasterDisp++, aTitleCsPassageWayDL);

    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -6836.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);

    gSPDisplayList(gMasterDisp++, aTitleCsPassageWayDL);

    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -6836.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);

    gSPDisplayList(gMasterDisp++, aTitleCsPassageWayDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    Matrix_Pop(&gGfxMatrix);
}

#if 1
RECOMP_PATCH void Title_CsTakeOff_Update(void) __attribute__((optnone)) {
    s32 i;
    f32 temp[4];

    switch (sSceneState) {
        case 0:
            D_menu_801B7BEC = 0;

            F_80177D80 = 0;
            Title_CsTakeOff_Setup();
            Audio_SetEnvSfxReverb(104);

            sTimer1 = 30;

            sTitleArwing[TEAM_SLIPPY].timer = 70;
            sTitleArwing[TEAM_PEPPY].timer = 102;
            sTitleArwing[TEAM_FALCO].timer = 132;
            sTitleArwing[TEAM_FOX].timer = 171;

            D_menu_801B8294 = D_menu_801B6C60;
            D_menu_801B8298 = 0;
            D_menu_801B828C = 0.0f;
            D_menu_801B8290 = 0.024f;
            sSceneState = 10;
            break;

        case 10:
            if (sTimer1 == 0) {
                AUDIO_PLAY_SFX(NA_SE_LIFT_UP, gDefaultSfxSource, 4);
                sTimer3 = 0;
                sSceneState = 1;
            }
            break;

        case 1:
            Title_GetCamRot(&D_menu_801B86A8, &D_menu_801B86AC);
            Title_SetCamUp(D_menu_801B86A8, D_menu_801B86AC);
            Title_Cutscene_SetCamera(D_menu_801B8294, 10, D_menu_801B8290);

            if (D_menu_801B9048 > 0.0f) {
                D_menu_801B9048 -= 0.41f;
                if (D_menu_801B9048 < 0.0f) {
                    D_menu_801B9048 = 0.0f;
                    AUDIO_PLAY_SFX(NA_SE_LIFT_UP_END, gDefaultSfxSource, 4);
                }
            }

            if (sTimer3 == 60) {
                sTitleCardState = 2; // TITLE_CARD_ARWING
            }

            for (i = ARRAY_COUNT(sTitleArwing) - 1; i >= 0; i--) {
                if (sTitleArwing[i].timer > 0) {
                    sTitleArwing[i].timer--;
                }

                if (sTitleArwing[i].timer == 1) {
                    AUDIO_PLAY_SFX(NA_SE_ARWING_HATCH, sTitleArwing[i].sfxSource, 0);
                };

                if (sTitleArwing[i].timer == 0) {
                    Math_SmoothStepToF(&sTitleArwing[i].laserGunsYpos, 0.0f, sTitleArwing[i].vel, 100.0f, 0.01f);
                    temp[i] = Math_SmoothStepToF(&(sTitleArwing[i].cockpitGlassXrot), 0.0f, sTitleArwing[i].vel, 100.0f,
                                                 0.01f);
                    sTitleArwing[i].vel *= 1.04f;
                    if (temp[i] == 0.0f) {
                        Audio_KillSfxBySourceAndId(sTitleArwing[i].sfxSource, NA_SE_ARWING_HATCH);
                    }
                }
            }

            if (sTimer3 == 226) {
                D_menu_801B7BE4 = 0;
                sTitleArwing[TEAM_FOX].draw = false;
            }

            if (sTimer3 == 226) {
                sTitleArwing[TEAM_FALCO].draw = false;

                for (i = 0; i < 4; i++) {
                    sTitleArwing[i].drawShadow = false;
                }
            }

            if (sTimer3 == 226) {
                sTitleArwing[TEAM_FOX].draw = true;
                sTitleArwing[TEAM_FALCO].draw = true;
            }

            if (sTimer3 == 226) {
                sTitleCardState = 0; // TITLE_CARD_NONE

                AUDIO_PLAY_SFX(NA_SE_ENGINE_START, sTitleArwing[TEAM_SLIPPY].sfxSource, 0);

                D_menu_801B86BC = 105.0f;
                D_menu_801B86C0 = 0.0f;
                D_menu_801B86C4 = -12.0f;
                D_menu_801B86A8 = 9.4f;
                D_menu_801B86AC = 110.0f;
                D_menu_801B86B4 = 36.0f;
                D_menu_801B86B8 = -70.0f;

                Title_SetCamUp2(D_menu_801B86BC, D_menu_801B86C0, D_menu_801B86C4, &gCsCamEyeX, &gCsCamEyeY,
                                &gCsCamEyeZ, D_menu_801B86B4, &gCsCamAtX, &gCsCamAtY, &gCsCamAtZ, D_menu_801B86B8,
                                D_menu_801B86A8, D_menu_801B86AC);

                sTimer3 = 0;

                sTitleArwing[TEAM_SLIPPY].unk_44 = 0.0f;

                D_menu_801B86C8 = 60.0f;
                D_menu_801B86CC = 0.0f;
                D_menu_801B82EC = 60.0f;
                D_menu_801B82F0 = 45.0f;
                D_menu_801B82F4 = 60.0f;

                gLight1R = D_menu_801B82EC;
                gLight1G = D_menu_801B82F0;
                gLight1B = D_menu_801B82F4;

                gAmbientR = 35;
                gAmbientG = 24;
                gAmbientB = 46;

                sSceneState++;
                sSkipInterpolation = 1;
            }
            sTimer3++;
            break;

        case 2:
            gStarCount = 800;
            if (sTimer3 == 30) {
                sTitleArwing[TEAM_SLIPPY].unk_40 = 1;
            }

            if (sTimer3 == 40) {
                sTitleArwing[TEAM_SLIPPY].unk_3C = 1;
            }

            if (sTimer3 > 40) {
                sTitleArwing[TEAM_SLIPPY].unk_44 += 0.002f;

                if (sTitleArwing[TEAM_SLIPPY].unk_44 > 0.2f) {
                    sTitleArwing[TEAM_SLIPPY].unk_44 = 0.2f;
                }

                Math_SmoothStepToF(&D_menu_801B82EC, 240.0f, 0.06f, 2.0f, 0.01f);
                Math_SmoothStepToF(&D_menu_801B82F0, 240.0f, 0.06f, 2.0f, 0.01f);
                Math_SmoothStepToF(&D_menu_801B82F4, 255.0f, 0.06f, 2.0f, 0.01f);

                gLight1R = D_menu_801B82EC;
                gLight1G = D_menu_801B82F0;
                gLight1B = D_menu_801B82F4;
            }

            if ((sTimer3 > 60) && (gFillScreenAlpha < 255)) {
                gFillScreenAlpha++;
            }

            if (gFillScreenAlpha == 128) {
                sTimer1 = 7;
                sTitleArwing[TEAM_SLIPPY].unk_3C = 0;
                sTitleArwing[TEAM_SLIPPY].unk_40 = 0;
                gFillScreenAlpha = 255;
                sSceneState++;
            }

            if (sTimer3 == 50 * 4) {
                sTitleArwing[TEAM_FOX].draw = false;
            }

            if (sTimer3 == 60 * 4) {
                sTitleArwing[TEAM_FALCO].draw = false;
            }

            Math_SmoothStepToF(&D_menu_801B86B4, 16.0f, 0.01f, 100.0f, 0.01f);
            Math_SmoothStepToF(&D_menu_801B86AC, 138.0f, 0.01f, 100.0f, 0.01f);

            Title_SetCamUp2(D_menu_801B86BC + 9, D_menu_801B86C0, D_menu_801B86C4, &gCsCamEyeX, &gCsCamEyeY,
                            &gCsCamEyeZ, D_menu_801B86B4, &gCsCamAtX, &gCsCamAtY, &gCsCamAtZ, D_menu_801B86B8,
                            D_menu_801B86A8, D_menu_801B86AC);
            sTimer3++;
            gStarCount = 800;
            break;

        case 3:
            if (sTimer1 == 0) {
                Audio_KillSfxBySourceAndId(sTitleArwing[TEAM_SLIPPY].sfxSource, NA_SE_ENGINE_START);
                AUDIO_PLAY_SFX(NA_SE_ARWING_BOOST, sTitleArwing[TEAM_SLIPPY].sfxSource, 0);

                sTimer1 = 4;

                gFillScreenAlpha = 0;

                for (i = 0; i < 4; i++) {
                    sTitleArwing[i].unk_3C = 1;
                    sTitleArwing[i].unk_44 = 0.9f;
                }

                sTimer3 = 0;

                gStarCount = 800;

                sSceneState++;
            }
            break;

        case 4:
            if (sTimer1 == 0) {
                sTitleArwing[TEAM_SLIPPY].unk_44 = 1.2f;

                Math_SmoothStepToF(&sTitleArwing[TEAM_SLIPPY].pos.z, 900.0f, 0.06f, 100.0f, 0.0001f);
                Math_SmoothStepToF(&D_menu_801B86AC, 154.0f, 0.2f, 100.0f, 0.01f);
                Math_SmoothStepToF(&D_menu_801B86A8, -5.0f, 0.2f, 100.0f, 0.01f);
                Math_SmoothStepToF(&D_menu_801B86BC, 100.0f, 0.2f, 100.0f, 0.01f);
                Math_SmoothStepToF(&D_menu_801B86B4, 40.0f, 0.2f, 100.0f, 0.01f);

                Title_SetCamUp2(D_menu_801B86BC, D_menu_801B86C0, D_menu_801B86C4, &gCsCamEyeX, &gCsCamEyeY,
                                &gCsCamEyeZ, D_menu_801B86B4, &gCsCamAtX, &gCsCamAtY, &gCsCamAtZ, D_menu_801B86B8,
                                D_menu_801B86A8, D_menu_801B86AC);

                if (sTimer3 > 8) {
                    Audio_SetEnvSfxReverb(0);
                    sSceneState = 0;
                    sCutsceneState = 5; // TITLE_TAKE_OFF_SPACE
                    sSkipInterpolation = 1;
                }
                sTimer3++;
            }
            break;
    }
    Title_SetLightRot(D_menu_801B86C8, D_menu_801B86CC, 100.0f, &D_menu_801B82E0, &D_menu_801B82E4, &D_menu_801B82E8);
    D_menu_801B7BEC++;
}
#endif

#endif
