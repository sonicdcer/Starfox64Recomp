#define RECOMP_PATCH __attribute__((section(".recomp_patch")))

#include "patches.h"

#if 1 // full scope

#define TEAM_HEAD_XROT (19)
#define TEAM_HEAD_YROT (20)

extern Vec3f sViewPos;
extern bool sDrewActor;
extern Vec3f gLockOnTargetViewPos[];
extern Matrix D_edisplay_801615F0;
extern Vec3f D_edisplay_801615D0;
extern Gfx D_VE2_6007650[];

void Boss_SetMatrix(Object* obj, s32 drawType);
void Object_SetCullDirection(s32 arg0);
void Object_SetMatrix(Object* obj, s32 drawType);
void Corneria_SpawnClouds(void);
void Corneria_CsTeamSetup(ActorCutscene* this, s32 teamIdx);
void Macbeth_Effect357_Spawn2(f32 xPos, f32 yPos, f32 zPos, f32 arg3);
void Macbeth_801AF27C(ActorCutscene* this, s32 arg1);
void Macbeth_801AF44C(void);
void Audio_FadeOutAll(u8);
void PlayerShot_DrawLaser(PlayerShot* shot);
void Solar_801A0DF8(f32 xPos, f32 zPos, f32 yRot, s32 index, f32 yPos);
void Solar_801A0FD4(ActorCutscene* this, s32 index);
void Solar_SoFlare_Spawn3(f32 xPos, f32 yPos, f32 zPos, f32 scale2);

// for draw distance tests
#if 0
extern int gXoffsetBounds;
extern int gYoffsetBounds;
extern int gBackZdist;
extern int gFrontZdist;
#endif

#if 1
RECOMP_PATCH void Actor_DrawAllRange(Actor* this) {
    s32 playerNum;
    Vec3f srcViewPos = { 0.0f, 0.0f, 0.0f };
    f32 var_fa1;
    f32 var_ft5;
    f32 var_fv0;
    f32 var_fv1;

    sDrewActor = false;

    if (this->info.drawType == 2) {
        Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, gPathProgress, MTXF_APPLY);
        Matrix_Translate(gCalcMatrix, this->obj.pos.x, this->obj.pos.y, this->obj.pos.z, MTXF_NEW);
        Matrix_Push(&gGfxMatrix);
        Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
        Matrix_MultVec3f(gGfxMatrix, &srcViewPos, &sViewPos);
        Matrix_Pop(&gGfxMatrix);

        var_fv0 = 0.0f;
        var_fv1 = -12000.0f;
        if ((this->obj.id == OBJ_ACTOR_ALLRANGE) && (this->aiType >= AI360_GREAT_FOX)) {
            var_fv0 = 1000.0f;
            var_fv1 = -25000.0f;
        }

        // @recomp draw no matter what
        goto render;

        if ((var_fv0 > sViewPos.z) && (sViewPos.z > var_fv1)) {
            // @recomp: Extend Actor visibility to widescreen.
            if (fabsf(sViewPos.x) < (fabsf(sViewPos.z * /*0.5f*/ 1.5f) + 500.0f)) {
                if (fabsf(sViewPos.y) < (fabsf(sViewPos.z * 0.5f) + 500.0f)) {
                render:
                    Matrix_RotateY(gCalcMatrix, this->obj.rot.y * M_DTOR, MTXF_APPLY);
                    Matrix_RotateX(gCalcMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
                    Matrix_RotateZ(gCalcMatrix, this->obj.rot.z * M_DTOR, MTXF_APPLY);
                    this->info.draw(&this->obj);
                    sDrewActor = true;
                    if ((gPlayer[0].state == PLAYERSTATE_ACTIVE) && (this->obj.id == OBJ_ACTOR_ALLRANGE) &&
                        (this->aiType == AI360_MISSILE)) {
                        gTeamArrowsViewPos[0] = sViewPos;
                    }
                }
            }
        }
    } else {
        Matrix_Translate(gGfxMatrix, this->obj.pos.x, this->obj.pos.y, this->obj.pos.z, MTXF_APPLY);
        Matrix_MultVec3f(gGfxMatrix, &srcViewPos, &sViewPos);

        if ((gPlayer[0].state == PLAYERSTATE_LEVEL_INTRO) || (gPlayer[0].state == PLAYERSTATE_STANDBY) ||
            ((this->obj.id == OBJ_ACTOR_ALLRANGE) && (this->aiType >= AI360_GREAT_FOX)) ||
            ((this->obj.id == OBJ_ACTOR_CUTSCENE) && (this->info.bonus != 0))) {
            var_ft5 = var_fv0 = 3000.0f;
            var_fv1 = -29000.0f;
            var_fa1 = 0.7f;
        } else {
            var_ft5 = 500.0f;
            var_fv0 = 0.0f;
            var_fv1 = -20000.0f;
            var_fa1 = 0.5f;
        }

        // @recomp draw no matter what
        goto render2;

        if ((var_fv0 > sViewPos.z) && (sViewPos.z > var_fv1)) {
            if (fabsf(sViewPos.x) < (fabsf(sViewPos.z * var_fa1) + var_ft5)) {
                if (fabsf(sViewPos.y) < (fabsf(sViewPos.z * var_fa1) + var_ft5)) {
                render2:
                    if (this->info.draw != NULL) {
                        Matrix_RotateY(gGfxMatrix, this->obj.rot.y * M_DTOR, MTXF_APPLY);
                        Matrix_RotateX(gGfxMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
                        Matrix_RotateZ(gGfxMatrix, this->obj.rot.z * M_DTOR, MTXF_APPLY);
                        Matrix_SetGfxMtx(&gMasterDisp);
                        this->info.draw(&this->obj);
                        sDrewActor = true;
                        if ((gPlayer[0].state == PLAYERSTATE_ACTIVE) &&
                            (((this->obj.id == OBJ_ACTOR_ALLRANGE) &&
                              ((this->aiType <= AI360_PEPPY) || (this->aiType == AI360_KATT) ||
                               (this->aiType == AI360_BILL))) ||
                             (this->obj.id == OBJ_ACTOR_TEAM_BOSS))) {
                            gTeamArrowsViewPos[this->aiType] = sViewPos;
                        }
                    }
                }
            }
        }
    }

    if (!sDrewActor) {
        this->lockOnTimers[gPlayerNum] = 0;
        if ((this->obj.id == OBJ_ACTOR_DEBRIS) || (this->obj.id == OBJ_ACTOR_AND_BRAIN_WASTE)) {
            Object_Kill(&this->obj, this->sfxSource);
        }
    }

    for (playerNum = 0; playerNum < gCamCount; playerNum++) {
        if (this->lockOnTimers[playerNum] != 0) {
            gLockOnTargetViewPos[playerNum] = sViewPos;
            gLockOnTargetViewPos[playerNum].y += this->info.targetOffset;
            if ((playerNum == gPlayerNum) && (gLockOnTargetViewPos[playerNum].z > -300.0f)) {
                this->lockOnTimers[playerNum] = 0;
            }
        }
    }

    Object_SetSfxSourceToView(this->sfxSource, &sViewPos);
    this->iwork[24] = sDrewActor;
}
#endif

/**
 * Problems:
 * Causing KaSaucerer not to spawn in Katina. Provisional patch: Except katina in the patch
 */
#if 1
RECOMP_PATCH void Boss_Draw(Boss* this, s32 arg1) {
    f32 var_fa1;
    f32 var_ft5;
    f32 var_fv0;
    f32 var_fv1;
    f32 sp3C;
    Vec3f origin = { 0.0f, 0.0f, 0.0f };

    if (this->info.unk_19 != 0) {
        this->obj.pos.y += this->yOffset + gCameraShakeY;
        Boss_SetMatrix(&this->obj, this->info.drawType);
        this->obj.pos.y -= this->yOffset + gCameraShakeY;
    } else {
        Boss_SetMatrix(&this->obj, this->info.drawType);
    }

    Matrix_MultVec3f(&D_edisplay_801615F0, &origin, &D_edisplay_801615D0);
    Object_SetSfxSourceToView(this->sfxSource, &D_edisplay_801615D0);

    if ((this->obj.id == OBJ_BOSS_KA_SAUCERER) || (this->obj.id == OBJ_BOSS_KA_FLBASE)) {
        var_fa1 = 6000.0f;
        var_fv0 = 6000.0f;
        var_ft5 = 0.9f;
        var_fv1 = -20000.0f;

    } else if (this->obj.id == OBJ_BOSS_SZ_GREAT_FOX) {
        var_fv1 = -25000.0f;
        var_ft5 = 0.7f;
        var_fa1 = 3000.0f;
        var_fv0 = 2000.0f;
    } else {
        var_ft5 = 0.5f;
        var_fa1 = 2000.0f;
        var_fv0 = 1000.0f;
        var_fv1 = -15000.0f;
    }

    sp3C = -1.0f;

    // @recomp draw no matter what
    if ((gCurrentLevel != LEVEL_KATINA) &&
        (gCurrentLevel != LEVEL_SECTOR_Y)) { // Excepting Katina because of KaSaucerer's bug
        goto render;
    }

    if ((D_edisplay_801615D0.z < var_fv0) && (var_fv1 < D_edisplay_801615D0.z)) {
        if (fabsf(D_edisplay_801615D0.x) < (fabsf(D_edisplay_801615D0.z * (var_ft5 + 1.0f)) + var_fa1)) {
            if (fabsf(D_edisplay_801615D0.y) < (fabsf(D_edisplay_801615D0.z * var_ft5) + var_fa1)) {
            render:
                sp3C = 1.0f;
                if (this->obj.id != OBJ_BOSS_BO_BASE) {
                    if (this->obj.id != OBJ_BOSS_KA_SAUCERER) {
                        Display_SetSecondLight(&this->obj.pos);
                    }
                    if (this->info.drawType != 2) {
                        Matrix_SetGfxMtx(&gMasterDisp);
                    }
                    if (arg1 < 0) {
                        Object_ApplyWaterDistortion();
                    }
                    this->info.draw(&this->obj);
                }
            }
        }
    }
    D_edisplay_801615D0.y = sp3C;

    if (this->obj.id == OBJ_BOSS_BO_BASE) {
        this->vwork[30] = D_edisplay_801615D0;
        Display_SetSecondLight(&this->obj.pos);
        Matrix_SetGfxMtx(&gMasterDisp);
        this->info.draw(&this->obj);
    }
}
#endif

/**
 * Problems:
 * Causing crash in Sector Y Boss, omitting that level for now.
 */
#if 1
RECOMP_PATCH void Scenery360_Draw(Scenery360* this) {
    Vec3f src = { 0.0f, 0.0f, 0.0f };
    Vec3f dest;

    f32 behindZdist = 1000.0f;
    f32 frontZdist = -12000.0f;
    f32 xyOffsetBounds = 2000.0f + 1000.0f;
    f32 xyObjDistBoundMod = 0.5f;

    if (this->obj.id == OBJ_SCENERY_SY_SHOGUN_SHIP) {
        behindZdist = 4000.0f;
        frontZdist = -13000.0f;
        xyOffsetBounds = 4500.0f;
    } else if (gCurrentLevel == LEVEL_VENOM_ANDROSS) {
        frontZdist = -20000.0f;
        xyObjDistBoundMod = 0.4f;
    } else if (this->obj.id == OBJ_SCENERY_VS_KA_FLBASE) {
        behindZdist = 6000.0f;
        frontZdist = -20000.0f;
        xyOffsetBounds = 6000.0f;
        xyObjDistBoundMod = 0.9f;
    }

    if ((gLevelType == LEVELTYPE_PLANET) || (gCurrentLevel == LEVEL_BOLSE)) {
        Matrix_Translate(gGfxMatrix, this->obj.pos.x, this->obj.pos.y + gCameraShakeY, this->obj.pos.z, MTXF_APPLY);
    } else {
        Matrix_Translate(gGfxMatrix, this->obj.pos.x, this->obj.pos.y, this->obj.pos.z, MTXF_APPLY);
    }

    Matrix_MultVec3f(gGfxMatrix, &src, &dest);

    if (gCurrentLevel == LEVEL_SECTOR_Z) {
        behindZdist = 6000.0f;
        frontZdist = -20000.0f * 2;
        xyOffsetBounds = 6000.0f * 2;
        xyObjDistBoundMod = 0.9f;
        goto check;
    }

    if ((gCurrentLevel != LEVEL_SECTOR_Y) && (gCurrentLevel != LEVEL_VENOM_ANDROSS)) {
        goto render;
    }

check:
    if ((dest.z < behindZdist) && (frontZdist < dest.z)) {
        if (fabsf(dest.y) < (fabsf(dest.z * xyObjDistBoundMod) + xyOffsetBounds)) {
            if (fabsf(dest.x) < (fabsf(dest.z * xyObjDistBoundMod + 1.0f) + xyOffsetBounds)) {
            render:
                Display_SetSecondLight(&this->obj.pos);

                if (this->obj.id == OBJ_SCENERY_AND_PASSAGE) {
                    Matrix_RotateY(gGfxMatrix, this->obj.rot.y * M_DTOR, MTXF_APPLY);
                    Matrix_RotateX(gGfxMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, this->obj.rot.z * M_DTOR, MTXF_APPLY);
                    Matrix_RotateY(gGfxMatrix, M_PI / 2, MTXF_APPLY);
                    Matrix_Translate(gGfxMatrix, -551.0f, 0.0f, 0.0f, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, D_VE2_6007650);
                } else {
                    Matrix_RotateY(gGfxMatrix, this->obj.rot.y * M_DTOR, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);

                    gSPDisplayList(gMasterDisp++, this->info.dList);
                }
            }
        }
    }
}

/*
void SectorY_8019AEC0(SyShogun*);
void SectorY_80198244(SyShogun*);
extern f32 D_SY_60342A0[];
extern f32 D_SY_6034304[];
extern Gfx aSySaruzinDL[];

RECOMP_PATCH void SectorY_SyShogun_Init(SyShogun* this) {
    gScenery360[0].info.dList = NULL; // @recomp initialize dList to NULL to avoid crash
    this->fwork[9] = 0.0f;
    this->swork[33] = 5500;
    this->timer_050 = 10;
    this->timer_058 = 0;

    D_ctx_80177A10[8] = 0;

    if (this->index == 0) {
        this->info.hitbox = SEGMENTED_TO_VIRTUAL(D_SY_60342A0);
        this->health = 150;

        this->swork[28] = 5;
        this->fwork[43] = 3.5f;
        this->fwork[45] = 40.0f;

        if (gPlayer[0].state == PLAYERSTATE_START_360) {
            this->obj.pos.z = -28900.0f;
            gScenery360[0].obj.pos.z = -30000.0f;
        }

        this->rot_078.y = 0.0f;
        SectorY_8019AEC0(this);
    } else {
        this->fwork[34] = 2.8f;
        this->info.hitbox = SEGMENTED_TO_VIRTUAL(D_SY_6034304);
        this->health = 100;
        this->swork[28] = 0;
        this->swork[25] = 1;
        this->fwork[45] = 35.0f;

        if (this->index == 1) {
            this->rot_078.y = 15.0f;
        } else {
            this->rot_078.y = 345.0f;
        }

        this->vel.x = SIN_DEG(this->rot_078.y) * this->fwork[45] * 0.2f;
        this->vel.z = COS_DEG(this->rot_078.y) * this->fwork[45] * 0.2f;
        SectorY_80198244(this);
        this->timer_056 = 250;
    }

    if (gLevelMode == LEVELMODE_ON_RAILS) {
        if (gPlayer[0].state == PLAYERSTATE_ACTIVE) {
            gPlayer[0].state = PLAYERSTATE_START_360;
            gPlayer[0].csState = 0;
            SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM, 50);
            SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_FANFARE, 50);
        }
        Object_Kill(&this->obj, this->sfxSource);
    }
}
*/
#endif

/**
 * Problems:
 * Game crash before the Titania's Boss with 50% more cullingdistance
 */
#if 1
RECOMP_PATCH void Scenery_Move(Scenery* this) {
    if (gPlayer[0].state == PLAYERSTATE_LEVEL_INTRO) {
        this->obj.pos.z += this->effectVel.z;
        if ((this->info.cullDistance * 1.5f) < this->obj.pos.z) {
            Object_Kill(&this->obj, this->sfxSource);
        }
    } else if ((gLevelMode == LEVELMODE_ON_RAILS) && (gBossActive != 2)) {
        f32 temp_fv0 = fabsf(this->obj.pos.x - gPlayer[0].cam.eye.x);
        f32 var_fa0 = 500.0f;

        if ((this->obj.id == OBJ_SCENERY_CO_HIGHWAY_1) || (this->obj.id == OBJ_SCENERY_CO_HIGHWAY_2)) {
            var_fa0 = 1000.0f;
        }
        temp_fv0 = ((temp_fv0 - var_fa0) < 0.0f) ? 0.0f : (temp_fv0 - var_fa0) * 1.7f;
        if ((fabsf(gPlayer[0].yRot_114) > 1.0f) || (gCurrentLevel == LEVEL_MACBETH)) {
            temp_fv0 = 0.0f;
        }
        temp_fv0 -= gPlayer[0].cam.eye.z;

        // @recomp increase cullDistance by 50%.
        f32 recompCulldistance = 1.5f;

        if (gCurrentLevel == LEVEL_TITANIA) {
            recompCulldistance = 1.0f;
        }

        if (((this->info.cullDistance * recompCulldistance) - temp_fv0) < (this->obj.pos.z + gPathProgress)) {
            Object_Kill(&this->obj, this->sfxSource);
        }
    }
}
#endif

#if 1
RECOMP_PATCH void Item_Draw(Item* this, s32 arg1) {
    Vec3f src = { 0.0f, 0.0f, 0.0f };
    Vec3f dest;
    u8 drawn;

    Matrix_Translate(gGfxMatrix, this->obj.pos.x, this->obj.pos.y, this->obj.pos.z + gPathProgress, MTXF_APPLY);
    Matrix_MultVec3f(gGfxMatrix, &src, &dest);

    drawn = false;

    if ((dest.z < 0.0f) && (dest.z > -12000.0f)) {
        // @recomp: Extend draw distance up to 32/9
        if (fabsf(dest.x) < (fabsf(dest.z * /* 0.5f */ 1.5f) + 500.0f)) {
            if (fabsf(dest.y) < (fabsf(dest.z * 0.5f) + 500.0f)) {
                if (this->info.draw != NULL) {
                    Matrix_RotateY(gGfxMatrix, this->obj.rot.y * M_DTOR, MTXF_APPLY);
                    Matrix_RotateX(gGfxMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, this->obj.rot.z * M_DTOR, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    if (this->info.drawType == 0) {
                        gSPDisplayList(gMasterDisp++, this->info.dList);
                    } else {
                        this->info.draw(&this->obj);
                    }
                }
                drawn = true;
            }
        }
    }

    Object_SetSfxSourceToView(this->sfxSource, &dest);

    if (!drawn && (gLevelMode == LEVELMODE_ALL_RANGE) && (gCamCount == 1) && (this->obj.id < OBJ_ITEM_GOLD_RING) &&
        (gCurrentLevel != LEVEL_VENOM_ANDROSS)) {
        Object_Kill(&this->obj, this->sfxSource);
    }
}
#endif

#if 1
RECOMP_PATCH void Effect_DrawAllRange(Effect* this) {
    Vec3f src = { 0.0f, 0.0f, 0.0f };
    Vec3f dest;
    f32 minZ;
    u8 drawn = false;

    if (this->info.unk_14 == -1) {
        Matrix_Translate(gGfxMatrix, this->obj.pos.x, this->obj.pos.y + gCameraShakeY, this->obj.pos.z + gPathProgress,
                         MTXF_APPLY);
    } else {
        Matrix_Translate(gGfxMatrix, this->obj.pos.x, this->obj.pos.y, this->obj.pos.z + gPathProgress, MTXF_APPLY);
    }

    Matrix_MultVec3f(gGfxMatrix, &src, &dest);

    if ((gCurrentLevel == LEVEL_SECTOR_Z) || (gCurrentLevel == LEVEL_BOLSE)) {
        minZ = -20000.0f;
    } else {
        minZ = -10000.0f;
    }

    if ((dest.z < 0.0f) && (minZ < dest.z)) {
        // @recomp: Extend draw distance up to 32/9
        if (fabsf(dest.x) < (fabsf(dest.z * /* 0.5f */ 1.5f) + 500.0f)) {
            if (fabsf(dest.y) < (fabsf(dest.z * 0.5f) + 500.0f)) {
                if (this->info.draw != NULL) {
                    Matrix_RotateY(gGfxMatrix, this->obj.rot.y * M_DTOR, MTXF_APPLY);
                    Matrix_RotateX(gGfxMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, this->obj.rot.z * M_DTOR, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    this->info.draw(&this->obj);
                }
                drawn = true;
            }
        }
    }

    Object_SetSfxSourceToView(this->sfxSource, &dest);

    if (!drawn && (this->obj.id != OBJ_EFFECT_CLOUDS) && (this->obj.id != OBJ_EFFECT_TIMED_SFX) && (!gVersusMode)) {
        Object_Kill(&this->obj, this->sfxSource);
    }
}
#endif

#if 1
// @recomp: Fix Falco's positioning to account for widescreen.
RECOMP_PATCH void Corneria_LevelStart(Player* player) {
    s32 i;
    ActorCutscene* falco = &gActors[0];
    ActorCutscene* slippy = &gActors[1];
    ActorCutscene* peppy = &gActors[2];
    f32 sp44;
    f32 sp40;
    f32 x;
    f32 y;
    f32 z;
    f32 sp30;
    f32 sp2C;

    // Corneria_SpawnTerrainBumps(); // This function call was probably around here.

    PRINTF("Enms[1].obj.mode %d\n", gActors[1].obj.status);

    if (gCsFrameCount < 815) {
        x = player->pos.x;
        y = player->pos.y + 15.0f;
        z = player->trueZpos - 20.0f;
    } else {
        if (gCsFrameCount < 1009) {
            x = falco->obj.pos.x;
            y = falco->obj.pos.y + 15.0f;
            z = falco->obj.pos.z - 20.0f;
        } else if (gCsFrameCount < 1198) {
            x = peppy->obj.pos.x;
            y = peppy->obj.pos.y + 15.0f;
            z = peppy->obj.pos.z - 20.0f;
        } else {
            x = slippy->obj.pos.x;
            y = slippy->obj.pos.y + 15.0f;
            z = slippy->obj.pos.z - 20.0f;
        }
    }

    sp2C = -Math_Atan2F(player->cam.eye.x - x, player->cam.eye.z - z);
    sp30 = -Math_Atan2F(player->cam.eye.y - y, sqrtf(SQ(player->cam.eye.z - z) + SQ(player->cam.eye.x - x)));

    sp44 = Math_RadToDeg(sp2C) - D_ctx_80177A48[4];
    sp40 = Math_RadToDeg(sp30) - D_ctx_80177A48[5];

    if (sp44 > 180.0f) {
        sp44 -= 360.0f;
    }
    if (sp44 < -180.0f) {
        sp44 += 360.0f;
    }
    if (sp40 > 180.0f) {
        sp40 -= 360.0f;
    }
    if (sp40 < -180.0f) {
        sp40 += 360.0f;
    }

    D_ctx_80177A48[6] += fabsf(sp44);
    D_ctx_80177A48[7] += fabsf(sp40);

    // Cloud reflexions on Arwing windshields
    if (sp2C >= 0.0f) {
        Lib_Texture_Scroll(aAwCockpitGlassClouldReflextionTex, 64, 32, 2);
        Lib_Texture_Scroll(aAwCockpitGlassClouldReflextionTex, 64, 32, 2);
    } else {
        Lib_Texture_Scroll(aAwCockpitGlassClouldReflextionTex, 64, 32, 3);
        Lib_Texture_Scroll(aAwCockpitGlassClouldReflextionTex, 64, 32, 3);
    }

    for (i = 0; (i < 40) && (D_ctx_80177A48[6] >= 0.2f); i++, D_ctx_80177A48[6] -= 0.2f) {
        if (sp44 >= 0) {
            Lib_Texture_Scroll(aAwCockpitGlassClouldReflextionTex, 64, 32, 2);
        } else {
            Lib_Texture_Scroll(aAwCockpitGlassClouldReflextionTex, 64, 32, 3);
        }
    }

    for (i = 0; (i < 40) && (D_ctx_80177A48[7] >= 0.3f); i++, D_ctx_80177A48[7] -= 0.3f) {
        if (sp40 >= 0) {
            Lib_Texture_Scroll(aAwCockpitGlassClouldReflextionTex, 64, 32, 0);
        } else {
            Lib_Texture_Scroll(aAwCockpitGlassClouldReflextionTex, 64, 32, 1);
        }
    }

    D_ctx_80177A48[4] = Math_RadToDeg(sp2C);
    D_ctx_80177A48[5] = Math_RadToDeg(sp30);

    player->flags_228 = 0;

    D_ctx_80177950 = -1.0f;
    if ((Math_RadToDeg(gPlayer[0].camYaw) < 90.0f) || (Math_RadToDeg(gPlayer[0].camYaw) > 270.0f)) {
        D_ctx_80177950 = 1.0f;
    }

    player->vel.z = 0.0f;
    player->pos.z = player->pos.z;
    player->trueZpos = player->pos.z + player->camDist;
    player->bobPhase += 10.0f;
    player->yBob = -SIN_DEG(player->bobPhase) * 0.5f;
    player->rockPhase += 3.0f;
    player->rockAngle = SIN_DEG(player->rockPhase) * 1.5f;

    Corneria_SpawnClouds();

    player->arwing.teamFaceXrot = 0;

    switch (player->csState) {
        case 0: // LevelStart initialization
            gCsFrameCount = 0;
            player->csState = 1;
            player->csTimer = 600;
            player->pos.y = 6000.0f;
            player->pos.x = 0.1f;

            Corneria_CsTeamSetup(falco, 0);
            Corneria_CsTeamSetup(slippy, 1);
            Corneria_CsTeamSetup(peppy, 2);

            falco->iwork[14] = 2;
            slippy->iwork[14] = 3;
            peppy->iwork[14] = 4;

            player->cam.eye.x = gCsCamEyeX = player->pos.x - 400.0f;
            gPlayer[0].cam.eye.y = gCsCamEyeY = player->pos.y + 600.0f;
            player->cam.eye.z = gCsCamEyeZ = player->trueZpos + 2000.0f;

            player->cam.at.x = gCsCamAtX = player->pos.x;
            player->cam.at.y = gCsCamAtY = player->pos.y;
            player->cam.at.z = gCsCamAtZ = player->trueZpos + 300.0f;

            D_ctx_80177A48[0] = 0;
            D_ctx_80177A48[1] = D_ctx_80177A48[2] = 0;

            gFillScreenAlphaTarget = 255;
            gFillScreenAlpha = 255;
            gFillScreenRed = gFillScreenGreen = gFillScreenBlue = 255;
            break;

        case 1: // cs phase: camera focus on fox (open the wings!)
            if (player->csTimer < 550) {
                gFillScreenAlphaTarget = 0;
                gFillScreenAlphaStep = 3;
                Math_SmoothStepToF(&D_ctx_80177A48[0], 0.01f, 1.0f, 0.0005f, 0.0f);
            }

            gCsCamEyeX = player->pos.x - 150.0f;
            gCsCamEyeY = player->pos.y - 70.0f;
            gCsCamEyeZ = player->trueZpos + 150.0f;

            gCsCamAtX = player->pos.x;
            gCsCamAtY = player->pos.y;
            gCsCamAtZ = player->trueZpos;

            if (player->csTimer == 0) {
                player->csState = 2;
                player->csTimer = 130;
                D_ctx_80177A48[0] = 0.0f;
            }

            if (player->csTimer == 315) {
                player->pos.x = 0.0f;
            }

            if (player->csTimer == 270) {
                gHideRadio = false;
                Radio_PlayMessage(gMsg_ID_2005, RCID_FOX);
            }

            if (player->csTimer == 180) {
                AUDIO_PLAY_SFX(NA_SE_WING_OPEN, player->sfxSource, 0);
            }

            if (player->csTimer == 120) {
                AUDIO_PLAY_SFX(NA_SE_WING_OPEN_END, player->sfxSource, 0);
            }

            if ((player->csTimer < 190) && (player->csTimer > 150)) {
                Math_SmoothStepToF(&player->arwing.wingsZrot, 2.0f, 0.2f, 0.5f, 0.0f);
            }

            if (player->csTimer < 150) {
                player->wingPosition = 0;
            }

            if ((player->csTimer < 120) && ((player->csTimer % 16) == 0)) {
                D_ctx_80177A48[1] = RAND_FLOAT_CENTERED(60.0f);
                D_ctx_80177A48[2] = RAND_FLOAT_CENTERED(60.0f);
            }

            if (player->csTimer == 0) {
                D_ctx_80177A48[1] = 0.0f;
                D_ctx_80177A48[2] = D_ctx_80177A48[1];
            }
            break;

        case 2: // camera goes to the side of Fox.
            Math_SmoothStepToF(&D_ctx_80177A48[0], 0.1f, 1.0f, 0.001f, 0.0f);

            gCsCamEyeX = player->pos.x - 50.0f;
            gCsCamEyeY = player->pos.y + 10.0f;
            gCsCamEyeZ = player->trueZpos - 10.0f;

            gCsCamAtX = player->pos.x;
            gCsCamAtY = player->pos.y + 10.0f;
            gCsCamAtZ = player->trueZpos + 10.0f;

            if (player->csTimer == 20) {
                Radio_PlayMessage(gMsg_ID_2010, RCID_FOX);
            }

            if (player->csTimer == 0) {
                player->csState = 3;
                player->csTimer = 180;
                player->unk_004 = 0.0f;
                falco->state = 0;
                peppy->state = 0;
                slippy->state = 0;
                falco->obj.pos.y = player->pos.y + 80.0f;
                // @recomp: adjust Falco's position so he appears from further right
                falco->obj.pos.z += 3.0f * 100.0f;
            }

            if (gMsgCharIsPrinting && ((gGameFrameCount & 2) != 0)) {
                player->arwing.teamFaceXrot = 5.0f;
            }
            break;

        case 3: // Falco appears on scene from behind

            // @recomp: adjust Falco's maxStep so he flies faster in compensation for the previous change
            if (fabsf(Math_SmoothStepToF(&falco->obj.pos.z, player->pos.z + 100.0f, 0.05f, 3.0f * 5.0f, 0.0f)) < 1.0f) {
                player->csState = 4;
                D_ctx_80177A48[0] = 0.0f;
                player->csTimer = 190;
            }

            if (gMsgCharIsPrinting && ((gGameFrameCount & 2) != 0)) {
                player->arwing.teamFaceXrot = 5.0f;
            }

            gCsCamEyeY = player->pos.y + 10.0f;
            gCsCamAtY = player->pos.y + 10.0f;
            break;

        case 4:
            if (gMsgCharIsPrinting && ((gGameFrameCount & 2) != 0)) {
                player->arwing.teamFaceXrot = 5.0f;
            }

            Math_SmoothStepToF(&D_ctx_80177A48[0], 0.1f, 1.0f, 0.001f, 0.0f);

            // Focus camera on Falco.
            gCsCamEyeX = falco->obj.pos.x - 50.0f;
            gCsCamEyeY = falco->obj.pos.y + 10.0f;
            gCsCamEyeZ = falco->obj.pos.z - 10.0f;

            gCsCamAtX = falco->obj.pos.x;
            gCsCamAtY = falco->obj.pos.y + 10.0f;
            gCsCamAtZ = falco->obj.pos.z + 10.0f;

            if (player->csTimer == 0) {
                player->csState = 5;
                player->csTimer = 5;
            }

            if (player->csTimer == 80) {
                Radio_PlayMessage(gMsg_ID_2020, RCID_FALCO);
            }

            // Falco looks towards the camera.
            if (player->csTimer < 100) {
                Math_SmoothStepToF(&falco->fwork[TEAM_HEAD_XROT], 50.0f, 0.1f, 3.0f, 0.01f);
            }

            // Falco's head rocks back and forth as he speaks.
            falco->fwork[TEAM_HEAD_YROT] = 0.0f;
            if (gMsgCharIsPrinting && ((gGameFrameCount & 2) != 0)) {
                falco->fwork[TEAM_HEAD_YROT] = 5.0f;
            }
            break;

        case 5:
            // Falco's head rotates back.
            Math_SmoothStepToF(&falco->fwork[TEAM_HEAD_XROT], 0.0f, 0.1f, 3.0f, 0.01f);

            if (player->csTimer == 0) {
                player->csState = 6;
                D_ctx_80177A48[0] = 0.0f;
                player->csTimer = 190;
            }

            // Camera moves towards Peppy.
            gCsCamEyeY = falco->obj.pos.y + 10.0f;
            gCsCamAtY = falco->obj.pos.y + 10.0f;
            break;

        case 6:
            // Falco's head rotates back.
            Math_SmoothStepToF(&falco->fwork[TEAM_HEAD_XROT], 0.0f, 0.1f, 3.0f, 0.01f);
            Math_SmoothStepToF(&D_ctx_80177A48[0], 0.1f, 1.0f, 0.001f, 0.0f);

            D_ctx_80177A48[3] -= 0.5f;

            // Focus camera on Peppy
            gCsCamEyeX = peppy->obj.pos.x + 100.0f + D_ctx_80177A48[3];
            gCsCamEyeY = peppy->obj.pos.y + 10.0f;
            gCsCamEyeZ = peppy->obj.pos.z - 70.0f;

            gCsCamAtX = peppy->obj.pos.x + 20.0f + (D_ctx_80177A48[3] * 0.5f);
            gCsCamAtY = peppy->obj.pos.y + 10.0f;
            gCsCamAtZ = peppy->obj.pos.z + 10.0f;

            if (player->csTimer == 0) {
                player->csState = 7;
                player->csTimer = 190;
                D_ctx_80177A48[0] = 0.0f;
                falco->obj.pos.y = player->pos.y;
                falco->obj.pos.z = player->trueZpos + 240.0f;
            }

            if (player->csTimer == 80) {
                Radio_PlayMessage(gMsg_ID_2030, RCID_PEPPY);
            }

            // Peppy's head rocks back and forth as he speaks.
            peppy->fwork[TEAM_HEAD_YROT] = 0.0f;
            if (gMsgCharIsPrinting && ((gGameFrameCount & 2) != 0)) {
                peppy->fwork[TEAM_HEAD_YROT] = 5.0f;
            }
            break;

        case 7:
            Math_SmoothStepToF(&D_ctx_80177A48[0], 0.1f, 1.0f, 0.001f, 0.0f);

            // Focus camera on Slippy.
            gCsCamEyeX = slippy->obj.pos.x + 20.0f;
            gCsCamEyeY = slippy->obj.pos.y + 10.0f;
            gCsCamEyeZ = slippy->obj.pos.z - 50.0f;

            gCsCamAtX = slippy->obj.pos.x + 10.0f;
            gCsCamAtY = slippy->obj.pos.y + 10.0f;
            gCsCamAtZ = slippy->obj.pos.z + 10.0f;

            if (player->csTimer == 0) {
                player->csState = 8;
                D_ctx_80177A48[0] = 0.0f;
                player->csTimer = 300;
                D_ctx_80177A48[8] = 50.0f;
                D_ctx_80177A48[3] = 0.0f;
            }

            if (player->csTimer == 80) {
                Radio_PlayMessage(gMsg_ID_2040, RCID_SLIPPY);
                player->pos.x = 0.1f;
            }

            if (player->csTimer < 100) {
                Math_SmoothStepToF(&slippy->fwork[TEAM_HEAD_XROT], -20.0f, 0.1f, 3.0f, 0.01f);
            }

            // Slippy's head rocks back and forth as he speaks.
            slippy->fwork[TEAM_HEAD_YROT] = 0.0f;
            if (gMsgCharIsPrinting && ((gGameFrameCount & 2) != 0)) {
                slippy->fwork[TEAM_HEAD_YROT] = 5.0f;
            }
            break;

        case 8:
            Math_SmoothStepToF(&D_ctx_80177A48[0], 0.1f, 1.0f, 0.001f, 0.0f);

            if (player->csTimer < 150) {
                D_ctx_80177A48[3] += player->unk_004;
                Math_SmoothStepToF(&player->unk_004, 2.0f, 1.0f, 0.2f, 0.0f);
            }

            // Focus camera on Fox.
            gCsCamEyeX = player->pos.x;
            gCsCamEyeZ = (player->trueZpos - 600.0f) + D_ctx_80177A48[3];
            gCsCamEyeY = player->pos.y + D_ctx_80177A48[8];

            gCsCamAtX = player->pos.x;
            gCsCamAtY = player->pos.y + 20.0f;
            gCsCamAtZ = player->trueZpos + 100.0f;

            if (player->csTimer < 100) {
                Math_SmoothStepToF(&D_ctx_80177A48[8], 10.0f, 0.1f, 0.7f, 0.0f);
            }

            if (player->csTimer == 200) {
                Radio_PlayMessage(gMsg_ID_2050, RCID_FOX);
            }

            // Fox's head rotates back and forth as he speaks.
            player->arwing.teamFaceXrot = 0.0f;
            if (gMsgCharIsPrinting && ((gGameFrameCount & 2) != 0)) {
                player->arwing.teamFaceXrot = 5.0f;
            }

            if (player->csTimer == 80) {
                falco->fwork[29] = 5.0f;
            }

            if (player->csTimer == 60) {
                slippy->fwork[29] = 5.0f;
            }

            if (player->csTimer == 40) {
                peppy->fwork[29] = 5.0f;
            }

            if ((player->csTimer > 70) && (player->csTimer < 80)) {
                falco->iwork[11] = 2;
            }

            if ((player->csTimer > 50) && (player->csTimer < 60)) {
                slippy->iwork[11] = 2;
            }

            if ((player->csTimer > 30) && (player->csTimer < 40)) {
                peppy->iwork[11] = 2;
            }

            if (player->csTimer == 70) {
                falco->state = 1;
                Play_PlaySfxFirstPlayer(player->sfxSource, NA_SE_ARWING_BOOST);
            }

            if (player->csTimer == 50) {
                slippy->state = 2;
                Play_PlaySfxFirstPlayer(player->sfxSource, NA_SE_ARWING_BOOST);
            }

            if (player->csTimer == 30) {
                peppy->state = 3;
                Play_PlaySfxFirstPlayer(player->sfxSource, NA_SE_ARWING_BOOST);
            }

            if (player->csTimer == 0) {
                player->csState = 9;
                Play_PlaySfxFirstPlayer(player->sfxSource, NA_SE_ARWING_BOOST);
                player->csTimer = 3;
                player->unk_194 = 5.0f;
                player->unk_190 = 5.0f;
            }
            break;

        case 9:
            gCsCamEyeX = player->pos.x;
            gCsCamEyeY = player->pos.y;
            gCsCamEyeZ = player->trueZpos + 1000.0f;

            gCsCamAtX = player->pos.x;
            gCsCamAtY = player->pos.y;
            gCsCamAtZ = player->trueZpos + 1100.0f;

            D_ctx_80177A48[0] = 0.03f;

            player->unk_190 = 2.0f;

            if (player->csTimer == 0) {
                gFillScreenAlphaTarget = 255;
                gFillScreenAlphaStep = 48;
                gFillScreenRed = gFillScreenGreen = gFillScreenBlue = 255;
            }

            if (gFillScreenAlpha == 255) {
                AUDIO_PLAY_BGM(gBgmSeqId);

                Object_Kill(&falco->obj, falco->sfxSource);
                Object_Kill(&slippy->obj, slippy->sfxSource);
                Object_Kill(&peppy->obj, peppy->sfxSource);

                gLevelStartStatusScreenTimer = 80;

                player->pos.y = 350.0f;
                player->cam.eye.x = player->pos.x;
                player->cam.eye.y = (player->pos.y * player->unk_148) + 50.0f;
                player->cam.eye.z = 30.0f;
                player->state = PLAYERSTATE_ACTIVE;
                player->csState = 0;
                player->cam.at.x = player->pos.x;
                player->cam.at.y = (player->pos.y * player->unk_148) + 20.0f;
                player->cam.at.z = player->trueZpos;

                D_ctx_80177950 = 1.0f;

                gPlayerGlareAlphas[0] = gPlayerGlareAlphas[1] = gPlayerGlareAlphas[2] = gPlayerGlareAlphas[3] = 0;
                gLoadLevelObjects = 1;
                gFillScreenAlphaTarget = 0;
                player->csTimer = 15;
            }
            break;

        case 10:
            break;
    }

    Math_SmoothStepToF(&player->cam.eye.x, gCsCamEyeX, D_ctx_80177A48[0], 20000.0f, 0.0f);
    Math_SmoothStepToF(&player->cam.eye.y, player->yBob + gCsCamEyeY, D_ctx_80177A48[0], 20000.0f, 0.0f);
    Math_SmoothStepToF(&player->cam.eye.z, gCsCamEyeZ, D_ctx_80177A48[0], 20000.0f, 0.0f);

    Math_SmoothStepToF(&player->cam.at.x, gCsCamAtX, D_ctx_80177A48[0], 20000.0f, 0.0f);
    Math_SmoothStepToF(&player->cam.at.y, gCsCamAtY - player->yBob, D_ctx_80177A48[0], 20000.0f, 0.0f);
    Math_SmoothStepToF(&player->cam.at.z, gCsCamAtZ, D_ctx_80177A48[0], 20000.0f, 0.0f);

    Math_SmoothStepToF(&player->arwing.upperRightFlapYrot, D_ctx_80177A48[1], 0.2f, 1.0f, 0.0f);
    Math_SmoothStepToF(&player->arwing.upperLeftFlapYrot, D_ctx_80177A48[2], 0.2f, 1.0f, 0.0f);

    player->arwing.bottomRightFlapYrot = player->arwing.upperRightFlapYrot;
    player->arwing.bottomLeftFlapYrot = player->arwing.upperLeftFlapYrot;

    player->cam.eye.y -= 3.0f;
    player->cam.at.y -= 3.0f;
    player->pos.y -= 3.0f;

    falco->vwork[20].y -= 3.0f;
    falco->obj.pos.y -= 3.0f;
    peppy->vwork[20].y -= 3.0f;
    peppy->obj.pos.y -= 3.0f;
    slippy->vwork[20].y -= 3.0f;
    slippy->obj.pos.y -= 3.0f;
}
#endif

RECOMP_PATCH void Macbeth_LevelComplete2(Player* player) {
    s32 i;
    s32 j;
    Vec3f spE4;
    Vec3f spD8;
    f32 zeroVar = 0.0f;

    switch (player->csState) {
        case 0:
            gCsFrameCount = 0;
            gLoadLevelObjects = 1;
            player->draw = false;

            SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM, 30);
            SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_FANFARE, 30);

            player->csState = 1;

            D_ctx_80177A48[0] = 0.0f;
            D_ctx_80177A48[4] = -60.0f;
            D_ctx_80177A48[5] = 240.0f;

            player->gravity = 3.0f;

            gCameraShakeY = player->vel.x = player->vel.y = player->vel.z = player->baseSpeed = 0.0f;

            if (player->shields <= 0) {
                player->shields = 1;
            }

            player->cam.eye.x = gCsCamEyeX = gActors[D_i5_801BE318].obj.pos.x - 250.0f;
            player->cam.eye.y = gCsCamEyeY = gActors[D_i5_801BE318].obj.pos.y + 150.0f;
            player->cam.eye.z = gCsCamEyeZ = gActors[D_i5_801BE318].obj.pos.z + gPathProgress + 500.0f;
            player->cam.at.x = gCsCamAtX = gActors[D_i5_801BE318].obj.pos.x;
            player->cam.at.y = gCsCamAtY = gActors[D_i5_801BE318].obj.pos.y + 200.0f;
            player->cam.at.z = gCsCamAtZ = gActors[D_i5_801BE318].obj.pos.z + gPathProgress;

            player->pos.x = 500.0f;
            player->pos.y = -3.0f;

            sMaTrainSpeedTarget = 0.0f;
            player->aerobaticPitch = 0.0f;
            player->camRoll = 0.0f;
            player->boostSpeed = 0.0f;
            player->arwing.upperLeftFlapYrot = player->arwing.bottomRightFlapYrot = player->arwing.bottomLeftFlapYrot =
                player->zRotBarrelRoll = player->zRotBank = 0.0f;
            player->yBob = 0.0f;
            player->rockAngle = 0.0f;
            player->unk_16C = 0.0f;
            player->unk_170 = 0.0f;
            player->arwing.upperRightFlapYrot = 0.0f;
            /* fallthrough */
        case 1:
            gShowBossHealth = false;
            if (gCsFrameCount == 70) {
                gObjectLoadIndex = 498;

                for (i = 0; i < ARRAY_COUNT(gActors); i++) {
                    if (gActors[i].obj.id == OBJ_ACTOR_MA_RAILWAY_SIGNAL) {
                        gActors[i].state++;
                        break;
                    }
                }

                player->csState++;

                gCsCamEyeX = -100.0f;
                gCsCamEyeY = 250.0f;
                gCsCamEyeZ = 2090.0f;
                gCsCamAtX = -250.0f;
                gCsCamAtY = 10.0f;
                gCsCamAtZ = -3210.0f;

                player->cam.eye.x = -250.0f;
                player->cam.eye.y = 50.0f;
                player->cam.eye.z = 1580.0f;
                player->cam.at.x = -250.0f;
                player->cam.at.y = 10.0f;
                player->cam.at.z = -3210.0f;

                D_i5_801BE312 = 1;

                player->pos.x = 500.0f;
                player->pos.y = -3.0f;
                player->pos.z = player->trueZpos = -111130.0f;
                player->zPath = gPathProgress = 115930.0f;
                player->flags_228 = PFLAG_228_4;

                gLastPathChange = OBJ_ITEM_PATH_TURN_RIGHT;

                sMaTrainSpeedTarget = 0.0f;

                gActors[D_i5_801BE314].obj.pos.x = -431.0f;
                gActors[D_i5_801BE314].obj.pos.y = 0.0f;
                gActors[D_i5_801BE314].obj.pos.z = -111174.0f;
                gActors[D_i5_801BE314].vel.x = 0.0f;
                gActors[D_i5_801BE314].vel.y = 0.0f;
                gActors[D_i5_801BE314].vel.z = 0.0f;
                gActors[D_i5_801BE316].obj.pos.x = -864.0f;
                gActors[D_i5_801BE316].obj.pos.y = 874.0f;
                gActors[D_i5_801BE316].obj.pos.z = -109864.0f;
            }
            break;

        case 2:
            if (gCsFrameCount > 105) {
                Math_SmoothStepToF(&D_ctx_80177A48[0], 1.0f, 1.0f, 0.005f, 0.0f);
            }
            if (gCsFrameCount < 160) {
                gActors[D_i5_801BE314].obj.pos.x = -431.0f;
                gActors[D_i5_801BE314].obj.pos.y = 0.0f;
                gActors[D_i5_801BE314].obj.pos.z = -111174.0f;
                gActors[D_i5_801BE314].vel.x = 0.0f;
                gActors[D_i5_801BE314].vel.y = 0.0f;
                gActors[D_i5_801BE314].vel.z = 0.0f;
                gActors[D_i5_801BE316].obj.pos.x = -864.0f;
                gActors[D_i5_801BE316].obj.pos.y = 874.0f;
                gActors[D_i5_801BE316].obj.pos.z = -109864.0f;
            } else {
                D_ctx_80177A48[0] = 0.0f;
                sMaTrainSpeedTarget = -40.0f;
                player->csState++;
            }
            /* fallthrough */
        case 3:
            player->vel.z = gActors[D_i5_801BE314].vel.z;
            if (gCsFrameCount == 252) {
                gCsCamEyeY = 570.0f;
                player->pathChangeTimer = 1000;
                player->xPathTarget = 20000.0f;
                player->pathChangeYaw = -23.0f;
                player->pathStep = 0.0f;
            }
            if (gCsFrameCount >= 252) {
                player->pos.x = gActors[D_i5_801BE314].obj.pos.x + 500.0f;
                player->zPathVel = -gActors[D_i5_801BE314].vel.z;
                player->zPath += player->zPathVel;

                gPathVelZ = player->zPathVel;
                gPathProgress = player->zPath;
                gPathTexScroll += player->zPathVel;

                player->pos.z = player->trueZpos = -(gPathProgress + 210.0f);

                Math_SmoothStepToF(&D_ctx_80177A48[0], 1.0f, 0.1f, 0.00025f, 0.0f);

                gCsCamAtX = gActors[D_i5_801BE314].obj.pos.x + 2500.0f;
                gCsCamAtZ = gActors[D_i5_801BE314].obj.pos.z + gPathProgress - 2000.0f;
                gCsCamEyeX = gCsCamAtX - 2340.0f;
                gCsCamEyeZ = gCsCamAtZ + 1410.0f;
            }
            if (gCsFrameCount == 389) {
                player->csState++;
                player->vel.z = 0.0f;
                player->zPathVel = 0.0f;

                gPathVelX = 0.0f;
                gPathVelY = 0.0f;

                player->pathChangeTimer = 0;

                player->cam.eye.x = gCsCamEyeX = 2750.0f;
                player->cam.eye.y = gCsCamEyeY = 50.0f;
                player->cam.eye.z = gCsCamEyeZ = -620.0f;
                player->cam.at.x = gCsCamAtX = -650.0f;
                player->cam.at.y = gCsCamAtY = 30.0f;
                player->cam.at.z = gCsCamAtZ = 1900.0f;

                player->yRot_114 = 0.0f;
                sMaTrainSpeedTarget = -100.0f;
                Audio_KillSfxById(NA_SE_EN_FREIGHT_TRAIN);
                AUDIO_PLAY_SFX(NA_SE_EN_TRAIN_BREAK, gActors[D_i5_801BE314].sfxSource, 4);
                D_i5_801BE313 = 0;
            }
            break;

        case 4:
            if (gCsFrameCount == 403) {
                gFillScreenRed = gFillScreenGreen = gFillScreenBlue = 255;
                gFillScreenAlphaTarget = 255;
                gFillScreenAlphaStep = 20;
            }
            if (gCsFrameCount == 416) {
                player->csState++;

                D_ctx_80177A48[0] = 1.0f;

                player->yRot_114 = 30.0f;
                player->xPath = 4600.0f;

                sMaTrainSpeedTarget = -80.0f;

                player->zPath = gPathProgress += 300.0f;

                D_ctx_80177A48[5] = 0.0f;
                D_ctx_80177A48[6] = 0.5f;
                D_ctx_80177A48[7] = 1560.0f;
                D_ctx_80177A48[8] = 2400.0f;

                player->cam.at.x = gCsCamAtX = gActors[D_i5_801BE314].obj.pos.x;
                player->cam.at.z = gCsCamAtZ = (gActors[D_i5_801BE314].obj.pos.z + gPathProgress) - 2000.0f;
                player->cam.eye.y = gCsCamEyeY = 50.0f;
                player->cam.at.y = gCsCamAtY = 10.0f;
                player->cam.eye.x = gCsCamEyeX = D_ctx_80177A48[7] + gCsCamAtX;
                player->cam.eye.z = gCsCamEyeZ = D_ctx_80177A48[8] + gCsCamAtZ;

                player->pathChangeYaw = -30.0f;
                player->xPathTarget = 10014.0f;
                player->pathChangeTimer = 1000;
                player->pathStep = 0.0f;

                gFillScreenAlphaTarget = 0;
                gFillScreenAlphaStep = 127;
            }
            break;

        case 5:
            if (gCsFrameCount <= 625) {
                Math_SmoothStepToF(&sMaTrainSpeedTarget, 0.0f, 0.1f, 0.05f, 0.0f);
            }
            if (gCsFrameCount >= 430) {
                Math_SmoothStepToF(&D_ctx_80177A48[6], 22.0f, 0.1f, 1.0f, 0.0f);
                Math_SmoothStepToF(&D_ctx_80177A48[7], 2600.0f, 0.1f, D_ctx_80177A48[6], 0.0f);
                Math_SmoothStepToF(&D_ctx_80177A48[8], 3800.0f, 0.1f, D_ctx_80177A48[6], 0.0f);
                Math_SmoothStepToF(&gCsCamEyeY, 1300.0f, 0.1f, 17.0f, 0.0f);
            }
            if (gCsFrameCount <= 505) {
                gCsCamAtZ = gActors[D_i5_801BE314].obj.pos.z + gPathProgress - 2000.0f;
                gCsCamAtX = gActors[D_i5_801BE314].obj.pos.x;
                player->vel.z = gActors[D_i5_801BE314].vel.z;
                player->pos.x = gActors[D_i5_801BE314].obj.pos.x + 500.0f;
                player->pos.z = player->trueZpos = -(gPathProgress + 210.0f);
                player->zPathVel = -gActors[D_i5_801BE314].vel.z;
                gCsCamEyeX = D_ctx_80177A48[7] + gCsCamAtX;
                gCsCamEyeZ = D_ctx_80177A48[8] + gCsCamAtZ;
                Effect_Effect390_Spawn(gActors[D_i5_801BE314].obj.pos.x + 480.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 15.0f,
                                       gActors[D_i5_801BE314].obj.pos.z - 400.0f, gActors[D_i5_801BE314].vel.x,
                                       gActors[D_i5_801BE314].vel.y, 0.0f, 0.2f, 10);
            } else {
                player->vel.z = 0.0f;
                if ((gGameFrameCount % 16) == 0) {
                    gFogRed -= 1;
                    gFogGreen -= 1;
                    gFogBlue -= 2;
                }

                Math_SmoothStepToF(&player->zPathVel, 0.0f, 1.0f, 0.6f, 0.0f);
                Math_SmoothStepToF(&gCsCamAtX, 11700.0f, 0.1f, 50.0f, 0.0f);
                Math_SmoothStepToF(&D_ctx_80177A48[5], 30.0f, 0.1f, 0.05f, 0.0f);
                Math_SmoothStepToF(&gCsCamAtY, 1600.0f, 0.1f, D_ctx_80177A48[5], 0.0f);

                if (player->zPathVel >= 2.5f) {
                    player->pos.x = gActors[D_i5_801BE314].obj.pos.x + 500.0f;
                    player->pos.z = player->trueZpos = -(gPathProgress + 210.0f);
                } else {
                    player->xPathTarget = player->xPath;
                    gPathVelX = 0.0f;
                }

                Effect_Effect362_Spawn(gActors[D_i5_801BE314].obj.pos.x + 190.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 30.0f,
                                       gActors[D_i5_801BE314].obj.pos.z - 100.0f, 6.0f);
                Effect_Effect362_Spawn(gActors[D_i5_801BE314].obj.pos.x + 190.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 30.0f,
                                       gActors[D_i5_801BE314].obj.pos.z - 120.0f, 6.0f);
                Effect_Effect362_Spawn(gActors[D_i5_801BE314].obj.pos.x - 190.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 30.0f,
                                       gActors[D_i5_801BE314].obj.pos.z - 100.0f, 6.0f);
                Effect_Effect362_Spawn(gActors[D_i5_801BE314].obj.pos.x - 190.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 30.0f,
                                       gActors[D_i5_801BE314].obj.pos.z - 120.0f, 6.0f);
            }

            player->zPath += player->zPathVel;
            gPathVelZ = player->zPathVel;
            gPathProgress = player->zPath;
            gPathTexScroll += player->zPathVel;

            if (gCsFrameCount == 630) {
                Audio_KillSfxBySourceAndId(gActors[D_i5_801BE314].sfxSource, NA_SE_EN_TRAIN_BREAK);
                AUDIO_PLAY_SFX(NA_SE_EN_EXPLOSION_L, gActors[D_i5_801BE314].sfxSource, 4);
                player->csState++;
                player->vel.z = 0.0f;
                player->zPathVel = 0.0f;

                gPathVelZ = 0.0f;
                gPathVelX = 0.0f;
                gPathVelY = 0.0f;

                player->pathChangeTimer = 10000;

                D_ctx_80177A48[0] = 1.0f;

                sMaTrainSpeedTarget = 0.0f;

                gControllerRumbleFlags[0] = 1;
                gControllerRumbleTimers[0] = 10;

                gCameraShake = 10;

                D_ctx_80177A48[4] = -70.0f;
                D_ctx_80177A48[5] = 280.0f;
            }
            break;

        case 6:
            Math_SmoothStepToF(&D_ctx_80177A48[4], -60.0f, 1.0f, 0.5f, 0.0f);
            Math_SmoothStepToF(&D_ctx_80177A48[5], 240.0f, 1.0f, 2.0f, 0.0f);

            if ((gGameFrameCount % 2) == 0) {
                Effect_FireSmoke1_SpawnMoving(11000.0f, 200.0f, -139000.0f, D_ctx_80177A48[4], 4.0f, D_ctx_80177A48[5],
                                              20.0f);
            }

            if (gCsFrameCount == 710) {
                gFillScreenRed = gFillScreenGreen = gFillScreenBlue = gFillScreenAlpha = 255;
                gFillScreenAlphaTarget = 0;
                gFillScreenAlphaStep = 128;

                Effect_Effect367_Spawn(gActors[D_i5_801BE314].obj.pos.x + 1000.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 20.0f, -138500.0f, 100.0f, 1000.0f, 0);
                Effect_Effect383_Spawn(gActors[D_i5_801BE314].obj.pos.x + 200.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 700.0f, -137750.0f, 100.0f);

                for (i = 0; i < 8; i++) {
                    Effect_Effect357_Spawn50(gActors[D_i5_801BE314].obj.pos.x + RAND_FLOAT_CENTERED(1000.0f),
                                             gActors[D_i5_801BE314].obj.pos.y + 1300.0f,
                                             gActors[D_i5_801BE314].obj.pos.z + 700.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             6.0f);
                    Macbeth_Effect357_Spawn2(RAND_FLOAT_CENTERED(650.0f) + 11250.0f, 300.0f,
                                             RAND_FLOAT(650.0f) + -139000.0f, 0.7f);
                }

                gActors[58].scale = 0.8f;

                Effect_FireSmoke1_Spawn4(gActors[D_i5_801BE314].obj.pos.x + 500.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.y + 500.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.z + 3000.0f + RAND_FLOAT_CENTERED(700.0f),
                                         30.0f);
            }
            if ((gCsFrameCount > 710) && ((gGameFrameCount % 8) == 0)) {
                Effect_FireSmoke1_Spawn4(gActors[D_i5_801BE314].obj.pos.x + 500.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.y + 500.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.z + 3000.0f + RAND_FLOAT_CENTERED(700.0f),
                                         30.0f);
            }
            if ((gCsFrameCount > 725) && ((gGameFrameCount % 8) == 0)) {
                Effect_FireSmoke1_Spawn4(gActors[D_i5_801BE314].obj.pos.x - 1500.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.y + 1000.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.z + 1700.0f + RAND_FLOAT_CENTERED(700.0f),
                                         30.0f);
            }
            if ((gCsFrameCount > 740) && ((gGameFrameCount % 8) == 0)) {
                Effect_FireSmoke1_Spawn4(gActors[D_i5_801BE314].obj.pos.x - 3500.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.y + 600.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.z + 1200.0f + RAND_FLOAT_CENTERED(700.0f),
                                         30.0f);
            }
            if ((gCsFrameCount > 755) && ((gGameFrameCount % 8) == 0)) {
                Effect_FireSmoke1_Spawn4(gActors[D_i5_801BE314].obj.pos.x - 3000.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.y + 400.0f + RAND_FLOAT_CENTERED(700.0f),
                                         gActors[D_i5_801BE314].obj.pos.z + 1300.0f + RAND_FLOAT_CENTERED(700.0f),
                                         30.0f);
            }
            if (gCsFrameCount == 800) {
                Effect_Effect383_Spawn(gActors[D_i5_801BE314].obj.pos.x - 3000.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 500.0f,
                                       gActors[D_i5_801BE314].obj.pos.z + 1600.0f, 10000.0f);
                for (i = 0; i < 8; i++) {
                    Effect_Effect357_Spawn50(gActors[D_i5_801BE314].obj.pos.x - 3000.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             gActors[D_i5_801BE314].obj.pos.y + 700.0f,
                                             gActors[D_i5_801BE314].obj.pos.z + 1600.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             6.0f);
                    Macbeth_Effect357_Spawn2(RAND_FLOAT_CENTERED(650.0f) + 11250.0f, 300.0f,
                                             RAND_FLOAT(650.0f) + -139000.0f, 0.7f);
                }
            }
            if (gCsFrameCount == 820) {
                Effect_Effect383_Spawn(gActors[D_i5_801BE314].obj.pos.x, gActors[D_i5_801BE314].obj.pos.y + 500.0f,
                                       gActors[D_i5_801BE314].obj.pos.z + 3300.0f, 14000.0f);
                for (i = 0; i < 8; i++) {
                    Effect_Effect357_Spawn50(gActors[D_i5_801BE314].obj.pos.x + 500.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             gActors[D_i5_801BE314].obj.pos.y + 900.0f,
                                             gActors[D_i5_801BE314].obj.pos.z + 3300.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             6.0f);
                    Macbeth_Effect357_Spawn2(RAND_FLOAT_CENTERED(650.0f) + 11250.0f, 300.0f,
                                             RAND_FLOAT(650.0f) + -139000.0f, 0.7f);
                }
            }
            if (gCsFrameCount == 835) {
                Effect_Effect383_Spawn(gActors[D_i5_801BE314].obj.pos.x - 1500.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 500.0f,
                                       gActors[D_i5_801BE314].obj.pos.z + 2000.0f, 80.0f);
                for (i = 0; i < 8; i++) {
                    Effect_Effect357_Spawn50(gActors[D_i5_801BE314].obj.pos.x - 1500.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             gActors[D_i5_801BE314].obj.pos.y + 1200.0f,
                                             gActors[D_i5_801BE314].obj.pos.z + 2000.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             6.0f);
                    Macbeth_Effect357_Spawn2(RAND_FLOAT_CENTERED(650.0f) + 11250.0f, 300.0f,
                                             RAND_FLOAT(650.0f) + -139000.0f, 0.7f);
                }
            }
            if (gCsFrameCount == 845) {
                Effect_Effect383_Spawn(gActors[D_i5_801BE314].obj.pos.x - 500.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 500.0f,
                                       gActors[D_i5_801BE314].obj.pos.z + 2800.0f, 10000.0f);
                for (i = 0; i < 11; i++) {
                    Effect_Effect357_Spawn50(gActors[D_i5_801BE314].obj.pos.x - 500.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             gActors[D_i5_801BE314].obj.pos.y + 900.0f,
                                             gActors[D_i5_801BE314].obj.pos.z + 2800.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             6.0f);
                    Macbeth_Effect357_Spawn2(RAND_FLOAT_CENTERED(650.0f) + 11250.0f, 300.0f,
                                             RAND_FLOAT(650.0f) + -139000.0f, 0.7f);
                }
            }
            if (gCsFrameCount == 853) {
                Effect_Effect383_Spawn(gActors[D_i5_801BE314].obj.pos.x - 3500.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 500.0f,
                                       gActors[D_i5_801BE314].obj.pos.z + 2500.0f, 14000.0f);
                for (i = 0; i < 8; i++) {
                    Effect_Effect357_Spawn50(gActors[D_i5_801BE314].obj.pos.x - 3500.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             gActors[D_i5_801BE314].obj.pos.y + 900.0f,
                                             gActors[D_i5_801BE314].obj.pos.z + 1500.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             6.0f);
                    Macbeth_Effect357_Spawn2(RAND_FLOAT_CENTERED(650.0f) + 11250.0f, 300.0f,
                                             RAND_FLOAT(650.0f) + -139000.0f, 0.7f);
                }
                gCameraShake = 30;
            }
            if (gCsFrameCount == 860) {
                Effect_Effect367_Spawn(gActors[D_i5_801BE314].obj.pos.x + 1000.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 20.0f, -138500.0f, 100.0f, 1000.0f, 0);
                Effect_Effect383_Spawn(gActors[D_i5_801BE314].obj.pos.x - 1500.0f,
                                       gActors[D_i5_801BE314].obj.pos.y + 700.0f,
                                       gActors[D_i5_801BE314].obj.pos.z + 3000.0f, 180.0f);
                for (i = 0; i < 8; i++) {
                    Effect_Effect357_Spawn50(gActors[D_i5_801BE314].obj.pos.x - 1500.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             gActors[D_i5_801BE314].obj.pos.y + 1300.0f,
                                             gActors[D_i5_801BE314].obj.pos.z + 2500.0f + RAND_FLOAT_CENTERED(1000.0f),
                                             6.0f);
                    Macbeth_Effect357_Spawn2(RAND_FLOAT_CENTERED(650.0f) + 11250.0f, 300.0f,
                                             RAND_FLOAT(650.0f) + -139000.0f, 0.7f);
                }
            }
            if (gCsFrameCount == 880) {
                gControllerRumbleFlags[0] = 1;
                gControllerRumbleTimers[0] = 10;

                gFillScreenRed = gFillScreenGreen = gFillScreenBlue = 255;
                gFillScreenAlphaTarget = 255;
                gFillScreenAlphaStep = 20;

                AUDIO_PLAY_SFX(NA_SE_EN_STAR_EXPLOSION, gActors[D_i5_801BE314].sfxSource, 4);
                gBossActive = gLoadLevelObjects = false;
            }
            if (gCsFrameCount == 940) {
                player->csState++;

                Play_ClearObjectData();

                player->pos.z = player->trueZpos = -(gPathProgress + 210.0f);
                player->pos.x = player->xPath;

                Effect_Effect383_Spawn(player->pos.x - 1800.0f, -50.0f, player->pos.z + 5000.0f, 40.0f);

                gFillScreenRed = gFillScreenGreen = gFillScreenBlue = gFillScreenAlpha = 255;
                gFillScreenAlphaTarget = 0;
                gFillScreenAlphaStep = 8;

                D_ctx_80177A48[0] = 0.0f;

                player->draw = true;

                player->cam.at.x = gCsCamAtX = player->cam.eye.x = gCsCamEyeX = player->pos.x;
                player->cam.at.y = gCsCamAtY = player->pos.y + 70.0f;
                player->cam.eye.y = gCsCamEyeY = player->pos.y + 30.0f;
                player->cam.eye.z = gCsCamEyeZ = (player->trueZpos + gPathProgress) - 300.0f;
                player->cam.at.z = gCsCamAtZ = player->trueZpos + gPathProgress;

                player->savedAlternateView = player->pathChangeTimer = 0;
                player->unk_190 = player->unk_194 = player->unk_188 = player->unk_18C = player->pathChangeYaw =
                    player->yRot_114 = player->aerobaticPitch = player->camRoll = player->unk_174 = player->unk_178 =
                        player->unk_17C = player->unk_180 = player->unk_184 = player->arwing.upperRightFlapYrot =
                            player->unk_170 = player->unk_16C = player->rockAngle = player->yBob =
                                player->arwing.upperLeftFlapYrot = player->arwing.bottomRightFlapYrot =
                                    player->arwing.bottomLeftFlapYrot = player->zRotBarrelRoll = player->zRotBank =
                                        player->boostSpeed = 0.0f;

                player->baseSpeed = 5.0f;

                D_ctx_80177A48[3] = D_ctx_80177A48[6] = D_ctx_80177A48[7] = D_ctx_80177A48[8] = 0.0f;

                player->grounded = true;
                player->hitTimer = 0;
                player->pos.y = gGroundHeight - 3.0f;
                player->vel.y = -3.0f;

                player->rollState = player->boostCooldown = player->boostMeter = player->unk_184 = player->rot_104.y =
                    player->rot_104.z = player->rot.y = player->rot.x = player->rot_104.x = 0.0f;
                player->rollInputTimerL = player->sfx.bank = 0;
            }
            break;

        case 7:
            if ((gGameFrameCount % 4) == 0) {
                Effect_FireSmoke1_Spawn4(player->pos.x - 1750.0f + RAND_FLOAT_CENTERED(700.0f), 10.0f,
                                         player->pos.z + 5000.0f + RAND_FLOAT_CENTERED(700.0f), 5.0f);
                Effect_Effect343_Spawn(player->pos.x - 1800.0f + RAND_FLOAT_CENTERED(40.0f), -100.0f,
                                       player->pos.z + 5000.0f + RAND_FLOAT_CENTERED(40.0f), 9.0f);
            }

            if ((gCsFrameCount >= 1059) && (gCsFrameCount < 2120)) {
                Math_SmoothStepToF(&D_ctx_80177A48[0], 1.0f, 1.0f, 0.005f, 0.0f);
                Math_SmoothStepToF(&D_ctx_80177A48[3], 1080.0f, 0.1f, D_ctx_80177A48[0] * 1.2f, 0.0f);
                Matrix_RotateY(gCalcMatrix, -D_ctx_80177A48[3] * M_DTOR, MTXF_NEW);
                spE4.x = 0.0f;
                spE4.y = 0.0f;
                spE4.z = -300.0f;
                Matrix_MultVec3fNoTranslate(gCalcMatrix, &spE4, &spD8);
                gCsCamEyeX = player->pos.x + spD8.x;
                gCsCamEyeZ = player->pos.z + gPathProgress + spD8.z;
            }
            if (gCsFrameCount >= 2120) {
                Math_SmoothStepToF(&D_ctx_80177A48[0], 1.0f, 1.0f, 0.001f, 0.0f);
                Math_SmoothStepToF(&D_ctx_80177A48[8], 20.0f, 0.1f, 0.1f, 0.0f);
                Math_SmoothStepToF(&gCsCamEyeX, player->pos.x - 2000.0f, 0.1f, D_ctx_80177A48[8], 0.0f);
                gCsCamEyeZ -= 1.0f;
            }
            //! @BUG: The following condition is always true:
            // if ((gCsFrameCount >= 2120) || (gCsFrameCount < 2175)) {

            // @recomp: Fix the camera bug
            if ((gCsFrameCount >= 2120) && (gCsFrameCount < 2175)) {
                Math_SmoothStepToF(&gCsCamAtY, player->pos.y + 70.0f, 0.1f, 8.0f, 0.0f);
            }
            if (gCsFrameCount == 2120) {
                D_ctx_80177A48[0] = 0.0f;
            }
            if (gCsFrameCount >= 2160) {
                player->vel.y = 4.0f;
                Math_SmoothStepToF(&player->unk_170, 2.0f, 1.0f, 0.3f, 0.0f);
                Math_SmoothStepToF(&player->unk_16C, 2.0f, 1.0f, 0.3f, 0.0f);
                if ((gCsFrameCount >= 2165) && (gCsFrameCount <= 2180)) {
                    Effect_Effect362_Spawn(player->pos.x + RAND_FLOAT_CENTERED(30.0f), 0.0f,
                                           RAND_FLOAT_CENTERED(30.0f) + player->trueZpos, RAND_FLOAT(2.0f) + 3.5f);
                    Effect_Effect362_Spawn(player->pos.x - RAND_FLOAT_CENTERED(30.0f), 0.0f,
                                           RAND_FLOAT_CENTERED(30.0f) + player->trueZpos, RAND_FLOAT(2.0f) + 3.5f);
                }
                Math_SmoothStepToF(&D_ctx_80177A48[7], 4.5f, 0.1f, 0.3f, 0.0f);
                player->trueZpos = player->pos.z;
            }

            func_tank_80045130(player);
            func_tank_80044868(player);
            func_tank_800444BC(player);

            Player_UpdatePath(player);

            if (gCsFrameCount >= 2175) {
                Math_SmoothStepToF(&gCsCamAtZ, gActors[3].obj.pos.z + gPathProgress - 300.0f, 0.1f, 20.0f, 0.0f);
                Math_SmoothStepToF(&gCsCamAtY, gActors[3].obj.pos.y + 100.0f, 0.1f, 10.0f, 0.0f);
            } else {
                player->cam.at.z = gCsCamAtZ = player->trueZpos + gPathProgress;
            }
            Math_SmoothStepToF(&player->pos.y, 260.0f, 0.1f, D_ctx_80177A48[7], 0.0f);
            break;
    }

    switch (gCsFrameCount) {
        case 973:
            gLevelClearScreenTimer = 100;
            break;

        case 340:
            Radio_PlayMessage(gMsg_ID_17471, RCID_BOSS_MACBETH);
            break;

        case 520:
            Radio_PlayMessage(gMsg_ID_17472, RCID_BOSS_MACBETH);
            break;

        case 710:
        case 800:
        case 820:
        case 835:
        case 845:
        case 860:
            gCameraShake = 20;
            D_ctx_80177A48[4] = -90.0f;
            D_ctx_80177A48[5] = 360.0f;
            break;

        case 910:
            AUDIO_PLAY_BGM(NA_BGM_COURSE_CLEAR);
            break;

        case 1040:
            Radio_PlayMessage(gMsg_ID_20010, RCID_FOX);
            break;

        case 1060:
            if (gTeamShields[TEAM_ID_SLIPPY] > 0) {
                Macbeth_801AF27C(&gActors[5], 0);
            }
            break;

        case 1130:
            switch (gTeamShields[TEAM_ID_SLIPPY]) {
                case 0:
                    Radio_PlayMessage(gMsg_ID_20345, RCID_ROB64);
                    break;
                case -1:
                    Radio_PlayMessage(gMsg_ID_20333, RCID_ROB64);
                    break;
                default:
                    Radio_PlayMessage(gMsg_ID_17330, RCID_SLIPPY);
                    break;
            }
            break;

        case 1240:
            if (gTeamShields[TEAM_ID_PEPPY] > 0) {
                Macbeth_801AF27C(&gActors[6], 1);
            }
            break;

        case 1310:
            D_i5_801BA874[5] = 220.0f;
            break;

        case 1350:
            switch (gTeamShields[TEAM_ID_PEPPY]) {
                case 0:
                    Radio_PlayMessage(gMsg_ID_20344, RCID_ROB64);
                    break;
                case -1:
                    Radio_PlayMessage(gMsg_ID_20332, RCID_ROB64);
                    break;
                default:
                    Radio_PlayMessage(gMsg_ID_17310, RCID_PEPPY);
                    break;
            }
            break;

        case 1420:
            if (gLeveLClearStatus[LEVEL_ZONESS] != 0) {
                Macbeth_801AF44C();
            }
            break;

        case 1460:
            D_i5_801BA874[6] = 170.0f;
            break;

        case 1500:
            if (gLeveLClearStatus[LEVEL_ZONESS] != 0) {
                Radio_PlayMessage(gMsg_ID_17300, RCID_KATT);
            }
            break;

        case 1640:
            if (gTeamShields[TEAM_ID_FALCO] > 0) {
                Macbeth_801AF27C(&gActors[7], 2);
            }
            break;

        case 1660:
            switch (gTeamShields[TEAM_ID_FALCO]) {
                case 0:
                    Radio_PlayMessage(gMsg_ID_20340, RCID_ROB64);
                    break;
                case -1:
                    Radio_PlayMessage(gMsg_ID_20331, RCID_ROB64);
                    break;
                default:
                    Radio_PlayMessage(gMsg_ID_17320, RCID_FALCO);
                    break;
            }
            break;

        case 1661:
            D_i5_801BA854[6] = -0.5f;
            break;

        case 1810:
            Object_Kill(&gActors[8].obj, gActors[8].sfxSource);
            gSceneSetup = 1;
            break;

        case 1817:
            gShowLevelClearStatusScreen = true;
            break;

        case 1960:
            Macbeth_801AF27C(&gActors[3], 3);
            gActors[3].info.hitbox = SEGMENTED_TO_VIRTUAL(gNoHitbox);
            break;

        case 2017:
            gShowLevelClearStatusScreen = false;
            break;

        case 2040:
            gActors[5].state++;
            break;

        case 2080:
            gActors[6].state++;
            break;

        case 2130:
            gActors[7].state++;
            break;

        case 2160:
            gProjectFar = 30000.0f;
            player->hideShadow = true;
            Audio_StopPlayerNoise(0);
            AUDIO_PLAY_SFX(NA_SE_TANK_GO_UP, player->sfxSource, 0);
            break;

        case 2310:
            gActors[3].state++;
            break;

        case 2440:
            SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM, 60);
            SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_FANFARE, 60);
            break;
    }

    // @recomp: Make the Great Fox carry the landmaster for real.
    if (gCsFrameCount >= 2300) {
        player->pos = gActors[3].obj.pos;
    }

    if (player->pathChangeTimer != 0) {
        player->pathChangeTimer--;
        Math_SmoothStepToF(&player->yRot_114, -player->pathChangeYaw, 0.03f, 0.5f, 0.0001f);
        Math_SmoothStepToF(&player->pathStep, gPathVelZ * .3f, 0.1f, 2.0f, 0.0001f);
        gPathVelX = Math_SmoothStepToF(&player->xPath, player->xPathTarget, 0.1f, player->pathStep, 0.0001f);
        gPathVelY = Math_SmoothStepToF(&player->yPath, player->yPathTarget, 0.1f, player->pathStep, 0.0001f);
    } else {
        Math_SmoothStepToF(&player->yRot_114, 0.0f, 0.03f, 0.5f, 0.0001f);
    }

    Math_SmoothStepToF(&player->cam.eye.x, gCsCamEyeX, D_ctx_80177A48[0], 20000.0f, 0);
    Math_SmoothStepToF(&player->cam.eye.y, gCsCamEyeY, D_ctx_80177A48[0], 20000.0f, 0);
    Math_SmoothStepToF(&player->cam.eye.z, gCsCamEyeZ, D_ctx_80177A48[0], 20000.0f, 0);
    Math_SmoothStepToF(&player->cam.at.x, gCsCamAtX, D_ctx_80177A48[0], 20000.0f, 0);
    Math_SmoothStepToF(&player->cam.at.y, gCsCamAtY, D_ctx_80177A48[0], 20000.0f, 0);
    Math_SmoothStepToF(&player->cam.at.z, gCsCamAtZ, D_ctx_80177A48[0], 20000.0f, 0);

    player->cam.at.y += zeroVar;

    if (player->csState >= 6) {
        player->cam.eye.y += gCameraShakeY * 10.0f;
    }

    if (gCsFrameCount > 2500) {
        gFillScreenRed = gFillScreenGreen = gFillScreenBlue = 0;
        gFillScreenAlphaTarget = 255;

        if (gFillScreenAlpha == 255) {
            player->state = PLAYERSTATE_NEXT;
            gFadeoutType = 4;
            Play_ClearObjectData();
            Audio_FadeOutAll(10);
            gLeveLClearStatus[LEVEL_MACBETH] = Play_CheckMedalStatus(150) + 1;
        }
    }
}

RECOMP_PATCH bool Display_CheckPlayerVisible(s32 index, s32 reflectY) {
    Vec3f src = { 0.0f, 0.0f, 0.0f };
    Vec3f dest;

    Matrix_MultVec3f(gGfxMatrix, &src, &dest);

    if ((dest.z < 200.0f) && (dest.z > -12000.0f)) {
        // @recomp: Extend player visibility to widescreen.
        if (fabsf(dest.x) < (fabsf(dest.z * /*0.5f*/ 1.5f) + 500.0f)) {
            if (fabsf(dest.y) < (fabsf(dest.z * 0.5f) + 500.0f)) {
                if (reflectY == 0) {
                    sPlayersVisible[index] = true;
                }
                return true;
            }
        }
    }

    if (reflectY == 0) {
        sPlayersVisible[index] = false;
    }
    return false;
}

#if 1
RECOMP_PATCH void Solar_LevelStart(Player* player) {
    s32 i;
    Vec3f sp50;
    Vec3f sp44;

    if (D_ctx_80177A10[0] != 0) {
        D_ctx_80177A10[0]--;
    }
    if (D_ctx_80177A10[1] != 0) {
        D_ctx_80177A10[1]--;
    }

    switch (player->csState) {
        case 0:
            // @recomp: Get the player away from the screen
            player->pos.z = 10000.0f;

            gCsFrameCount = 0;
            Rand_SetSeed(1, 29100, 9786);
            player->csState = 1;
            player->cam.eye.x = -2000.0f;
            gCsCamEyeX = -2800.0f;
            player->cam.eye.y = gCsCamEyeY = 120.0f;
            player->cam.eye.z = -1800.0f;
            gCsCamEyeZ = -2800.0f;

            player->cam.at.x = gCsCamAtX = -1000.0f;
            player->cam.at.y = 1400.0f;
            gCsCamAtY = 400.0f;
            player->cam.at.z = gCsCamAtZ = -2300.0f;

            D_ctx_80177A48[0] = 0.0045f;

            player->camRoll = 0.0f;
            player->baseSpeed = 0.0f;

            gFillScreenAlpha = 255;
            gFillScreenAlphaTarget = 0;
            Solar_801A0DF8(-750.0f, -2600.0f, 300.0f, 2, 1.0f);
            AUDIO_PLAY_SFX(NA_SE_OB_MAGMA_BUBBLE, player->sfxSource, 0);
            break;

        case 1:
            gPathTexScroll += 30.0f;

            if (gCsFrameCount == 140) {
                Solar_801A0DF8(-400.0f, -2000.0f, 45.0f, 3, 1.0f);
            }
            if (gCsFrameCount == 280) {
                Solar_801A0DF8(-1730.0f, -2300.0f, 250.0f, 1, 1.0f);
            }

            if (gCsFrameCount == 100) {
                Object_Kill(&gActors[4].obj, gActors[4].sfxSource);
            }
            if (gCsFrameCount == 240) {
                Object_Kill(&gActors[5].obj, gActors[5].sfxSource);
            }
            if (gCsFrameCount == 380) {
                Object_Kill(&gActors[6].obj, gActors[6].sfxSource);
            }

            if (gCsFrameCount < 3) {
                gFillScreenAlpha = 255;
            }

            if (gCsFrameCount == 370) {
                gFillScreenAlphaTarget = 255;
                gFillScreenRed = 255;
                gFillScreenGreen = 255;
                gFillScreenBlue = 255;
                gFillScreenAlphaStep = 8;
            }

            if ((gGameFrameCount % 8) == 0) {
                Solar_SoFlare_Spawn3(RAND_FLOAT_CENTERED(6000.0f), RAND_FLOAT_CENTERED(5.0f) - 90.0f,
                                     RAND_FLOAT(2000.0f) - 6000.0f + gPathProgress, RAND_FLOAT(20.0f) + 20.0f);
            }

            if (gCsFrameCount == 380) {
                for (i = 0; i < ARRAY_COUNT(gEffects); i++) {
                    Object_Kill(&gEffects[i].obj, gEffects[i].sfxSource);
                }
                Solar_801A0DF8(400.0f, -2800.0f, 340.0f, 1, 1.0f);
            }

            if (gCsFrameCount == 410) {
                player->csState++;
                player->cam.eye.x = 200.0f;
                gCsCamEyeX = 800.0f;
                player->cam.eye.y = 140.0f;
                gCsCamEyeY = player->pos.y + 200.0f;
                player->cam.eye.z = gCsCamEyeZ = 400.0f;

                player->cam.at.x = 400.0f;
                gCsCamAtX = 200.0f;
                player->cam.at.y = 140.0f;
                gCsCamAtY = 1200.0f;
                player->cam.at.z = -500.0f;

                player->pos.y = 1200.0f;
                player->pos.z = -2500.0f;

                Solar_801A0FD4(&gActors[0], 0);
                Solar_801A0FD4(&gActors[1], 1);
                Solar_801A0FD4(&gActors[2], 2);

                gCsCamAtZ = -3000.0f;
                gCsCamEyeZ = -3400.0f;

                Audio_KillSfxBySourceAndId(player->sfxSource, NA_SE_OB_MAGMA_BUBBLE);

                AUDIO_PLAY_BGM(NA_BGM_SO_START_DEMO);

                Display_SetupPlayerSfxPos(player);
                Audio_StartPlayerNoise(gPlayerNum);

                D_ctx_80177A48[0] = 0.01f;
            }
            break;

        case 2:
            gPathTexScroll += 60.0f;
            gFillScreenAlphaTarget = 0;
            gFillScreenAlphaStep = 8;

            Math_SmoothStepToF(D_ctx_80177A48, 0.02f, 1.0f, 0.0000003f, 0.0f);

            if (gCsFrameCount == 525) {
                Radio_PlayMessage(gMsg_ID_10010, RCID_FOX);
            }
            if (gCsFrameCount == 550) {
                Object_Kill(&gActors[5].obj, gActors[5].sfxSource);
                Solar_801A0DF8(30.0f, -1500.0f, 160.0f, 3, 2.0f);
            }

            if (player->cam.eye.z <= -2465.0f) {
                player->csState++;
                gCsCamEyeY = player->pos.y + 180.0f;
                gCsCamAtX = 0.0f;
                gCsCamEyeX = 0.0f;
                gCsCamAtZ = -2800.0f;
                D_ctx_80177A48[0] = 0.06f;
            }
            break;

        case 3:
            gPathTexScroll += 60.0f;

            if ((gGameFrameCount % 4) == 0) {
                Solar_SoFlare_Spawn3(RAND_FLOAT_CENTERED(6000.0f), -400.0f - ((player->cam.eye.y - 1380.0f) * 0.3f),
                                     RAND_FLOAT_CENTERED(2000.0f) + 500.0f + gPathProgress, RAND_FLOAT(20.0f) + 20.0f);
            }

            if (gCsFrameCount == 615) {
                Play_PlaySfxFirstPlayer(player->sfxSource, NA_SE_ARWING_BOOST);
                gActors[0].fwork[29] = gActors[1].fwork[29] = gActors[2].fwork[29] = 5.0f;
                gActors[0].state = 3;
                gActors[1].state = 2;
                gActors[2].state = 1;
                player->csTimer = 3;
                player->unk_190 = 5.0f;
                player->csState++;
                player->unk_194 = 5.0f;
            }
            break;

        case 4:
            gCsCamEyeX = player->pos.x;
            gCsCamEyeY = player->pos.y;
            gCsCamEyeZ = player->trueZpos + 1000.0f;

            gCsCamAtX = player->pos.x;
            gCsCamAtY = player->pos.y;
            gCsCamAtZ = player->trueZpos + 1100.0f;

            D_ctx_80177A48[0] = 0.03f;
            D_ctx_801779A8[gMainController] = 60.0f;

            player->unk_190 = 2.0f;

            if (player->csTimer == 0) {
                gFillScreenAlphaTarget = 255;
                gFillScreenAlphaStep = 24;
                gFillScreenRed = gFillScreenGreen = gFillScreenBlue = 255;
            }

            if (gFillScreenAlpha == 255) {
                AUDIO_PLAY_BGM(NA_BGM_STAGE_SO);
                player->pos.z = 0.0f;
                player->baseSpeed = gArwingSpeed;
                Play_ClearObjectData();
                gLevelStartStatusScreenTimer = 50;
                player->state = PLAYERSTATE_ACTIVE;
                player->csState = 0;
                player->pos.y = 350.0f;

                player->cam.eye.x = player->pos.x;
                player->cam.eye.y = 50.0f + player->pos.y * player->unk_148;
                player->cam.eye.z = 30.0f;

                player->cam.at.x = player->pos.x;
                player->cam.at.y = 20.0f + player->pos.y * player->unk_148;
                player->cam.at.z = player->trueZpos;

                D_ctx_80177950 = 1.0f;
                Audio_SetHeatAlarmParams(255, 3);
                AUDIO_PLAY_SFX(NA_SE_OVERHEAT_ALARM, gDefaultSfxSource, 4);
                gLoadLevelObjects = 1;
                gFillScreenAlphaTarget = 0;
                player->csTimer = 15;
                gPlayer[0].shields = 255;
            }
            break;

        case 10:
            gPathTexScroll += 60.0f;
            break;
    }
    Math_SmoothStepToF(&player->cam.eye.x, gCsCamEyeX, D_ctx_80177A48[0], 20000.0f, 0);
    Math_SmoothStepToF(&player->cam.eye.y, gCsCamEyeY, D_ctx_80177A48[0], 20000.0f, 0);
    Math_SmoothStepToF(&player->cam.eye.z, gCsCamEyeZ, D_ctx_80177A48[0], 20000.0f, 0);

    Math_SmoothStepToF(&player->cam.at.x, gCsCamAtX, D_ctx_80177A48[0], 20000.0f, 0);
    Math_SmoothStepToF(&player->cam.at.y, gCsCamAtY, D_ctx_80177A48[0], 20000.0f, 0);
    Math_SmoothStepToF(&player->cam.at.z, gCsCamAtZ, D_ctx_80177A48[0], 20000.0f, 0);

    Matrix_RotateY(gCalcMatrix, (player->rot.y + 180.0f) * M_DTOR, MTXF_NEW);
    Matrix_RotateX(gCalcMatrix, -(player->rot.x * M_DTOR), MTXF_APPLY);

    sp50.x = 0.f;
    sp50.y = 0.0f;
    sp50.z = player->baseSpeed;

    Matrix_MultVec3fNoTranslate(gCalcMatrix, &sp50, &sp44);

    player->vel.x = sp44.x;
    player->vel.z = sp44.z;
    player->vel.y = sp44.y;

    player->pos.x += player->vel.x;
    player->pos.y += player->vel.y;
    player->pos.z += player->vel.z;
    player->trueZpos = player->pos.z + player->camDist;
}
#endif

#endif // full scope
