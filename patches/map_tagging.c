#include "patches.h"
#include "fox_map.h"

extern PlanetId sCurrentPlanetId;
extern Planet sPlanets[15];
extern s32 sMapState;
extern s32 D_menu_801CD95C;
extern Vec3f sPlanetPositions[15];
extern f32 D_menu_801CEA18[15];
extern f32 D_menu_801CEEBC;
extern f32 D_menu_801CEEC0;
extern s32 D_menu_801CEB58[3][10];
extern s32 D_menu_801CEBD0[3][10];
extern f32 D_menu_801CEC48[3][10];
extern f32 D_menu_801CECC0[3][10];
extern f32 D_menu_801CED38[3][10];
extern f32 D_menu_801CEDB0[3][10];
extern f32 D_menu_801CEE28[3][10];
extern bool sPlanetExplosions[EXPLOSIONS_MAX];
extern Matrix D_menu_801CDA60[15];
extern f32 D_menu_801CDA10;
extern f32 D_menu_801CDA0C;
extern ObjPosition sMapArea6Ships[4];
extern s32 D_menu_801CD97C;
extern f32 sCursorYpos;
extern f32 D_menu_801CEAB0;
extern f32 D_menu_801CEAAC;
extern s32 D_menu_801CEEA8;
extern Vec3f D_menu_801CEEB0;
extern f32 D_menu_801AFFF4;
extern f32 D_menu_801AFFFC;
extern Matrix D_menu_801CDE20[15];

void Map_PathPlanetMedal_Draw(f32 x, f32 y, f32 z);
void Map_SolarRays_Draw(PlanetId);
void Map_VenomCloud2_Draw(PlanetId planetId);
void Map_PlanetShadow_Draw(PlanetId planetId);
void Map_PlanetMedal_Draw(PlanetId planetId);
void Map_PlanetExplosions_Draw(s32, PlanetExplosions);
void Map_PlanetCleared_Draw(PlanetId planetId);
void Map_PlanetAnim2(PlanetId planetId);
void Map_PlanetCleared2_Draw(PlanetId planetId);
s32 Map_CheckPlanetMedal(PlanetId planetId);
void Map_Prologue_Draw(void);
void Map_LylatCard_Draw(void);
void Map_GameOver_Draw(void);
void Map_RestartLevelLifeDown_Draw(void);
void Map_801A9DE8(void);
void Map_801A9A8C(void);
void Map_Wipe_Draw(void);
void Map_8019E800(void);
void Map_Setup(void);
void Map_Setup_Play(void);
void Map_Setup_GameOver(void);
void Map_Setup_Menu(void);
void Map_TeamStatus_Refill(void);
void Map_Level_CamSetup(void);
void Map_PathStatus_Setup(void);
void Map_CurrentPlanet_SetAlpha(void);
void Map_PlanetExplosions_Setup(void);
bool Map_PlanetSaveSlot_Setup(LevelId, PlanetSaveSlotTypes);
void Map_Update(void);
s32 Map_801A05B4(void);
void Map_Starfield_Setup(void);
void Map_Prologue_Update(void);
void Map_Prologue_Draw(void);
void Map_LylatCard_Update(void);
void Map_PrologueArwing_Draw(void);
void Map_LylatCard_Draw(void);
void Map_ZoomPlanet_Update(void);
bool Map_GralPepper_Talk(void);
f32 Map_GetPlanetCamZDist(PlanetId);
void Map_801A2674(void);
void Map_801A281C(void);
void Map_ZoomPlanetPath_Update(void);
void Map_801A2EB8(void);
void Map_801A3440(f32);
void Map_801A3550(Vec3f*, Vec3f*, Vec3f*, f32);
void Map_GameOver_Update(void);
void Map_GameOver_3DFont(void);
void Map_801A4394(void);
void Map_GameOver_Draw(void);
void Map_801A48C0(f32 speed);
void Map_801A4A38(f32 arg0);
void Map_PathChange_Update(void);
void Map_PathChange_UpdateEntry(u32);
void Map_PathChange_Input(void);
void Map_GoToGameOver(void);
void Map_RetryCourse(void);
void Map_ChangePath(void);
void Map_PathChange_DrawOptions(void);
void Map_RestartLevelLifeDown_Draw(void);
bool Map_Input_CursorY(void);
void Map_LevelStart_Update(void);
void Map_CurrentLevel_Setup(void);
void Map_LevelStart_AudioSpecSetup(LevelId level);
bool Map_LevelPlayedStatus_Check(PlanetId planet);
s32 Map_GetPathId(PlanetId start, PlanetId end);
void Map_PlayLevel(void);
void Map_PositionPlanets(void);
void Map_PlanetOrderZpos(void);
void Map_Fade_Update(void);
void Map_Planet_Draw(PlanetId);
s32 Map_CheckPlanetMedal(PlanetId planetId);
void Map_PlanetAnim2(PlanetId planetId);
void Map_PlanetAnim(PlanetId planetId);
void Map_SolarRays_Draw(PlanetId);
void Map_VenomCloud2_Draw(PlanetId planetId);
void Map_PlanetShadow_Draw(PlanetId planetId);
void Map_Titania_DrawRings1(PlanetId planetId);
void Map_Titania_DrawRings2(PlanetId planetId);
void Map_VenomCloud_Draw(f32* zAngle, f32 next, f32 scale);
void Map_PlanetCleared2_Draw(PlanetId planetId);
void Map_PositionCursor(void);
void Map_TotalHits_Draw(void);
void Map_Idle_Update(void);
void Map_SetCamRot(f32, f32, f32, f32*, f32*, f32*, f32, f32, f32);
PlanetId Map_GetPlanetId(LevelId level);
void Map_PlanetMedal_Draw(PlanetId planetId);
void Map_CorneriaExplosion_Draw(void);
void Map_Cursor_draw(void);
void Map_MeteoMeteors_Draw(void);
void Map_Area6Ships_Draw(void);
void Map_Wipe_Draw(void);
void Map_TitleCards_Draw(void);
void Map_801A9A8C(void);
void Map_801A9DE8(void);
void Map_PathLineBox_Draw(s32);
void Map_PathInfo_Draw(s32, f32, f32, s32);
void Map_PathPlanet_Draw(s32, f32, f32, PlanetId);
void Map_PathPlanetMedal_Draw(f32 x, f32 y, f32 z);
void Map_BriefingRadio_Setup(void);
void Map_BriefingRadio_Update(void);
void Map_BriefingRadio_Draw(s32);
void Map_TeamDownWrench_Draw(s32 teamIdx, s32 alpha);
void Map_GralPepperFace_Draw(void);
void Map_Path_Draw(s32);
void Map_Arwing_Draw(s32 index);
void Map_PathLine_Draw(PathType);
void Map_PathLines_Draw(s32);
void Map_PathLinePos(s32 index, Vec3f* src, Vec3f* dest);
void Map_CamMatrixRot(void);
void Map_Texture_Sphere(u8* textureDest, u8* textureSrc, f32* offset);

#if 1

RECOMP_PATCH void Map_Planet_Draw(PlanetId planetId) __attribute__((optnone)) {
    s32 mask;
    PlanetStatus planetStatus;

    if ((sPlanets[planetId].alpha == 0) && (planetId != sCurrentPlanetId)) {
        return;
    }

    if ((planetId == sCurrentPlanetId) && (sMapState == MAP_ZOOM_PLANET) && (D_menu_801CD95C != 0)) {
        mask = 0x00000001;
    } else {
        mask = 0xFFFFFFFF;
    }

    planetStatus = Map_CheckPlanetMedal(planetId);
    Map_PlanetAnim2(planetId);
    Map_PlanetCleared2_Draw(planetId);
    Matrix_Push(&gGfxMatrix);

    if ((gGameFrameCount & mask) != 0) {
        if (planetId == PLANET_TITANIA) {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_TITANIA_RINGS, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);
            Map_Titania_DrawRings1(planetId);
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }

        if ((planetStatus == PLANET_CLEARED) && (sPlanetPositions[planetId].z > D_menu_801CEA18[planetId]) &&
            (planetId != PLANET_AREA_6) && (planetId != PLANET_BOLSE)) {
            Map_PlanetCleared_Draw(planetId);
        }

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_PLANET_ANIM + planetId, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);
        Map_PlanetAnim(planetId);
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        if (planetId == PLANET_SOLAR) {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, (TAG_SOLAR_RAYS) + planetId, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);
            Map_SolarRays_Draw(planetId);
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }

        if (sPlanets[planetId].anim == PL_ANIM_SPIN) {
            if (planetId != PLANET_VENOM) {
                if ((planetId != PLANET_AQUAS) && (planetId != PLANET_TITANIA)) {
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_VENOM_CLOUD_2 + planetId, G_EX_PUSH,
                                                   G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

                    Map_VenomCloud2_Draw(planetId);

                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                }
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_PLANET_SHADOW + planetId, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);
                Map_PlanetShadow_Draw(planetId);
                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            }
            if (planetId == PLANET_VENOM) {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_VENOM_CLOUD_1, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);
                Map_VenomCloud_Draw(&D_menu_801CEEBC, +0.1f, 3.1f);
                Map_VenomCloud_Draw(&D_menu_801CEEC0, -0.1f, 2.9f);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            }
        }

        if (planetId == PLANET_TITANIA) {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_TITANIA_RINGS + 1, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);
            Map_Titania_DrawRings2(planetId);
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }

        if ((planetId == PLANET_CORNERIA) && sPlanetExplosions[EXPLOSIONS_CORNERIA]) {
            Map_PlanetExplosions_Draw(PLANET_CORNERIA, EXPLOSIONS_CORNERIA);
        }

        if ((planetId == PLANET_KATINA) && sPlanetExplosions[EXPLOSIONS_KATINA]) {
            Map_PlanetExplosions_Draw(PLANET_KATINA, EXPLOSIONS_KATINA);
        }

        if ((planetId == PLANET_SECTOR_Y) && sPlanetExplosions[EXPLOSIONS_SECTOR_Y]) {
            Map_PlanetExplosions_Draw(PLANET_SECTOR_Y, EXPLOSIONS_SECTOR_Y);
        }

        if ((planetStatus == PLANET_CLEARED) && (sPlanetPositions[planetId].z <= D_menu_801CEA18[planetId]) &&
            (planetId != PLANET_AREA_6) && (planetId != PLANET_BOLSE)) {
            Map_PlanetCleared_Draw(planetId);
        }

        if ((planetStatus == PLANET_CLEARED) && ((planetId == PLANET_AREA_6) || (planetId == PLANET_BOLSE))) {
            Map_PlanetCleared_Draw(planetId);
        }

        if (planetStatus == PLANET_MEDAL) {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposed(gMasterDisp++, (TAG_MEDAL + planetId), G_EX_PUSH, G_MTX_MODELVIEW,
                                     G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);

            Map_PlanetMedal_Draw(planetId);
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }
    Matrix_Pop(&gGfxMatrix);
}

#if 0
RECOMP_PATCH void Map_PlanetMedal_Draw(PlanetId planetId) {
    static f32 D_menu_801B6A78 = 0.0f;
    s32 i;
    s32 alpha;
    f32 scale;

    if (D_menu_801CD98C != 1) {
        alpha = D_menu_801CD900[planetId];
    } else {
        alpha = D_menu_801CD984;
        D_menu_801CD900[planetId] = 255;
    }

    if (alpha == 0) {
        return;
    }

    if (D_menu_801CD98C == 1) {
        switch (D_menu_801CD990) {
            case 0:
                RCP_SetupDL(&gMasterDisp, SETUPDL_67);

                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, alpha);
                gDPSetEnvColor(gMasterDisp++, 80, 80, 0, 0);

                Matrix_Push(&gGfxMatrix);

                Matrix_Copy(gGfxMatrix, &D_menu_801CE1E0[planetId]);
                Matrix_Translate(gGfxMatrix, 0.0f, -30.0f, 0.0f, MTXF_APPLY);

                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, (TAG_MEDAL + planetId) | 0x00100000, G_EX_PUSH,
                                               G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, D_MAP_604D680);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                Matrix_Pop(&gGfxMatrix);

                D_menu_801CEAB8[planetId] += 45.0f;
                if (alpha == 255) {
                    D_menu_801CD9C8 = 15;
                    D_menu_801CD998 = 0;
                    D_menu_801CD994 = 255;
                    D_menu_801CD99C = 0.0f;
                    D_menu_801CD990++;
                }
                break;

            case 1:
                scale = 5.0f + RAND_FLOAT(4.0f);

                Math_SmoothStepToF(&D_menu_801CD99C, 150.0f, 0.09f, 100.0f, 0.1f);

                RCP_SetupDL(&gMasterDisp, SETUPDL_67);

                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, D_menu_801CD994);
                gDPSetEnvColor(gMasterDisp++, 80, 80, 0, 0);
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, (TAG_MEDAL + planetId) | 0x00200000, G_EX_PUSH,
                                                G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
                for (i = 0; i < 8; i++) {
                    Matrix_Push(&gGfxMatrix);

                    Matrix_Copy(gGfxMatrix, &D_menu_801CE5A0[planetId]);
                    Matrix_Translate(gGfxMatrix, 0.0f, -30.0f, 0.0f, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, M_DTOR * (i * -45.0f), MTXF_APPLY);
                    Matrix_Translate(gGfxMatrix, 0.0f, D_menu_801CD99C, 0.0f, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, M_DTOR * (D_menu_801B6A78), MTXF_APPLY);
                    Matrix_Scale(gGfxMatrix, scale, scale, scale, MTXF_APPLY);

                    Matrix_SetGfxMtx(&gMasterDisp);

                    gSPDisplayList(gMasterDisp++, D_MAP_604D680);
                    Matrix_Pop(&gGfxMatrix);
                }
                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

                D_menu_801B6A78 += 45.0f;
                D_menu_801CD994 -= 16;

                if (D_menu_801CD994 <= 0) {
                    D_menu_801CD994 = 0;
                }

                RCP_SetupDL(&gMasterDisp, SETUPDL_64);

                gDPSetTextureFilter(gMasterDisp++, G_TF_POINT);
                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, D_menu_801CD998);

                Matrix_Push(&gGfxMatrix);

                Matrix_Copy(gGfxMatrix, &D_menu_801CE5A0[planetId]);
                Matrix_Translate(gGfxMatrix, 0.0f, -30.0f, 0.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 3.0f, 3.0f, 3.0f, MTXF_APPLY);
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, (TAG_MEDAL + planetId) | 0x00300000, G_EX_PUSH,
                                               G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, aMapMedalDL);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                Matrix_Pop(&gGfxMatrix);

                D_menu_801CEAF8[planetId] = -90.0f;

                D_menu_801CD998 += 8;

                if (D_menu_801CD998 == 8) {
                    AUDIO_PLAY_SFX(NA_SE_GET_EMBLEM, gDefaultSfxSource, 4);
                }

                if (D_menu_801CD998 >= 255) {
                    D_menu_801CD998 = 255;
                }
                if (D_menu_801CD998 == 255) {
                    if (!D_menu_801CD9C8) {
                        D_menu_801CD98C = 0;
                    } else {
                        D_menu_801CD9C8--;
                    }
                }
                break;
        }
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_64);
        gDPSetTextureFilter(gMasterDisp++, G_TF_POINT);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, alpha);

        Matrix_Push(&gGfxMatrix);

        Matrix_Copy(gGfxMatrix, &D_menu_801CE5A0[planetId]);
        Matrix_Translate(gGfxMatrix, 0.0f, -30.0f, 0.0f, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, 3.0f, 3.0f, 3.0f, MTXF_APPLY);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, (TAG_MEDAL + planetId) | 0x00400000, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        Matrix_SetGfxMtx(&gMasterDisp);

        gSPDisplayList(gMasterDisp++, aMapMedalDL);
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        Matrix_Pop(&gGfxMatrix);

        D_menu_801CEAF8[planetId] = -90.0f;
    }
}
#endif

extern ObjPosition sMapMeteors[42];
extern Gfx* sMapPlanets[15];

void Map_CamMatrixRot(void);

RECOMP_PATCH void Map_MeteoMeteors_Draw(void) {
    s32 i;
    s32 mask;

    if ((sPlanets[PLANET_METEO].alpha != 0) || (sCurrentPlanetId == PLANET_METEO)) {
        if ((sCurrentPlanetId == PLANET_METEO) && (sMapState == MAP_ZOOM_PLANET) && (D_menu_801CD95C != 0)) {
            mask = 0x00000001;
        } else {
            mask = 0xFFFFFFFF;
        }

        if ((sMapState == MAP_IDLE) || ((sCurrentPlanetId == PLANET_METEO) && (sMapState != MAP_GAME_OVER))) {
            RCP_SetupDL(&gMasterDisp, SETUPDL_53);
        } else {
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, sPlanets[PLANET_METEO].alpha);
        }

        if ((gGameFrameCount & mask) != 0) {
            for (i = 0; i < ARRAY_COUNT(sMapMeteors); i++) {
                Matrix_Push(&gGfxMatrix);

                Matrix_RotateY(gGfxMatrix, M_DTOR * sMapMeteors[i].angle, MTXF_APPLY);
                Matrix_Translate(gGfxMatrix, sMapMeteors[i].x, sMapMeteors[i].y, 0.0f, MTXF_APPLY);
                Matrix_RotateY(gGfxMatrix, M_DTOR * -sMapMeteors[i].angle, MTXF_APPLY);

                Map_CamMatrixRot();

                Matrix_Scale(gGfxMatrix, sMapMeteors[i].scale, sMapMeteors[i].scale, sMapMeteors[i].scale, MTXF_APPLY);

                Matrix_SetGfxMtx(&gMasterDisp);

                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_PLANET_METEOR + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                gSPDisplayList(gMasterDisp++, sMapPlanets[PLANET_METEO]);

                Matrix_Pop(&gGfxMatrix);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            }
        }
    }
}

// Workaround to fix compiler mips generation (Thanks clang...)
s32 checkIfPlanetIdNotEq9(s32 planetId) __attribute__((optnone)) {
    if (planetId != 9) {
        return 5;
    } else {
        return 10;
    }
}

bool checkExplosionInKatina(s32 explosionIdx) __attribute__((optnone)) {
    if (explosionIdx == EXPLOSIONS_KATINA) {
        return true;
    } else {
        return false;
    }
}

#if 1
RECOMP_PATCH void Map_PlanetExplosions_Draw(s32 planetId, PlanetExplosions explosionIdx) {
    s32 i;
    f32 temp;

    if (sPlanets[planetId].alpha == 0) {
        return;
    }

    if (!sPlanetExplosions[explosionIdx]) {
        return;
    }

    RCP_SetupDL(&gMasterDisp, SETUPDL_67);

    gDPSetEnvColor(gMasterDisp++, 255, 0, 0, 0);

    // @recomp Tag the transform.
    // gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_PLANET_EXPLOSIONS, G_EX_PUSH, G_MTX_MODELVIEW,
    // G_EX_EDIT_ALLOW);

    for (i = 0; i < checkIfPlanetIdNotEq9(planetId); i++) {
        switch (D_menu_801CEB58[explosionIdx][i]) {
            case 0:
                D_menu_801CEC48[explosionIdx][i] += 0.1f;
                if (D_menu_801CEC48[explosionIdx][i] >= D_menu_801CECC0[explosionIdx][i]) {
                    D_menu_801CEC48[explosionIdx][i] = D_menu_801CECC0[explosionIdx][i];
                    D_menu_801CEB58[explosionIdx][i] = 1;
                }
                break;

            case 1:
                D_menu_801CEE28[explosionIdx][i]--;
                if (D_menu_801CEE28[explosionIdx][i] <= 0) {
                    D_menu_801CEB58[explosionIdx][i] = 2;
                }
                break;

            case 2:
                D_menu_801CEBD0[explosionIdx][i] -= 48;
                if (D_menu_801CEBD0[explosionIdx][i] < 0) {
                    D_menu_801CEBD0[explosionIdx][i] = 0;
                    D_menu_801CEB58[explosionIdx][i] = 3;
                }
                break;

            case 3:
                D_menu_801CEB58[explosionIdx][i] = RAND_INT(3.0f);
                D_menu_801CEBD0[explosionIdx][i] = 255;
                D_menu_801CEC48[explosionIdx][i] = 0.0f;
                D_menu_801CECC0[explosionIdx][i] = 0.5f + RAND_FLOAT(0.3f);
                D_menu_801CEE28[explosionIdx][i] = 1 + RAND_INT(4.0f);

                if (checkExplosionInKatina(explosionIdx)) {
                    temp = 50.0f;
                } else {
                    temp = 110.0f;
                }

                D_menu_801CED38[explosionIdx][i] = temp + RAND_INT(30.0f);
                D_menu_801CEDB0[explosionIdx][i] = -10.0f + RAND_FLOAT(-60.0f);
                break;
        }

        if ((D_menu_801CECC0[explosionIdx][i] == 0.0f) || (D_menu_801CEBD0[explosionIdx][i] == 0)) {
            continue;
        }

        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 200, 200, D_menu_801CEBD0[explosionIdx][i]);

        gEXMatrixGroupDecomposed(gMasterDisp++, TAG_CORNERIA_EXPLOSIONS + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                 G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO, G_EX_COMPONENT_AUTO,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,
                                 G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);

        Matrix_Push(&gGfxMatrix);

        Matrix_Mult(gGfxMatrix, &D_menu_801CDA60[planetId], MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, M_DTOR * D_menu_801CEDB0[explosionIdx][i], MTXF_APPLY);
        Matrix_Translate(gGfxMatrix, 0.0f, D_menu_801CED38[explosionIdx][i], 0.0f, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, D_menu_801CEC48[explosionIdx][i], D_menu_801CEC48[explosionIdx][i],
                     D_menu_801CEC48[explosionIdx][i], MTXF_APPLY);

        Matrix_SetGfxMtx(&gMasterDisp);

        gSPDisplayList(gMasterDisp++, aMapPlanetExplosionDL);

        Matrix_Pop(&gGfxMatrix);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}
#endif

RECOMP_PATCH void Map_Area6Ships_Draw(void) {
    Vec3f src;
    Vec3f dest;
    f32 x1;
    f32 y1;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    s32 mask;
    static f32 D_menu_801B6A80 = -22.0f;

    if ((sPlanets[PLANET_AREA_6].alpha == 0) && (sCurrentPlanetId != PLANET_AREA_6)) {
        return;
    }

    if ((sCurrentPlanetId == PLANET_AREA_6) && (sMapState == MAP_ZOOM_PLANET) && (D_menu_801CD95C != 0)) {
        mask = 0x00000001;
    } else {
        mask = 0xFFFFFFFF;
    }

    if ((sMapState == MAP_IDLE) || (sCurrentPlanetId == PLANET_AREA_6)) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_23);
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_46);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, sPlanets[PLANET_AREA_6].alpha);
    }

    dest.x = 0.0f;
    dest.y = 0.0f;
    dest.z = 0.0f;

    if ((gGameFrameCount & mask) != 0) {

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_AREA6_SHIPS, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        for (i = 0; i < ARRAY_COUNT(sMapArea6Ships); i++) {
            Matrix_Push(&gGfxMatrix);

            Matrix_RotateY(gGfxMatrix, M_DTOR * sMapArea6Ships[i].angle, MTXF_APPLY);
            Matrix_Translate(gGfxMatrix, sMapArea6Ships[i].x, sMapArea6Ships[i].y, 0.0f, MTXF_APPLY);
            Matrix_RotateY(gGfxMatrix, M_DTOR * -(sMapArea6Ships[i].angle), MTXF_APPLY);

            Map_CamMatrixRot();

            Matrix_RotateY(gGfxMatrix, M_DTOR * D_menu_801B6A80, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, sMapArea6Ships[i].scale, sMapArea6Ships[i].scale, sMapArea6Ships[i].scale,
                         MTXF_APPLY);

            Matrix_SetGfxMtx(&gMasterDisp);

            Matrix_MultVec3f(gGfxMatrix, &dest, &src);

            x = sPlanetPositions[PLANET_SOLAR].x - src.x;
            y = sPlanetPositions[PLANET_SOLAR].y - src.y;
            z = sPlanetPositions[PLANET_SOLAR].z - src.z;

            x1 = Math_Atan2F(y, sqrtf(SQ(x) + SQ(z)));
            y1 = -Math_Atan2F(x, z);

            dest.x = 0.0f;
            dest.y = 0.0f;
            dest.z = 100.0f;

            Matrix_RotateY(gCalcMatrix, M_DTOR * (-D_menu_801CDA10 - y1), MTXF_NEW);
            Matrix_RotateX(gCalcMatrix, M_DTOR * (-D_menu_801CDA0C - x1), MTXF_APPLY);

            Matrix_MultVec3f(gCalcMatrix, &dest, &src);
            Lights_SetOneLight(&gMasterDisp, src.x, src.y, src.z, 80, 80, 60, 0, 0, 0);

            gSPDisplayList(gMasterDisp++, aMapArea6ShipDL);

            Matrix_Pop(&gGfxMatrix);
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}

RECOMP_PATCH void Map_Arwing_Draw(s32 index) {
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    Vec3f dest;
    Vec3f src;
    f32 x1;
    f32 y1;
    f32 x;
    f32 y;
    f32 z;

    src.x = 0.0f;
    src.y = 0.0f;
    src.z = 0.0f;

    if (D_menu_801CEEA8 == 255) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_23);
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_46);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, D_menu_801CEEA8);
    }

    Matrix_Push(&gGfxMatrix);

    Matrix_Translate(gGfxMatrix, D_menu_801CEEB0.x, D_menu_801CEEB0.y, D_menu_801CEEB0.z, MTXF_APPLY);

    Matrix_RotateY(gGfxMatrix, gTexturedLines[index].yRot, MTXF_APPLY);
    Matrix_RotateX(gGfxMatrix, M_DTOR * -90.0f, MTXF_APPLY);
    Matrix_RotateX(gGfxMatrix, gTexturedLines[index].xRot, MTXF_APPLY);

    Matrix_Scale(gGfxMatrix, 0.8f, 0.8f, 0.8f, MTXF_APPLY);

    Matrix_SetGfxMtx(&gMasterDisp);

    Matrix_MultVec3f(gGfxMatrix, &src, &dest);

    x = sPlanetPositions[PLANET_SOLAR].x - dest.x;
    y = sPlanetPositions[PLANET_SOLAR].y - dest.y;
    z = sPlanetPositions[PLANET_SOLAR].z - dest.z;

    x1 = Math_Atan2F(y, sqrtf(SQ(x) + SQ(z)));
    y1 = -Math_Atan2F(x, z);

    src.x = 0.0f;
    src.y = 0.0f;
    src.z = 10.0f;

    Matrix_RotateY(gCalcMatrix, M_DTOR * (-D_menu_801CDA10 - y1), MTXF_NEW);
    Matrix_RotateX(gCalcMatrix, M_DTOR * (-D_menu_801CDA0C - x1), MTXF_APPLY);
    Matrix_MultVec3f(gCalcMatrix, &src, &dest);

    dirX = dest.x;
    dirY = dest.y;
    dirZ = dest.z;

    Lights_SetOneLight(&gMasterDisp, dirX, dirY, dirZ, 50, 50, 40, 0, 0, 0);
    if (sPaths[index].unk_14 == 2) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_ARWING, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ARWING, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    }

    gSPDisplayList(gMasterDisp++, aMapArwingDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    Matrix_Pop(&gGfxMatrix);
}

RECOMP_PATCH void Map_PathPlanet_Draw(s32 missionIdx, f32 x, f32 y, PlanetId planetId) __attribute__((optnone)) {
    s32 mask = 0xFFFFFFFF;

    if ((gGameState == GSTATE_MAP) && (planetId == sCurrentPlanetId)) {
        mask = 0x00000010;
    }

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_PATH_PLANET, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    switch (planetId) {
        case PLANET_SOLAR:
            if ((gGameFrameCount & mask) != 0) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_67);

                gDPSetPrimColor(gMasterDisp++, 0, 0, 240, 0, 0, 255);
                gDPSetEnvColor(gMasterDisp++, 31, 0, 0, 0);

                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, x, y, 0.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, M_DTOR * (sPlanets[planetId].orbit.tilt), MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.11f, 0.11f, 0.11f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, sMapPlanets[sPlanets[planetId].id]);

                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 128);
                gDPSetEnvColor(gMasterDisp++, 31, 0, 0, 0);

                Matrix_Scale(gGfxMatrix, 0.8f, 0.8f, 0.8f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, sMapPlanets[sPlanets[planetId].id]);

                Matrix_Pop(&gGfxMatrix);
            }
            break;

        case PLANET_METEO:
            if ((gGameFrameCount & mask) != 0) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_62);

                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

                Matrix_Push(&gGfxMatrix);

                Matrix_Translate(gGfxMatrix, x - 0.4f, y + 0.9f, 0.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, M_DTOR * sPlanets[planetId].orbit.tilt, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.1f, 0.1f, 0.1f, MTXF_APPLY);

                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, sMapPlanets[sPlanets[planetId].id]);

                Matrix_Translate(gGfxMatrix, 18.0f, -20.0f, 0.0f, MTXF_APPLY);

                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, sMapPlanets[sPlanets[planetId].id]);
                Matrix_Pop(&gGfxMatrix);
            }
            break;

        case PLANET_SECTOR_X:
        case PLANET_SECTOR_Y:
        case PLANET_SECTOR_Z:
            if ((gGameFrameCount & mask) != 0) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_62);

                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 144);

                Matrix_Push(&gGfxMatrix);

                Matrix_Translate(gGfxMatrix, x, y, 0.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, M_DTOR * sPlanets[planetId].orbit.tilt, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.09f, 0.09f, 0.09f, MTXF_APPLY);

                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, sMapPlanets[sPlanets[planetId].id]);

                Matrix_Pop(&gGfxMatrix);
            }
            break;

        case PLANET_BOLSE:
            if ((gGameFrameCount & mask) != 0) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_23);

                Lights_SetOneLight(&gMasterDisp, 0, 0, 100, 50, 50, 40, 100, 100, 100);

                Matrix_Push(&gGfxMatrix);

                Matrix_Translate(gGfxMatrix, x, y, 0.0f, MTXF_APPLY);
                Matrix_RotateX(gGfxMatrix, M_DTOR * 20.0f, MTXF_APPLY);
                Matrix_RotateY(gGfxMatrix, M_DTOR * D_menu_801AFFF4, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, M_DTOR * sPlanets[planetId].orbit.tilt, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.004f, 0.004f, 0.004f, MTXF_APPLY);

                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, sMapPlanets[sPlanets[planetId].id]);

                Matrix_Pop(&gGfxMatrix);
            }
            break;

        case PLANET_AREA_6:
            if ((gGameFrameCount & mask) != 0) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_23);

                Lights_SetOneLight(&gMasterDisp, 0, 0, 100, 50, 50, 40, 100, 100, 100);

                Matrix_Push(&gGfxMatrix);

                Matrix_Translate(gGfxMatrix, x, y, 0.0f, MTXF_APPLY);
                Matrix_RotateX(gGfxMatrix, M_DTOR * 20.0f, MTXF_APPLY);
                Matrix_RotateY(gGfxMatrix, M_DTOR * D_menu_801AFFFC, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, M_DTOR * sPlanets[planetId].orbit.tilt, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.003f, 0.003f, 0.003f, MTXF_APPLY);

                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, sMapPlanets[sPlanets[planetId].id]);

                Matrix_Pop(&gGfxMatrix);
            }
            break;

        default:
            if ((gGameFrameCount & mask) != 0) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_62);

                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

                Matrix_Push(&gGfxMatrix);

                Matrix_Translate(gGfxMatrix, x, y, 0.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, M_DTOR * sPlanets[planetId].orbit.tilt, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.1f, 0.1f, 0.1f, MTXF_APPLY);

                Matrix_SetGfxMtx(&gMasterDisp);

                gSPDisplayList(gMasterDisp++, sMapPlanets[sPlanets[planetId].id]);

                if (sPlanets[planetId].anim == PL_ANIM_SPIN) {
                    if (planetId != PLANET_VENOM && planetId != PLANET_AQUAS) {
                        Matrix_SetGfxMtx(&gMasterDisp);
                        if (planetId == PLANET_MACBETH) {
                            gDPSetPrimColor(gMasterDisp++, 0, 0, 64, 64, 64, 255);
                        } else {
                            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
                        }
                        gSPDisplayList(gMasterDisp++, gMapVenomCloudDL);
                    }
                    Matrix_Scale(gGfxMatrix, 1.6f, 1.6f, 1.6f, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, D_MAP_605C230);
                }
                Matrix_Pop(&gGfxMatrix);
            }
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    if (gMissionMedal[missionIdx] != 0) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_PATH_PLANET_MEDAL + missionIdx, G_EX_PUSH, G_MTX_MODELVIEW,
                                        G_EX_EDIT_ALLOW);

        Map_PathPlanetMedal_Draw(x, y, 0.0f);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}

RECOMP_PATCH void Map_Draw(void) {
    s32 i;
    s32* ptr;

    Matrix_Push(&gGfxMatrix);
    Matrix_LookAt(gGfxMatrix, sMapCamEyeX, sMapCamEyeY, sMapCamEyeZ, sMapCamAtX, sMapCamAtY, sMapCamAtZ, sMapCamUpX,
                  sMapCamUpY, sMapCamUpZ, MTXF_APPLY);
    Matrix_Translate(gGfxMatrix, D_menu_801CEA58, D_menu_801CEA5C, D_menu_801CEA60, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);

    if (D_menu_801CD974 && (sMapState != MAP_PROLOGUE)) {
        for (i = 0; i < 24; i++) {
            if (gPlanetPathStatus[i] == 0) {
                continue;
            }
            if (sPaths[i].alpha == 0) {
                continue;
            }

            Map_PathLines_Draw(i);

            if (sPaths[i].unk_14 != 0) {
                Map_Path_Draw(i);
            }

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_TEXTURED_LINE + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            TexturedLine_DrawPath(i);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }

    Map_Cursor_draw();

    Map_MeteoMeteors_Draw();

    Map_Area6Ships_Draw();

    for (ptr = D_menu_801CD8A0, i = 0; i < 15; i++, ptr++) {
        Map_Planet_Draw(*ptr);
    }

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_CORNERIA_EXPLOSION, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Map_CorneriaExplosion_Draw();

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    Matrix_Pop(&gGfxMatrix);

    if (D_menu_801CD96C) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_PEPPER_FACE, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        Map_GralPepperFace_Draw();

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }

    Map_BriefingRadio_Update();

    if (D_menu_801CEFC4) {
        Map_PathChange_DrawOptions();
    }

    if (D_menu_801CD964) {
        Map_TitleCards_Draw();
    }

    Map_Wipe_Draw();

    if (D_menu_801CD968) {
        Map_801A9A8C();
    }

    Map_801A9DE8();

    Map_RestartLevelLifeDown_Draw();

    if ((sMapState == MAP_GAME_OVER) && (sMapSubState == 1)) {
        Map_GameOver_Draw();
    }
    if (sMapState == MAP_PROLOGUE) {
        Map_Prologue_Draw();
    }
    if (sMapState == MAP_LYLAT_CARD) {
        Map_LylatCard_Draw();
    }

    if (D_menu_801CEEC8 == 0) {
        Map_Texture_Sphere(gBSSMapPlanetTextures[8], SEGMENTED_TO_VIRTUAL(gAssetMapPlanetTextures[8]),
                           &D_menu_801CD818[8]);
        D_menu_801CEEC8 = 5;
    } else {
        D_menu_801CEEC8--;
    }

    Map_Texture_Sphere(gBSSMapPlanetTextures[D_menu_801CEEC4 * 2],
                       SEGMENTED_TO_VIRTUAL(gAssetMapPlanetTextures[D_menu_801CEEC4 * 2]),
                       &D_menu_801CD818[D_menu_801CEEC4 * 2]);
    Map_Texture_Sphere(gBSSMapPlanetTextures[(D_menu_801CEEC4 * 2) + 1],
                       SEGMENTED_TO_VIRTUAL(gAssetMapPlanetTextures[(D_menu_801CEEC4 * 2) + 1]),
                       &D_menu_801CD818[(D_menu_801CEEC4 * 2) + 1]);

    D_menu_801CEEC4++;
    if (D_menu_801CEEC4 > 3) {
        D_menu_801CEEC4 = 0;
    }

    Lib_Texture_Mottle((u16*) aMapVenomCloud1Tex, (u16*) D_MAP_6048F80, 5);
}

RECOMP_PATCH void Map_PlanetCleared_Draw(PlanetId planetId) {
    s32 alpha;

    if (D_menu_801CD980 != true) {
        alpha = D_menu_801CD900[planetId];
    } else {
        alpha = D_menu_801CD984;
        D_menu_801CD900[planetId] = 255;
    }

    if (alpha != 0) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_PLANET_CLEARED_MARK + planetId, G_EX_PUSH, G_MTX_MODELVIEW,
                                        G_EX_EDIT_ALLOW);

        RCP_SetupDL(&gMasterDisp, SETUPDL_67);

        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, alpha);
        gDPSetEnvColor(gMasterDisp++, 80, 80, 0, 0);

        Matrix_Push(&gGfxMatrix);

        Matrix_Copy(gGfxMatrix, &D_menu_801CE1E0[planetId]);
        Matrix_SetGfxMtx(&gMasterDisp);

        gSPDisplayList(gMasterDisp++, D_MAP_604D680);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        Matrix_Pop(&gGfxMatrix);

        D_menu_801CEAB8[planetId] += 45.0f;
        if (D_menu_801CD980 == false) {
            D_menu_801CEAF8[planetId] += 5.0f;
        }
    }
}

#if 0
RECOMP_PATCH void Map_Path_Draw(s32 index) {
    Vec3f v;
    f32 r;
    f32 r2;
    Vec3f vec;

    v.x = gTexturedLines[index].posBB.x - gTexturedLines[index].posAA.x;
    v.y = gTexturedLines[index].posBB.y - gTexturedLines[index].posAA.y;
    v.z = gTexturedLines[index].posBB.z - gTexturedLines[index].posAA.z;

    r = VEC3F_MAG(&v);

    if (r == 0.0f) {
        r = 1.0f;
    }

    if (sPaths[index].unk_14 == 2) {
        vec.x = (v.x / r) * (r / (sPaths[index].length - 1));
        vec.y = (v.y / r) * (r / (sPaths[index].length - 1));
        vec.z = (v.z / r) * (r / (sPaths[index].length - 1));

        D_menu_801CEEB0.x = gTexturedLines[index].posAA.x + (vec.x * D_menu_801CEEA0);
        D_menu_801CEEB0.y = gTexturedLines[index].posAA.y + (vec.y * D_menu_801CEEA0);
        D_menu_801CEEB0.z = gTexturedLines[index].posAA.z + (vec.z * D_menu_801CEEA0);

        switch (D_menu_801CEEA4) {
            case 0:
                break;

            case 100:
                sMapTimer2 = 5;
                D_menu_801CEEA4 = 10;
                break;

            case 10:
                if (sMapTimer2 != 0) {
                    break;
                }
                sMapTimer2 = 5;
                D_menu_801CEEA4 = 20;
                break;

            case 20:
                if (sMapTimer2 != 0) {
                    break;
                }

                D_menu_801CEEA0++;
                if (D_menu_801CEEA0 > sPaths[index].length - 1) {
                    D_menu_801CEEA0 = 0;
                }

                D_menu_801CEEA4 = 100;
                break;
        }

        if (D_menu_801CEEA4 != 10) {
            return;
        }
    } else {
        vec.x = (v.x / r) * gTexturedLines[index].zScale;
        vec.y = (v.y / r) * gTexturedLines[index].zScale;
        vec.z = (v.z / r) * gTexturedLines[index].zScale;

        D_menu_801CEEB0.x = gTexturedLines[index].posAA.x + vec.x;
        D_menu_801CEEB0.y = gTexturedLines[index].posAA.y + vec.y;
        D_menu_801CEEB0.z = gTexturedLines[index].posAA.z + vec.z;

        v.x = D_menu_801CEEB0.x - gTexturedLines[index].posAA.x;
        v.y = D_menu_801CEEB0.y - gTexturedLines[index].posAA.y;
        v.z = D_menu_801CEEB0.z - gTexturedLines[index].posAA.z;
        r2 = VEC3F_MAG(&v);

        if (((r / 9.0f) * 8.0f) < r2) {
            D_menu_801CEEAC -= 16;
            if (D_menu_801CEEAC < 0) {
                D_menu_801CEEAC = 0;
            }
        }
    }

    if (sPaths[index].unk_14 == 5) {

        // @recomp Tag the transform.
        //gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_WARP + index, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        Map_PathLine_Draw(sPaths[index].type);

        // @recomp Pop the transform id.
       // gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    } else {
        Map_Arwing_Draw(index);
    }
}
#endif

RECOMP_PATCH void Map_Prologue_Draw(void) {
    s32 i;
    static f32 sPrologueTextFadeTexUpperYpos = 71.0f;
    static f32 sPrologueTextFadeTexBottomYpos = 205.0f;
    static u16* sPrologueTextures[] = {
        aMapPrologue1Tex, aMapPrologue2Tex, aMapPrologue3Tex, aMapPrologue4Tex,
        aMapPrologue5Tex, aMapPrologue6Tex, aMapPrologue7Tex,
    };

    Map_PrologueArwing_Draw();

    RCP_SetupDL(&gMasterDisp, SETUPDL_81);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 180, 180, 180, 255);

    // Prologue text
    Message_DisplayScrollingText(&gMasterDisp, gMsg_ID_1, sPrologueTextXpos, sPrologueTextYpos, 218, 70,
                                 Message_GetCharCount(gMsg_ID_1));

    RCP_SetupDL(&gMasterDisp, SETUPDL_76);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

    // Vertical Fade Margins for prologue text
    Lib_TextureRect_IA8(&gMasterDisp, aMapPrologueTextFadeTex, 8, 16, 0.0f, sPrologueTextFadeTexUpperYpos, 40.0f,
                        1.66f);
    Lib_TextureRect_IA8_MirY(&gMasterDisp, aMapPrologueTextFadeTex, 8, 16, 0.0f, sPrologueTextFadeTexBottomYpos, 40.0f,
                             1.68f);

    // @recomp:
    // Background_DrawPartialStarfield(71, 118);
    // Background_DrawPartialStarfield(205, 239);
    Background_DrawStarfield();

    RCP_SetupDL(&gMasterDisp, SETUPDL_76);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, sPrologueCurrentTexAlpha);

    // Current Prologue texture
    for (i = 0; i < 13; i++) {
        Lib_TextureRect_RGBA16(&gMasterDisp, sPrologueTextures[sPrologueTexIdx] + (96 * 4 * i), 96, 4, 109.0f,
                               24.0f + (4.0f * i), 1.0f, 1.0f);
    }

    // Next Prologue texture
    if ((sPrologueNextTexAlpha != 0) && ((sPrologueTexIdx + 1) < 7)) {
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, sPrologueNextTexAlpha);

        for (i = 0; i < 13; i++) {
            Lib_TextureRect_RGBA16(&gMasterDisp, sPrologueTextures[sPrologueTexIdx + 1] + (96 * 4 * i), 96, 4, 109.0f,
                                   24.0f + (i * 4.0f), 1.0f, 1.0f);
        }

        if (sPrologueNextTexAlpha == 255) {
            sPrologueNextTexAlpha = 0;
            sPrologueTexIdx++;
        }
    }
}

#endif