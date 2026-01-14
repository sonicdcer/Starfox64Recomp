#include "patches.h"

#define SHADOW_ID (0x2000)
#define PLAYER_SHADOW_ID (0x3000)

void ActorAllRange_DrawShadow(ActorAllRange* this);
void Object_SetShadowDL(ObjectId objId, s32 index);

RECOMP_PATCH void Object_DrawShadow(s32 index, Object* obj) {
    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, gPlayerNum + PLAYER_SHADOW_ID, G_EX_PUSH, G_MTX_MODELVIEW,
                                        G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupSimple(gMasterDisp++, index + SHADOW_ID, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP);
    }

    RCP_SetupDL(&gMasterDisp, SETUPDL_66);
    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 0, 0, 0, 180);

    if (obj->id == OBJ_ACTOR_ALLRANGE) {
        ActorAllRange_DrawShadow(&gActors[index]);
    } else {
        switch (obj->id) {
            case OBJ_ACTOR_TI_BOULDER:
                Matrix_Translate(gGfxMatrix, obj->pos.x, gGroundHeight + 2.0f + gActors[index].fwork[1],
                                 obj->pos.z + gPathProgress, MTXF_APPLY);
                break;

            case OBJ_ACTOR_TI_DESERT_CRAWLER:
            case OBJ_ACTOR_TI_BOMB:
                Matrix_Translate(gGfxMatrix, obj->pos.x, gGroundHeight + 2.0f + gActors[index].fwork[0],
                                 obj->pos.z + gPathProgress, MTXF_APPLY);
                break;

            case OBJ_BOSS_KA_SAUCERER:
                Matrix_Translate(gGfxMatrix, obj->pos.x, gGroundHeight + 2.0f + gCameraShakeY, obj->pos.z, MTXF_APPLY);
                break;

            default:
                Matrix_Translate(gGfxMatrix, obj->pos.x, gGroundHeight + 2.0f, obj->pos.z + gPathProgress, MTXF_APPLY);
                break;
        }

        if ((gGroundType != 4) || (obj->id == OBJ_ACTOR_EVENT) || (obj->id == OBJ_ACTOR_CUTSCENE) ||
            (obj->id == OBJ_ACTOR_TEAM_BOSS)) {
            Matrix_Scale(gGfxMatrix, 1.0f, 0.0f, 1.0f, MTXF_APPLY);
            Matrix_RotateY(gGfxMatrix, obj->rot.y * M_DTOR, MTXF_APPLY);
        }

        if ((obj->id < OBJ_ACTOR_MAX) && (obj->id != OBJ_ACTOR_TI_BOULDER) &&
            (obj->id != OBJ_ACTOR_TI_DESERT_CRAWLER) && (obj->id != OBJ_ACTOR_TI_BOMB)) {
            Matrix_RotateX(gGfxMatrix, obj->rot.x * M_DTOR, MTXF_APPLY);
            Matrix_RotateZ(gGfxMatrix, obj->rot.z * M_DTOR, MTXF_APPLY);
        }
        Matrix_SetGfxMtx(&gMasterDisp);
        Object_SetShadowDL(obj->id, index);
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

extern f32 sPlayerShadowing;
extern Gfx aArwingShadowDL[];
extern Gfx aLandmasterShadowDL[];
extern Gfx D_versus_301E570[];
extern Gfx aBallDL[];

RECOMP_PATCH void Display_PlayerShadow_Draw(Player* player) {
    Math_SmoothStepToF(&sPlayerShadowing, player->shadowing, 1.0f, 10.0f, 0.0f);
    RCP_SetupDL(&gMasterDisp, SETUPDL_66);
    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 0, 0, 0, (s32) sPlayerShadowing);

    if (player->groundPos.y > 30.0f) {
        gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
    }

    if ((gGroundSurface == SURFACE_WATER) && (player->groundPos.y < 10.0f)) {
        player->shadowing = 90;
    } else {
        player->shadowing = 180;
    }

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, gPlayerNum + PLAYER_SHADOW_ID, G_EX_PUSH, G_MTX_MODELVIEW,
                                        G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupSimple(gMasterDisp++, gPlayerNum + PLAYER_SHADOW_ID, G_EX_PUSH, G_MTX_MODELVIEW,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);
    }

    switch (player->form) {
        case FORM_ARWING:
            Matrix_Scale(gGfxMatrix, 1.5f * 1.67f, 1.5f, 1.5f * 1.67f, MTXF_APPLY);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 30.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aArwingShadowDL);
            break;

        case FORM_LANDMASTER:
            Matrix_Push(&gGfxMatrix);
            Matrix_Scale(gGfxMatrix, 1.17f, 1.17f, 1.17f, MTXF_APPLY);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 20.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            if (gCurrentLevel == LEVEL_MACBETH) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_65);
            }
            if (!gVersusMode) {
                gSPDisplayList(gMasterDisp++, aLandmasterShadowDL);
            } else {
                gSPDisplayList(gMasterDisp++, D_versus_301E570);
            }
            Matrix_Pop(&gGfxMatrix);
            break;

        case FORM_BLUE_MARINE:
            break;

        case FORM_ON_FOOT:
            Matrix_Push(&gGfxMatrix);
            Matrix_Scale(gGfxMatrix, 0.5f, 0.5f, 0.5f, MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
            gSPDisplayList(gMasterDisp++, aBallDL);
            gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
            Matrix_Pop(&gGfxMatrix);
            break;
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}