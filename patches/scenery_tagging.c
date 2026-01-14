#include "patches.h"

#if 1

// Custom Scenery struct uses 32 bits of compiler-inserted padding to hold the skipRot status.
typedef struct SceneryRecomp {
    /* 0x00 */ Object obj;
    /* 0x1C */ ObjectInfo info;
    /* 0x40 */ s32 index;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 state;
    /* 0x4C */ s32 timer_4C;
    /* 0x50 */ s8 dmgType;
    /* 0x54 */ s32 dmgPart;
    /* 0x58 */ Vec3f effectVel;
    /* 0x64 */ Vec3f vel;
    /* 0x70 */ f32 sfxSource[3];
    /* 0x7C */ bool skipRot; // Should rotation be skipped for this Scenery actor.
} SceneryRecomp;             // size = 0x80

void Object_SetMatrix(Object* obj, s32 drawType);
void Object_SetCullDirection(s32);
void Recomp_CoBuilding_Draw(SceneryRecomp* this);
bool Macbeth_801A0DD8(s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3f* rot, void* thisx);
void Macbeth_801A0E2C(s32 limbIndex, Vec3f* rot, void* thisx);

RECOMP_PATCH void Scenery_Draw(Scenery* this, s32 cullDir) {
    this->obj.pos.y += gCameraShakeY;
    Object_SetMatrix(&this->obj, this->info.drawType);
    this->obj.pos.y -= gCameraShakeY;

    if (this->info.drawType == 0) {
        if ((this->obj.id == OBJ_SCENERY_CO_TOWER) || (this->obj.id == OBJ_SCENERY_CO_ROCKWALL) ||
            (this->obj.id == OBJ_SCENERY_CO_HIGHWAY_4) || (this->obj.id == OBJ_SCENERY_VE1_WALL_3)) {
            RCP_SetupDL_57(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);

            if (cullDir < 0) {
                Object_ApplyWaterDistortion();
            }
            // @recomp Tag the transform.
            // gEXMatrixGroupDecomposedSkipRot(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW,
            //                                G_EX_EDIT_ALLOW);
            gSPDisplayList(gMasterDisp++, this->info.dList);
            // @recomp Pop the transform id.
            // EXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
        } else {
            if (this->obj.id == OBJ_SCENERY_CO_HIGHWAY_3) {
                if (cullDir < 0) {
                    return; // weird control flow
                }
                RCP_SetupDL_60(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            }

            Object_SetCullDirection(cullDir);

            if (cullDir < 0) {
                Object_ApplyWaterDistortion();
            }

            // @recomp: Intercepting specific actors.
            switch (this->obj.id) {
                case OBJ_SCENERY_CO_BUILDING_5:
                case OBJ_SCENERY_CO_BUILDING_6:
                case OBJ_SCENERY_CO_BUILDING_7:
                case OBJ_SCENERY_CO_BUILDING_8:
                    Recomp_CoBuilding_Draw((SceneryRecomp*) this);
                    return;

                default:
                    // @recomp Tag the transform.
                    // gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW,
                    //                                G_EX_EDIT_ALLOW);
                    break;
            }
            gSPDisplayList(gMasterDisp++, this->info.dList);

            // @recomp Pop the transform id.
            // gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            gSPClearGeometryMode(gMasterDisp++, G_TEXTURE_GEN);

            if (this->obj.id == OBJ_SCENERY_CO_HIGHWAY_3) {
                RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            }
        }
    } else if (this->info.draw != NULL) {
        Object_SetCullDirection(cullDir);
        this->info.draw(&this->obj);
    }
}

#if 0
RECOMP_PATCH void Sprite_Draw(Sprite* this, s32 arg1) {
    if (arg1 >= 0) {
        this->obj.pos.y += gCameraShakeY;
        Object_SetMatrix(&this->obj, 0);
        this->obj.pos.y -= gCameraShakeY;

        if (this->info.drawType == 0) {
            // @recomp Tag the transform.
            // gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW,
            //                                G_EX_EDIT_ALLOW);

            gSPDisplayList(gMasterDisp++, this->info.dList);

            // @recomp Pop the transform id.
            // gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        } else if (this->info.draw != NULL) {
            this->info.draw(&this->obj);
        }
    }
}
#endif

#if 0
RECOMP_PATCH void CoBuilding9_Draw(CoBuilding9* this) {
    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -95.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aCoBuilding9DL);
}
#endif

RECOMP_PATCH void CoBuilding10_Draw(CoBuilding10* this) {
    // @recomp Replace aCoBuilding10DL for aCoBuilding9DL, a full model of the same building.
    gSPDisplayList(gMasterDisp++, aCoBuilding9DL);
}

#if 0
RECOMP_PATCH void Corneria_CoBuildingOnFire_Draw(CoBuildingOnFire* this) {
    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
    // @recomp Tag the transform.
    // gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    gSPDisplayList(gMasterDisp++, D_CO_60199D0);
    // @recomp Pop the transform id.
    // gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
}
#endif

// @recomp: Replacement draw function for OBJ_SCENERY_CO_BUILDING_8
void Recomp_CoBuilding_Draw(SceneryRecomp* this) {
    f32 prevRot;

    if (this->skipRot) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedSkipRot(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    } else {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    }

    prevRot = this->obj.rot.y;

    if (gPlayer[0].cam.eye.x < this->obj.pos.x) {
        this->obj.rot.y = 271.0f;
    } else {
        this->obj.rot.y = 0.0f;
    }

    this->skipRot = prevRot != this->obj.rot.y;

    switch (this->obj.id) {
        case OBJ_SCENERY_CO_BUILDING_5:
            gSPDisplayList(gMasterDisp++, aCoBuilding5DL);
            break;
        case OBJ_SCENERY_CO_BUILDING_6:
            gSPDisplayList(gMasterDisp++, aCoBuilding6DL);
            break;
        case OBJ_SCENERY_CO_BUILDING_7:
            gSPDisplayList(gMasterDisp++, aCoBuilding7DL);
            break;
        case OBJ_SCENERY_CO_BUILDING_8:
            gSPDisplayList(gMasterDisp++, aCoBuilding8DL);
            break;
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    gSPClearGeometryMode(gMasterDisp++, G_TEXTURE_GEN);
}

RECOMP_PATCH void SceneryRotateTowardsCamera(Scenery* this) {
    switch (this->obj.id) {
        case OBJ_SCENERY_CO_BUILDING_5:
        case OBJ_SCENERY_CO_BUILDING_6:
        case OBJ_SCENERY_CO_BUILDING_7:
        case OBJ_SCENERY_CO_BUILDING_8:
        case OBJ_SCENERY_CO_BUILDING_10:
            return;
    }

    if (gPlayer[0].cam.eye.x < this->obj.pos.x) {
        this->obj.rot.y = 271.0f;
    } else {
        this->obj.rot.y = 0.0f;
    }
}
#if 0
RECOMP_PATCH void CoIBeam_Draw(CoIBeam* this) {
    // @recomp Tag the transform.
    // gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    gSPDisplayList(gMasterDisp++, aCoIBeamDL);
    // @recomp Pop the transform id.
    // gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH s32 SyShipDebris_Draw(SyShipDebris* this) {
    // @recomp Tag the transform.
    // gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    RCP_SetupDL(&gMasterDisp, SETUPDL_60);
    gSPDisplayList(gMasterDisp++, aSyDebrisDL);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);
    // @recomp Pop the transform id.
    // gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    return 0;
}

RECOMP_PATCH void Aquas_AqBump2_Draw(AqBump2* this) {
    // @recomp Tag the transform.
    // gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    Matrix_Scale(gGfxMatrix, 0.5f, 0.5f, 0.5f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aAqBump2DL);
    // @recomp Pop the transform id.
    // gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void FogShadow_Draw(FogShadow* this) {
    // @recomp Tag the transform.
    // gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL_47(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 180);

    switch (this->sceneryId) {
        case OBJ_SCENERY_CO_STONE_ARCH:
            Matrix_Scale(gGfxMatrix, 2.0f, 1.0f, 0.7f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aCoShadow1DL);
            break;

        case OBJ_SCENERY_CO_ARCH_2:
            Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.7f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aCoShadow1DL);
            break;

        case OBJ_SCENERY_CO_HIGHWAY_1:
        case OBJ_SCENERY_CO_HIGHWAY_2:
            Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 10.55f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aCoHighwayShadowDL);
            break;

        case OBJ_SCENERY_CO_DOORS:
            Matrix_Scale(gGfxMatrix, 1.6f, 1.0f, 1.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aCoShadow1DL);
            break;

        case OBJ_SCENERY_CO_ARCH_1:
            Matrix_Scale(gGfxMatrix, 1.2f, 1.0f, 1.3f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aCoShadow1DL);
            break;

        case OBJ_SCENERY_CO_ARCH_3:
            Matrix_Scale(gGfxMatrix, 2.2f, 1.0f, 1.4f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aCoShadow1DL);
            break;
    }
    // @recomp Pop the transform id.
    // gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    RCP_SetupDL_60(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
}

RECOMP_PATCH void Ve1Wall1_Draw(Ve1Wall1* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL_57(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    Matrix_RotateY(gGfxMatrix, M_PI, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aVe1Wall1DL);
    RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Ve1Wall2_Draw(Ve1Wall2* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL_57(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    Matrix_RotateY(gGfxMatrix, M_PI, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aVe1Wall2DL);
    RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Titania_TiPillar_Draw(TiPillar* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    switch (this->unk_44) {
        case 0:
            if (this->unk_44 == 0) {
                gSPDisplayList(gMasterDisp++, D_TI1_7002270);
            } else {
                gSPDisplayList(gMasterDisp++, D_TI1_7000A80);
            }
            break;

        case 1:
        case 2:
            if (this->obj.rot.z != 0.0f) {
                if (this->obj.rot.z > 0.0f) {
                    Matrix_Translate(gGfxMatrix, 75.0f, 0.0f, 0.0f, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                } else {
                    Matrix_Translate(gGfxMatrix, -75.0f, 0.0f, 0.0f, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                }
            }

            if (this->unk_44 == 0) { // Can never be true, this condition is never going to pass.
                gSPDisplayList(gMasterDisp++, D_TI1_7002270);
            } else {
                gSPDisplayList(gMasterDisp++, D_TI1_7000A80);
            }
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Macbeth_MaTower_Draw(Scenery* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL(&gMasterDisp, SETUPDL_57);
    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
    gSPDisplayList(gMasterDisp++, aMaTowerBottomDL);
    gSPDisplayList(gMasterDisp++, aMaTowerTopDL);
    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
    RCP_SetupDL(&gMasterDisp, SETUPDL_29);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Macbeth_MaProximityLight_Draw(MaProximityLight* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if ((gPlayer[0].trueZpos - this->obj.pos.z) < this->vel.z) {
        if (gPlayState != PLAY_PAUSE) {
            Math_SmoothStepToF(&this->vel.x, 30, 0.5f, 30.0f, 0.0f);
            if (this->vel.x < 31.0f) {
                this->vel.x = 255.0f;
            }
        }
        RCP_SetupDL(&gMasterDisp, SETUPDL_34);
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, this->vel.x, 0, 0, 255);
    }
    gSPDisplayList(gMasterDisp++, aMaProximityLightSidesDL);
    RCP_SetupDL(&gMasterDisp, SETUPDL_29);
    gSPDisplayList(gMasterDisp++, aMaProximityLightTopDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

// Scenery 77 to 82, and 84 to 91
RECOMP_PATCH void Macbeth_IndicatorSign_Draw(Scenery* this) {
    if (gPlayer[0].state == PLAYERSTATE_LEVEL_COMPLETE) {
        Object_Kill(&this->obj, this->sfxSource);
    }

    RCP_SetupDL(&gMasterDisp, SETUPDL_57);

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    switch (this->obj.id) {
        case OBJ_SCENERY_MA_INDICATOR_SIGN:
            gSPDisplayList(gMasterDisp++, aMaIndicatorSignDL);
            break;
        case OBJ_SCENERY_MA_DISTANCE_SIGN_1:
            gSPDisplayList(gMasterDisp++, aMaDistanceSign1DL);
            break;
        case OBJ_SCENERY_MA_DISTANCE_SIGN_2:
            gSPDisplayList(gMasterDisp++, aMaDistanceSign2DL);
            break;
        case OBJ_SCENERY_MA_DISTANCE_SIGN_3:
            gSPDisplayList(gMasterDisp++, aMaDistanceSign3DL);
            break;
        case OBJ_SCENERY_MA_DISTANCE_SIGN_4:
            gSPDisplayList(gMasterDisp++, aMaDistanceSign4DL);
            break;
        case OBJ_SCENERY_MA_DISTANCE_SIGN_5:
            gSPDisplayList(gMasterDisp++, aMaDistanceSign5DL);
            break;
        case OBJ_SCENERY_MA_RAILROAD_SWITCH_1:
            gSPDisplayList(gMasterDisp++, aMaRailroadSwitch1DL);
            break;
        case OBJ_SCENERY_MA_RAILROAD_SWITCH_2:
            gSPDisplayList(gMasterDisp++, aMaRailroadSwitch2DL);
            break;
        case OBJ_SCENERY_MA_RAILROAD_SWITCH_3:
            gSPDisplayList(gMasterDisp++, aMaRailroadSwitch3DL);
            break;
        case OBJ_SCENERY_MA_RAILROAD_SWITCH_4:
            gSPDisplayList(gMasterDisp++, aMaRailroadSwitch4DL);
            break;
        case OBJ_SCENERY_MA_RAILROAD_SWITCH_5:
            gSPDisplayList(gMasterDisp++, aMaRailroadSwitch5DL);
            break;
        case OBJ_SCENERY_MA_RAILROAD_SWITCH_6:
            gSPDisplayList(gMasterDisp++, aMaRailroadSwitch6DL);
            break;
        case OBJ_SCENERY_MA_RAILROAD_SWITCH_7:
            gSPDisplayList(gMasterDisp++, aMaRailroadSwitch7DL);
            break;
        case OBJ_SCENERY_MA_RAILROAD_SWITCH_8:
            gSPDisplayList(gMasterDisp++, aMaRailroadSwitch8DL);
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    RCP_SetupDL(&gMasterDisp, SETUPDL_29);
}

RECOMP_PATCH void Macbeth_TrainTrack_Draw(Scenery* this) {
    Vec3f frameTable[50];

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL(&gMasterDisp, SETUPDL_57);

    switch (this->obj.id) {
        case OBJ_SCENERY_MA_TRAIN_TRACK_3:
        case OBJ_SCENERY_MA_TRAIN_TRACK_6:
            if ((gPlayer[0].state != PLAYERSTATE_LEVEL_COMPLETE) &&
                ((gPlayer[0].trueZpos - this->obj.pos.z) < -2500.0f)) {
                Object_Kill(&this->obj, this->sfxSource);
            }
            gSPDisplayList(gMasterDisp++, D_MA_6026860);
            break;

        case OBJ_SCENERY_MA_TRAIN_TRACK_4:
        case OBJ_SCENERY_MA_TRAIN_TRACK_7:
            if ((gPlayer[0].state != PLAYERSTATE_LEVEL_COMPLETE) &&
                ((gPlayer[0].trueZpos - this->obj.pos.z) < -2500.0f)) {
                Object_Kill(&this->obj, this->sfxSource);
            }
            gSPDisplayList(gMasterDisp++, D_MA_602FBF0);
            break;

        case OBJ_SCENERY_MA_TRAIN_TRACK_5:
        case OBJ_SCENERY_MA_TRAIN_TRACK_8:
            gSPDisplayList(gMasterDisp++, D_MA_6022610);
            break;

        case OBJ_SCENERY_MA_TRAIN_TRACK_9:
        case OBJ_SCENERY_MA_TRAIN_TRACK_11:
            gSPDisplayList(gMasterDisp++, D_MA_60309D0);
            break;

        case OBJ_SCENERY_MA_TRAIN_TRACK_10:
        case OBJ_SCENERY_MA_TRAIN_TRACK_12:
            gSPDisplayList(gMasterDisp++, D_MA_6030750);
            break;

        case OBJ_SCENERY_MA_TRAIN_TRACK_13:
            RCP_SetupDL(&gMasterDisp, SETUPDL_29);
            gSPDisplayList(gMasterDisp++, D_MA_602D380);
            break;

        case OBJ_SCENERY_MA_TRAIN_TRACK_1:
            gSPDisplayList(gMasterDisp++, D_MA_60014A0);
            break;

        case OBJ_SCENERY_MA_TRAIN_TRACK_2:
            gSPDisplayList(gMasterDisp++, D_MA_6001180);
            break;

        default:
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    switch (this->obj.id) {
        case OBJ_SCENERY_MA_SWITCH_TRACK:
            Matrix_Push(&gGfxMatrix);
            Animation_GetFrameData(&D_MA_6025CA0, 0, frameTable);
            Animation_DrawSkeleton(1, D_MA_6025DAC, frameTable, Macbeth_801A0DD8, Macbeth_801A0E2C, this,
                                   &gIdentityMatrix);
            Matrix_Pop(&gGfxMatrix);

            if ((this->state == 1) && (this->timer_4C <= 0)) {
                Matrix_Push(&gGfxMatrix);
                RCP_SetupDL_49();
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                gDPSetEnvColor(gMasterDisp++, 255, 48, 0, 255);
                Matrix_Translate(gGfxMatrix, D_i5_801BE688[0].x, D_i5_801BE688[0].y + 50.0f,
                                 D_i5_801BE688[0].z + 100.0f, MTXF_APPLY);
                Matrix_RotateX(gCalcMatrix, (D_PI / 2), MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->vel.z, this->vel.z / 2, this->vel.z, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);
                Matrix_Pop(&gGfxMatrix);
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, D_i5_801BE688[1].x, D_i5_801BE688[1].y + 50.0f,
                                 D_i5_801BE688[1].z + 100.0f, MTXF_APPLY);
                Matrix_RotateX(gCalcMatrix, (D_PI / 2), MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->vel.z, this->vel.z / 2, this->vel.z, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);
                RCP_SetupDL(&gMasterDisp, SETUPDL_29);
                Matrix_Pop(&gGfxMatrix);
            } else if (this->state == 2) {
                Matrix_Push(&gGfxMatrix);
                RCP_SetupDL_49();
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                gDPSetEnvColor(gMasterDisp++, 255, 48, 0, 255);
                Matrix_Translate(gGfxMatrix, D_i5_801BE688[1].x - 50.0f, D_i5_801BE688[1].y + 50.0f,
                                 D_i5_801BE688[1].z + 50.0f, MTXF_APPLY);
                Matrix_RotateX(gCalcMatrix, (D_PI / 2), MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->vel.z, this->vel.z / 2, this->vel.z, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);
                RCP_SetupDL(&gMasterDisp, SETUPDL_29);
                Matrix_Pop(&gGfxMatrix);
            }

            if (D_MA_801BE2F0[5] != 0) {
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -1800.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_MA_601C170);
                Matrix_Pop(&gGfxMatrix);
            } else {
                Matrix_Push(&gGfxMatrix);
                Matrix_RotateY(gGfxMatrix, -(D_PI / 18), MTXF_APPLY);
                Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -1800.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_MA_601C170);
                Matrix_Pop(&gGfxMatrix);
            }
            break;

        default:
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    RCP_SetupDL(&gMasterDisp, SETUPDL_29);
}

RECOMP_PATCH void MeteoTunnel_Draw(MeTunnel* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    gSPDisplayList(gMasterDisp++, aMeMeteoTunnelDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Scenery_DrawTitaniaBones(Scenery* this) {
    // @recomp Tag the transform.
    // gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_SCENERY(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if (this->obj.id == OBJ_SCENERY_TI_SKULL) {
        gSPDisplayList(gMasterDisp++, D_TI1_7007350);
    } else {
        Graphics_SetScaleMtx(D_edisplay_800CFA2C[this->obj.id - 29]);
        gSPDisplayList(gMasterDisp++, D_TI1_700BB10);
    }

    // @recomp Pop the transform id.
    // gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}
#endif

#endif
