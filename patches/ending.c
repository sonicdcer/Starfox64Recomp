#include "patches.h"

typedef struct {
    /* 0x00 */ Animation* anim;
    /* 0x04 */ Limb** skeleton;
    /* 0x08 */ s16 setupDL;
    /* 0x0C */ Vec3f pos;
    /* 0x18 */ Vec3f rot;
    /* 0x24 */ Vec3f scale;
    /* 0x30 */ f32 unk_30;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ Color_RGBA32 prim;
} UnkStruct_196D08; // size = 0x40

extern UnkStruct_196D08 D_ending_80196D08[10];
extern Vec3f D_ending_80198600[300];
extern AssetInfo D_ending_801934B4[94];

void Ending_80191700(u32 arg0, AssetInfo* asset);
void Ending_80191710(u32 arg0, AssetInfo* asset);
void Ending_80191C58(u32 arg0, AssetInfo* asset);
void Ending_80191C7C(u32 arg0, AssetInfo* asset);
bool Ending_8018BCB0(void);
void Ending_8018A570(void);
void Ending_8018A8FC(void);
void Ending_8018B3D8(void);
void Ending_8018ABE8(void);
void Ending_8018A828(void);
void Ending_801926D4(void);
void Ending_8018C21C(void);
void Ending_8018CE20(s32);
void Animation_DrawSkeleton_SkipInterpolation(s32 mode, Limb** skeletonSegment, Vec3f* jointTable,
                                              OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data,
                                              Matrix* transform);

int gEndingBorderFrame = 0;

void DrawBorders(void) {
#if ENDING_BORDERS == 1
    // static int frame = 0;
    int max_frame = 1800; // Duration of the animation in frames
    int margin = 20;
    int extraOffset = SCREEN_WIDTH * 2; // Increase this value to push rectangles further out
    float t;

    if (gGameState == GSTATE_ENDING) {
        if (gEndingBorderFrame > max_frame) {
            gEndingBorderFrame = max_frame;
        }

        t = (float) gEndingBorderFrame / max_frame;

        // Set up rendering state for opaque red rectangles
        RCP_SetupDL_12();
        gDPSetRenderMode(gMasterDisp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(gMasterDisp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 0, 0, 255); // Set color to black

        // Left rectangle slides in from further off-screen to its final position
        int left_end_x = (-SCREEN_WIDTH + margin + 1) * 2;
        int left_start_x = left_end_x - extraOffset * 2;
        int current_left_x = left_start_x + (int) (t * (left_end_x - left_start_x));

        gEXTextureRectangle(gMasterDisp++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_CENTER, current_left_x, 0, current_left_x,
                            SCREEN_HEIGHT * 4, 0, 0, 0, 0, 0);

        // Right rectangle slides in from further off-screen to its final position
        int right_end_x = (SCREEN_WIDTH - margin) * 2;
        int right_start_x = right_end_x + extraOffset * 2;
        int current_right_x = right_start_x - (int) (t * (right_start_x - right_end_x));

        gEXTextureRectangle(gMasterDisp++, G_EX_ORIGIN_CENTER, G_EX_ORIGIN_RIGHT, current_right_x, 0, current_right_x,
                            SCREEN_HEIGHT * 4, 0, 0, 0, 0, 0);

        gDPSetScissor(gMasterDisp++, 3, (SCREEN_WIDTH - 320) / 2, 0, (SCREEN_WIDTH + 320) / 2, 240);

        gEndingBorderFrame++; // Increment frame count for animation
    } else {
        gEndingBorderFrame = 0; // Reset frame when not in ending state
    }
#endif
}

// Team shadows in the ending
RECOMP_PATCH void Ending_801876A4(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        Matrix_Push(&gGfxMatrix);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ADDRESS(aTeamShadowDL) + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        RCP_SetupDL(&gMasterDisp, SETUPDL_62);
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 240);
        Matrix_Translate(gGfxMatrix, D_ending_80196D08[i].pos.x, D_ending_80196D08[i].pos.y, D_ending_80196D08[i].pos.z,
                         MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, D_ending_80196D08[i].scale.x + 4.0f, D_ending_80196D08[i].scale.y + 4.0f,
                     D_ending_80196D08[i].scale.z + 4.0f, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, D_ending_80196D08[i].rot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, (D_ending_80196D08[i].rot.x + 5.0f) * M_DTOR, MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, D_ending_80196D08[i].rot.z * M_DTOR, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, aTeamShadowDL);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        Matrix_Pop(&gGfxMatrix);
    }
}

#if DEBUG_ENDING == 1
extern bool sLevelStartState;
extern s32 sWipeHeight;

RECOMP_PATCH void Title_NextState_OptionMenu(void) {
    if (gControllerLock == 0) {
        switch (sLevelStartState) {
            case 0: // Wait for input
                if (((gControllerPress[gMainController].button & START_BUTTON) ||
                     (gControllerPress[gMainController].button & A_BUTTON)) &&
                    !sLevelStartState) {
                    AUDIO_PLAY_SFX(NA_SE_DECIDE, gDefaultSfxSource, 4);
                    SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM, 30);
                    SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_FANFARE, 30);
                    sWipeHeight = 0;
                    sLevelStartState = true;
                }
                break;

            case 1: // Go to the Option Menu
                if (sWipeHeight < 120) {
                    sWipeHeight += 18;
                } else {
                    gGameState = GSTATE_ENDING;
                    gGreatFoxIntact = 1;
                    gLeveLClearStatus[LEVEL_ZONESS] = 1;
                    gLeveLClearStatus[LEVEL_KATINA] = 1;
                    gNextGameStateTimer = 2;
                    gOptionMenuStatus = OPTION_WAIT;
                    gDrawMode = DRAW_NONE;
                    gStarCount = 0;
                    sLevelStartState = 0;
                    sWipeHeight = 0;
                    gControllerLock = 3;
                }
                break;
        }
    }
}
#endif

// Characters running in the ending
#if 1
void Animation_DrawSkeletonEnding(s32 mode, Limb** skeletonSegment, Vec3f* jointTable,
                                  OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data,
                                  Matrix* transform);

RECOMP_PATCH void Ending_80191C7C(u32 arg0, AssetInfo* asset) {
    f32 temp;

    RCP_SetupDL(&gMasterDisp, asset->unk_08);

    gSPFogPosition(gMasterDisp++, asset->fogNear, asset->fogFar);
    gDPSetFogColor(gMasterDisp++, asset->fog.r, asset->fog.g, asset->fog.b, 0);
    gDPSetEnvColor(gMasterDisp++, asset->env.r, asset->env.g, asset->env.b, asset->env.a);
    gDPSetPrimColor(gMasterDisp++, 0, 0, asset->prim.r, asset->prim.g, asset->prim.b, asset->prim.a);

    Matrix_Translate(gGfxMatrix, asset->unk_18.x + (arg0 - asset->unk_0C) * asset->unk_3C.x,
                     asset->unk_18.y + (arg0 - asset->unk_0C) * asset->unk_3C.y,
                     asset->unk_18.z + (arg0 - asset->unk_0C) * asset->unk_3C.z, MTXF_APPLY);

    Matrix_Scale(gGfxMatrix, asset->unk_30.x, asset->unk_30.y, asset->unk_30.z, MTXF_APPLY);

    temp = __sinf(arg0 * 0.1f + asset->unk_70);

    Matrix_RotateY(gGfxMatrix,
                   M_DTOR * (asset->unk_24.y + temp * asset->unk_54.y + (arg0 - asset->unk_0C) * asset->unk_48.y),
                   MTXF_APPLY);
    Matrix_RotateX(gGfxMatrix,
                   M_DTOR * (asset->unk_24.x + temp * asset->unk_54.x + (arg0 - asset->unk_0C) * asset->unk_48.x),
                   MTXF_APPLY);
    Matrix_RotateZ(gGfxMatrix,
                   M_DTOR * (asset->unk_24.z + temp * asset->unk_54.z + (arg0 - asset->unk_0C) * asset->unk_48.z),
                   MTXF_APPLY);

    Matrix_SetGfxMtx(&gMasterDisp);

    Animation_GetFrameData(asset->unk_00,
                           (u32) ((arg0 + asset->unk_70) * asset->unk_14) % Animation_GetFrameCount(asset->unk_00),
                           D_ending_80198600);

    if ((asset->unk_04 == aFoxSkel) || (asset->unk_04 == aFalcoSkel) || (asset->unk_04 == aPeppySkel) ||
        (asset->unk_04 == aSlippySkel)) {
        // @recomp: Use gEXMatrixGroup in the ending running sequence of the ending.
        Animation_DrawSkeletonEnding(0, asset->unk_04, D_ending_80198600, NULL, NULL, NULL, &gIdentityMatrix);
    } else {
        // @recomp: Normal for everything else.
        Animation_DrawSkeleton(0, asset->unk_04, D_ending_80198600, NULL, NULL, NULL, &gIdentityMatrix);
    }
}
#endif

#if 1
RECOMP_PATCH void Ending_Floor_Draw(u32 arg0, AssetInfo* asset) {
    f32 temp;

    gStarCount = 0;

    RCP_SetupDL(&gMasterDisp, asset->unk_08);

    gSPFogPosition(gMasterDisp++, asset->fogNear, asset->fogFar);
    gDPSetFogColor(gMasterDisp++, asset->fog.r, asset->fog.g, asset->fog.b, 0);
    gDPSetPrimColor(gMasterDisp++, 0, 0, asset->prim.r, asset->prim.g, asset->prim.b, asset->prim.a);

    Matrix_Translate(gGfxMatrix, asset->unk_18.x + (arg0 - asset->unk_0C) * asset->unk_3C.x,
                     asset->unk_18.y + (arg0 - asset->unk_0C) * asset->unk_3C.y,
                     asset->unk_18.z + (arg0 - asset->unk_0C) * asset->unk_3C.z, MTXF_APPLY);

    Matrix_Scale(gGfxMatrix, asset->unk_30.x, asset->unk_30.y, asset->unk_30.z, MTXF_APPLY);

    temp = __sinf(arg0 * 0.1f + asset->unk_70);

    Matrix_RotateY(gGfxMatrix,
                   M_DTOR * (asset->unk_24.y + temp * asset->unk_54.y + (arg0 - asset->unk_0C) * asset->unk_48.y),
                   MTXF_APPLY);
    Matrix_RotateX(gGfxMatrix,
                   M_DTOR * (asset->unk_24.x + temp * asset->unk_54.x + (arg0 - asset->unk_0C) * asset->unk_48.x),
                   MTXF_APPLY);
    Matrix_RotateZ(gGfxMatrix,
                   M_DTOR * (asset->unk_24.z + temp * asset->unk_54.z + (arg0 - asset->unk_0C) * asset->unk_48.z),
                   MTXF_APPLY);

    Matrix_SetGfxMtx(&gMasterDisp);

    // @recomp Tag the transform
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ADDRESS(aEndFloorDL), G_EX_PUSH, G_MTX_MODELVIEW,
                                   G_EX_EDIT_ALLOW);

    gDPLoadTextureBlock(gMasterDisp++, aEndFloorTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP | G_TX_NOMIRROR,
                        G_TX_WRAP | G_TX_NOMIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetupTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, arg0 * 14, 0, G_TX_NOMIRROR | G_TX_WRAP,
                 G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

    gSPDisplayList(gMasterDisp++, aEndFloorDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}
#endif

// @recomp: Hide Venom under 1700 frames
RECOMP_PATCH void Ending_80190648(s32 arg0, AssetInfo* asset) {
    RCP_SetupDL(&gMasterDisp, asset->unk_08);

    Matrix_RotateY(gGfxMatrix, M_DTOR * D_ending_801985F0.y, MTXF_NEW);
    Matrix_RotateX(gGfxMatrix, M_DTOR * D_ending_801985F0.x, MTXF_APPLY);
    Matrix_RotateZ(gGfxMatrix, M_DTOR * D_ending_801985F0.z, MTXF_APPLY);

    Matrix_Translate(gGfxMatrix, asset->unk_18.x, asset->unk_18.y, asset->unk_18.z, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, asset->unk_30.x, asset->unk_30.y, asset->unk_30.z, MTXF_APPLY);

    Matrix_SetGfxMtx(&gMasterDisp);

    if (D_ending_80192E70 < 1700) {
        gSPDisplayList(gMasterDisp++, aEndVenomDL);
    }
}

// @recomp: Borders & ending timer printing
RECOMP_PATCH void Ending_Draw(void) {
    Matrix_Push(&gGfxMatrix);
    Matrix_LookAt(gGfxMatrix, gCsCamEyeX, gCsCamEyeY, gCsCamEyeZ, gCsCamAtX, gCsCamAtY, gCsCamAtZ, 0.0f, 100.0f, 0.0f,
                  MTXF_NEW);

    switch (D_ending_80196D00) {
        case 1:
            Ending_801926D4();
            break;

        case 3:
            Ending_8018C21C();
            break;

        case 6:
            Ending_8018A828();
            break;

        case 7:
            Ending_801926D4();
            break;
    }

    Ending_8018CE20(D_ending_80196D04);
    D_ending_80196D04++;
    Radio_Draw();

#if ENDING_BORDERS == 1
    if (gGameState == GSTATE_ENDING) {
        DrawBorders();
    }
#endif

#if 0
    // @recomp: Ending timer.
    if ((D_ending_80192E70 >= 0) && (D_ending_80192E70 < 10000)) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_83);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
        Graphics_DisplaySmallText(10, 220, 1.0f, 1.0f, "TIMER");
        Graphics_DisplaySmallNumber(80, 220, D_ending_80192E70);
    }
#endif

    Matrix_Pop(&gGfxMatrix);

    // Recording debug
#if 0
    RCP_SetupDL(&gMasterDisp, SETUPDL_83);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplaySmallText(10 + 210, 180, 1.0f, 1.0f, "VIS:");
    Graphics_DisplaySmallNumber(60 + 210, 180, (int) gVIsPerFrame);
    Graphics_DisplaySmallText(10 + 210, 190, 1.0f, 1.0f, "GMFMS:");
    Graphics_DisplaySmallNumber(60 + 210, 190, (int) gGameFrameCount);
    Graphics_DisplaySmallText(10 + 210, 200, 1.0f, 1.0f, "ENDSTATE:");
    Graphics_DisplaySmallNumber(60 + 220, 210, (int) D_ending_80196D00);
#endif
}
