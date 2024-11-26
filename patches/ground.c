#include "patches.h"
#include "water_effect.inc"

extern Gfx* dynaFloor1;
extern Gfx* dynaFloor2;
extern Vtx* dynaFloor1Vtx;
extern Vtx* dynaFloor2Vtx;

#if 0
f32 gTestVarF = 0.0f;
#endif

void Bolse_DrawDynamicGround(void);

static f32 sGroundPositions360x_FIX[] = {
    5999.0f, -5999.0f, 5999.0f, -5999.0f, /* 5999.0f * 2.0f, 5999.0f * 2.0f, -5999.0f * 2.0f, -5999.0f * 2.0f,*/
};
static f32 sGroundPositions360z_FIX[] = {
    5999.0f, 5999.0f, -5999.0f, -5999.0f, /* 5999.0f * 2.0f, 5999.0f * 2.0f, -5999.0f * 2.0f, -5999.0f * 2.0f,*/
};

#if 1
RECOMP_PATCH void Background_DrawGround(void) {
    f32 sp1D4;
    s32 i;
    u32 temp_fv0;
    u32 temp_s0;
    u16* sp1C4;
    Gfx* sp1C0;

    if ((gCurrentLevel != LEVEL_VENOM_2) && ((gPlayer[0].cam.eye.y > 4000.0f) || !gDrawGround)) {
        return;
    }
    if ((gCurrentLevel == LEVEL_BOLSE) && gBolseDynamicGround) {
        Bolse_DrawDynamicGround();
        return;
    }

    sp1D4 = 0.0f;
    if ((gGroundType != 10) && (gGroundType != 11)) {
        sp1D4 = -4000.0f;
    }
    if (gGroundType == 7) {
        sp1D4 = 0.0f;
        gPlayer[gPlayerNum].xPath = 0.0f;
    }

    if (gLevelMode == LEVELMODE_ALL_RANGE) {
        Vec3f sp1B4;
        Vec3f sp1A8;
        f32 temp_fv1;
        f32 temp_fa0;

        sp1D4 = 0.0f;
        gPlayer[gPlayerNum].xPath = 0.0f;

        sp1B4.x = 0;
        sp1B4.y = 0;
        sp1B4.z = -5500.0f;

        Matrix_RotateY(gCalcMatrix, -gPlayer[gPlayerNum].camYaw, MTXF_NEW);
        Matrix_MultVec3fNoTranslate(gCalcMatrix, &sp1B4, &sp1A8);

        temp_fv1 = gPlayer[gPlayerNum].cam.eye.x + sp1A8.x;
        temp_fa0 = gPlayer[gPlayerNum].cam.eye.z + sp1A8.z;

        if (temp_fv1 > 6000.0f) {
            gPlayer[gPlayerNum].xPath = 12000.0f;
        }
        if (temp_fv1 > 18000.0f) {
            gPlayer[gPlayerNum].xPath = 24000.0f;
        }
        if (temp_fv1 < -6000.0f) {
            gPlayer[gPlayerNum].xPath = -12000.0f;
        }
        if (temp_fv1 < -18000.0f) {
            gPlayer[gPlayerNum].xPath = -24000.0f;
        }

        if (temp_fa0 > 6000.0f) {
            sp1D4 = 12000.0f;
        }
        if (temp_fa0 > 18000.0f) {
            sp1D4 = 24000.0f;
        }
        if (temp_fa0 < -6000.0f) {
            sp1D4 = -12000.0f;
        }
        if (temp_fa0 < -18000.0f) {
            sp1D4 = -24000.0f;
        }
    }

    Matrix_Push(&gGfxMatrix);
    Matrix_Translate(gGfxMatrix, gPlayer[gPlayerNum].xPath, -3.0f + gCameraShakeY, sp1D4, MTXF_APPLY);

    if (gFovYMode == 2) {
        Matrix_Scale(gGfxMatrix, 1.2f, 1.2f, 1.0f, MTXF_APPLY);
    }

    Matrix_SetGfxMtx(&gMasterDisp);

    switch (gCurrentLevel) {
        case LEVEL_CORNERIA:
            if (gGroundClipMode != 0) {
                RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            } else {
                RCP_SetupDL_20(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            }

            if (gLevelMode == LEVELMODE_ON_RAILS) {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GROUND_ON_RAILS, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                gDPSetTextureImage(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, SEGMENTED_TO_VIRTUAL(D_CO_601B6C0));
                temp_s0 = fabsf(Math_ModF(2.0f * (gPathTexScroll * 0.2133333f), 128.0f)); // 0.64f / 3.0f
                temp_fv0 = Math_ModF((10000.0f - gPlayer[gPlayerNum].xPath) * 0.32f, 128.0f);
                gDPSetupTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, temp_fv0, temp_s0,
                             G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
                switch (gGroundSurface) {
                    case SURFACE_GRASS:
                        gDPLoadTileTexture(gMasterDisp++, D_CO_601B6C0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32);
                        gBgColor = 0x845; // 8, 8, 32
                        break;
                    case SURFACE_ROCK:
                        gDPLoadTileTexture(gMasterDisp++, D_CO_6028260, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32);
                        gBgColor = 0x845; // 8, 8, 32
                        break;
                    case SURFACE_WATER:
                        RCP_SetupDL_45(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
                        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 128);
                        gDPLoadTileTexture(gMasterDisp++, D_CO_6028A60, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32);
                        gBgColor = 0x190F; // 24, 32, 56
                        break;
                }
                // Drawing the original water in the middle
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -3000.0f, MTXF_APPLY); // Center water
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_CO_601B640);
                Matrix_Pop(&gGfxMatrix);

                // Extend water to the left
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, -3000.0f, MTXF_APPLY); // Left water
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_CO_601B640);
                Matrix_Pop(&gGfxMatrix);

                // Extend water to the right
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, -3000.0f, MTXF_APPLY); // Right water
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_CO_601B640);
                Matrix_Pop(&gGfxMatrix);

                // Drawing the original water in the middle
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 3000.0f, MTXF_APPLY); // Center water
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_CO_601B640);
                Matrix_Pop(&gGfxMatrix);

                // Extend water to the left
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, 3000.0f, MTXF_APPLY); // Left water
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_CO_601B640);
                Matrix_Pop(&gGfxMatrix);

                // Extend water to the right
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, 3000.0f, MTXF_APPLY); // Right water
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_CO_601B640);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

                Matrix_Pop(&gGfxMatrix);
            } else {
                gGroundSurface = SURFACE_GRASS;
                gBgColor = 0x845; // 8, 8, 32

                // @recomp Tag the transform.
                gEXMatrixGroupDecomposed(gMasterDisp++, TAG_GROUND_ALL_RANGE, G_EX_PUSH, G_MTX_MODELVIEW,
                                         G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

                for (i = 0; i < ARRAY_COUNT(sGroundPositions360x); i++) {
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, sGroundPositions360x_FIX[i], 0.0f, sGroundPositions360z_FIX[i],
                                     MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, D_CO_601EAA0);
                    Matrix_Pop(&gGfxMatrix);
                }

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            }
            break;

        case LEVEL_VENOM_1:
        case LEVEL_MACBETH:
            RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            switch (gCurrentLevel) {
                case LEVEL_VENOM_1:
                    sp1C4 = D_VE1_6006750;
                    sp1C0 = D_VE1_60066D0;
                    gDPLoadTextureBlock(gMasterDisp++, sp1C4, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
                                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
                                        G_TX_NOLOD);
                    break;
                case LEVEL_MACBETH:
                    sp1C4 = D_MA_602DCB8;
                    sp1C0 = D_MA_60306D0;
                    gDPLoadTextureBlock(gMasterDisp++, sp1C4, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
                                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
                                        G_TX_NOLOD);
                    break;
            }
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GROUND_ON_RAILS, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            gDPSetTextureImage(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, sp1C4);
            temp_s0 = fabsf(Math_ModF(2.0f * (gPathTexScroll * 0.2133333f), 128.0f));
            temp_fv0 = Math_ModF((10000.0f - gPlayer[gPlayerNum].xPath) * 0.32f, 128.0f);
            gDPSetupTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, temp_fv0, temp_s0,
                         G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

            // CENTER FAR
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -3000.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, sp1C0);
            Matrix_Pop(&gGfxMatrix);

            // LEFT FAR (Mirrored)
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling for mirrored object
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, -3000.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, -1.0f, 1.0f, 0.5f, MTXF_APPLY); // Apply negative X scaling to mirror
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, sp1C0);
            Matrix_Pop(&gGfxMatrix);
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling

            // RIGHT FAR
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, -3000.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, sp1C0);
            Matrix_Pop(&gGfxMatrix);

            // CENTER
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 3000.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, sp1C0);
            Matrix_Pop(&gGfxMatrix);

            // LEFT (Mirrored)
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling for mirrored object
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, 3000.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, -1.0f, 1.0f, 0.5f, MTXF_APPLY); // Apply negative X scaling to mirror
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, sp1C0);
            Matrix_Pop(&gGfxMatrix);
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling

            // RIGHT
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, 3000.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, sp1C0);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            Matrix_Pop(&gGfxMatrix);
            break;

        case LEVEL_TRAINING:
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedVertsOrderAuto(gMasterDisp++, TAG_GROUND, G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
            //// @recomp Tag the transform.
            // gEXMatrixGroupDecomposed(gMasterDisp++, TAG_GROUND_ALL_RANGE, G_EX_PUSH, G_MTX_MODELVIEW,
            //                          G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
            //                          G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
            //                          G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

            RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

            static Vtx trainingGroundVtx_FIX[] = {
                { { { 4000, 0, -6000 }, 0, { 20947, -19923 }, { 0, 120, 0, 255 } } },
                { { { -4000, 0, -6000 }, 0, { 0, -19923 }, { 0, 120, 0, 255 } } },
                { { { -4000, 0, 0 }, 0, { 0, -9449 }, { 0, 120, 0, 255 } } },
                { { { 4000, 0, 0 }, 0, { 20947, -9449 }, { 0, 120, 0, 255 } } },
                { { { -4000, 0, 6000 }, 0, { 0, 1023 }, { 0, 120, 0, 255 } } },
                { { { 4000, 0, 6000 }, 0, { 20947, 1023 }, { 0, 120, 0, 255 } } },
            };

            static Gfx trainingGroundDL_FIX[] = {
                gsSPVertex(trainingGroundVtx_FIX, 6, 0),
                gsSP2Triangles(1, 2, 3, 0, 1, 3, 0, 0),
                gsSP2Triangles(4, 5, 3, 0, 4, 3, 2, 0),
                gsSPEndDisplayList(),
            };

            sp1C4 = D_TR_6005938;
            sp1C0 = trainingGroundDL_FIX;
            gDPLoadTextureBlock(gMasterDisp++, sp1C4, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
            RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

            if (gLevelMode == LEVELMODE_ON_RAILS) {
                // if (gPathTexScroll > 290.0f) {
                //     gPathTexScroll -= 290.0f;
                // }
                gDPSetTextureImage(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, sp1C4);
                temp_s0 = fabsf(Math_ModF(2.0f * (gPathTexScroll * 0.2133333f), 128.0f)); // 0.64f / 3.0f
                temp_fv0 = Math_ModF((10000.0f - gPlayer[gPlayerNum].xPath) * 0.32f, 128.0f);
                gDPSetupTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, temp_fv0, temp_s0,
                             G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

                // Original Display (Center)
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -2500.0f /* + gPathTexScroll */, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);

                // Original Display for the second section (Center)
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -8435.0f + 65 /* + gPathTexScroll */, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);

                gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);

                // Mirrored Display - Left Side
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, -2500.0f /* + gPathTexScroll */,
                                 MTXF_APPLY); // Move left by the width of the original (-8000.0f)
                Matrix_Scale(gGfxMatrix, -1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);

                // Mirrored Display - Right Side
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, -2500.0f /* + gPathTexScroll */,
                                 MTXF_APPLY); // Move right by the width of the original (+8000.0f)
                Matrix_Scale(gGfxMatrix, -1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);

                // Mirrored Display - Left Side for the second section
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, -8435.0f + 65 /* + gPathTexScroll */,
                                 MTXF_APPLY); // Move left by the width of the original (-8000.0f)
                Matrix_Scale(gGfxMatrix, -1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);

                // Mirrored Display - Right Side for the second section
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, -8435.0f + 65 /* + gPathTexScroll */,
                                 MTXF_APPLY); // Move right by the width of the original (+8000.0f)
                Matrix_Scale(gGfxMatrix, -1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposed(gMasterDisp++, TAG_GROUND_ALL_RANGE, G_EX_PUSH, G_MTX_MODELVIEW,
                                         G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

                for (i = 0; i < ARRAY_COUNT(sGroundPositions360x_FIX); i++) {
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, sGroundPositions360x_FIX[i], 0.0f, sGroundPositions360z_FIX[i],
                                     MTXF_APPLY);
                    Matrix_Scale(gGfxMatrix, 1.5f, 1.0f, 1.0f, MTXF_APPLY);
                    // Matrix_SetGfxMtx(&gMasterDisp);
                    Matrix_ToMtx(gGfxMtx);
                    gEXMatrix(gMasterDisp++, gGfxMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW | G_EX_MTX_PREVIOUS,
                              &gIdentityMtx);
                    gGfxMtx++;
                    gSPDisplayList(gMasterDisp++, D_TR_6005880);
                    Matrix_Pop(&gGfxMatrix);
                }

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            }
            break;

        case LEVEL_AQUAS:
            RCP_SetupDL(&gMasterDisp, SETUPDL_20);
            sp1C0 = D_AQ_600AB10;
            gSPFogPosition(gMasterDisp++, gFogNear, gFogFar);

            if ((D_bg_8015F964 == 0) && ((gAqDrawMode == 0) || (gAqDrawMode == 2))) {

                gDPLoadTileTexture(gMasterDisp++, SEGMENTED_TO_VIRTUAL(D_AQ_600AB68), G_IM_FMT_RGBA, G_IM_SIZ_16b, 32,
                                   32);
                gDPSetTextureImage(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, SEGMENTED_TO_VIRTUAL(D_AQ_600AB68));
                temp_s0 = fabsf(Math_ModF(2.0f * (gPathTexScroll * 0.2133333f), 128.0f));
                temp_fv0 = Math_ModF((10000.0f - gPlayer[gPlayerNum].xPath) * 0.32f, 128.0f);
                gDPSetupTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, temp_fv0, temp_s0,
                             G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GROUND_AQ, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                // CENTER FAR
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -3000.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);
                // LEFT
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, -3000.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);
                // RIGHT
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, -3000.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);

                // CENTER
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 3000.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);
                // LEFT
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, 3000.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);
                Matrix_Pop(&gGfxMatrix);
                // RIGHT
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, 3000.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, sp1C0);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

                Matrix_Pop(&gGfxMatrix);
            }

            if ((D_bg_8015F964 != 0) || (gAqDrawMode == 0)) {
                gDPLoadTileTexture(gMasterDisp++, SEGMENTED_TO_VIRTUAL(D_AQ_602ACC0), G_IM_FMT_RGBA, G_IM_SIZ_16b, 32,
                                   32);
                gDPSetTextureImage(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, SEGMENTED_TO_VIRTUAL(D_AQ_602ACC0));
                temp_s0 = fabsf(Math_ModF(2.0f * (gPathTexScroll * 0.2133333f), 128.0f));
                temp_fv0 = Math_ModF((10000.0f - gPlayer[gPlayerNum].xPath) * 0.32f, 128.0f);
                gDPSetupTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, temp_fv0, temp_s0,
                             G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
                if (gAqDrawMode != 0) {
                    RCP_SetupDL(&gMasterDisp, SETUPDL_47);
                } else {
                    RCP_SetupDL(&gMasterDisp, SETUPDL_37);
                }

                if ((gPlayer[0].state_1C8 == PLAYERSTATE_1C8_LEVEL_INTRO) && (gPlayer[0].csState < 2)) {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                } else if (gPlayer[0].state_1C8 == PLAYERSTATE_1C8_LEVEL_COMPLETE) {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, D_bg_8015F974, D_bg_8015F978, D_bg_8015F97C,
                                    D_bg_8015F980);
                } else {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, (s32) D_AQ_801C4188);
                }

                // Render the original object in the middle TEST
                Matrix_Push(&gGfxMatrix);

                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GROUND_AQ_2, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                Matrix_Translate(gGfxMatrix, 0.0f, D_bg_8015F970, -9000.0f, MTXF_APPLY); // Center
                Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_AQ_602AC40);
                Matrix_Pop(&gGfxMatrix);
                // Render the extended object to the left
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -16000.0f, D_bg_8015F970, -9000.0f,
                                 MTXF_APPLY); // Left (-6000.0f on X-axis)
                Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_AQ_602AC40);
                Matrix_Pop(&gGfxMatrix);

                // Render the extended object to the right
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 16000.0f, D_bg_8015F970, -9000.0f,
                                 MTXF_APPLY); // Right (6000.0f on X-axis)
                Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_AQ_602AC40);
                Matrix_Pop(&gGfxMatrix);

                // Render the original object in the middle
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, D_bg_8015F970, -3000.0f, MTXF_APPLY); // Center
                Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_AQ_602AC40);
                Matrix_Pop(&gGfxMatrix);

                // Render the extended object to the left
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -16000.0f, D_bg_8015F970, -3000.0f,
                                 MTXF_APPLY); // Left (-6000.0f on X-axis)
                Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_AQ_602AC40);
                Matrix_Pop(&gGfxMatrix);

                // Render the extended object to the right
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 16000.0f, D_bg_8015F970, -3000.0f,
                                 MTXF_APPLY); // Right (6000.0f on X-axis)
                Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_AQ_602AC40);
                Matrix_Pop(&gGfxMatrix);

                // Render the original object in the middle (other row)
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, D_bg_8015F970, 3000.0f, MTXF_APPLY); // Center (other row)
                Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_AQ_602AC40);
                Matrix_Pop(&gGfxMatrix);

                // Render the extended object to the left (other row)
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -16000.0f, D_bg_8015F970, 3000.0f,
                                 MTXF_APPLY); // Left (-6000.0f on X-axis, other row)
                Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_AQ_602AC40);
                Matrix_Pop(&gGfxMatrix);

                // Render the extended object to the right (other row)
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 16000.0f, D_bg_8015F970, 3000.0f,
                                 MTXF_APPLY); // Right (6000.0f on X-axis, other row)
                Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_AQ_602AC40);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

                Matrix_Pop(&gGfxMatrix);
            }
            break;

        case LEVEL_FORTUNA:
        case LEVEL_KATINA:
        case LEVEL_BOLSE:
        case LEVEL_VENOM_2:
            if ((gGroundClipMode != 0) || (gCurrentLevel == LEVEL_BOLSE)) {
                RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            } else {
                RCP_SetupDL_20(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            }

            if ((gCurrentLevel == LEVEL_FORTUNA) && !(gPlayer[0].state_1C8 == PLAYERSTATE_1C8_ACTIVE)) {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GROUND_ON_RAILS, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);
            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposed(gMasterDisp++, TAG_GROUND_ALL_RANGE, G_EX_PUSH, G_MTX_MODELVIEW,
                                         G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                         G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);
            }

            for (i = 0; i < ARRAY_COUNT(sGroundPositions360x_FIX); i++) {
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, sGroundPositions360x_FIX[i], 0.0f, sGroundPositions360z_FIX[i],
                                 MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                if (gCurrentLevel == LEVEL_FORTUNA) {
                    gSPDisplayList(gMasterDisp++, D_FO_6001360);
                } else if (gCurrentLevel == LEVEL_KATINA) {
                    gSPDisplayList(gMasterDisp++, D_KA_6009250);
                } else if (gCurrentLevel == LEVEL_BOLSE) {
                    gSPDisplayList(gMasterDisp++, D_BO_600A810);
                } else if (gCurrentLevel == LEVEL_VENOM_2) {
                    gSPDisplayList(gMasterDisp++, D_VE2_6010700);
                }
                Matrix_Pop(&gGfxMatrix);
            }

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            break;

        case LEVEL_VERSUS:
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposed(gMasterDisp++, TAG_GROUND_ALL_RANGE, G_EX_PUSH, G_MTX_MODELVIEW,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

            if (gGroundClipMode != 0) {
                RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            } else {
                RCP_SetupDL_20(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            }

            for (i = 0; i < ARRAY_COUNT(sGroundPositions360x); i++) {
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, sGroundPositions360x[i], 0.0f, sGroundPositions360z[i], MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                if (gVersusStage == VS_STAGE_CORNERIA) {
                    gSPDisplayList(gMasterDisp++, D_versus_3018800);
                } else {
                    gSPDisplayList(gMasterDisp++, D_versus_30160A0);
                }
                Matrix_Pop(&gGfxMatrix);
            }

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            break;

        case LEVEL_SOLAR: // WIP
            RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GROUND_ALL_RANGE, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);
            // Render the object at the center (No mirroring)
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -2000.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, 3.0f, 2.0f, 3.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, (gGameFrameCount % 2) ? D_SO_60005B0 : D_SO_6002E60);
            Matrix_Pop(&gGfxMatrix);
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            // Render mirrored objects on both sides (Left: -1, Right: 1)
            // Render mirrored objects on both sides (Left: -1, Right: 1)
            for (s32 i = -1; i <= 1; i += 2) {
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, i * 4800.0f, 0.0f, -2000.0f,
                                 MTXF_APPLY);                            // Translate left (-) or right (+)
                Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY); // Mirror geometry (negative X scale)
                Matrix_SetGfxMtx(&gMasterDisp);

                // Disable backface culling for mirrored geometry
                gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);

                // Apply texture mirroring
                gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

                // Adjust UV mapping for the left side only
                if (i == -1) {
                    // Adjust UVs slightly for the left side to fix texture alignment
                    gDPSetTileSize(gMasterDisp++, G_TX_RENDERTILE, 4 << G_TEXTURE_IMAGE_FRAC, 0,
                                   (31 << G_TEXTURE_IMAGE_FRAC), (31 << G_TEXTURE_IMAGE_FRAC));
                } else {
                    // Standard UVs for the right side
                    gDPSetTileSize(gMasterDisp++, G_TX_RENDERTILE, 0 << G_TEXTURE_IMAGE_FRAC, 0,
                                   (31 << G_TEXTURE_IMAGE_FRAC), (31 << G_TEXTURE_IMAGE_FRAC));
                }

                gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5,
                           G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, 5, G_TX_NOLOD);

                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GROUND_ALL_RANGE, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                /*
                memcpy2(dynaFloor1, SEGMENTED_TO_VIRTUAL(D_SO_6002E60), 724 * sizeof(Gfx));
                memcpy2(dynaFloor2, SEGMENTED_TO_VIRTUAL(D_SO_60005B0), 724 * sizeof(Gfx));
                memcpy2(dynaFloor1Vtx, SEGMENTED_TO_VIRTUAL(D_SO_6001C50), 17 * 17 * sizeof(Vtx));
                memcpy2(dynaFloor2Vtx, SEGMENTED_TO_VIRTUAL(D_SO_6004500), 17 * 17 * sizeof(Vtx));
                */

                // Render the display list based on the current frame
                gSPDisplayList(gMasterDisp++, (gGameFrameCount % 2) ? D_SO_60005B0_copy : D_SO_6002E60_copy);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

                // Re-enable backface culling
                gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                Matrix_Pop(&gGfxMatrix);
            }

            break;

        case LEVEL_ZONESS:
            RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_GROUND_ALL_RANGE, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            // Center Further (main object)
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -3000.0f, MTXF_APPLY); // Center Further
            Matrix_Scale(gGfxMatrix, 3.0f, 2.0f, 3.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            Matrix_Pop(&gGfxMatrix);

            // Center Further - Left Mirror
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, -4800.0f, 0.0f, -3000.0f, MTXF_APPLY); // Left (-4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);            // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // Center Further - Right Mirror
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 4800.0f, 0.0f, -3000.0f, MTXF_APPLY); // Right (4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);           // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // Center Far (main object)
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -6000.0f, MTXF_APPLY); // Center Far
            Matrix_Scale(gGfxMatrix, 3.0f, 2.0f, 3.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            Matrix_Pop(&gGfxMatrix);

            // Center Far - Left Mirror
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, -4800.0f, 0.0f, -6000.0f, MTXF_APPLY); // Left (-4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);            // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // Center Far - Right Mirror
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 4800.0f, 0.0f, -6000.0f, MTXF_APPLY); // Right (4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);           // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // Center (Main object at center, no mirroring)
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -1500.0f, MTXF_APPLY); // Center
            Matrix_Scale(gGfxMatrix, 3.0f, 2.0f, 3.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            Matrix_Pop(&gGfxMatrix);

            // Center - Left Mirror
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, -4800.0f, 0.0f, -1500.0f, MTXF_APPLY); // Left (-4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);            // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // Center - Right Mirror
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 4800.0f, 0.0f, -1500.0f, MTXF_APPLY); // Right (4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);           // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // Center (Main object at center, no mirroring) Near
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 0.0f, MTXF_APPLY); // Center
            Matrix_Scale(gGfxMatrix, 3.0f, 2.0f, 3.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            Matrix_Pop(&gGfxMatrix);

            // Center - Left Mirror Near
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, -4800.0f, 0.0f, 0.0f, MTXF_APPLY); // Left (-4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);        // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // Center - Right Mirror Near
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 4800.0f, 0.0f, 0.0f, MTXF_APPLY); // Right (4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);       // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // Center (Main object at center, no mirroring) Nearer
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 1500.0f, MTXF_APPLY); // Center
            Matrix_Scale(gGfxMatrix, 3.0f, 2.0f, 3.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            Matrix_Pop(&gGfxMatrix);

            // Center - Left Mirror Nearer
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, -4800.0f, 0.0f, 1500.0f, MTXF_APPLY); // Left (-4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);           // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // Center - Right Mirror Nearer
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 4800.0f, 0.0f, 1500.0f, MTXF_APPLY); // Right (4800.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);          // Mirror by negative X scale
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK); // Disable backface culling
            gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0, G_TX_WRAP, 5, G_TX_NOLOD,
                       G_TX_WRAP, 5, G_TX_NOLOD);
            if ((gGameFrameCount % 2) != 0) {
                gSPDisplayList(gMasterDisp++, D_ZO_6008830);
            } else {
                gSPDisplayList(gMasterDisp++, D_ZO_600B0E0);
            }
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK); // Re-enable backface culling
            Matrix_Pop(&gGfxMatrix);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            break;
    }
    Matrix_Pop(&gGfxMatrix);
}
#endif

#if 1 // Play_UpdateDynaFloor  ********

RECOMP_PATCH void Play_UpdateDynaFloor(void) {
    Vec3f spC4;
    Vec3f spB8;
    Vtx* spB4;
    u16* spB0;
    s32 i;
    s32 spA8;
    f32* var_s3;
    f32* var_s5;
    f32* var_s4;
    f32* var_s0;
    f32* var_s1;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;

    gDynaFloorTimer++;

    switch (gCurrentLevel) {
        case LEVEL_SOLAR:
            if ((gGameFrameCount % 2) != 0) {
                spB4 = SEGMENTED_TO_VIRTUAL(D_SO_6001C50);
            } else {
                spB4 = SEGMENTED_TO_VIRTUAL(D_SO_6004500);
            }
            spB0 = SEGMENTED_TO_VIRTUAL(D_SO_6022760);
            spA8 = 16 - 1;
            sp90 = 70.0f;
            sp8C = 0.1f;
            sp88 = 2.2f;
            sp84 = 0.5f;
            break;

        case LEVEL_ZONESS:
            if ((gGameFrameCount % 2) != 0) {
                spB4 = SEGMENTED_TO_VIRTUAL(D_ZO_6009ED0);
            } else {
                spB4 = SEGMENTED_TO_VIRTUAL(D_ZO_600C780);
            }
            spB0 = SEGMENTED_TO_VIRTUAL(D_ZO_602AC50);
            spA8 = 8 - 1;
            sp90 = 40.0f;
            sp8C = 0.2f;
            sp88 = 1.0f;
            sp84 = 2.0f;
            break;
    }

    var_s3 = D_ctx_801782CC;
    var_s5 = D_ctx_801782D4;
    var_s4 = D_ctx_801782DC;
    var_s0 = D_ctx_801782E4;
    var_s1 = D_ctx_801782EC;

    for (i = 0; i < 17 * 17; i++, var_s3++, var_s5++, var_s4++, var_s0++, var_s1++, spB0++) {
        Math_SmoothStepToF(var_s3, *var_s5, sp8C, *var_s4, 0.0f);
        Math_SmoothStepToF(var_s4, 100.0f, 1.0f, sp84, 0.0f);

        if ((gDynaFloorTimer & spA8) == (i & spA8)) {
            *var_s5 = RAND_FLOAT(sp90);
            *var_s4 = 0.0f;
        }

        *var_s0 += *var_s1;
        *var_s1 -= 0.5f;
        if (*var_s0 < 0.0f) {
            *var_s1 = *var_s0 = 0.0f;
        }

        spB4[*spB0].n.ob[1] = (s16) *var_s3 + (s16) *var_s0;

        Matrix_RotateZ(gCalcMatrix, *var_s3 * sp88 * M_DTOR, MTXF_NEW);

        spC4.x = 120.0f;
        spC4.y = 0.0f;
        spC4.z = 0.0f;

        Matrix_MultVec3fNoTranslate(gCalcMatrix, &spC4, &spB8);

        if (gCurrentLevel == LEVEL_SOLAR) {
            spB4[*spB0].n.n[0] = spB8.x;
        }
        spB4[*spB0].n.n[1] = spB8.y;
        spB4[*spB0].n.n[2] = spB8.z;
    }

    Vtx* spB4_copy;

    switch (gCurrentLevel) {
        case LEVEL_SOLAR:
            if ((gGameFrameCount % 2) != 0) {
                spB4 = SEGMENTED_TO_VIRTUAL(D_SO_6001C50);
                spB4_copy = D_SO_6001C50_copy;
            } else {
                spB4 = SEGMENTED_TO_VIRTUAL(D_SO_6004500);
                spB4_copy = D_SO_6004500_copy;
            }
            spB0 = SEGMENTED_TO_VIRTUAL(D_SO_6022760);

            memcpy2(spB4_copy, spB4, 17 * 17 * sizeof(Vtx));

            for (i = 0; (i < 17 * 17); i++, spB0++) {
                spB4_copy[*spB0].n.n[0] *= -1.0f;
                // spB4_copy[*spB0].n.n[1] *= -1.0f;
                // spB4_copy[*spB0].n.n[2] *= -1.0f;
            }
            break;
            /*
            case LEVEL_ZONESS:
                if ((gGameFrameCount % 2) != 0) {
                    spB4 = SEGMENTED_TO_VIRTUAL(D_ZO_6009ED0);
                    spB4_copy = D_ZO_6009ED0_copy;
                } else {
                    spB4 = SEGMENTED_TO_VIRTUAL(D_ZO_600C780);
                    spB4_copy = D_ZO_600C780_copy;
                }
                spB0 = SEGMENTED_TO_VIRTUAL(D_ZO_602AC50);

                memcpy2(spB4_copy, spB4, 17 * 17 * sizeof(Vtx));

                for (i = 0; (i < 17 * 17); i++, spB0++) {
                    // spB4_copy[*spB0] = spB4[*spB0];
                    spB4_copy[*spB0].n.n[0] *= -1.0f; // Disable to fix mirror
                    // spB4_copy[*spB0].n.n[1] *= -1.0f;
                    // spB4_copy[*spB0].n.n[2] *= -1.0f;
                }
                break;
                */
    }
}
#endif

extern u16 D_BO_6008BB8[];
extern u16 D_BO_600AD80[];
extern Gfx D_BO_600BEC0[];

RECOMP_PATCH void Bolse_DrawDynamicGround(void) {
    Vec3f spDC = { 0.0f, 0.0f, 0.0f };
    Vec3f spD0;
    f32 rnd;
    f32 x;
    f32 z;

    gDPSetFogColor(gMasterDisp++, gFogRed, gFogGreen, gFogBlue, gFogAlpha);
    gSPFogPosition(gMasterDisp++, gFogNear, gFogFar);

    if (gBosses[1].obj.status == OBJ_ACTIVE) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_34);
        if ((gGameFrameCount % 2) != 0) {
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 128, 160, 255);
        } else {
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 192, 224, 255);
        }
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_33);
    }

    Matrix_Push(&gGfxMatrix);
    Rand_SetSeed(1, 29100, 9786);
    Matrix_Translate(gGfxMatrix, 0.0f, gCameraShakeY, 0.0f, MTXF_APPLY);
    Matrix_RotateY(gGfxMatrix, gBosses[0].obj.rot.y * M_DTOR, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 5.0f, 1.0f, 5.0f, MTXF_APPLY);

    for (z = -3200.0f; z <= 3200.0f; z += 800.0f) {
        for (x = -3200.0f; x <= 3200.0f; x += 800.0f) {
            rnd = Rand_ZeroOneSeeded();
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, x, 0.0f, z, MTXF_APPLY);
            Matrix_MultVec3f(gGfxMatrix, &spDC, &spD0);
            if ((spD0.z < 3000.0f) && (spD0.z > -13000.0f) && (fabsf(spD0.x) < (fabsf(spD0.z * 0.7f) + 9000.0f)) &&
                (fabsf(spD0.y) < (fabsf(spD0.z * 0.5f) + 4000.0f))) {
                if (rnd < 0.3f) {
                    gDPLoadTileTexture(gMasterDisp++, D_BO_6008BB8, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32);
                } else {
                    gDPLoadTileTexture(gMasterDisp++, D_BO_600AD80, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32);
                }
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_BO_600BEC0);
            }
            Matrix_Pop(&gGfxMatrix);
        }
    }
    Matrix_Pop(&gGfxMatrix);
}