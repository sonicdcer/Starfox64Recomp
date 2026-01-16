#include "patches.h"
#include "i3.h"

extern Gfx aPlanetArwingAllRangeDL[];
extern Gfx aMeCorneriaBgDL[];
extern Gfx aOrbDL[];
extern Gfx aCommanderDL[];
extern Gfx aKattShipDL[];
extern Gfx aSzSpaceJunk3DL[];
extern Gfx aSzSpaceJunk1DL[];
extern Gfx aSzInvaderIIIDL[];
extern Gfx D_1021E20[];
extern Gfx D_BO_600BAA0[];
extern Gfx D_BO_6008770[];
extern Gfx D_BO_6000D80[];
extern Gfx D_101ABD0[];
extern Gfx aBillShipDL[];
extern Gfx aKaCornerianFighterDL[];
extern Gfx aKaEnemyDL[];
extern Gfx aSyShip1DL[];
extern Gfx aSyShip2DL[];
extern Animation* D_demo_800CA1F4[];
extern Animation aAqSeaweedAnim;
extern Limb* D_SY_602D140[];
extern Limb* aAqSeaweedSkel[];
extern Gfx D_SY_60132A0[];
extern Gfx D_SY_6014A40[];
extern Gfx aAqBump2DL[];
extern Gfx aAqCoralReef2DL[];
extern Gfx aAqRockDL[];
extern Gfx aTitleArwingEngineGlowDL[];
extern Gfx aEndOrbDL[];
extern Gfx aGreatFoxIntactDL[];
extern Gfx aGreatFoxDamagedDL[];
extern Vec3f sGreatFoxLightPos2[];
extern f32 D_demo_800CA170[];
extern Vec3f sGreatFoxLightPos[3];
extern f32 sGreatFoxLightScale2[2];
extern f32 D_demo_800CA1B4[8];
extern f32 sGreatFoxLightScale3[8];
extern f32 sMeGreatFoxLightScale[7];

extern void ActorCutscene_SyShip_Setup(ActorCutscene*, s32);
extern void Aquas_801BE0F0(ActorCutscene*);

/*
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_CS_GREAT_FOX + , G_EX_PUSH, G_MTX_MODELVIEW,
    G_EX_EDIT_ALLOW);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

*/

RECOMP_PATCH void Cutscene_DrawGreatFox(void) {
    Vec3f* var_s6_2;
    s32 i;
    s32 j;
    f32 sp9C[4];
    Gfx* dList;

    PRINTF("Demo_Time=%d\n");
    PRINTF("Demo_Time=%d\n");
    PRINTF("d Enm->wrk0 %d\n");

    if (gGameState == GSTATE_TITLE) {
        dList = aTitleArwingEngineGlowDL;
    } else if (gGameState == GSTATE_ENDING) {
        dList = aEndOrbDL;
    } else {
        dList = aOrbDL;
    }

    if (gCurrentLevel == LEVEL_TITANIA) {
        RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, 1005);
    }

    if (gCamera1Skipped || ((gCurrentLevel == LEVEL_AQUAS) && (gCsFrameCount <= 2) && (gPlayer[0].csState == 1))) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_CS_GREAT_FOX, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_CS_GREAT_FOX, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    }

    if (gGreatFoxIntact) {
        gSPDisplayList(gMasterDisp++, aGreatFoxIntactDL);
    } else {
        gSPDisplayList(gMasterDisp++, aGreatFoxDamagedDL);
    }

    if ((gCurrentLevel != LEVEL_AQUAS) &&
        ((gCurrentLevel != LEVEL_SECTOR_Z) || (gPlayer[0].state == PLAYERSTATE_LEVEL_COMPLETE))) {
        RCP_SetupDL_49();
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

        for (i = 0, var_s6_2 = sGreatFoxLightPos2; i < ARRAY_COUNT(sp9C); i++, var_s6_2++) {
            if ((i != 1) || gGreatFoxIntact) {
                sp9C[i] = 0.0f;

                if (i < 2) {
                    if ((gGameFrameCount & ((64 - 1) & ~(8 - 1))) == 0) {
                        sp9C[i] = D_demo_800CA170[gGameFrameCount % 8U];
                    }
                    gDPSetEnvColor(gMasterDisp++, 255, 32, 32, 128);
                } else {
                    if (((gGameFrameCount + 32) & 0x38) == 0) {
                        sp9C[i] = D_demo_800CA170[gGameFrameCount % 8U];
                    }
                    gDPSetEnvColor(gMasterDisp++, 32, 32, 255, 128);
                }

                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, var_s6_2->x, var_s6_2->y, var_s6_2->z, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, sp9C[i], sp9C[i], 1.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, dList);
                Matrix_Pop(&gGfxMatrix);
            }
        }

        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 48);
        gDPSetEnvColor(gMasterDisp++, 255, 255, 0, 48);

        for (i = 0, var_s6_2 = sGreatFoxLightPos; i < 3; i++, var_s6_2++) {
            sp9C[i] = sGreatFoxLightScale2[gGameFrameCount % 2U];
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, var_s6_2->x, var_s6_2->y, var_s6_2->z, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, sp9C[i], sp9C[i], 1.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);

            for (j = 0; j < 4; j++) {
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, D_demo_800CA1B4[2 * j], MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, sGreatFoxLightScale3[2 * j], sGreatFoxLightScale3[2 * j], 1.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, dList);
                Matrix_Pop(&gGfxMatrix);
            }
            Matrix_Pop(&gGfxMatrix);
        }

        if ((gCurrentLevel == LEVEL_METEO) && (gPlayer[0].csEventTimer != 0)) {
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 128);
            gDPSetEnvColor(gMasterDisp++, 255, 255, 32, 128);
            Matrix_Translate(gGfxMatrix, D_ctx_80177A48[3] * (-74.0f), -232.0f, 1190.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, sMeGreatFoxLightScale[gPlayer[0].csEventTimer],
                         sMeGreatFoxLightScale[gPlayer[0].csEventTimer], 1.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);
        }
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

#if 1
RECOMP_PATCH void ActorCutscene_Draw(ActorCutscene* this) {
    static f32 D_800CA210 = 0.0f;
    static f32 D_800CA214 = 0.0f;
    static f32 D_800CA218 = 0.0f;
    f32 sp2DC;
    f32 sp2D8;
    f32 sp2D4;
    s32 i;
    s32 pad2C4[3];
    Vec3f sp2B8;
    Vec3f sp2AC;
    Vec3f sp144[30];
    s32 pad[3];
    s32 animFrameData;
    f32 camX;
    f32 camY;
    f32 camZ;
    f32 y;
    f32 x;

    switch (this->animFrame) {
        case ACTOR_CS_JAMES_ARWING:
            RCP_SetupDL(&gMasterDisp, SETUPDL_45);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->work_046);
            gSPDisplayList(gMasterDisp++, aPlanetArwingAllRangeDL);

            if (this->work_046 > 50) {
                Actor_DrawEngineAndContrails(this);
            }
            break;

        case ACTOR_CS_TEAM_ARWING:
            ActorTeamArwing_Draw(this);
            break;

        case ACTOR_CS_GREAT_FOX:
            if (gCurrentLevel == LEVEL_SECTOR_Z) {
                gSPFogPosition(gMasterDisp++, gFogNear, 1005);
            }
            this->info.bonus = 1;
            Cutscene_DrawGreatFox();
            break;

        case ACTOR_CS_ME_CORNERIA_BG:
            RCP_SetupDL_40();
            Matrix_Scale(gGfxMatrix, 60.0f, 60.0f, 1.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aMeCorneriaBgDL);
            break;

        case ACTOR_CS_FO_EXPLOSION:
            RCP_SetupDL_64_2();
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
            Matrix_Scale(gGfxMatrix, 60.0f, 60.0f, 1.0f, MTXF_APPLY);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 10.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, this->scale, this->scale, 1.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);

            RCP_SetupDL_49();
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->iwork[0]);
            gDPSetEnvColor(gMasterDisp++, 255, 255, 00, this->iwork[0]);
            gSPDisplayList(gMasterDisp++, aOrbDL);

            // @recomp: Add rumble to this explosion
            if (gCsFrameCount == 100) {
                gControllerRumbleTimers[0] = 4;
            }
            break;

        case ACTOR_CS_COMMANDER:
            gSPDisplayList(gMasterDisp++, aCommanderDL);
            break;

        case ACTOR_CS_KATT:
            gSPDisplayList(gMasterDisp++, aKattShipDL);
            Actor_DrawEngineAndContrails(this);
            break;

        case ACTOR_CS_SZ_SPACE_JUNK:
            if ((this->index % 2) != 0) {
                gSPDisplayList(gMasterDisp++, aSzSpaceJunk3DL);
            } else {
                gSPDisplayList(gMasterDisp++, aSzSpaceJunk1DL);
            }
            break;

        case ACTOR_CS_SZ_INVADER:
            gSPDisplayList(gMasterDisp++, aSzInvaderIIIDL);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -60.0f, MTXF_APPLY);
            Actor_DrawEngineGlow(this, EG_GREEN);
            break;

        case ACTOR_CS_COMMANDER_GLOW:
            gSPDisplayList(gMasterDisp++, aCommanderDL);
            Matrix_Translate(gGfxMatrix, 0.f, 0.f, -60.0f, MTXF_APPLY);
            Actor_DrawEngineGlow(this, EG_GREEN);
            break;

        case ACTOR_CS_BO_SATELLITE:
            Display_SetSecondLight(&this->obj.pos);

            if (this->work_046 != 0) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_55);
                gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                Rand_SetSeed(1, 29000, 9876);

                for (i = 0; i < 30; i++) {
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, RAND_FLOAT_CENTERED_SEEDED(3000.0f) * this->fwork[20],
                                     RAND_FLOAT_CENTERED_SEEDED(3000.0f) * this->fwork[20],
                                     RAND_FLOAT_CENTERED_SEEDED(3000.0f) * this->fwork[20], MTXF_APPLY);
                    Matrix_RotateY(gGfxMatrix, 2.0f * RAND_FLOAT_SEEDED(M_PI), MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, (2.0f * gGameFrameCount) * M_DTOR, MTXF_APPLY);
                    Matrix_RotateX(gGfxMatrix, 2.0f * RAND_FLOAT_SEEDED(M_PI), MTXF_APPLY);

                    switch ((s32) (i % 4U)) {
                        case 0:
                            Matrix_Scale(gGfxMatrix, 1.0f, 0.3f, 1.0f, MTXF_APPLY);
                            break;

                        case 1:
                            Matrix_Scale(gGfxMatrix, 0.3f, 1.0f, 1.0f, MTXF_APPLY);
                            break;

                        case 2:
                            Matrix_Scale(gGfxMatrix, 1.0f, 0.5f, 1.0f, MTXF_APPLY);
                            break;

                        case 3:
                            Matrix_Scale(gGfxMatrix, 0.5f, 1.0f, 1.0f, MTXF_APPLY);
                            break;
                    }

                    Matrix_Scale(gGfxMatrix, RAND_FLOAT_SEEDED(8.0f) + 8.0f, RAND_FLOAT_SEEDED(8.0f) + 8.0f, 10.0f,
                                 MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, aMetalDebrisDL);
                    Matrix_Pop(&gGfxMatrix);
                }
                break;
            }

            if (gPlayer[0].state == PLAYERSTATE_LEVEL_INTRO) {
                Matrix_Push(&gGfxMatrix);
                Matrix_RotateX(gGfxMatrix, 20.0f * M_DTOR, MTXF_APPLY);
                Matrix_RotateY(gGfxMatrix, (gGameFrameCount * 0.5f) * M_DTOR, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 2.0f, 2.0f, 2.0f, MTXF_APPLY);
            } else {
                Matrix_RotateX(gGfxMatrix, -10.0f * M_DTOR, MTXF_APPLY);
                Matrix_RotateY(gGfxMatrix, (gGameFrameCount * 0.3f) * M_DTOR, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 4.0f, 4.0f, 4.0f, MTXF_APPLY);
                Matrix_Push(&gGfxMatrix);
            }

            Matrix_Translate(gGfxMatrix, 0.f, -590.0f, 0.f, MTXF_APPLY);

            Matrix_SetGfxMtx(&gMasterDisp);

            gSPDisplayList(gMasterDisp++, aBoSatelliteDL);

            Matrix_Push(&gGfxMatrix);

            RCP_SetupDL(&gMasterDisp, SETUPDL_64);

            switch ((gGameFrameCount >> 3) % 4U) {
                case 0:
                    sp2DC = 255.0f;
                    sp2D8 = 0.0f;
                    sp2D4 = 0.0f;
                    break;

                case 1:
                    sp2DC = 0.0f;
                    sp2D8 = 255.0f;
                    sp2D4 = 0.0f;
                    break;

                case 2:
                    sp2DC = 0.0f;
                    sp2D8 = 0.0f;
                    sp2D4 = 255.0f;
                    break;

                case 3:
                    sp2DC = 255.0f;
                    sp2D8 = 255.0f;
                    sp2D4 = 0.0f;
                    break;
            }

            Math_SmoothStepToF(&D_800CA210, sp2DC, 1.0f, 20.0f, 0);
            Math_SmoothStepToF(&D_800CA214, sp2D8, 1.0f, 20.0f, 0);
            Math_SmoothStepToF(&D_800CA218, sp2D4, 1.0f, 20.0f, 0);

            gDPSetPrimColor(gMasterDisp++, 0, 0, (s32) D_800CA210, (s32) D_800CA214, (s32) D_800CA218, 128);

            if (gPlayer[0].state == PLAYERSTATE_LEVEL_INTRO) {
                Matrix_Scale(gGfxMatrix, 1.02f, 1.02f, 1.02f, MTXF_APPLY);
            } else {
                Matrix_Scale(gGfxMatrix, 0.97f, 0.97f, 0.97f, MTXF_APPLY);
            }

            Matrix_SetGfxMtx(&gMasterDisp);

            gDPSetTextureFilter(gMasterDisp++, G_TF_POINT);
            gSPDisplayList(gMasterDisp++, aBoSatelliteLightsDL);
            gDPSetTextureFilter(gMasterDisp++, G_TF_BILERP);

            Matrix_Pop(&gGfxMatrix);
            Matrix_Pop(&gGfxMatrix);

            if (gPlayer[0].state == PLAYERSTATE_LEVEL_COMPLETE) {
                Matrix_Scale(gGfxMatrix, 0.075f, 0.075f, 0.075f, MTXF_APPLY);
                break;
            }
            break;

        case ACTOR_CS_BO_LASER_SHOT:
            RCP_SetupDL_21();
            Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 2.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aLaserShotRedDL);
            break;

        case ACTOR_CS_BO_FIGHTER:
            gSPDisplayList(gMasterDisp++, aBoFighterDL);
            break;

        case ACTOR_CS_CORNERIAN_FIGHTER:
            if ((this->index == 3) && (gPlayer[0].state == PLAYERSTATE_LEVEL_COMPLETE)) {
                gSPDisplayList(gMasterDisp++, aBillShipDL);
            } else {
                gSPDisplayList(gMasterDisp++, aKaCornerianFighterDL);
            }
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -60.0f, MTXF_APPLY);
            Actor_DrawEngineGlow(this, EG_RED);
            break;

        case ACTOR_CS_KA_ENEMY:
            gSPDisplayList(gMasterDisp++, aKaEnemyDL);
            break;

        case ACTOR_CS_SY_SHIP_1_SHRINK:
            Matrix_Scale(gGfxMatrix, 0.125f, 0.125f, 0.125f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            /* fallthrough */
        case ACTOR_CS_SY_SHIP_1:
            gSPDisplayList(gMasterDisp++, aSyShip1DL);
            ActorCutscene_SyShip_Setup(this, 4);
            break;

        case ACTOR_CS_SY_SHIP_2:
            Matrix_Scale(gGfxMatrix, this->scale, this->scale, this->scale, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aSyShip2DL);
            ActorCutscene_SyShip_Setup(this, 0);
            break;

        case ACTOR_CS_ORB_GLOW:
            // @recomp:
            // Fixes the white flash on the right side of the screen during the Sector Y Intro Cutscene.
            if ((gCurrentLevel == LEVEL_SECTOR_Y) && (gCsFrameCount == 350) && (gPlayer[0].csState == 2)) {
                break;
            }
            RCP_SetupDL_49();
            gDPSetPrimColor(gMasterDisp++, 0, 0, this->iwork[0], this->iwork[1], this->iwork[2], this->iwork[3]);
            gDPSetEnvColor(gMasterDisp++, this->iwork[4], this->iwork[5], this->iwork[6], this->iwork[7]);
            Matrix_Scale(gGfxMatrix, this->scale, this->fwork[4], 1.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            break;

        case ACTOR_CS_SY_ROBOT:
            animFrameData = Animation_GetFrameData(D_demo_800CA1F4[this->iwork[4]], this->iwork[5], sp144);
            Math_SmoothStepToVec3fArray(sp144, this->vwork, 1, animFrameData, this->fwork[2], 100.0f, .0f);
            RCP_SetupDL_30(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 150, 255, 150, 255);
            Animation_DrawSkeleton(1, D_SY_602D140, this->vwork, 0, 0, this, &gIdentityMatrix);

            if (this->fwork[0] != 0.0f) {
                RCP_SetupDL_49();
                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
                gDPSetEnvColor(gMasterDisp++, 255, 32, 32, 255);
                Matrix_Translate(gGfxMatrix, 10.0f, -5.0f, -40.0f, MTXF_APPLY);
                Matrix_RotateY(gGfxMatrix, (-this->obj.rot.y) * M_DTOR, MTXF_APPLY);
                Graphics_SetScaleMtx(1.0f);
                Matrix_Scale(gGfxMatrix, this->fwork[0], this->fwork[0], this->fwork[0], MTXF_APPLY);

                camX = gPlayer[0].cam.eye.x - this->obj.pos.x;
                camY = gPlayer[0].cam.eye.y - this->obj.pos.y;
                camZ = gPlayer[0].cam.eye.z - (this->obj.pos.z + gPathProgress);

                y = -Math_Atan2F(camX, camZ);
                x = Math_Atan2F(camY, sqrtf(SQ(camZ) + SQ(camX)));

                Matrix_RotateY(gGfxMatrix, -y, MTXF_APPLY);
                Matrix_RotateX(gGfxMatrix, -x, MTXF_APPLY);

                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);
                Math_SmoothStepToF(&this->fwork[0], 0.0f, 0.1f, 0.2f, 0.05f);
            }

            if (this->fwork[6] != 0.0f) {
                RCP_SetupDL_49();
                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
                gDPSetEnvColor(gMasterDisp++, 255, 48, 0, 255);
                Matrix_Pop(&gGfxMatrix);
                Matrix_Push(&gGfxMatrix);
                Matrix_RotateY(gCalcMatrix, (this->obj.rot.y - 90.0f) * M_DTOR, MTXF_NEW);

                sp2B8.x = 210.0f;
                sp2B8.y = 0.0f;
                sp2B8.z = 40.0f;

                Matrix_MultVec3fNoTranslate(gCalcMatrix, &sp2B8, &sp2AC);
                Matrix_Translate(gGfxMatrix, this->obj.pos.x + sp2AC.x, this->obj.pos.y + sp2AC.y,
                                 this->obj.pos.z + sp2AC.z, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->fwork[6], this->fwork[6], this->fwork[6], MTXF_APPLY);

                camX = gPlayer[0].cam.eye.x - this->obj.pos.x;
                camY = gPlayer[0].cam.eye.y - this->obj.pos.y;
                camZ = gPlayer[0].cam.eye.z - (this->obj.pos.z + gPathProgress);

                y = -Math_Atan2F(camX, camZ);
                x = Math_Atan2F(camY, sqrtf(SQ(camZ) + SQ(camX)));

                Matrix_RotateY(gGfxMatrix, -y, MTXF_APPLY);
                Matrix_RotateX(gGfxMatrix, -x, MTXF_APPLY);

                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);
                Math_SmoothStepToF(&this->fwork[6], 0.00f, 0.1f, 0.6f, 0);
            }
            break;

        case ACTOR_CS_SY_SHOGUN_PLATFORM:
            gSPDisplayList(gMasterDisp++, aSyShogunPlatformDL);
            break;

        case ACTOR_CS_AQ_FISHGROUP:
            Aquas_CsAqFishGroup_Draw(this);
            break;

        case ACTOR_CS_SY_ROBOT_LASER:
            gSPDisplayList(gMasterDisp++, aSyRobotLaserDL);
            break;

        case ACTOR_CS_SY_EXPLOSION:
            RCP_SetupDL_49();

            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->iwork[0]);
            gDPSetEnvColor(gMasterDisp++, 255, 192, 128, 255);

            Matrix_RotateY(
                gGfxMatrix,
                Math_Atan2F(gPlayer[0].cam.eye.x - gPlayer[0].cam.at.x, gPlayer[0].cam.eye.z - gPlayer[0].cam.at.z),
                MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, this->fwork[0], this->fwork[0], this->fwork[0], MTXF_APPLY);

            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aOrbDL);

            if (this->iwork[0] != 0) {
                this->iwork[0] -= 7;
            }
            this->fwork[0] += 0.2f;
            break;

        case ACTOR_CS_AQ_SEAWEED:
            Animation_GetFrameData(&aAqSeaweedAnim1, this->iwork[0], this->vwork);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
            Animation_DrawSkeleton(1, aAqSeaweedSkel, this->vwork, 0, 0, &this->index, &gIdentityMatrix);
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
            break;

        case ACTOR_CS_AQ_BUMP_2:
            Matrix_Scale(gGfxMatrix, 0.5f, 0.5f, 0.5f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aAqBump2DL);
            break;

        case ACTOR_CS_AQ_CORAL_REEF_2:
            gSPDisplayList(gMasterDisp++, aAqCoralReef2DL);
            break;

        case ACTOR_CS_AQ_ROCK:
            gSPDisplayList(gMasterDisp++, aAqRockDL);
            break;
    }
}
#endif