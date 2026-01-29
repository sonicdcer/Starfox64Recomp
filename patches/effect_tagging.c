#include "patches.h"

bool func_effect_800798C4(s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3f* rot, void* data);

RECOMP_PATCH void TexturedLine_Draw(void) {
    s32 i;

    if (gCurrentLevel == LEVEL_MACBETH) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_33);
        gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
    } else if ((gCurrentLevel == LEVEL_AQUAS) || (gCurrentLevel == LEVEL_VENOM_ANDROSS)) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_41);
    } else {
        RCP_SetupDL_14();
    }

    for (i = 0; i < ARRAY_COUNT(gTexturedLines); i++) {
        TexturedLine* texLine = &gTexturedLines[i];

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_TEXTURED_LINE + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        if (gTexturedLines[i].mode != 0) {
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, texLine->posAA.x, texLine->posAA.y, texLine->posAA.z + gPathProgress,
                             MTXF_APPLY);
            Matrix_RotateY(gGfxMatrix, texLine->yRot, MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, texLine->xRot, MTXF_APPLY);
            Matrix_RotateZ(gGfxMatrix, texLine->yRot, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, texLine->xyScale, texLine->xyScale, texLine->zScale, MTXF_APPLY);

            if ((gCurrentLevel == LEVEL_AQUAS) || (gCurrentLevel == LEVEL_VENOM_ANDROSS)) {
                s32 alpha = ((gGameFrameCount % 2) != 0) ? 180 : 50;

                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, alpha);
                if (gCurrentLevel == LEVEL_AQUAS) {
                    Matrix_Scale(gGfxMatrix, 0.01f, 0.3f, 0.0025f, MTXF_APPLY);
                    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -200.0f, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 5.0f * M_DTOR, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, D_AQ_60119A0);
                } else if (gCurrentLevel == LEVEL_VENOM_ANDROSS) {
                    Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, 0.0025f, MTXF_APPLY);
                    Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -200.0f, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 25.0f * M_DTOR, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, aAndLightningAttackDL);
                }
            } else {
                Matrix_SetGfxMtx(&gMasterDisp);
                if (gCurrentLevel == LEVEL_MACBETH) {
                    gSPDisplayList(gMasterDisp++, D_MA_6012C00);
                } else {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, texLine->prim.r, texLine->prim.g, texLine->prim.b,
                                    texLine->prim.a);
                    gSPDisplayList(gMasterDisp++, gTexturedLineDL);
                }
            }
            Matrix_Pop(&gGfxMatrix);
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}

extern Gfx D_A6_6012550[];
// extern int gUvOn; /* Debug */

RECOMP_PATCH void Effect_Effect395_Draw(Effect395* this) {
    //! FAKE: Probably some debug stuff printing different messages depending on what state is.
    if ((this->state != 0) && (this->state != 6)) {
        if (this->unk_4A && this->unk_4A) {}
        PRINTF("ＺＯ＿ＤＩＳＰ ＨＥＬＰ %d\n", this->state);
    }
    if ((this->state != 1) && (this->state != 2) && (this->state != 4) && (this->state != 5) && (this->state != 7)) {
        if (this->unk_4A && !this->scale2) {}
        PRINTF("ＡＣ＿ＤＩＳＰ ＨＥＬＰ %d\n", this->state);
    }

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this) + this->state, G_EX_PUSH, G_MTX_MODELVIEW,
                                   G_EX_EDIT_ALLOW);

    switch (this->state) {
        case 0:
            RCP_SetupDL(&gMasterDisp, SETUPDL_49);
            Matrix_Scale(gGfxMatrix, 0.7f, 0.7f, 1.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
            gDPSetEnvColor(gMasterDisp++, 32, 32, 255, 255);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            break;

        case 1:
            RCP_SetupDL(&gMasterDisp, SETUPDL_41);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
            Graphics_SetScaleMtx(this->scale2);
            gSPDisplayList(gMasterDisp++, aStarDL);
            break;

        case 2:
            RCP_SetupDL(&gMasterDisp, SETUPDL_49);
            Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
            gDPSetEnvColor(gMasterDisp++, 32, 32, 255, 255);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            break;

        case 3:
            RCP_SetupDL_60(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            gSPDisplayList(gMasterDisp++, aA6SpaceMineDL);
            break;

        case 4:
            Graphics_SetScaleMtx(this->scale2);
            RCP_SetupDL(&gMasterDisp, SETUPDL_68);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 0, 0, this->unk_4A);
            gDPSetEnvColor(gMasterDisp++, 0, 0, 0, 0);
            gSPDisplayList(gMasterDisp++, D_1023750);
            break;

        case 5:
        case 7:
            Graphics_SetScaleMtx(this->scale2);
            RCP_SetupDL(&gMasterDisp, SETUPDL_60);
            if (gCurrentLevel == LEVEL_AQUAS) {
                gSPDisplayList(gMasterDisp++, D_AQ_600A220);
            }
            if (gCurrentLevel == LEVEL_ZONESS) {
                gSPDisplayList(gMasterDisp++, D_ZO_6016880);
            }
            break;

        case 6:
            Graphics_SetScaleMtx(this->scale2);
            if (gCurrentLevel == LEVEL_ZONESS) {
                Graphics_SetScaleMtx(this->scale2);
            }
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 255, 0, this->unk_4A);
            gSPDisplayList(gMasterDisp++, D_1023750);
            break;

        case 8:
            RCP_SetupDL(&gMasterDisp, SETUPDL_49);
            Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
            gDPSetEnvColor(gMasterDisp++, 255, 0, 128, 255);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            break;

        case 9:
            RCP_SetupDL(&gMasterDisp, SETUPDL_49);
            Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->scale1);
            gDPSetEnvColor(gMasterDisp++, 255, 255, 128, 255);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            break;

        case 10:
            if (gPlayState != PLAY_PAUSE) {
                //    Lib_Texture_Scroll(D_A6_6012840, 16, 16, 0);
                // @Recomp use UV scrolling instead of CPU
                Gfx_Texture_UV_Scroll(D_A6_6012550 + 6, 16, 16, 0, 1);
            }
            RCP_SetupDL(&gMasterDisp, SETUPDL_53);
            Matrix_Scale(gGfxMatrix, this->orient.x, this->orient.y, this->orient.z, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
            gSPDisplayList(gMasterDisp++, D_A6_6012550);
            break;

        case 11:
            RCP_SetupDL(&gMasterDisp, SETUPDL_67);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 111, 111, 111, (s32) this->scale1);
            gDPSetEnvColor(gMasterDisp++, 255, 255, 255, 255);
            Graphics_SetScaleMtx(this->scale2);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            break;

        case 12:
            Effect_Effect383_Draw(this);
            break;
    }

    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

#if 0 // Global Scope

RECOMP_PATCH void Katina_LaserEnergyParticlesDraw(Effect358* this) {
    RCP_SetupDL(&gMasterDisp, SETUPDL_67);

    gDPSetPrimColor(gMasterDisp++, 0, 0, 48, 48, 255, this->unk_44);
    gDPSetEnvColor(gMasterDisp++, 0, 0, 0, this->unk_44);

    Matrix_Scale(gGfxMatrix, 0.4f, 0.4f, 0.4f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    gSPDisplayList(gMasterDisp++, aOrbDL);
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);
}

RECOMP_PATCH void Effect_Effect343_Draw(Effect343* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    Graphics_SetScaleMtx(this->scale2);
    if (this->unk_44 == 0) {
        gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 0, 0, this->unk_4A);
        gSPDisplayList(gMasterDisp++, D_102A010);
    } else {
        Matrix_Scale(gGfxMatrix, 1.5f, 1.5f, 1.5f, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_4A);
        gSPDisplayList(gMasterDisp++, D_BG_PLANET_20031B0);
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Meteo_Effect370_Draw(Effect370* this) {
    if (this->state == 1) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        Matrix_RotateX(gGfxMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, M_PI / 2, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
        gSPDisplayList(gMasterDisp++, D_ME_60263F0);
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}

RECOMP_PATCH void Meteo_Effect369_Draw(Effect369* this) {
    if (this->state != 0) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        Matrix_RotateX(gGfxMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2 * 3.0f, MTXF_APPLY);

        if ((gGameFrameCount % 2) != 0) {
            Matrix_RotateZ(gGfxMatrix, M_PI, MTXF_APPLY);
        }

        Matrix_RotateX(gGfxMatrix, -M_PI / 2, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 160, 255, 160, this->unk_44);
        gSPDisplayList(gMasterDisp++, D_102F5E0);
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}

RECOMP_PATCH void Meteo_Effect371_Draw(Effect371* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    Matrix_RotateX(gGfxMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);

    switch (this->unk_4C) {
        case 0:
            gSPDisplayList(gMasterDisp++, D_ME_6023810);
        case 1:
            break;
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect372_Draw(Effect372* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
    gSPDisplayList(gMasterDisp++, D_arwing_3016B30);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect382_Draw(Effect382* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL_49();
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
    gDPSetEnvColor(gMasterDisp++, 255, 255, 255, this->unk_44);
    Matrix_Scale(gGfxMatrix, this->scale1, this->scale2, 1.0f, MTXF_APPLY);
    Matrix_Translate(gGfxMatrix, 0.0f, 20.0f, 0.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, D_ZO_6024220);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect384_Draw(Effect384* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);

    if ((this->scale1 == 71.0f) ||
        ((gPlayer[0].state == PLAYERSTATE_LEVEL_COMPLETE) && (gCurrentLevel == LEVEL_CORNERIA))) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_38);
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_67);
    }

    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);

    switch (this->unk_4C) {
        case 0:
            gDPSetEnvColor(gMasterDisp++, 0, 128, 255, this->unk_44);
            break;
        case 1:
            gDPSetEnvColor(gMasterDisp++, 255, 64, 255, this->unk_44);
            break;
        case 2:
            gDPSetEnvColor(gMasterDisp++, 255, 128, 0, this->unk_44);
            break;
        case 3:
            gDPSetEnvColor(gMasterDisp++, 255, 48, 48, this->unk_44);
            break;
        case 4:
            gDPSetEnvColor(gMasterDisp++, 255, 255, 48, this->unk_44);
            break;
        case 5:
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, this->unk_44);
            gDPSetEnvColor(gMasterDisp++, 255, 32, 32, this->unk_44);
            break;
        case 6:
            if (gCurrentLevel == LEVEL_BOLSE) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_38);
            }
            switch ((this->index + gGameFrameCount) % 4U) {
                case 0:
                    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 0, 0, this->unk_44);
                    break;
                case 1:
                    gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 255, 0, this->unk_44);
                    break;
                case 2:
                    gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 0, 255, this->unk_44);
                    break;
                case 3:
                    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, this->unk_44);
                    break;
            }
            gDPSetEnvColor(gMasterDisp++, 0, 0, 0, this->unk_44);
            break;
        case 7:
            gDPSetEnvColor(gMasterDisp++, 48, 48, 255, this->unk_44);
            break;
        case 10:
            gDPSetEnvColor(gMasterDisp++, 255, 255, 32, this->unk_44);
            break;
        case 11:
            gDPSetEnvColor(gMasterDisp++, 255, 32, 32, this->unk_44);
            break;
        case 12:
            gDPSetEnvColor(gMasterDisp++, 32, 255, 32, this->unk_44);
            break;
        case 13:
            gDPSetEnvColor(gMasterDisp++, 32, 32, 255, this->unk_44);
            break;
    }

    gSPDisplayList(gMasterDisp++, aOrbDL);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect385_Draw(Effect385* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    RCP_SetupDL_49();
    RCP_SetupDL(&gMasterDisp, SETUPDL_38);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 64, 192, 255, this->unk_44);
    gDPSetEnvColor(gMasterDisp++, 0, 0, 0, this->unk_44);
    gSPDisplayList(gMasterDisp++, aOrbDL);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}



RECOMP_PATCH void Effect_Effect347_Draw(Effect347* this) {
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    gSPDisplayList(gMasterDisp++, D_BG_SPACE_2006F50);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect351_Draw(Effect351* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_4A);
    gSPDisplayList(gMasterDisp++, aBallDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Clouds_Draw(EffectClouds* this) {
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_46);
    Graphics_SetScaleMtx(this->scale2);
    Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, D_BG_PLANET_200D750);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect366_Draw(Effect366* this) {
    RCP_SetupDL(&gMasterDisp, SETUPDL_67);
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_4A);
    gDPSetEnvColor(gMasterDisp++, 101, 138, 153, 255);
    gSPDisplayList(gMasterDisp++, D_blue_marine_3000660);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect367_Draw(Effect367* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if (this->timer_50 == 0) {
        Graphics_SetScaleMtx(this->scale2);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
        gSPDisplayList(gMasterDisp++, D_BG_PLANET_20112C0);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect344_Draw(Effect344* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL_60(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
    Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
    if (this->unk_44 >= 2) {
        Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
    }
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, D_1029780);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect345_Draw(Effect345* this) {
    RCP_SetupDL_60(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
    Matrix_Scale(gGfxMatrix, this->scale2 * 0.6f, 1.0f, this->scale2 * 3.5f, MTXF_APPLY);
    Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, D_1029780);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

// s32 D_800D173C[] = { 255, 255, 255, 0, 0, 0, 255, 0, 255, 0, 0, 0, 255, 255, 0, 0, 0, 255, 0, 0 };

RECOMP_PATCH void Effect_Effect346_Draw(Effect346* this) {
    s32 temp_ft3;
    s32 tmp;

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    switch (gCurrentLevel) {
        case LEVEL_METEO:
            Graphics_SetScaleMtx(this->scale2);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 128, 128, 128, 255);
            gSPDisplayList(gMasterDisp++, D_ME_601FF80);
            break;

        case LEVEL_AQUAS:
            RCP_SetupDL(&gMasterDisp, SETUPDL_67);
            temp_ft3 = Math_ModF(this->index, 4.0f);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, (s32) this->scale1);
            tmp = temp_ft3 * 4.0f;
            gDPSetEnvColor(gMasterDisp++, D_800D173C[tmp + 0], D_800D173C[tmp + 1], D_800D173C[tmp + 2], 255);
            Graphics_SetScaleMtx(this->scale2);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect348_Draw(Effect348* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
    gSPDisplayList(gMasterDisp++, D_BG_PLANET_20112C0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect355_Draw(Effect355* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 180);
    Graphics_SetScaleMtx(this->scale2 * (13.0f + ((s32) (gGameFrameCount % 2U) * 2.5f)));
    gSPDisplayList(gMasterDisp++, aBlueSphereDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect353_Draw(Effect353* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL_21();
    gSPDisplayList(gMasterDisp++, D_101ABD0);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect377_Draw(Effect377* this) {
    f32 scale;

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 160);

    scale = 3.0f;
    if ((gGameFrameCount % 2) != 0) {
        scale *= 1.2f;
    }

    Graphics_SetScaleMtx(this->scale2 * scale);
    gSPDisplayList(gMasterDisp++, D_SX_600F8A0);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect356_Draw(Effect356* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    Graphics_SetScaleMtx(this->scale2);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 192);
    gSPDisplayList(gMasterDisp++, D_ME_6000A80);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect376_Draw(Effect376* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_4A);
    Graphics_SetScaleMtx(this->scale2);
    gSPDisplayList(gMasterDisp++, D_CO_6033000);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect393_Draw(Effect393* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if (this->state != 0) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_67);
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_63);
    }

    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
    gDPSetEnvColor(gMasterDisp++, 255, 255, 0, 255);
    Graphics_SetScaleMtx(this->scale2 * this->scale1);
    gSPDisplayList(gMasterDisp++, aStarDL);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect357_Draw(Effect357* this) {
    Vec3f frameJointTable[50];

    if ((gCurrentLevel == LEVEL_BOLSE) && (gPlayer[0].state == PLAYERSTATE_LEVEL_COMPLETE)) {
        Display_SetSecondLight(&this->obj.pos);
    }

    RCP_SetupDL(&gMasterDisp, SETUPDL_29);

    if (gCurrentLevel == LEVEL_KATINA) {
        gSPFogPosition(gMasterDisp++, gFogNear, 1005);
    }

    Graphics_SetScaleMtx(this->scale2);

    switch (gCurrentLevel) {
        case LEVEL_MACBETH:
            switch (this->unk_44) {
                case 1:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    if ((this->index % 2) != 0) {
                        gSPDisplayList(gMasterDisp++, D_MA_6022530);
                    } else {
                        gSPDisplayList(gMasterDisp++, D_MA_6022450);
                    }
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 2:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_57);
                    if ((this->index % 2) != 0) {
                        gSPDisplayList(gMasterDisp++, D_MA_601A7A0);
                    } else {
                        gSPDisplayList(gMasterDisp++, D_MA_60223C0);
                    }
                    RCP_SetupDL(&gMasterDisp, SETUPDL_29);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 7:
                    Animation_GetFrameData(&D_MA_6027320, 0, frameJointTable);
                    Animation_DrawSkeleton(1, D_MA_602742C, frameJointTable, func_effect_800798C4, NULL, this,
                                           &gIdentityMatrix);
                    break;

                case 8:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_57);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, D_MA_6020E60);
                    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 9:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_57);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, D_MA_6020A30);
                    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 10:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_57);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_67);
                    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
                    gDPSetEnvColor(gMasterDisp++, 255, 80, 0, 255);
                    Graphics_SetScaleMtx(this->scale1);
                    gSPDisplayList(gMasterDisp++, aOrbDL);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 11:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_57);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, D_MA_60200E0);
                    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 12:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, D_MA_600F2F0);
                    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 13:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, D_MA_6001A60);
                    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 14:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, D_MA_600F160);
                    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 15:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, D_MA_600F3D0);
                    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 16:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, aSyShip4DestroyedDL);
                    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 17:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, D_MA_600F9B0);
                    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                default:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    if ((this->index % 2) != 0) {
                        gSPDisplayList(gMasterDisp++, D_10194C0);
                    } else {
                        gSPDisplayList(gMasterDisp++, D_1024290);
                    }
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;
            }
            break;

        case LEVEL_AQUAS:
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);
            Matrix_SetGfxMtx(&gMasterDisp);
            RCP_SetupDL(&gMasterDisp, SETUPDL_60);
            gSPDisplayList(gMasterDisp++, D_AQ_600A220);
            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            break;

        default:
            switch (this->unk_4C) {
                case 0:
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    if ((this->index % 2) != 0) {
                        gSPDisplayList(gMasterDisp++, D_10194C0);
                    } else {
                        gSPDisplayList(gMasterDisp++, D_1024290);
                    }
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;

                case 1:
                    switch ((s32) (this->index % 4U)) {
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
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);
                    Matrix_SetGfxMtx(&gMasterDisp);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_57);
                    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                    gSPDisplayList(gMasterDisp++, D_1021E20);
                    RCP_SetupDL(&gMasterDisp, SETUPDL_64);
                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                    break;
            }
            break;
    }

    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    if (gCurrentLevel == LEVEL_KATINA) {
        gSPFogPosition(gMasterDisp++, gFogNear, gFogFar);
    }
}

RECOMP_PATCH void Effect_Effect383_Draw(Effect383* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if (gReflectY > 0) {
        Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        RCP_SetupDL_64_2();
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
        gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
        gSPDisplayList(gMasterDisp++, D_10182C0);
        gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
        RCP_SetupDL_64_2();
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect359_Draw(Effect359* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL(&gMasterDisp, SETUPDL_68);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 31, 10, 00, this->unk_44);
    gDPSetEnvColor(gMasterDisp++, 141, 73, 5, 0);
    Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, 1.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, D_800D178C[this->unk_4C]);
    this->unk_4C = this->unk_48;
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect360_361_362_Draw(Effect* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_4A);
    gSPDisplayList(gMasterDisp++, D_BG_PLANET_2010A30);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_FireSmoke_Draw(EffectFireSmoke* this) {
    f32 scale;

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);

    if (gLevelType == LEVELTYPE_PLANET) {
        gDPSetPrimColor(gMasterDisp++, 0, 0, D_800D184C[this->unk_4C].r, D_800D184C[this->unk_4C].g,
                        D_800D184C[this->unk_4C].b, D_800D184C[this->unk_4C].a);
        scale = D_800D17F8[this->unk_4C] - 0.5f;
        Matrix_Scale(gGfxMatrix, scale, scale, 1.0f, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, D_800D17A4[this->unk_4C]);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        return;
    }

    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 100, this->unk_44);

    if (this->unk_4C == 0) {
        Matrix_Scale(gGfxMatrix, 2.5f, 2.5f, 2.5f, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, D_800D18A0[this->unk_4C]);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        return;
    }
    gSPDisplayList(gMasterDisp++, D_800D18A0[this->unk_4C]);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect375_Draw(Effect375* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    gSPDisplayList(gMasterDisp++, D_CO_6004900);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect374_Draw(Effect374* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    switch (this->state) {
        case 0:
            Graphics_SetScaleMtx(this->scale2);
            RCP_SetupDL_60(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
            gSPDisplayList(gMasterDisp++, D_ENMY_PLANET_4008CE0);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;

        case 1:
            Matrix_Scale(gGfxMatrix, this->scale1, this->scale2, 2.5f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            RCP_SetupDL_40();
            gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
            gSPDisplayList(gMasterDisp++, D_ENMY_PLANET_4008F70);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect342_Draw(Effect342* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    if (this->unk_44 == 0) {
        gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 0, 0, this->unk_4A);
    } else {
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_4A);
    }
    gSPDisplayList(gMasterDisp++, D_BG_PLANET_2010A30);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect365_Draw(Effect365* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
    gSPDisplayList(gMasterDisp++, D_BG_PLANET_2010A30);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect398_Draw(Effect398* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    switch (this->unk_44) {
        case 0:
            RCP_SetupDL(&gMasterDisp, SETUPDL_49);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
            gDPSetEnvColor(gMasterDisp++, 255, 0, 0, 255);
            Matrix_Scale(gGfxMatrix, 4.0f, 2.0f, 4.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, D_ZO_6018AF0);
            break;

        case 1:
            RCP_SetupDL(&gMasterDisp, SETUPDL_49);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
            gDPSetEnvColor(gMasterDisp++, 255, 0, 0, 255);
            Matrix_RotateZ(gGfxMatrix, 30.0f * M_DTOR, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, 3.0f, 1.5f, 3.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect394_Draw(Effect394* this) {
    static s32 D_800D18E4 = 0;
    s32 i;
    s32 alpha;

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    switch (this->unk_7A) {
        case 0:
            if (this->unk_44 != 64) {
                RCP_SetupDL(&gMasterDisp, this->unk_44);
            }
            gSPDisplayList(gMasterDisp++, this->unk_74);
            if (this->unk_44 != 64) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            }
            break;

        case 1:
            if (this->unk_48 == 0) {
                this->unk_48 = this->unk_46;
            }

            if (this->unk_44 != 64) {
                RCP_SetupDL(&gMasterDisp, this->unk_44);
            }

            alpha = (((f32) this->unk_46 / (f32) this->unk_48) * 255.0f);
            if (alpha > 255) {
                alpha = 255;
            }

            if (alpha < 32) {
                Object_Kill(&this->obj, this->sfxSource);
            } else {
                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, alpha);
                gSPDisplayList(gMasterDisp++, this->unk_74);
            }

            if (this->unk_44 != 64) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            }
            break;

        case 10:
            RCP_SetupDL(&gMasterDisp, SETUPDL_65);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_48);
            Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, this->scale2, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, D_TI_A000000);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;

        case 11:
            RCP_SetupDL(&gMasterDisp, SETUPDL_68);
            Graphics_SetScaleMtx(this->scale2);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 120, 60, 0, this->unk_44);
            gDPSetEnvColor(gMasterDisp++, 0, 0, 0, 0);
            gSPDisplayList(gMasterDisp++, D_1023750);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;

        case 12:
            Graphics_SetScaleMtx(this->scale2);
            RCP_SetupDL(&gMasterDisp, SETUPDL_23);
            gSPDisplayList(gMasterDisp++, D_VE1_9013C20);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;

        case 13:
            Graphics_SetScaleMtx(this->scale2);
            RCP_SetupDL(&gMasterDisp, SETUPDL_68);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
            gDPSetEnvColor(gMasterDisp++, 36, 45, 28, 0);
            gSPDisplayList(gMasterDisp++, D_1023750);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;

        case 14:
            Graphics_SetScaleMtx(this->scale2);
            RCP_SetupDL(&gMasterDisp, SETUPDL_68);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
            gDPSetEnvColor(gMasterDisp++, 255, 255, 255, 0);
            gSPDisplayList(gMasterDisp++, D_1023750);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;

        case 15:
            Graphics_SetScaleMtx(this->scale2);
            RCP_SetupDL(&gMasterDisp, SETUPDL_68);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_4A);
            gDPSetEnvColor(gMasterDisp++, 36, 45, 28, 0);
            gSPDisplayList(gMasterDisp++, D_1023750);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;

        case 16:
            Graphics_SetScaleMtx(this->scale2);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 96, 96, 255, 255);
            Matrix_Translate(gGfxMatrix, 34.14f, 0.0f, 0.0f, MTXF_APPLY);
            Matrix_Push(&gGfxMatrix);
            Matrix_Scale(gGfxMatrix, 0.25f, 1.0f, 1.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, D_102F5E0);
            Matrix_Pop(&gGfxMatrix);

            for (i = 0; i < 7; i++) {
                Matrix_Translate(gGfxMatrix, 0.0f, 10.0f, 0.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, M_PI / 4, MTXF_APPLY);
                Matrix_Translate(gGfxMatrix, 1.0f, 20.0f, 0.0f, MTXF_APPLY);
                Matrix_Push(&gGfxMatrix);
                Matrix_Scale(gGfxMatrix, 0.25f, 1.0f, 1.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_102F5E0);
                Matrix_Pop(&gGfxMatrix);
            }
            this->obj.rot.z += 1.0f;
            break;

        case 17:
            //! DEBUG: some code used for debugging with the 4th controller
            if ((gControllerPress[3].button & U_JPAD) && (D_800D18E4 > 0)) {
                D_800D18E4--;
            }
            if ((gControllerPress[3].button & D_JPAD) && (D_800D18E4 < 12)) {
                D_800D18E4++;
            }

            if (((this->index + gGameFrameCount) % 2) == 0) {
                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
            } else {
                gDPSetPrimColor(gMasterDisp++, 0, 0, 96, 96, 255, 255);
            }

            Graphics_SetScaleMtx(this->scale2);

            for (i = 0; i < 10; i++) {
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, -60.0f, 0.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.8f, 3.0f, 1.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_102F5E0);
                Matrix_Pop(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, -120.0f, 0.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, D_800D1534[D_800D18E4][i] * M_DTOR, MTXF_APPLY);
            }
            break;

        case 18:
            RCP_SetupDL(&gMasterDisp, SETUPDL_48);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 190, 255);
            gDPSetEnvColor(gMasterDisp++, 239, 15, 0, 255);
            Graphics_SetScaleMtx(this->scale2);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect391_Draw(Effect391* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL_49();
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
    gDPSetEnvColor(gMasterDisp++, 36, 45, 28, 255);
    Matrix_Scale(gGfxMatrix, this->orient.x, this->scale2 * this->orient.y, this->scale2, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, D_ZO_601BAD0);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Effect_Effect399_Draw(Effect399* this) {
    s32 tmp;

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if (gCurrentLevel != LEVEL_AQUAS) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_49);
        Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
        tmp = this->unk_44 * 4;
        gDPSetEnvColor(gMasterDisp++, D_800D18F0[tmp + 0], D_800D18F0[tmp + 1], D_800D18F0[tmp + 2], 255);
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_49);
        Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, D_800D1950[this->unk_44]);
        gDPSetEnvColor(gMasterDisp++, 255, 0, 0, 255);
    }
    gSPDisplayList(gMasterDisp++, aOrbDL);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void SectorY_Effect354_Draw(Effect354* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL_21();
    gSPDisplayList(gMasterDisp++, D_SY_6014A40);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Aquas_Effect363_Draw(Effect363* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if ((gPlayer[0].state == PLAYERSTATE_LEVEL_INTRO) && (gCurrentLevel == LEVEL_AQUAS) &&
        (gPlayer[0].csState < 2)) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_68);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 21, 34, this->unk_4A);
        gDPSetEnvColor(gMasterDisp++, 255, 255, 251, 0);
        Graphics_SetScaleMtx(this->scale2);
        gSPDisplayList(gMasterDisp++, D_1023750);
    } else {
        Graphics_SetScaleMtx(this->scale2);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_4A);
        gSPDisplayList(gMasterDisp++, D_1023750);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Titania_Effect368_Draw(Effect368* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Matrix_Scale(gGfxMatrix, this->scale2 * 0.5f, this->scale2, this->scale2, MTXF_APPLY);
    Matrix_RotateX(gGfxMatrix, -(M_DTOR * 90), MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, this->unk_44);
    gSPDisplayList(gMasterDisp++, D_landmaster_3007E70);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Macbeth_Effect378_Draw(Effect378* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL(&gMasterDisp, SETUPDL_60);
    gSPDisplayList(gMasterDisp++, D_MA_601A840);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Macbeth_Effect379_Draw(Effect379* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL(&gMasterDisp, SETUPDL_67);

    if (this->scale2 >= 3.0f) {
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, this->unk_44);
        gDPSetEnvColor(gMasterDisp++, 255, 0, 0, this->unk_44);
    } else {
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, this->unk_44);
        gDPSetEnvColor(gMasterDisp++, 0, 128, 255, this->unk_44);
    }

    Matrix_Scale(gGfxMatrix, 0.5f, 0.5f, 0.5f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, aOrbDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    RCP_SetupDL(&gMasterDisp, SETUPDL_64);
}

RECOMP_PATCH void Macbeth_Effect380_Draw(Effect380* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL(&gMasterDisp, SETUPDL_67);
    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 112, 255, 243, this->unk_44);
    gDPSetEnvColor(gMasterDisp++, 255, 255, 255, this->unk_44);
    Matrix_SetGfxMtx(&gMasterDisp);
    gSPDisplayList(gMasterDisp++, D_MA_6012A60);
    RCP_SetupDL(&gMasterDisp, SETUPDL_64);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Solar_Effect392_Draw(Effect392* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Graphics_SetScaleMtx(this->scale2);
    switch (this->state) {
        case 0:
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 128, 128, this->unk_44);
            gSPDisplayList(gMasterDisp++, D_i3_801BF92C[this->unk_4C]);
            break;
        case 1:
        case 2:
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 128, 128, this->unk_44);
            gSPDisplayList(gMasterDisp++, D_i3_801BF944[this->unk_4C]);
            break;
        case 3:
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 128, 128, this->unk_44);
            gSPDisplayList(gMasterDisp++, D_SO_601C820);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            RCP_SetupDL(&gMasterDisp, SETUPDL_53);
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 128, 128, this->unk_44);
            gSPDisplayList(gMasterDisp++, D_i3_801BF92C[this->unk_4C]);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Andross_Effect396_Draw(Effect396* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if (this->state == 10) {
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, this->unk_44);
        Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, D_BG_PLANET_2010A30);
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_29);
        if (this->unk_4C < 8) {
            Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
        }
        gSPDisplayList(gMasterDisp++, D_i6_801A6790[this->unk_4C]);
        RCP_SetupDL(&gMasterDisp, SETUPDL_64);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Bolse_Effect397_Draw(Effect397* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    switch (this->state) {
        case 0:
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 192);
            Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, this->scale2, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, D_BO_6006910);
            break;

        case 1:
            RCP_SetupDL(&gMasterDisp, SETUPDL_67);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->unk_44);
            gDPSetEnvColor(gMasterDisp++, 0, 128, 255, this->unk_44);
            Matrix_Scale(gGfxMatrix, this->scale2, this->scale2, this->scale2, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            RCP_SetupDL(&gMasterDisp, SETUPDL_64);
            break;
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

#endif

#if 1
RECOMP_PATCH void Effect_ElectricArc_Draw(EffectElectricArc* this) {
    s32 i;

    if (this->unk_4A > 10) {
        if (((this->index + gGameFrameCount) % 2) == 0) {
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 128, 128, 32);
        } else {
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 128, 128, 128);
        }
    } else if (((this->index + gGameFrameCount) % 2) == 0) {
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
    } else {
        gDPSetPrimColor(gMasterDisp++, 0, 0, 96, 96, 255, 255);
    }

    Graphics_SetScaleMtx(this->scale2);

    for (i = 0; i < 10; i++) {
        if ((i >= this->unk_48) && (i < this->unk_46)) {
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, -60.0f, 0.0f, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, 0.8f, 3.0f, 1.0f, MTXF_APPLY);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_EFFECT(this) + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, D_102F5E0);
            Matrix_Pop(&gGfxMatrix);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
        Matrix_Translate(gGfxMatrix, 0.0f, -120.0f, 0.0f, MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, D_800D1534[this->unk_4C][i] * M_DTOR, MTXF_APPLY);
    }
}
#endif

// Texture scrolling for explosions
#if 1
RECOMP_PATCH void Effect_Effect383_Update(Effect383* this) {
    //    Lib_Texture_Scroll(D_10190C0, 16, 32, 0);
    // @Recomp use UV tex scrolling instead
    Gfx_Texture_UV_Scroll(D_10182C0 + 6, 16, 32, 0, 1);

    gGroundClipMode = 2;
    this->obj.rot.y += 1.0f;
    Math_SmoothStepToF(&this->scale2, this->scale1, 0.05f, 1.5f, 0.001f);

    if (this->timer_50 > 10) {
        D_ctx_801779A8[0] = 60.0f;
    }
    if (this->timer_50 == 48) {
        gFillScreenAlpha = 150;
    }
    if (this->timer_50 > 45) {
        gFillScreenAlphaTarget = 0;
        gFillScreenRed = gFillScreenGreen = gFillScreenBlue = 255;
    }

    gFillScreenAlphaStep = 3;

    if (this->timer_50 == 0) {
        this->alpha -= 2;
        if (this->alpha < 0) {
            this->alpha = 0;
            Object_Kill(&this->obj, this->sfxSource);
            gGroundClipMode = 0;
        }
    }
}
#endif

// Texture scrolling for Corneria's waterfall
#if 1
void Obj54_8006AA3C(f32 xPos, f32 yPos, f32 zPos);

RECOMP_PATCH void CoWaterfall_Update(CoWaterfall* this) {
    Vec3f dest;
    Vec3f src;

    // Lib_Texture_Scroll(aCoWaterfallTex2, 32, 32, 1);
    // @Recomp use UV tex scrolling instead
    Gfx_Texture_UV_Scroll(aCoWaterfallDL + 26, 32, 32, 1, 1);

    if ((gGameFrameCount % 4) == 0) {
        Matrix_RotateY(gCalcMatrix, this->obj.rot.y * M_DTOR, MTXF_NEW);

        src.x = RAND_FLOAT_CENTERED(700.0f);
        src.y = RAND_FLOAT(50.0f);
        src.z = 700.0f;

        Matrix_MultVec3fNoTranslate(gCalcMatrix, &src, &dest);
        Obj54_8006AA3C(this->obj.pos.x + dest.x, this->obj.pos.y + dest.y + 50.0f, this->obj.pos.z + dest.z);
    }
}
#endif

// Tank tracks UV Scrolling
#if 1
void func_tank_80043280(u16* text0, u16* text1, f32 zRot);
void func_tank_80043B18(Player* player);

#if 0
extern int gUvOn;
#endif

RECOMP_PATCH void func_tank_80044868(Player* player) {
    f32 pad;
    f32 stickTilt;
    f32 sp2C;

    // @recomp: invert_y_axis_mode
    stickTilt = (gInputPress->stick_y * 0.7f) * INVERT_Y_AXIS_COND_RES - 8.0f;
    if (stickTilt < -40.0f) {
        stickTilt = -40.0f;
    }
    if (stickTilt > 0.0f) {
        stickTilt = 0.0f;
    }

    if (player->unk_17C < stickTilt) {
        player->unk_17C += 3.0f;
    }

    if (stickTilt < player->unk_17C) {
        player->unk_17C -= 3.0f;
    }

    if (player->unk_180 < 0.0f) {
        player->unk_180 += 3.0f;
    }
    if (player->unk_180 > 0.0f) {
        player->unk_180 -= 3.0f;
    }

    gPlayerTurnRate = 3.0f;
    gPlayerTurnStickMod = 0.66f;

    stickTilt = gInputPress->stick_x;
    if (stickTilt == 0.0f) {
        Math_SmoothStepToF(&player->rot.y, -stickTilt * gPlayerTurnStickMod, 0.1f, gPlayerTurnRate * 0.5f, 0.1f);
    } else {
        Math_SmoothStepToF(&player->rot.y, -stickTilt * gPlayerTurnStickMod, 0.1f, gPlayerTurnRate, 0.1f);
    }

    player->rot_104.y = player->rot.y;
    sp2C = player->baseSpeed;

    if ((gCurrentLevel == LEVEL_MACBETH) && (sp2C < 3.0f)) {
        sp2C = 3.0f;
    }

    if (player->unk_16C > 0.2f) {
        Math_SmoothStepToF(&player->unk_184, player->baseSpeed * 0.5f, 1.0f, 1.0f, 0.0f);
    }

    if (player->unk_170 > 0.2f) {
        Math_SmoothStepToF(&player->unk_184, -player->baseSpeed * 0.5f, 1.0f, 1.0f, 0.0f);
    }
    if (!(player->unk_170 > 0.2f) && !(player->unk_16C > 0.2f) && player->grounded) {
        Math_SmoothStepToF(&player->unk_184, 0.0f, 1.0f, 0.75f, 0.0f);
    }

    if (player->rollState != 0) {
        if (player->rollRate < 0) {
            player->unk_184 = 15.0f;
        }
        if (player->rollRate > 0) {
            player->unk_184 = -15.0f;
        }
    }

    player->vel.z = -(COS_DEG(player->rot_104.y) * COS_DEG(player->rot_104.x) * sp2C);
    if ((player->vel.x < 20.0f) && (player->rot_104.z < -30.0f)) {
        Math_SmoothStepToF(&player->vel.x, -player->rot_104.z * 0.5f, 0.2f, 3.0f, 0.0f);
        player->unk_184 = 0.0f;
    } else if ((player->vel.x > -20.0f) && (player->rot_104.z > 30.0f)) {
        Math_SmoothStepToF(&player->vel.x, -player->rot_104.z * 0.5f, 0.2f, 3.0f, 0.0f);
        player->unk_184 = 0.0f;
    } else {
        Math_SmoothStepToF(&player->vel.x, player->unk_184 - (SIN_DEG(player->rot_104.y) * sp2C), 0.5f, 5.0f, 0.0f);
    }

    player->vel.z += fabsf((player->unk_184 * 0.4f * player->baseSpeed) / 15.0f);

    if (player->unk_000 == 0) {
        player->vel.z += SIN_DEG(player->rot.x) * player->boostSpeed;
    }

    if (D_800C9F00 != 0) {
        player->vel.z = 2.0f * D_800C9F00;
    }

    player->pos.x += player->vel.x;

    if ((player->xPath + (player->pathWidth - 100.0f)) < player->pos.x) {
        player->flags_228 = PFLAG_228_0;
    }

    if (player->pos.x < (player->xPath - (player->pathWidth - 100.0f))) {
        player->flags_228 = PFLAG_228_1;
    }

    if (player->pathWidth + player->xPath < player->pos.x) {
        player->pos.x = player->pathWidth + player->xPath;
        player->vel.x = 0.0f;
    }

    if (player->pos.x < player->xPath - player->pathWidth) {
        player->pos.x = player->xPath - player->pathWidth;
        player->vel.x = 0.0f;
    }

    player->pos.y += player->vel.y;
    player->vel.y -= player->gravity;

    if (player->vel.y < -50.0f) {
        player->vel.y = -50.0f;
    }
    if (player->vel.y > 20.0f) {
        player->vel.y = 20.0f;
    }

    player->pos.z += player->vel.z;

    if (player->grounded) {
        player->pathHeight = 0.0f;
        player->rockPhase += player->baseSpeed * 5.0f;
        player->rockAngle = SIN_DEG(player->rockPhase) * 0.7f;

        if (D_800C9F04 == 0) {
            if (player->baseSpeed != 0.0f) {
                func_tank_80043B18(player);
            }
            if (player->rollState != 0.0f) {
                func_tank_80043B18(player);
            }
        }
    }

    if (player->baseSpeed > 0.0f) {
        // Lib_Texture_Scroll(aLandmasterModelTex6, 32, 32, 0);
        // @recomp: UV Scrolling
        Gfx_Texture_UV_Scroll(aLandmasterModelDL + 92, 32, 32, 0, 1);
        Gfx_Texture_UV_Scroll(aLandmasterModelDL + 141, 32, 32, 0, 1);
        Gfx_Texture_UV_Scroll(aLandmasterModelDL + 150, 32, 32, 0, 1);
        Gfx_Texture_UV_Scroll(aLandmasterModelDL + 177, 32, 32, 0, 1);

        if ((gCurrentLevel == LEVEL_TITANIA) && !gBossActive) {
            func_tank_80043280(aLandmasterModelTex4, D_TI_6009BB8, gGameFrameCount * -55.0f);
        }
        if ((gCurrentLevel == LEVEL_MACBETH) && (player->state == PLAYERSTATE_LEVEL_COMPLETE)) {
            func_tank_80043280(aLandmasterModelTex4, D_Tex_800DACB8, gGameFrameCount * -55.0f);
        }
    }

    if (player->baseSpeed > 10.0f) {
        // Lib_Texture_Scroll(aLandmasterModelTex6, 32, 32, 0);
        // @recomp: UV Scrolling
        Gfx_Texture_UV_Scroll(aLandmasterModelDL + 92, 32, 32, 0, 1);
        Gfx_Texture_UV_Scroll(aLandmasterModelDL + 141, 32, 32, 0, 1);
        Gfx_Texture_UV_Scroll(aLandmasterModelDL + 150, 32, 32, 0, 1);
        Gfx_Texture_UV_Scroll(aLandmasterModelDL + 177, 32, 32, 0, 1);

        if ((gCurrentLevel == LEVEL_TITANIA) && !gBossActive) {
            func_tank_80043280(aLandmasterModelTex4, D_TI_6009BB8, gGameFrameCount * -55.0f);
        }
    }

    Player_DamageEffects(player);
}
#endif

void Corneria_Garuda_HandleDamage(Actor* this);

// Garuda 1 texture scrolling
#if 1
s32 Corneria_CoGaruda1_CheckCollision(CoGaruda1* this);

RECOMP_PATCH void Corneria_CoGaruda1_Update(CoGaruda1* this) {
    Vec3f frameTable[20];
    f32 sin;
    f32 cos;

    Corneria_Garuda_HandleDamage(this);

    Math_SmoothStepToVec3fArray(frameTable, this->vwork, 0,
                                Animation_GetFrameData(&aCoGaruda1Anim, this->animFrame, frameTable), 1.0f, 1.0f, 1.0f);

    sin = SIN_DEG(this->obj.rot.y);
    this->vel.x = this->fwork[0] * sin;
    cos = COS_DEG(this->obj.rot.y);
    this->vel.z = this->fwork[0] * cos;

    switch (this->state) {
        case 0:
            this->fwork[1] += 20.0f;

            // @recomp: UV texture scrolling
            // Lib_Texture_Scroll(aCoGarudaTracksTex, 16, 16, 1);
            Gfx_Texture_UV_Scroll(ast_corneria_seg6_gfx_31ED0 + 59, 16, 16, 1, 1);

            this->animFrame = 0;

            this->fwork[0] += 1.0f;
            if (this->fwork[0] > 10.0f) {
                this->fwork[0] = 10.0f;
            }

            this->iwork[0] = Corneria_CoGaruda1_CheckCollision(this);
            if (this->iwork[0] != 0) {
                this->state++;
            }
            break;

        case 1:
            this->animFrame++;
            this->fwork[1] += 20.0f;
            this->fwork[0] = 0.0f;

            if (this->animFrame == 50) {
                gScenery[this->iwork[0] - 1].state = 1;
            }
            if (this->animFrame >= Animation_GetFrameCount(&aCoGaruda1Anim)) {
                this->state++;
            }
            break;

        case 2:
            this->animFrame = 0;
            this->fwork[1] += 20.0f;
            break;
    }
}
#endif

#if 1
RECOMP_PATCH void Corneria_CoGaruda2_Update(CoGaruda2* this) {
    Vec3f frameTable[20];
    Vec3f src;
    Vec3f dest;
    Scenery* scenery;
    f32 sin;
    f32 cos;

    Corneria_Garuda_HandleDamage(this);

    scenery = &gScenery[this->iwork[0]];

    sin = SIN_DEG(this->obj.rot.y);
    this->vel.x = this->fwork[0] * sin;
    cos = COS_DEG(this->obj.rot.y);
    this->vel.z = this->fwork[0] * cos;

    Matrix_RotateY(gCalcMatrix, this->obj.rot.y * M_DTOR, MTXF_NEW);

    switch (this->state) {
        case 0:
            this->timer_0BC = 40;
            this->state = 1;
            this->animFrame = 0;
            break;

        case 1:
            this->fwork[0] = -10.0f;

            // @recomp: UV texture scrolling
            // Lib_Texture_Scroll(aCoGarudaTracksTex, 16, 16, 1);
            Gfx_Texture_UV_Scroll(ast_corneria_seg6_gfx_31ED0 + 59, 16, 16, 1, 1);

            if (this->timer_0BC == 0) {
                this->state = 2;
                this->iwork[2] = RAND_INT(10.0f) + 10;
            }
            break;

        case 2:
            this->fwork[0] = -10.0f;

            // @recomp: UV texture scrolling
            // Lib_Texture_Scroll(aCoGarudaTracksTex, 16, 16, 1);
            Gfx_Texture_UV_Scroll(ast_corneria_seg6_gfx_31ED0 + 59, 16, 16, 1, 1);

            this->animFrame++;

            if (this->animFrame >= Animation_GetFrameCount(&D_CO_602AA04)) {
                this->state = 3;
            }
            if (this->animFrame == (Animation_GetFrameCount(&D_CO_602AA04) - this->iwork[2])) {
                this->iwork[1] = 1;
                scenery->state = 1; // I beam rotating
                src.x = 0.0f;
                src.y = 0.0f;
                src.z = 30.0f; // Ibeam flying towards the camera.
                Matrix_MultVec3fNoTranslate(gCalcMatrix, &src, &dest);
                scenery->vel.x = dest.x;
                scenery->vel.y = dest.y;
                scenery->vel.z = dest.z;
                AUDIO_PLAY_SFX(NA_SE_EN_THROW, this->sfxSource, 4);
            }
            break;

        case 3:
            break;
    }

    if (this->iwork[1] == 0) {
        scenery->obj.pos.x = this->fwork[2];
        scenery->obj.pos.y = this->fwork[6];
        scenery->obj.pos.z = this->fwork[10];
        scenery->obj.rot.y = this->obj.rot.y;
        scenery->vel.y = 0.0f;
    }

    Math_SmoothStepToVec3fArray(frameTable, this->vwork, 0,
                                Animation_GetFrameData(&D_CO_602AA04, this->animFrame, frameTable), 1.0f, 1.0f, 1.0f);
}
#endif

#if 1
RECOMP_PATCH void Corneria_CoGaruda3_Update(CoGaruda3* this) {
    s32 pad;
    Vec3f frameTable[20];
    Scenery* scenery;
    f32 sin;
    f32 cos;
    s32 pad2[4];

    Corneria_Garuda_HandleDamage(this);

    sin = SIN_DEG(this->obj.rot.y);
    this->vel.x = this->fwork[0] * sin;
    cos = COS_DEG(this->obj.rot.y);
    this->vel.z = this->fwork[0] * cos;

    switch (this->state) {
        case 0:
            this->state = 1;
            break;

        case 1:
            this->fwork[0] = 5.0f;
            this->fwork[1] += 5.0f;

            // @recomp: UV texture scrolling
            // Lib_Texture_Scroll(aCoGarudaTracksTex, 16, 16, 1);
            Gfx_Texture_UV_Scroll(ast_corneria_seg6_gfx_31ED0 + 59, 16, 16, 1, 1);

            this->animFrame++;
            if (this->animFrame >= Animation_GetFrameCount(&aCoGaruda3Anim)) {
                this->animFrame = 0;
            }
            break;
    }

    if (this->iwork[1] == 0) {
        scenery = &gScenery[this->iwork[0]];
        scenery->obj.pos.x = this->fwork[2];
        scenery->obj.pos.y = this->fwork[6];
        scenery->obj.pos.z = this->fwork[10];
        scenery->obj.rot.y = this->fwork[1];
        scenery->vel.y = 0.0f;
    }

    Math_SmoothStepToVec3fArray(frameTable, this->vwork, 0,
                                Animation_GetFrameData(&aCoGaruda3Anim, this->animFrame, frameTable), 1.0f, 1.0f, 1.0f);
}
#endif