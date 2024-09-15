#define RECOMP_PATCH __attribute__((section(".recomp_patch")))
#if 1
#define __sinf __sinf_recomp

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "gfx.h"
#include "sf64math.h"
#include "context.h"
#include "patches.h"
#include "sf64audio_external.h"

extern u8 gDrawBackdrop;
extern Gfx D_versus_302D4D0[];
extern Gfx D_CO_60059F0[];
extern Gfx D_versus_30146B0[];
extern Gfx D_versus_3011E40[];
extern Gfx D_FO_600D9F0[];
extern Gfx D_KA_600F1D0[];
extern Gfx D_VE2_600F670[];
extern Gfx D_VE1_60046F0[];
extern Gfx D_AQ_601C080[];
extern Gfx D_AQ_601AFF0[];
extern Gfx D_VE2_60038E0[];
extern Gfx D_ANDROSS_C039208[];
extern Gfx D_ZO_6013480[];
extern Gfx D_TI_6000A80[];
extern Gfx D_MA_6019220[];
extern Gfx D_SO_601E150[];
extern Gfx D_WZ_7001540[];
extern Gfx D_ME_600DDF0[];
extern Gfx D_SX_6029890[];
extern Gfx D_TR_6003760[];
extern Gfx D_A6_601BB40[];
extern Gfx D_FO_600B4B0[];
extern Gfx D_BO_600D190[];
extern Gfx D_SY_6001840[];
extern Gfx D_edisplay_800CFD80[];
extern f32 gAndrossUnkBrightness;
extern f32 gAndrossUnkAlpha;
extern f32 D_bg_8015F968;
extern f32 gWarpZoneBgAlpha;
extern f32 D_bg_8015F984;
extern Gfx aSzBackgroundDL[];
extern f32 __sinf(f32);
extern Gfx aMaProximityLightSidesDL[];
extern u16 D_CO_601B6C0[];
extern u16 D_CO_6028260[];
extern Gfx D_CO_601B640[];
extern Gfx D_CO_601EAA0[];
extern u16 D_VE1_6006750[];
extern Gfx D_VE1_60066D0[];
extern u16 D_MA_602DCB8[];
extern Gfx D_MA_60306D0[];
extern Gfx D_TR_6005880[];
extern Gfx D_AQ_600AB10[];
extern u16 D_AQ_600AB68[];
extern u16 D_AQ_602ACC0[];
extern f32 D_AQ_801C4188;
extern Gfx D_AQ_602AC40[];
extern Gfx D_FO_6001360[];
extern Gfx D_KA_6009250[];
extern Gfx D_BO_600A810[];
extern Gfx D_versus_3018800[];
extern Gfx D_versus_30160A0[];
extern Gfx D_SO_60005B0[];
extern Gfx D_SO_6002E60[];
extern Gfx D_ZO_6008830[];
extern Gfx D_ZO_600B0E0[];
extern u16 D_CO_6028A60[];

f32 sGroundPositions360x[4] = {
    5999.0f,
    -5999.0f,
    5999.0f,
    -5999.0f,
};
f32 sGroundPositions360z[4] = {
    5999.0f,
    5999.0f,
    -5999.0f,
    -5999.0f,
};

void Bolse_DrawDynamicGround(void);


#if 1 // Play_UpdateDynaFloor  ********

extern Vtx D_SO_6001C50[];
extern Vtx D_SO_6004500[];
extern u16 D_SO_6022760[];
extern Vtx D_ZO_6009ED0[];
extern Vtx D_ZO_600C780[];
extern u16 D_ZO_602AC50[];

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

        //spB4[*spB0].n.n[0] = spB8.x; // Disable to fix mirror
        spB4[*spB0].n.n[1] = spB8.y;
        spB4[*spB0].n.n[2] = spB8.z;
    }
}
#endif

#if 1 // Background_DrawBackdrop

RECOMP_PATCH void Background_DrawBackdrop(void) {
    f32 sp13C;
    f32 sp138;
    f32 sp134;
    f32 sp130;
    f32 sp12C;
    f32 sp128;
    s32 i;
    u8 levelType;
    s32 levelId;
    

    if (gDrawBackdrop == 0) {
        return;
    }

    levelType = gLevelType;
    if ((gCurrentLevel == LEVEL_VERSUS) && (gVersusStage == VS_STAGE_SECTOR_Z)) {
        levelType = LEVELTYPE_PLANET;
    }
    if (gCurrentLevel == LEVEL_TRAINING) {
        levelType = LEVELTYPE_SPACE;
    }
    levelId = gCurrentLevel;

    Matrix_Push(&gGfxMatrix);

    if (gFovYMode == 2) {
        Matrix_Scale(gGfxMatrix, 1.2f, 1.2f, 1.0f, MTXF_APPLY);
    }

    switch (levelType) {
        case LEVELTYPE_PLANET:
            RCP_SetupDL(&gMasterDisp, SETUPDL_17);
            switch (levelId) {
                case LEVEL_FORTUNA:
                case LEVEL_KATINA:
                case LEVEL_VENOM_2:
                case LEVEL_VERSUS:
                    sp134 = (gPlayer[gPlayerNum].camPitch * -6000.0f) - (gPlayer[gPlayerNum].cam.eye.y * 0.4f);
                    sp13C = Math_ModF(Math_RadToDeg(gPlayer[gPlayerNum].camYaw) * (-7280.0f / 360.0f) * 5.0f, 7280.0f);
                    Matrix_RotateZ(gGfxMatrix, gPlayer[gPlayerNum].camRoll * M_DTOR, MTXF_APPLY);

                    // Start by translating the matrix to the far left position
                    Matrix_Translate(gGfxMatrix, sp13C - 14560.0f, -2000.0f + sp134, -6000.0f, MTXF_APPLY);

                    if (gCurrentLevel == LEVEL_FORTUNA) {
                        Matrix_Translate(gGfxMatrix, 0.0f, -2000.0f, 0, MTXF_APPLY);
                    } else if (gCurrentLevel == LEVEL_KATINA) {
                        Matrix_Translate(gGfxMatrix, 0.0f, -2500.0f, 0, MTXF_APPLY);
                    }

                    Matrix_SetGfxMtx(&gMasterDisp);

                    // Render the textures across the screen (left to right)
                    for (int i = 0; i < 6; i++) {
                        switch (gCurrentLevel) {
                            case LEVEL_VERSUS:
                                if (gVersusStage == VS_STAGE_CORNERIA) {
                                    gSPDisplayList(gMasterDisp++, D_versus_302D4D0);
                                } else if (gVersusStage == VS_STAGE_KATINA) {
                                    gSPDisplayList(gMasterDisp++, D_versus_30146B0);
                                } else {
                                    gSPDisplayList(gMasterDisp++, D_versus_3011E40);
                                }
                                break;
                            case LEVEL_FORTUNA:
                                gSPDisplayList(gMasterDisp++, D_FO_600D9F0);
                                break;
                            case LEVEL_KATINA:
                                gSPDisplayList(gMasterDisp++, D_KA_600F1D0);
                                break;
                            case LEVEL_VENOM_2:
                                gSPDisplayList(gMasterDisp++, D_VE2_600F670);
                                break;
                        }

                        // Translate to the next position (move right by 7280.0f each time)
                        Matrix_Translate(gGfxMatrix, 7280.0f, 0.0f, 0.0f, MTXF_APPLY);
                        Matrix_SetGfxMtx(&gMasterDisp);
                    }
                    break;

                case LEVEL_CORNERIA:
                case LEVEL_VENOM_1: {
                    // Calculate vertical and horizontal offsets
                    f32 sp134 = (gPlayer[gPlayerNum].camPitch * -6000.0f) - (gPlayer[gPlayerNum].cam.eye.y * 0.6f);
                    f32 sp13C = Math_ModF(Math_RadToDeg(gPlayer[gPlayerNum].camYaw) * (-7280.0f / 360.0f) * 5.0f, 7280.0f);

                    // Apply camera roll and translate matrix to the starting position (far left)
                    Matrix_RotateZ(gGfxMatrix, gPlayer[gPlayerNum].camRoll * M_DTOR, MTXF_APPLY);
                    Matrix_Translate(gGfxMatrix, sp13C - 14560.0f, -2000.0f + sp134, -6000.0f, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);

                    // Render the textures across a wider range to cover the screen
                    for (int i = 0; i < 10; i++) {
                        switch ((s32) gCurrentLevel) {
                            case LEVEL_CORNERIA:
                                gSPDisplayList(gMasterDisp++, D_CO_60059F0);
                                break;
                            case LEVEL_VENOM_1:
                                gSPDisplayList(gMasterDisp++, D_VE1_60046F0);
                                break;
                        }

                        // Translate to the next position (move right by 7280.0f each time)
                        Matrix_Translate(gGfxMatrix, 7280.0f, 0.0f, 0.0f, MTXF_APPLY);
                        Matrix_SetGfxMtx(&gMasterDisp);
                    }

                    break;
                }

                case LEVEL_VENOM_ANDROSS: // WIP
                    if (gDrawBackdrop != 6) {

                        f32 aspectRatio = 1.777f; // Get screen aspect ratio (width / height)

                        if ((gDrawBackdrop == 2) || (gDrawBackdrop == 7)) {
                            Matrix_RotateZ(gGfxMatrix, gPlayer[gPlayerNum].camRoll * M_DTOR, MTXF_APPLY);
                            // Adjust Y translation based on aspect ratio
                            Matrix_Translate(gGfxMatrix, 0.0f, -4000.0f * aspectRatio, -7000.0f, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            gSPDisplayList(gMasterDisp++, D_VE2_600F670);
                        } else if ((gDrawBackdrop == 3) || (gDrawBackdrop == 4)) {
                            RCP_SetupDL(&gMasterDisp, SETUPDL_62);
                            if (gDrawBackdrop == 4) {
                                if ((gGameFrameCount & 8) == 0) {
                                    Math_SmoothStepToF(&gAndrossUnkBrightness, 0.0f, 1.0f, 30.0f, 0);
                                } else {
                                    Math_SmoothStepToF(&gAndrossUnkBrightness, 120.0f, 1.0f, 30.0f, 0);
                                }
                            } else {
                                gAndrossUnkBrightness = 255.0f;
                            }
                            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, (s32) gAndrossUnkBrightness,
                                            (s32) gAndrossUnkBrightness, (s32) gAndrossUnkAlpha);

                            sp134 = (gPlayer[gPlayerNum].camPitch * -6000.0f) - (gPlayer[gPlayerNum].cam.eye.y * 0.4f);
                            sp13C = Math_ModF(Math_RadToDeg(gPlayer[gPlayerNum].camYaw) * (-7280.0f / 360.0f) * 5.0f,
                                              7280.0f);
                            Matrix_RotateZ(gGfxMatrix, gPlayer[gPlayerNum].camRoll * M_DTOR, MTXF_APPLY);
                            // Adjust X and Y translations based on aspect ratio
                            Matrix_Translate(gGfxMatrix, sp13C * aspectRatio, (-2000.0f + sp134) * aspectRatio,
                                             -6000.0f, MTXF_APPLY);
                            Matrix_Translate(gGfxMatrix, 0.0f, -2500.0f * aspectRatio, 0.0f, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            gSPDisplayList(gMasterDisp++, D_VE2_60038E0);
                            // Adjust X translation based on aspect ratio
                            Matrix_Translate(gGfxMatrix, 7280.0f * aspectRatio, 0.0f, 0.0f, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            gSPDisplayList(gMasterDisp++, D_VE2_60038E0);
                        } else {
                            RCP_SetupDL(&gMasterDisp, SETUPDL_62);
                            if (gDrawBackdrop == 5) {
                                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 64);
                            } else {
                                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 0, 255, 128, (s32) gAndrossUnkAlpha);
                            }
                            // Adjust translation and scaling based on aspect ratio
                            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -290.0f, MTXF_APPLY);
                            Matrix_Push(&gGfxMatrix);
                            Matrix_Scale(gGfxMatrix, 11.0f * aspectRatio, 11.0f, 1.0f, MTXF_APPLY);
                            Matrix_RotateZ(gGfxMatrix, (gPlayer[0].camRoll + (gGameFrameCount * 1.5f)) * M_DTOR,
                                           MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            gSPDisplayList(gMasterDisp++, D_ANDROSS_C039208);
                            Matrix_Pop(&gGfxMatrix);
                            if (gDrawBackdrop != 5) {
                                Matrix_Push(&gGfxMatrix);
                                Matrix_Scale(gGfxMatrix, 10.0f * aspectRatio, 10.0f, 1.0f, MTXF_APPLY);
                                Matrix_RotateZ(gGfxMatrix, (gPlayer[0].camRoll + (gGameFrameCount * -1.3f)) * M_DTOR,
                                               MTXF_APPLY);
                                Matrix_SetGfxMtx(&gMasterDisp);
                                gSPDisplayList(gMasterDisp++, D_ANDROSS_C039208);
                                Matrix_Pop(&gGfxMatrix);
                            }
                        }
                    }
                    break;

                case LEVEL_AQUAS:
                    if (gPlayer[0].state_1C8 == PLAYERSTATE_1C8_LEVEL_INTRO) {
                        sp13C = Math_RadToDeg(gPlayer[gPlayerNum].camYaw) - gPlayer[gPlayerNum].yRot_114;
                        sp134 = (gPlayer[gPlayerNum].camPitch * -7000.0f) - (gPlayer[gPlayerNum].cam.eye.y * 0.6f);
                        sp13C = Math_ModF(sp13C * -40.44444f * 2.0f, 7280.0f); // close to 7280.0f / 180.0f
                        RCP_SetupDL_17();
                        Matrix_RotateZ(gGfxMatrix, gPlayer[gPlayerNum].camRoll * M_DTOR, MTXF_APPLY);
                        Matrix_Scale(gGfxMatrix, 1.5f, 1.0f, 1.0f, MTXF_APPLY);

                        // Start by translating the matrix to the far left position
                        Matrix_Translate(gGfxMatrix, sp13C - 14560.0f, sp134, -7000.0f, MTXF_APPLY);
                        Matrix_SetGfxMtx(&gMasterDisp);

                        // Render the textures across the screen (left to right)
                        for (int i = 0; i < 5; i++) {
                            if (gPlayer[0].state_1C8 == PLAYERSTATE_1C8_LEVEL_INTRO) {
                                gSPDisplayList(gMasterDisp++, D_AQ_601AFF0);
                            } else {
                                gSPDisplayList(gMasterDisp++, D_AQ_601C080);
                            }

                            // Translate to the next position (move right by 7280.0f each time)
                            Matrix_Translate(gGfxMatrix, 7280.0f, 0.0f, 0.0f, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                        }
                        Matrix_Pop(&gGfxMatrix);
                    }
                    break;

                case LEVEL_SOLAR:
                case LEVEL_ZONESS:
                case LEVEL_MACBETH:
                case LEVEL_TITANIA:
                    sp12C = Math_RadToDeg(gPlayer[gPlayerNum].camYaw) - gPlayer[gPlayerNum].yRot_114;
                    sp134 = (gPlayer[gPlayerNum].camPitch * -7000.0f) - (gPlayer[gPlayerNum].cam.eye.y * 0.6f);
                    sp13C = sp12C * -40.44444f * 2.0f; // close to 7280.0f / 180.0f

                    if ((gCurrentLevel == LEVEL_TITANIA) && (gPlayer[0].state_1C8 == PLAYERSTATE_1C8_LEVEL_INTRO) &&
                        (gPlayer[0].csState < 3)) {
                        D_bg_8015F968 += __sinf(gPlayer[0].camYaw) * 20.0f;
                        sp13C += D_bg_8015F968;
                    }
                    if ((gCurrentLevel == LEVEL_SOLAR) && (gPlayer[0].state_1C8 == PLAYERSTATE_1C8_LEVEL_INTRO) &&
                        (gPlayer[0].csState >= 2) && (gPlayer[0].cam.eye.z <= -1900.0f)) {
                        D_bg_8015F968 = __sinf(gPlayer[0].camPitch) * 7000.0f;
                        sp134 -= fabsf(D_bg_8015F968);
                    }

                    sp13C = Math_ModF(sp13C, 7280.0f);
                    RCP_SetupDL_17();
                    Matrix_RotateZ(gGfxMatrix, gPlayer[gPlayerNum].camRoll * M_DTOR, MTXF_APPLY);
                    Matrix_Scale(gGfxMatrix, 1.5f, 1.0f, 1.0f, MTXF_APPLY);

                    if ((gCurrentLevel == LEVEL_TITANIA) || (gCurrentLevel == LEVEL_ZONESS)) {
                        Matrix_Translate(gGfxMatrix, sp13C - 14560.0f, -3000.0f + sp134, -7000.0f, MTXF_APPLY);
                    } else if (gCurrentLevel == LEVEL_SOLAR) {
                        Matrix_Translate(gGfxMatrix, sp13C - 14560.0f, -3500.0f + sp134, -7000.0f, MTXF_APPLY);
                    } else if (gCurrentLevel == LEVEL_MACBETH) {
                        Matrix_Translate(gGfxMatrix, sp13C - 14560.0f, -4000.0f + sp134, -7000.0f, MTXF_APPLY);
                    }
                    Matrix_SetGfxMtx(&gMasterDisp);

                    // Render the textures across a wider range to cover the screen
                    for (int i = 0; i < 5; i++) {
                        if (gCurrentLevel == LEVEL_TITANIA) {
                            gSPDisplayList(gMasterDisp++, D_TI_6000A80);
                        } else if (gCurrentLevel == LEVEL_MACBETH) {
                            gSPDisplayList(gMasterDisp++, D_MA_6019220);
                        } else if (gCurrentLevel == LEVEL_ZONESS) {
                            gSPDisplayList(gMasterDisp++, D_ZO_6013480);
                        } else if (gCurrentLevel == LEVEL_SOLAR) {
                            gSPDisplayList(gMasterDisp++, D_SO_601E150);
                        }

                        // Move the matrix to the right by 7280.0f each time to draw the next texture
                        Matrix_Translate(gGfxMatrix, 7280.0f, 0.0f, 0.0f, MTXF_APPLY);
                        Matrix_SetGfxMtx(&gMasterDisp);
                    }
                    break;
            }
            break;

        case LEVELTYPE_SPACE: // WIP Needed (space levels have textures that wrap around the screen)
            if (gPlayer[0].state_1C8 != PLAYERSTATE_1C8_ENTER_WARP_ZONE) {
                Matrix_Push(&gGfxMatrix);
                sp12C = Math_RadToDeg(gPlayer[0].camYaw);
                sp130 = Math_RadToDeg(gPlayer[0].camPitch);
                if (((sp12C < 45.0f) || (sp12C > 315.0f)) && ((sp130 < 40.0f) || (sp130 > 325.0f))) {
                    RCP_SetupDL_36();
                    sp138 = gStarfieldX;
                    sp134 = gStarfieldY;
                    if (((gCurrentLevel == LEVEL_SECTOR_X) || (gCurrentLevel == LEVEL_METEO)) && (gLevelPhase == 1)) {
                        levelId = LEVEL_WARP_ZONE;
                    }
                    if (levelId == LEVEL_SECTOR_X) {
                        sp138 = Math_ModF(sp138 + 60.0f, 480.0f);
                        sp134 = Math_ModF(sp134 + 360.0f - 40.0f, 360.0f);
                    } else if (levelId == LEVEL_TRAINING) {
                        sp138 = Math_ModF(sp138 - 30.0f, 480.0f);
                        sp134 = Math_ModF(sp134 + 360.0f - 40.0f, 360.0f);
                    } else if ((levelId == LEVEL_SECTOR_Y) && (gLevelMode == LEVELMODE_ON_RAILS)) {
                        sp138 = Math_ModF(sp138 + 480.0f - 60.0f, 480.0f);
                        sp134 = Math_ModF(sp134, 360.0f);
                    } else if (levelId == LEVEL_FORTUNA) {
                        sp138 = Math_ModF(sp138 - 34.5f, 480.0f);
                        sp134 = Math_ModF(sp134 + 19.0f, 360.0f);
                    } else if (levelId == LEVEL_BOLSE) {
                        if ((gPlayer[0].state_1C8 != PLAYERSTATE_1C8_LEVEL_COMPLETE) || (gPlayer[0].csState < 10)) {
                            sp134 = Math_ModF(sp134 + 360.0f - 100.0f, 360.0f);
                        }
                    } else {
                        sp138 = Math_ModF(sp138, 480.0f);
                        sp134 = Math_ModF(sp134, 360.0f);
                    }

                    if ((sp12C < 180.0f) && (sp138 > 380.0f)) {
                        sp138 = -(480.0f - sp138);
                    }
                    if ((sp130 > 180.0f) && (sp134 > 280.0f)) {
                        sp134 = -(360.0f - sp134);
                    }

                    Matrix_RotateZ(gGfxMatrix, gStarfieldRoll, MTXF_APPLY);

                    switch (levelId) {

                        case LEVEL_WARP_ZONE:
                            if ((s32) gWarpZoneBgAlpha != 0) {
                                RCP_SetupDL_62();
                                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, (s32) gWarpZoneBgAlpha);
                                Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f), -290.0f, MTXF_APPLY);
                                Matrix_Scale(gGfxMatrix, 1.7f, 1.7f, 1.0f, MTXF_APPLY);
                                Matrix_Push(&gGfxMatrix);
                                Matrix_RotateZ(gGfxMatrix, -(f32) gGameFrameCount * 10.0f * M_DTOR, MTXF_APPLY);
                                Matrix_Scale(gGfxMatrix, 1.07f, 0.93f, 1.0f, MTXF_APPLY);
                                Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 10.0f * M_DTOR, MTXF_APPLY);
                                Matrix_Scale(gGfxMatrix, 1.07f, 0.93f, 1.0f, MTXF_APPLY);
                                Matrix_SetGfxMtx(&gMasterDisp);
                                gSPDisplayList(gMasterDisp++, D_WZ_7001540);
                                Matrix_Pop(&gGfxMatrix);
                            }

                            break;
                        case LEVEL_METEO:
                            if ((gPlayer[0].state_1C8 == PLAYERSTATE_1C8_LEVEL_COMPLETE) && (gCsFrameCount > 260)) {
                                Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f) - 30.0f, -290.0f,
                                                 MTXF_APPLY);
                                Matrix_Scale(gGfxMatrix, 0.5f, 0.5f, 1.0f, MTXF_APPLY);
                                Matrix_SetGfxMtx(&gMasterDisp);
                                gSPDisplayList(gMasterDisp++, D_ME_600DDF0);
                            } else if (gPathProgress > 185668.0f) {
                                Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f) - 130.0f, -290.0f,
                                                 MTXF_APPLY);
                                Matrix_Scale(gGfxMatrix, 0.4f, 0.4f, 1.0f, MTXF_APPLY);
                                Matrix_SetGfxMtx(&gMasterDisp);
                                gSPDisplayList(gMasterDisp++, D_ME_600DDF0);
                            }
                            break;

                        case LEVEL_SECTOR_X:
                            if (gSceneSetup == 0) {
                                Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f), -290.0f, MTXF_APPLY);
                                Matrix_Scale(gGfxMatrix, 3.0f, 3.0f, 1.0f, MTXF_APPLY);
                                Matrix_SetGfxMtx(&gMasterDisp);
                                RCP_SetupDL_62();
                                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 192);
                                gSPDisplayList(gMasterDisp++, D_SX_6029890);
                            }
                            break;

                        case LEVEL_TRAINING:
                            Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f), -290.0f, MTXF_APPLY);
                            Matrix_Scale(gGfxMatrix, 0.2f, 0.2f, 1.0f, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            RCP_SetupDL_62();
                            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                            gSPDisplayList(gMasterDisp++, D_TR_6003760);
                            break;

                        case LEVEL_AREA_6:
                        case LEVEL_UNK_4:
                            sp128 = (gPathProgress * 0.00004f) + 0.5f;
                            if (sp128 > 3.5f) {
                                sp128 = 3.5f;
                            }
                            if (gPlayer[0].state_1C8 == PLAYERSTATE_1C8_LEVEL_COMPLETE) {
                                sp128 = D_bg_8015F984;
                                if (sp128 > 3.5f) {
                                    sp128 = 3.5f;
                                }
                            }
                            sp128 = sp128;
                            Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f), -290.0f, MTXF_APPLY);
                            Matrix_Scale(gGfxMatrix, sp128 * 0.75, sp128 * 0.75f, 1.0f, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            gSPDisplayList(gMasterDisp++, D_A6_601BB40);
                            break;

                        case LEVEL_FORTUNA:
                            sp128 = 1.5f;
                            if ((gCsFrameCount > 400) && (gMissionStatus == MISSION_COMPLETE)) {
                                sp128 = 0.75f;
                            }
                            Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f), -290.0f, MTXF_APPLY);
                            Matrix_Scale(gGfxMatrix, sp128, sp128, sp128, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            gSPDisplayList(gMasterDisp++, D_FO_600B4B0);
                            break;

                        case LEVEL_BOLSE:
                            sp128 = 1.0f;
                            if ((gCsFrameCount > 500) && (gPlayer[0].state_1C8 == PLAYERSTATE_1C8_LEVEL_COMPLETE)) {
                                sp128 = 1.3f;
                            }
                            Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f), -290.0f, MTXF_APPLY);
                            Matrix_Scale(gGfxMatrix, sp128, sp128, sp128, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            gSPDisplayList(gMasterDisp++, D_BO_600D190);
                            break;

                        case LEVEL_SECTOR_Z:
                            Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f), -290.0f, MTXF_APPLY);
                            Matrix_Scale(gGfxMatrix, 0.5f, 0.5f, 0.5f, MTXF_APPLY);
                            Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            gSPDisplayList(gMasterDisp++, aSzBackgroundDL);
                            break;
                            
                        case LEVEL_SECTOR_Y:
                            Matrix_Translate(gGfxMatrix, sp138 - 120.0f, -(sp134 - 120.0f), -290.0f, MTXF_APPLY);
                            Matrix_Scale(gGfxMatrix, 0.4f, 0.4f, 1.0f, MTXF_APPLY);
                            Matrix_SetGfxMtx(&gMasterDisp);
                            RCP_SetupDL_62();
                            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 192);
                            gSPDisplayList(gMasterDisp++, D_SY_6001840);
                            break;
                    }
                }
                Matrix_Pop(&gGfxMatrix);
            }

            if (gStarWarpDistortion > 0.0f) {
                f32* xStar = gStarOffsetsX;
                f32* yStar = gStarOffsetsY;
                f32 zRot;

                RCP_SetupDL_14();
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 128, 128, 255, 255);

                zRot = 0.0f;
                for (i = 0; i < 300; i++, xStar++, yStar++) {
                    *xStar = RAND_FLOAT_SEEDED(480.0f) - 80.0f;
                    *yStar = RAND_FLOAT_SEEDED(360.0f) - 60.0f;
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, (*xStar - 160.0f) * 10.0f, (*yStar - 120.0f) * 10.0f, -5000.0f,
                                     MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, zRot, MTXF_APPLY);
                    Matrix_Scale(gGfxMatrix, 10.0f, 1.0f, -gStarWarpDistortion, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, D_edisplay_800CFD80);
                    Matrix_Pop(&gGfxMatrix);
                    zRot += M_PI / 4;
                }
            }
            break;
    }
    Matrix_Pop(&gGfxMatrix);
}
#endif

#if 1 // Play_InitLevel

#define MEM_ARRAY_ALLOCATE(arr, count) ((arr) = Memory_Allocate((count) * sizeof(*(arr))))

extern f32 D_Andross_801A7F58;
extern f32 D_Andross_801A7F60;
extern f32 D_Andross_801A7F68;
extern f32 D_Andross_801A7F70;
extern f32 D_Andross_801A7F78;
extern Vtx D_SO_6001C50[];
extern Vtx D_ZO_6009ED0[];
extern Vtx D_SO_6004500[];
extern Vtx D_ZO_600C780[];

void Macbeth_InitLevel(void);
void Titania_80188F30(void);
void Aquas_InitLevel(void);

RECOMP_PATCH void Play_InitLevel(void) {
    Vtx* mesh;
    u8* ptr;
    s32 i;
    f32* fptr;

    switch (gCurrentLevel) {
        case LEVEL_TRAINING:
            AUDIO_SET_SPEC(SFXCHAN_0, AUDIOSPEC_28);
            gTeamLowHealthMsgTimer = -1;
            break;
        case LEVEL_VENOM_1:
            AUDIO_SET_SPEC(SFXCHAN_0, AUDIOSPEC_6);
            break;
        case LEVEL_VENOM_2:
            AUDIO_SET_SPEC(SFXCHAN_0, AUDIOSPEC_6);
            break;
        case LEVEL_VENOM_ANDROSS:
            AUDIO_SET_SPEC(SFXCHAN_0, AUDIOSPEC_15);
            Audio_SetEnvSfxReverb(0);
            gTeamLowHealthMsgTimer = -1;
            break;
    }

    gUseDynaFloor = 0;

    switch (gCurrentLevel) {
        case LEVEL_SECTOR_Z:
            gGreatFoxIntact = true;
            break;

        case LEVEL_METEO:
        case LEVEL_SECTOR_X:
            if (gLevelPhase == 1) {
                gFogRed = 178;
                gFogGreen = 190;
                gFogBlue = 90;
                gLight1R = 200;
                gLight1G = 200;
                gLight1B = 120;
                gAmbientR = 0;
                gAmbientG = 50;
                gAmbientB = 100;
            }
            break;

        case LEVEL_VENOM_ANDROSS:
            gDrawGround = false;
            gDrawBackdrop = 6;
            D_Andross_801A7F58 = D_Andross_801A7F60 = D_Andross_801A7F68 = D_Andross_801A7F70 = D_Andross_801A7F78 =
                0.0f;
            break;

        case LEVEL_AQUAS:
            gVIsPerFrame = 3;
            D_bg_8015F970 = 1600.0f;
            D_AQ_801C4188 = 128.0f;
            Aquas_InitLevel();
            break;

        case LEVEL_TITANIA:
            Titania_80188F30();
            gTeamShields[TEAM_ID_SLIPPY] = -2;
            break;

        case LEVEL_MACBETH:
            Macbeth_InitLevel();
            break;

        case LEVEL_ZONESS:
            MEM_ARRAY_ALLOCATE(gZoDodoraPosRots, 200);
            ptr = (u8*) gZoDodoraPosRots;
            for (i = 0; i < 200 * sizeof(*gZoDodoraPosRots); i++, ptr++) {
                *ptr = 0;
            }
            gZoDodoraWaypointCount = 0;
            /* fallthrough */
        case LEVEL_SOLAR:
            gUseDynaFloor = true;
            gGroundHeight = -20000.0f;
            fptr = MEM_ARRAY_ALLOCATE(D_ctx_801782CC, 17 * 17);
            for (i = 0; i < 17 * 17; i++, fptr++) {
                *fptr = 0.0f;
            }
            fptr = MEM_ARRAY_ALLOCATE(D_ctx_801782D4, 17 * 17);
            for (i = 0; i < 17 * 17; i++, fptr++) {
                *fptr = 0.0f;
            }
            fptr = MEM_ARRAY_ALLOCATE(D_ctx_801782DC, 17 * 17);
            for (i = 0; i < 17 * 17; i++, fptr++) {
                *fptr = 0.0f;
            }
            fptr = MEM_ARRAY_ALLOCATE(D_ctx_801782E4, 17 * 17);
            for (i = 0; i < 17 * 17; i++, fptr++) {
                *fptr = 0.0f;
            }
            fptr = MEM_ARRAY_ALLOCATE(D_ctx_801782EC, 17 * 17);
            for (i = 0; i < 17 * 17; i++, fptr++) {
                *fptr = 0.0f;
            }

            switch (gCurrentLevel) {
                case LEVEL_SOLAR:
                    mesh = SEGMENTED_TO_VIRTUAL(D_SO_6001C50);
                    break;
                case LEVEL_ZONESS:
                    mesh = SEGMENTED_TO_VIRTUAL(D_ZO_6009ED0);
                    break;
            }

            for (i = 0; i < 17 * 17; i++, mesh++) {
                if (mesh->v.ob[0] == 800 * 2) {
                    mesh->v.ob[0] = 1400 * 2;
                }
                if (mesh->v.ob[0] == -800 * 2) {
                    mesh->v.ob[0] = -1400 * 2;
                }
                if (mesh->v.ob[2] == -800 * 2) {
                    mesh->v.ob[2] = -1400 * 2;
                }
            }

            switch (gCurrentLevel) {
                case LEVEL_SOLAR:
                    mesh = SEGMENTED_TO_VIRTUAL(D_SO_6004500);
                    break;
                case LEVEL_ZONESS:
                    mesh = SEGMENTED_TO_VIRTUAL(D_ZO_600C780);
                    break;
            }

            for (i = 0; i < 17 * 17; i++, mesh++) {
                if (mesh->v.ob[0] == 800 * 2) {
                    mesh->v.ob[0] = 1400 * 2;
                }
                if (mesh->v.ob[0] == -800 * 2) {
                    mesh->v.ob[0] = -1400 * 2;
                }
                if (mesh->v.ob[2] == -800 * 2) {
                    mesh->v.ob[2] = -1400 * 2;
                }
            }
            break;
    }
}
#endif

#if 1 // Background_DrawGround
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
                Matrix_Pop(&gGfxMatrix);
            } else {
                gGroundSurface = SURFACE_GRASS;
                gBgColor = 0x845; // 8, 8, 32
                for (i = 0; i < ARRAY_COUNT(sGroundPositions360x); i++) {
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, sGroundPositions360x[i], 0.0f, sGroundPositions360z[i], MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, D_CO_601EAA0);
                    Matrix_Pop(&gGfxMatrix);
                }
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
            Matrix_Pop(&gGfxMatrix);
            break;

        case LEVEL_TRAINING:
            RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            if (gLevelMode == LEVELMODE_ON_RAILS) {
                if (gPathTexScroll > 290.0f) {
                    gPathTexScroll -= 290.0f;
                }

                // CENTER FAR
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -3000.0f + gPathTexScroll, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_TR_6005880);
                Matrix_Pop(&gGfxMatrix);
                // LEFT
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, -3000.0f + gPathTexScroll, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_TR_6005880);
                Matrix_Pop(&gGfxMatrix);
                // RIGHT
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, -3000.0f + gPathTexScroll, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_TR_6005880);
                Matrix_Pop(&gGfxMatrix);

                // CENTER
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -9000.0f + gPathTexScroll, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_TR_6005880);
                Matrix_Pop(&gGfxMatrix);
                // LEFT
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -8000.0f, 0.0f, -9000.0f + gPathTexScroll, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_TR_6005880);
                Matrix_Pop(&gGfxMatrix);
                // RIGHT
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 8000.0f, 0.0f, -9000.0f + gPathTexScroll, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_TR_6005880);
                Matrix_Pop(&gGfxMatrix);

                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 3000.0f + gPathTexScroll, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.5f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_TR_6005880);
                Matrix_Pop(&gGfxMatrix);

            } else {
                for (i = 0; i < ARRAY_COUNT(sGroundPositions360x); i++) {
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, sGroundPositions360x[i], 0.0f, sGroundPositions360z[i], MTXF_APPLY);
                    Matrix_Scale(gGfxMatrix, 1.5f, 1.0f, 1.0f, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, D_TR_6005880);
                    Matrix_Pop(&gGfxMatrix);
                }
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

            for (i = 0; i < ARRAY_COUNT(sGroundPositions360x); i++) {
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, sGroundPositions360x[i], 0.0f, sGroundPositions360z[i], MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                if (gCurrentLevel == LEVEL_FORTUNA) {
                    gSPDisplayList(gMasterDisp++, D_FO_6001360);
                } else if (gCurrentLevel == LEVEL_KATINA) {
                    gSPDisplayList(gMasterDisp++, D_KA_6009250);
                } else if (gCurrentLevel == LEVEL_BOLSE) {
                    gSPDisplayList(gMasterDisp++, D_BO_600A810);
                } else if (gCurrentLevel == LEVEL_VENOM_2) {
                    gSPDisplayList(gMasterDisp++, aMaProximityLightSidesDL);
                }
                Matrix_Pop(&gGfxMatrix);
            }
            break;

        case LEVEL_VERSUS:
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
            break;

case LEVEL_SOLAR: // WIP
    RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

    // Render the object at the center (No mirroring)
    Matrix_Push(&gGfxMatrix);
    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -2000.0f, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 3.0f, 2.0f, 3.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, (gGameFrameCount % 2) ? D_SO_60005B0 : D_SO_6002E60);
    Matrix_Pop(&gGfxMatrix);

    // Render mirrored objects on both sides (Left: -1, Right: 1)
    // Render mirrored objects on both sides (Left: -1, Right: 1)
for (s32 i = -1; i <= 1; i += 2) {
    Matrix_Push(&gGfxMatrix);
    Matrix_Translate(gGfxMatrix, i * 4800.0f, 0.0f, -2000.0f, MTXF_APPLY); // Translate left (-) or right (+)
    Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);               // Mirror geometry (negative X scale)
    Matrix_SetGfxMtx(&gMasterDisp);

    // Disable backface culling for mirrored geometry
    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);

    // Apply texture mirroring
    gSPTexture(gMasterDisp++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    // Adjust UV mapping for the left side only
    if (i == -1) {
        // Adjust UVs slightly for the left side to fix texture alignment
        gDPSetTileSize(gMasterDisp++, G_TX_RENDERTILE, 4 << G_TEXTURE_IMAGE_FRAC, 0, (31 << G_TEXTURE_IMAGE_FRAC), (31 << G_TEXTURE_IMAGE_FRAC));
    } else {
        // Standard UVs for the right side
        gDPSetTileSize(gMasterDisp++, G_TX_RENDERTILE, 0 << G_TEXTURE_IMAGE_FRAC, 0, (31 << G_TEXTURE_IMAGE_FRAC), (31 << G_TEXTURE_IMAGE_FRAC));
    }

    gDPSetTile(gMasterDisp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_RENDERTILE, 0,
               G_TX_WRAP, 5, G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, 5, G_TX_NOLOD);

    // Render the display list based on the current frame
    gSPDisplayList(gMasterDisp++, (gGameFrameCount % 2) ? D_SO_60005B0 : D_SO_6002E60);

    // Re-enable backface culling
    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
    Matrix_Pop(&gGfxMatrix);
}

    break;

        case LEVEL_ZONESS:
            RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

            // Render the object at the center (no mirroring needed)
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

            // Render the object to the left (mirror the texture horizontally)
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, -4800.0f, 0.0f, -1500.0f, MTXF_APPLY); // Left (-8000.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);            // Negative X scale to mirror geometry
            Matrix_SetGfxMtx(&gMasterDisp);

            // Disable backface culling for the mirrored object
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);

            // Render the texture
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

            // Render the object to the right (mirror the texture horizontally)
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 4800.0f, 0.0f, -1500.0f, MTXF_APPLY); // Right (8000.0f on X-axis)
            Matrix_Scale(gGfxMatrix, -3.0f, 2.0f, 3.0f, MTXF_APPLY);           // Negative X scale to mirror geometry
            Matrix_SetGfxMtx(&gMasterDisp);

            // Disable backface culling for the mirrored object
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);

            // Render the texture
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

            break;
    }
    Matrix_Pop(&gGfxMatrix);
}

#endif

#endif