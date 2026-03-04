#include "patches.h"

extern Vec3f sViewPos;
extern bool sDrewActor;
extern Matrix D_BO_8019EE80;
extern Vec3f sShotViewPos;
extern f32 D_800C9BD8[10];
extern u8 D_800C9C00[4];
extern u8 D_800C9C04[4];
extern u8 D_800C9C08[4];
extern Gfx aItemBombDL[];
extern Gfx D_versus_301AD60[];
extern Gfx D_versus_301AEF0[];
extern Gfx aLandmasterShotDL[];
extern Gfx aOrbDL[];
extern Gfx aStarDL[];
extern Gfx D_1031EC0[];
extern Gfx D_GREAT_FOX_E00DFB0[];
extern Gfx D_AQ_600DB80[];

void BonusText_Draw(BonusText* bonus);
void PlayerShot_Draw(PlayerShot* shot);
void PlayerShot_DrawLaser(PlayerShot* shot);
void Object_DrawShadow(s32 index, Object* obj);
void Scenery360_Draw(Scenery360* this);
void Scenery_Draw(Scenery* this, s32 cullDirection);
void Item_Draw(Item* this, s32 arg1);
void Object_SetCullDirection(s32 cullDirection);
void Sprite_Draw(Sprite* this, s32 arg1);
void Boss_Draw(Boss* this, s32 arg1);
void Actor_DrawOnRails(Actor* this);
void Actor_DrawAllRange(Actor* this);
void Effect_DrawOnRails(Effect* this, s32 arg1);
void Effect_DrawAllRange(Effect* this);

#if 1
RECOMP_PATCH void Object_DrawAll(s32 cullDirection) {
    Vec3f spAC;
    s32 i;
    s32 pad[5]; // probably separate iterators for each loop
    Actor* actor;
    Boss* boss;
    Sprite* sprite;
    Scenery360* scenery360;
    Item* item;
    Scenery* scenery;

    if ((gLevelMode == LEVELMODE_ALL_RANGE) && (gCurrentLevel != LEVEL_KATINA)) {
        RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

        if (gDrawBackdrop == 5) {
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
        }

        for (i = 0, scenery360 = gScenery360; i < 200; i++, scenery360++) {
            if ((scenery360->obj.status == OBJ_ACTIVE) && (scenery360->obj.id != OBJ_SCENERY_LEVEL_OBJECTS)) {
                if (gCurrentLevel == LEVEL_BOLSE) {
                    spAC.x = scenery360->sfxSource[0];
                    spAC.y = scenery360->sfxSource[1];
                    spAC.z = scenery360->sfxSource[2];
                    Matrix_MultVec3fNoTranslate(&D_BO_8019EE80, &spAC, &scenery360->obj.pos);
                    scenery360->obj.rot.y = scenery360->unk_54 + gBosses->obj.rot.y;
                }

                if (gCamera1Skipped) {
                    // Skip
                    // @recomp Tag the transform
                    gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_SCENERY_360(scenery360), G_EX_PUSH,
                                                    G_MTX_MODELVIEW, G_EX_EDIT_NONE);
                } else {
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY_360(scenery360), G_EX_PUSH,
                                                   G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
                }

                Matrix_Push(&gGfxMatrix);
                Scenery360_Draw(scenery360);
                Matrix_Pop(&gGfxMatrix);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            }
        }
    } else {
        RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

        for (i = 0, scenery = &gScenery[0]; i < ARRAY_COUNT(gScenery); i++, scenery++) {
            if (scenery->obj.status >= OBJ_ACTIVE) {
                if (cullDirection > 0) {
                    Display_SetSecondLight(&scenery->obj.pos);
                }

                if (gCamera1Skipped) {
                    // Skip
                    // @recomp Tag the transform
                    gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_SCENERY(scenery), G_EX_PUSH, G_MTX_MODELVIEW,
                                                    G_EX_EDIT_NONE);
                } else {
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(scenery), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                }

                Matrix_Push(&gGfxMatrix);
                Scenery_Draw(scenery, cullDirection);
                Matrix_Pop(&gGfxMatrix);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

                Object_UpdateSfxSource(scenery->sfxSource);
            }
        }
    }

    for (i = 0, boss = &gBosses[0]; i < ARRAY_COUNT(gBosses); i++, boss++) {
        if ((boss->obj.status >= OBJ_ACTIVE) && (boss->obj.id != OBJ_BOSS_BO_BASE_SHIELD)) {
            if ((boss->timer_05C % 2) == 0) {
                RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            } else {
                RCP_SetupDL_27();
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 64, 64, 255, 255);
            }

            Object_SetCullDirection(cullDirection);

            if (gCamera1Skipped) {
                // Skip
                // @recomp Tag the transform
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_BOSS(boss), G_EX_PUSH, G_MTX_MODELVIEW,
                                                G_EX_EDIT_NONE);
            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(boss), G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);
            }

            Matrix_Push(&gGfxMatrix);
            Boss_Draw(boss, cullDirection);
            Matrix_Pop(&gGfxMatrix);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            if (boss->drawShadow && (D_edisplay_801615D0.y > 0.0f)) {
                Matrix_Push(&gGfxMatrix);
                Object_DrawShadow(i, &boss->obj);
                Matrix_Pop(&gGfxMatrix);
            }
        }
    }

    Lights_SetOneLight(&gMasterDisp, gLight1x, gLight1y, gLight1z, gLight1R, gLight1G, gLight1B, gAmbientR, gAmbientG,
                       gAmbientB);

    for (i = 0, sprite = &gSprites[0]; i < ARRAY_COUNT(gSprites); i++, sprite++) {
        if ((sprite->obj.status >= OBJ_ACTIVE) && func_enmy_80060FE4(&sprite->obj.pos, -12000.0f)) {

            if (gCamera1Skipped) {
                // Skip
                // @recomp Tag the transform
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_ADDRESS(sprite), G_EX_PUSH, G_MTX_MODELVIEW,
                                                G_EX_EDIT_NONE);
            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ADDRESS(sprite), G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);
            }

            Matrix_Push(&gGfxMatrix);

            if ((sprite->obj.id == OBJ_SPRITE_CO_RUIN1) || (sprite->obj.id == OBJ_SPRITE_CO_RUIN2)) {
                RCP_SetupDL_57(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            } else {
                RCP_SetupDL_60(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            }

            Sprite_Draw(sprite, cullDirection);
            Matrix_Pop(&gGfxMatrix);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }

    for (i = 0, actor = &gActors[0]; i < ARRAY_COUNT(gActors); i++, actor++) {
        if (actor->obj.status >= OBJ_ACTIVE) {

            if (gCamera1Skipped) {
                // Skip
                // @recomp Tag the transform
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_ACTOR(actor), G_EX_PUSH, G_MTX_MODELVIEW,
                                                G_EX_EDIT_NONE);
            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ACTOR(actor), G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);
            }

            if ((actor->timer_0C6 % 2) == 0) {
                if (gCurrentLevel == LEVEL_UNK_15) {
                    RCP_SetupDL_23();
                } else {
                    RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
                }
            } else {
                RCP_SetupDL_27();

                if (actor->scale >= 0.0f) {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 0, 0, 255);
                } else {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 64, 64, 255, 255);
                }
            }

            switch (gLevelMode) {
                case LEVELMODE_ON_RAILS:
                case LEVELMODE_TURRET:
                    Matrix_Push(&gGfxMatrix);

                    if ((gPlayer[0].state == PLAYERSTATE_LEVEL_INTRO) ||
                        (gPlayer[0].state == PLAYERSTATE_LEVEL_COMPLETE) || (gCurrentLevel == LEVEL_AQUAS)) {
                        Display_SetSecondLight(&actor->obj.pos);
                    }

                    Object_SetCullDirection(cullDirection);
                    Actor_DrawOnRails(actor);
                    Matrix_Pop(&gGfxMatrix);

                    if (actor->drawShadow) {
                        Matrix_Push(&gGfxMatrix);
                        Object_DrawShadow(i, &actor->obj);
                        Matrix_Pop(&gGfxMatrix);
                    }
                    break;

                case LEVELMODE_ALL_RANGE:
                    Matrix_Push(&gGfxMatrix);
                    Actor_DrawAllRange(actor);
                    Matrix_Pop(&gGfxMatrix);
                    if (actor->drawShadow && sDrewActor &&
                        ((sViewPos.z > -4000.0f) || (gCurrentLevel != LEVEL_KATINA))) {
                        Matrix_Push(&gGfxMatrix);
                        Object_DrawShadow(i, &actor->obj);
                        Matrix_Pop(&gGfxMatrix);
                    }
                    break;
            }
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }

    gDPSetFogColor(gMasterDisp++, gFogRed, gFogGreen, gFogBlue, gFogAlpha);
    gSPFogPosition(gMasterDisp++, gFogNear, gFogFar);

    Lights_SetOneLight(&gMasterDisp, -60, -60, 60, 150, 150, 150, 20, 20, 20);

    for (i = 0, item = &gItems[0]; i < ARRAY_COUNT(gItems); i++, item++) {
        if (item->obj.status >= OBJ_ACTIVE) {
            if (gCamera1Skipped) {
                // Skip
                // @recomp Tag the transform
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_ITEM(item), G_EX_PUSH, G_MTX_MODELVIEW,
                                                G_EX_EDIT_NONE);
            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ITEM(item), G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);
            }

            Matrix_Push(&gGfxMatrix);
            RCP_SetupDL(&gMasterDisp, SETUPDL_29);
            Object_SetCullDirection(cullDirection);
            Item_Draw(item, cullDirection);
            Matrix_Pop(&gGfxMatrix);
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }

    Lights_SetOneLight(&gMasterDisp, gLight1x, gLight1y, gLight1z, gLight1R, gLight1G, gLight1B, gAmbientR, gAmbientG,
                       gAmbientB);
}
#endif

Gfx SETUPDL_62_POINT[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsSPSetGeometryMode(0),
    gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ALPHACOMPARE, 3, G_AC_NONE | G_ZS_PIXEL),
    gsDPSetRenderMode(G_RM_CLD_SURF, G_RM_CLD_SURF2),
    gsSPSetOtherModeHi(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                       G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE),
    gsSPEndDisplayList(),
};

RECOMP_PATCH void BonusText_DrawAll(void) {
    BonusText* bonus;
    s32 i;

    // RCP_SetupDL(&gMasterDisp, SETUPDL_62);
    gSPDisplayList(gMasterDisp++, SETUPDL_62_POINT);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

    for (i = 0, bonus = gBonusText; i < ARRAY_COUNT(gBonusText); i++, bonus++) {
        if (bonus->hits != 0) {

            if (gCamera1Skipped) {
                // Skip
                // @recomp Tag the transform
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_LIMB_ADDRESS(bonus, bonus->hits), G_EX_PUSH,
                                                G_MTX_MODELVIEW, G_EX_EDIT_NONE);
            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_LIMB_ADDRESS(bonus, bonus->hits), G_EX_PUSH,
                                               G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
            }

            Matrix_Push(&gGfxMatrix);
            BonusText_Draw(bonus);
            Matrix_Pop(&gGfxMatrix);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }
}

RECOMP_PATCH void Effect_DrawAll(s32 arg0) {
    s32 i;
    Boss* boss;
    Effect* effect;

    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    for (i = 0, effect = &gEffects[0]; i < ARRAY_COUNT(gEffects); i++, effect++) {
        if (effect->obj.status >= OBJ_ACTIVE) {
            if (effect->info.unk_14 == 1) {
                effect->obj.rot.y = RAD_TO_DEG(-gPlayer[gPlayerNum].camYaw);
                effect->obj.rot.x = RAD_TO_DEG(gPlayer[gPlayerNum].camPitch);
            }

            if (gCamera1Skipped) {
                // Skip
                // @recomp Tag the transform
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_EFFECT(effect), G_EX_PUSH, G_MTX_MODELVIEW,
                                                G_EX_EDIT_NONE);
            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(effect), G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);
            }

            if (gLevelMode == LEVELMODE_ALL_RANGE) {
                Matrix_Push(&gGfxMatrix);
                Effect_DrawAllRange(effect);
                Matrix_Pop(&gGfxMatrix);
            } else {
                Matrix_Push(&gGfxMatrix);
                Effect_DrawOnRails(effect, arg0);
                Matrix_Pop(&gGfxMatrix);
                Object_UpdateSfxSource(effect->sfxSource);
                if (effect->obj.id == OBJ_EFFECT_FLAME_PILLAR) {
                    Matrix_Push(&gGfxMatrix);
                    Object_DrawShadow(i, &effect->obj);
                    Matrix_Pop(&gGfxMatrix);
                }
            }
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }

    for (i = 0, boss = &gBosses[0]; i < ARRAY_COUNT(gBosses); i++, boss++) {
        if ((boss->obj.status >= OBJ_ACTIVE) && (boss->obj.id == OBJ_BOSS_BO_BASE_SHIELD)) {
            if ((boss->timer_05C % 2) == 0) {
                RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            } else {
                RCP_SetupDL_27();
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 64, 64, 255, 255);
            }

            if (gCamera1Skipped) {
                // Skip
                // @recomp Tag the transform
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_BOSS(boss), G_EX_PUSH, G_MTX_MODELVIEW,
                                                G_EX_EDIT_NONE);
            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(boss), G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);
            }

            Matrix_Push(&gGfxMatrix);
            Boss_Draw(boss, arg0);
            Matrix_Pop(&gGfxMatrix);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }
}

#if 1 // Player Shots
RECOMP_PATCH void PlayerShot_DrawShot(PlayerShot* shot) {
    Vec3f sp11C = { 0.0f, 0.0f, 0.0f };
    s32 pad[4];
    f32 var_fv1;
    s32 isDrawn = false;

    Matrix_Translate(gGfxMatrix, shot->obj.pos.x, shot->obj.pos.y, shot->obj.pos.z + gPathProgress, MTXF_APPLY);
    Matrix_MultVec3f(gGfxMatrix, &sp11C, &sShotViewPos);

    if ((sShotViewPos.z < 0.0f) && (sShotViewPos.z > -10000.0f)) {
        var_fv1 = fabsf(sShotViewPos.x); // probably fake. The use of temps was likely different
        // @recomp: Adjust playershots to widescreen.
        if (var_fv1 < (fabsf(sShotViewPos.z * /* 0.5f */ 1.5f) + 500.0f)) {
            if (fabsf(sShotViewPos.y) < (fabsf(sShotViewPos.z * 0.5f) + 500.0f)) {
                isDrawn = true;
            }
        }
    }

    if (!isDrawn) {
        if ((shot->obj.id != PLAYERSHOT_BOMB) && (shot->timer < 10) && !gVersusMode) {
            Object_Kill(&shot->obj, shot->sfxSource);
        }
    } else {
        if (gCamera1Skipped) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_PLAYER_SHOT(shot), G_EX_PUSH, G_MTX_MODELVIEW,
                                            G_EX_EDIT_NONE);
        } else {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_PLAYER_SHOT(shot), G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);
        }

        if ((shot->obj.id == PLAYERSHOT_TANK) || (shot->obj.id == PLAYERSHOT_ON_FOOT) ||
            (shot->obj.id == PLAYERSHOT_7)) {
            shot->obj.rot.y = RAD_TO_DEG(-gPlayer[gPlayerNum].camYaw);
            shot->obj.rot.x = RAD_TO_DEG(gPlayer[gPlayerNum].camPitch);
        }
        if (shot->obj.id != PLAYERSHOT_2) {
            Matrix_RotateY(gGfxMatrix, shot->obj.rot.y * M_DTOR, MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, shot->obj.rot.x * M_DTOR, MTXF_APPLY);
            Matrix_RotateZ(gGfxMatrix, shot->obj.rot.z * M_DTOR, MTXF_APPLY);
            if ((shot->obj.id == PLAYERSHOT_BOMB) && (shot->unk_5C == 0)) {
                Matrix_RotateZ(gGfxMatrix, shot->vec_2C.z * M_DTOR, MTXF_APPLY);
                Matrix_RotateY(gGfxMatrix, shot->vec_2C.y * M_DTOR, MTXF_APPLY);
                Matrix_RotateX(gGfxMatrix, shot->vec_2C.x * M_DTOR, MTXF_APPLY);
            }
        }

        Matrix_Translate(gGfxMatrix, 0.f, 0.f, D_800C9BD8[shot->obj.id], MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);

        switch (shot->obj.id) {
            case PLAYERSHOT_SINGLE_LASER:
            case PLAYERSHOT_TWIN_LASER:

                PlayerShot_DrawLaser(shot);

                break;
            case PLAYERSHOT_BOMB:
                if (shot->unk_5C == 0) {
                    RCP_SetupDL(&gMasterDisp, SETUPDL_29);
                    gSPDisplayList(gMasterDisp++, aItemBombDL);
                    Matrix_Pop(&gGfxMatrix);
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, shot->obj.pos.x, shot->obj.pos.y, shot->obj.pos.z + gPathProgress,
                                     MTXF_APPLY);
                    Matrix_RotateY(gGfxMatrix, -gPlayer[gPlayerNum].camYaw, MTXF_APPLY);
                    Matrix_RotateX(gGfxMatrix, gPlayer[gPlayerNum].camPitch, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 40.0f * M_DTOR, MTXF_APPLY);
                    if (((gGameFrameCount % 2) == 0)) {
                        Matrix_Scale(gGfxMatrix, 1.7f, 1.7f, 1.7f, MTXF_APPLY);
                    } else {
                        Matrix_Scale(gGfxMatrix, 1.2f, 1.2f, 1.2f, MTXF_APPLY);
                    }
                    RCP_SetupDL(&gMasterDisp, SETUPDL_49);
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                    if (gVersusMode) {
                        switch (shot->sourceId) {
                            case 0:
                                gDPSetEnvColor(gMasterDisp++, 255, 255, 32, 128);
                                break;
                            case 1:
                                gDPSetEnvColor(gMasterDisp++, 255, 32, 32, 128);
                                break;
                            case 2:
                                gDPSetEnvColor(gMasterDisp++, 32, 255, 32, 128);
                                break;
                            case 3:
                                gDPSetEnvColor(gMasterDisp++, 32, 32, 255, 128);
                                break;
                        }
                    } else {
                        gDPSetEnvColor(gMasterDisp++, 0, 128, 255, 255);
                    }
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, aOrbDL);
                } else {
                    Matrix_Scale(gGfxMatrix, shot->scale, shot->scale, shot->scale, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    if (gVersusMode) {
                        RCP_SetupDL_49();
                        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, shot->unk_58);
                        switch (shot->sourceId) {
                            case 0:
                                gDPSetEnvColor(gMasterDisp++, 255, 255, 32, shot->unk_58);
                                break;
                            case 1:
                                gDPSetEnvColor(gMasterDisp++, 255, 32, 32, shot->unk_58);
                                break;
                            case 2:
                                gDPSetEnvColor(gMasterDisp++, 32, 255, 32, shot->unk_58);
                                break;
                            case 3:
                                gDPSetEnvColor(gMasterDisp++, 32, 32, 255, shot->unk_58);
                                break;
                        }
                    } else {
                        RCP_SetupDL_64_2();
                        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, shot->unk_58);
                    }
                    gSPDisplayList(gMasterDisp++, D_1031EC0);
                }
                break;
            case PLAYERSHOT_TANK:
                gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                RCP_SetupDL_40();
                if (gVersusMode) {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                    gDPSetEnvColor(gMasterDisp++, D_800C9C00[shot->sourceId], D_800C9C04[shot->sourceId],
                                   D_800C9C08[shot->sourceId], 255);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_49);
                    if (gLaserStrength[shot->sourceId] != LASERS_SINGLE) {
                        Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 48.0f * M_DTOR, MTXF_APPLY);
                        Matrix_Push(&gGfxMatrix);
                        Matrix_Translate(gGfxMatrix, 0.f, 50.0f, 0.0f, MTXF_APPLY);
                        Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 70.0f * M_DTOR, MTXF_APPLY);
                        Matrix_Scale(gGfxMatrix, 2.0f, 2.0f, 2.0f, MTXF_APPLY);
                        Matrix_SetGfxMtx(&gMasterDisp);
                        gSPDisplayList(gMasterDisp++, D_versus_301AD60);
                        Matrix_Pop(&gGfxMatrix);
                        Matrix_Translate(gGfxMatrix, 0.0f, -50.0f, 0.0f, MTXF_APPLY);
                        Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 70.0f * M_DTOR, MTXF_APPLY);
                        Matrix_Scale(gGfxMatrix, 2.0f, 2.0f, 2.0f, MTXF_APPLY);
                        Matrix_SetGfxMtx(&gMasterDisp);
                        gSPDisplayList(gMasterDisp++, D_versus_301AD60);
                    } else {
                        if (((gGameFrameCount % 2) == 0)) {
                            var_fv1 = M_PI;
                        } else {
                            var_fv1 = 0.0f;
                        }
                        Matrix_RotateY(gGfxMatrix, var_fv1, MTXF_APPLY);
                        Matrix_RotateZ(gGfxMatrix, 30.0f * M_DTOR, MTXF_APPLY);
                        Matrix_Scale(gGfxMatrix, 2.0f, 2.0f, 2.0f, MTXF_APPLY);
                        Matrix_SetGfxMtx(&gMasterDisp);
                        gSPDisplayList(gMasterDisp++, D_versus_301AD60);
                    }
                } else {
                    if (((gGameFrameCount % 2) == 0)) {
                        var_fv1 = M_PI;
                    } else {
                        var_fv1 = 0.0f;
                    }
                    Matrix_RotateY(gGfxMatrix, var_fv1, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, 30.0f * M_DTOR, MTXF_APPLY);
                    Matrix_Scale(gGfxMatrix, 1.0f, 0.65f, 1.0f, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    RCP_SetupDL_68();
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                    gDPSetEnvColor(gMasterDisp++, 0, 255, 0, 0);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, aLandmasterShotDL);
                }
                break;
            case PLAYERSHOT_ON_FOOT:
            case PLAYERSHOT_7:
                Matrix_Scale(gGfxMatrix, shot->scale, shot->scale, shot->scale, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                RCP_SetupDL(&gMasterDisp, SETUPDL_49);
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                gDPSetEnvColor(gMasterDisp++, D_800C9C00[shot->sourceId], D_800C9C04[shot->sourceId],
                               D_800C9C08[shot->sourceId], 255);
                if (gLaserStrength[shot->sourceId] != LASERS_SINGLE) {
                    Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 48.0f * M_DTOR, MTXF_APPLY);
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, 0.f, 40.0f, 0.0f, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, D_versus_301AEF0);
                    Matrix_Pop(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, 0.f, -40.0f, 0.0f, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, D_versus_301AEF0);
                } else {
                    gSPDisplayList(gMasterDisp++, D_versus_301AEF0);
                }
                break;
            case PLAYERSHOT_LOCK_ON:
                Matrix_RotateY(gGfxMatrix, M_PI, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                if (gCurrentLevel == LEVEL_AQUAS) {
                    RCP_SetupDL(&gMasterDisp, SETUPDL_60);
                    gSPDisplayList(gMasterDisp++, D_AQ_600DB80);
                    Matrix_Pop(&gGfxMatrix);
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, shot->obj.pos.x, gGroundHeight, shot->obj.pos.z + gPathProgress,
                                     MTXF_APPLY);
                    Matrix_Scale(gGfxMatrix, shot->vec_2C.x, shot->vec_2C.y, shot->vec_2C.z, MTXF_APPLY);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_49);
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 32, 32, 0, shot->unk_58);
                    gDPSetEnvColor(gMasterDisp++, 32, 16, 16, shot->unk_58);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, aOrbDL);
                } else if (shot->unk_5C == 0) {
                    if (shot->scale > 1.5f) {
                        break;
                    }
                    Matrix_Pop(&gGfxMatrix);
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, shot->obj.pos.x, shot->obj.pos.y, shot->obj.pos.z + gPathProgress,
                                     MTXF_APPLY);
                    Matrix_RotateY(gGfxMatrix, -gPlayer[gPlayerNum].camYaw, MTXF_APPLY);
                    Matrix_RotateX(gGfxMatrix, gPlayer[gPlayerNum].camPitch, MTXF_APPLY);
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Scale(gGfxMatrix, 20.0f, 20.0f, 20.0f, MTXF_APPLY);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_49);
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                    if (gVersusMode) {
                        switch (shot->sourceId) {
                            case 0:
                                gDPSetEnvColor(gMasterDisp++, 255, 255, 32, 255);
                                break;
                            case 1:
                                gDPSetEnvColor(gMasterDisp++, 255, 32, 32, 255);
                                break;
                            case 2:
                                gDPSetEnvColor(gMasterDisp++, 32, 255, 32, 255);
                                break;
                            case 3:
                                gDPSetEnvColor(gMasterDisp++, 32, 32, 255, 255);
                                break;
                        }
                    } else {
                        gDPSetEnvColor(gMasterDisp++, 0, 255, 0, 255);
                    }
                    Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 53.0f * M_DTOR, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, aStarDL);
                    Matrix_RotateZ(gGfxMatrix, gGameFrameCount * -53.0f * 2.0f * M_DTOR, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, aStarDL);
                    Matrix_Pop(&gGfxMatrix);
                    if (((gGameFrameCount % 2) == 0)) {
                        Matrix_Scale(gGfxMatrix, 1.7f, 1.7f, 1.7f, MTXF_APPLY);
                    } else {
                        Matrix_Scale(gGfxMatrix, 1.3f, 1.3f, 1.3f, MTXF_APPLY);
                    }
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, aOrbDL);
                } else {
                    RCP_SetupDL(&gMasterDisp, SETUPDL_29);
                    gSPDisplayList(gMasterDisp++, aItemBombDL);
                    Matrix_Pop(&gGfxMatrix);
                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, shot->obj.pos.x, shot->obj.pos.y, shot->obj.pos.z + gPathProgress,
                                     MTXF_APPLY);
                    Matrix_RotateY(gGfxMatrix, -gPlayer[gPlayerNum].camYaw, MTXF_APPLY);
                    Matrix_RotateX(gGfxMatrix, gPlayer[gPlayerNum].camPitch, MTXF_APPLY);
                    if (((gGameFrameCount % 2) == 0)) {
                        Matrix_Scale(gGfxMatrix, 1.2f, 1.2f, 1.2f, MTXF_APPLY);
                    } else {
                        Matrix_Scale(gGfxMatrix, 1.5f, 1.5f, 1.5f, MTXF_APPLY);
                    }
                    RCP_SetupDL(&gMasterDisp, SETUPDL_49);
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 0, 255);
                    gDPSetEnvColor(gMasterDisp++, 255, 128, 128, 255);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, aOrbDL);
                }
                break;
            case PLAYERSHOT_GFOX_LASER:
                Matrix_RotateY(gGfxMatrix, M_PI, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 3.0f, 3.0f, 20.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                RCP_SetupDL_40();
                gSPDisplayList(gMasterDisp++, D_GREAT_FOX_E00DFB0);
                break;
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1 // Hit Marks
RECOMP_PATCH void PlayerShot_DrawHitmark(PlayerShot* shot) {
    Vec3f src = { 0.0f, 0.0f, 0.0f };
    s32 isDrawn = false;

    Matrix_Translate(gGfxMatrix, shot->obj.pos.x, shot->obj.pos.y, shot->obj.pos.z + gPathProgress, MTXF_APPLY);
    Matrix_MultVec3f(gGfxMatrix, &src, &sShotViewPos);

    if ((sShotViewPos.z < 0.0f) && (sShotViewPos.z > -10000.0f)) {
        if (fabsf(sShotViewPos.x) < (fabsf(sShotViewPos.z * 0.5f) + 500.0f)) {
            if (fabsf(sShotViewPos.y) < (fabsf(sShotViewPos.z * 0.5f) + 500.0f)) {
                isDrawn = true;
            }
        }
    }

    if (!isDrawn) {
        if (!gVersusMode) {
            Object_Kill(&shot->obj, shot->sfxSource);
            return;
        }
    } else {
        if (gCamera1Skipped) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_PLAYER_SHOT(shot) | 0x01000000, G_EX_PUSH,
                                            G_MTX_MODELVIEW, G_EX_EDIT_NONE);
        } else {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_PLAYER_SHOT(shot) | 0x01000000, G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        }

        shot->obj.rot.y = -gPlayer[gPlayerNum].camYaw;
        Matrix_RotateY(gGfxMatrix, shot->obj.rot.y, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, 2.0f, 2.0f, 2.0f, MTXF_APPLY);

        if (shot->obj.id == PLAYERSHOT_7) {
            Matrix_Scale(gGfxMatrix, 0.2f, 0.2f, 0.2f, MTXF_APPLY);
        }
        Matrix_SetGfxMtx(&gMasterDisp);
        RCP_SetupDL_40();

        switch (shot->unk_60) {
            case 0:
                gSPDisplayList(gMasterDisp++, D_1026090);
                break;
            case 1:
                gSPDisplayList(gMasterDisp++, D_1025800);
                break;
            case 2:
                Matrix_Scale(gGfxMatrix, 1.5f, 0.7f, 1.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_1025800);
                break;
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}
#endif