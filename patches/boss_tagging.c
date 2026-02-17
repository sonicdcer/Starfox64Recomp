#include "patches.h"

extern Gfx D_VE2_600C2D0[];
extern Gfx D_VE2_600C560[];
extern Limb* aVe2AndBrainSkel[];
extern Gfx aKaFLBaseDL[];
extern Gfx aKaFLBaseDestroyedDL[];

void Meteo_MeCrusherEngineGlow_Draw(s32 scale);
bool Andross_AndBrain_OverrideLimbDraw(s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3f* rot, void* thisx);
bool SectorY_SyShogun_OverrideLimbDraw(s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3f* rot, void* thisx);
void SectorY_SyShogun_PostLimbDraw(s32 limbIndex, Vec3f* rot, void* thisx);

RECOMP_PATCH void Andross_AndBrain_Draw(AndBrain* this) {
    s32 j;
    s32 i;
    f32 temp;
    s32 pad;
    Vec3f spAC;
    Vec3f spA0;

    if (this->state != 21) {
        Matrix_RotateZ(gGfxMatrix, -(f32) gGameFrameCount * 20.0f * M_DTOR, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, this->fwork[23] + 1.0f, 1.0f - this->fwork[23], 1.0f, MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 20.0f * M_DTOR, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, this->fwork[23] + 1.0f, 1.0f - this->fwork[23], 1.0f, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, this->scale, this->scale, this->scale, MTXF_APPLY);
        Animation_DrawSkeleton(0, aVe2AndBrainSkel, this->vwork, Andross_AndBrain_OverrideLimbDraw, NULL, this,
                               &gIdentityMatrix);
        if (this->fwork[21] >= 254) {
            RCP_SetupDL(&gMasterDisp, SETUPDL_54);
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 64, 64, 255);
        } else {
            RCP_SetupDL(&gMasterDisp, SETUPDL_70);
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 64, 64, (s32) this->fwork[21]);
        }
        gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
        Matrix_Translate(gGfxMatrix, 0.0f, -16.0f, 0.0f, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, 0.3f, 0.6f, 0.3f, MTXF_APPLY);

        for (i = 0; i < 30; i++) {
            Matrix_Push(&gGfxMatrix);
            Matrix_RotateY(gCalcMatrix, i * 50.0f * M_DTOR, 0U);
            spAC.x = 0.0f;
            spAC.y = 0.0f;
            spAC.z = i * 0.8f;
            Matrix_MultVec3f(gCalcMatrix, &spAC, &spA0);
            Matrix_Translate(gGfxMatrix, spA0.x, 10.0f, spA0.z, MTXF_APPLY);
            Matrix_RotateY(gGfxMatrix, M_DTOR * -this->obj.rot.y - gPlayer[0].camYaw, MTXF_APPLY);

            for (j = 0; j < 10; j++) {
                Matrix_Translate(gGfxMatrix, 0.0f, -10.0f, 0.0f, MTXF_APPLY);
                temp = SIN_DEG((s32) this->fwork[19] * (i * 10 + j) + (gGameFrameCount * 5)) * this->fwork[10];
                Matrix_RotateZ(gGfxMatrix, (this->fwork[18] + temp) * M_DTOR, MTXF_APPLY);
                Matrix_RotateX(gGfxMatrix, this->fwork[17] * M_DTOR, MTXF_APPLY);
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 0.0f, -5.0f, 0.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + i + 0x01000000, G_EX_PUSH,
                                               G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
                if (j == 9) {
                    gSPDisplayList(gMasterDisp++, D_VE2_600C2D0);
                } else {
                    gSPDisplayList(gMasterDisp++, D_VE2_600C560);
                }

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                Matrix_Pop(&gGfxMatrix);
            }
            Matrix_Pop(&gGfxMatrix);
        }
    }
}

#if 1
RECOMP_PATCH void Meteo_MeCrusher_Draw(MeCrusher* this) {
    s32 i;
    f32 var_fs0;
    f32 var_fs1;
    s32 pad;

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    gSPDisplayList(gMasterDisp++, aMeCrusherDL);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    RCP_SetupDL_64();
    gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);

    for (i = 0; i < 6; i++) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + i + 1, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        if ((this->fwork[i + 3] != 0) && (this->fwork[11] == 0.0f)) {
            var_fs1 = 2.0f * this->fwork[3 + i];
            Matrix_Push(&gGfxMatrix);

            switch (i) {
                case 0:
                    Matrix_Translate(gGfxMatrix, 746.0f, 741.0f, 680.0f, MTXF_APPLY);
                    break;

                case 1:
                    Matrix_Translate(gGfxMatrix, -746.0f, 741.0f, 680.0f, MTXF_APPLY);
                    break;

                case 2:
                    Matrix_Translate(gGfxMatrix, -746.0f, -741.0f, 680.0f, MTXF_APPLY);
                    break;

                case 3:
                    Matrix_Translate(gGfxMatrix, 746.0f, -741.0f, 680.0f, MTXF_APPLY);
                    break;

                case 4:
                    Matrix_Translate(gGfxMatrix, 1283.0f, 0.0f, -340.0f, MTXF_APPLY);
                    break;

                case 5:
                    Matrix_Translate(gGfxMatrix, -1283.0f, 0.0f, -340.0f, MTXF_APPLY);
                    break;
            }

            if (i < 4) {
                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 32, 32, 128);
                Matrix_Scale(gGfxMatrix, var_fs1, var_fs1, var_fs1, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aBlueSphereDL);
            } else {
                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 128);
                Matrix_Scale(gGfxMatrix, var_fs1 * 6.0f, var_fs1 * 6.0f, var_fs1 * 6.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aBlueSphereDL);
            }
            Matrix_Pop(&gGfxMatrix);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }

    for (i = 0; i < 7; i++) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + i + 7, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        if (this->swork[2 + i] != 0) {
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, this->swork[19], 255);

            if ((this->swork[9 + i] % 2) != 0) {
                gDPSetPrimColor(gMasterDisp++, 0, 0, 80, 0, 0, 255);
            }

            Matrix_Push(&gGfxMatrix);

            if (i == 4) {
                Matrix_Translate(gGfxMatrix, 0.0f, 0, 300.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, M_PI / 4, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->fwork[21], this->fwork[21], 1.0f, MTXF_APPLY);
            }

            if (i == 5) {
                Matrix_Translate(gGfxMatrix, 0.0f, 330.0f, -1022.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->fwork[22] * 3.2f, 2.0f * this->fwork[22], 1.0f, MTXF_APPLY);
            }

            if (i == 6) {
                Matrix_Translate(gGfxMatrix, 0.0f, -330.0f, -1022.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->fwork[23] * 3.2f, 2.0f * this->fwork[23], 1.0f, MTXF_APPLY);
            }

            if (i < 4) {
                Matrix_RotateZ(gGfxMatrix, M_DTOR * 90.0f * i, MTXF_APPLY);
                Matrix_Translate(gGfxMatrix, 0.0f, 200.0f, 500.0f, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->fwork[17 + i], this->fwork[17 + i], 1.0f, MTXF_APPLY);
            }

            Matrix_SetGfxMtx(&gMasterDisp);

            if (i < 4) {
                gSPDisplayList(gMasterDisp++, D_ME_6024AD0);
            } else {
                gSPDisplayList(gMasterDisp++, D_ME_60236F0);
            }
            Matrix_Pop(&gGfxMatrix);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }

    gDPSetPrimColor(gMasterDisp++, 0, 0, 160, 255, 160, 128);

    for (i = 0; i < 4; i++) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + i + 11, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        if (this->fwork[11] != 0) {
            var_fs1 = this->fwork[11];
            if (this->fwork[11] > 15.0f) {
                var_fs1 = 15.0f;
            }

            Matrix_Push(&gGfxMatrix);
            switch (i) {
                case 0:
                    Matrix_Translate(gGfxMatrix, 746.0f, 741.0f, 680.0f, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, -M_PI / 4, MTXF_APPLY);
                    break;

                case 1:
                    Matrix_Translate(gGfxMatrix, -746.0f, 741.0f, 680.0f, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, M_PI / 4, MTXF_APPLY);
                    break;

                case 2:
                    Matrix_Translate(gGfxMatrix, -746.0f, -741.0f, 680.0f, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, 3 * M_PI / 4, MTXF_APPLY);
                    break;

                case 3:
                    Matrix_Translate(gGfxMatrix, 746.0f, -741.0f, 680.0f, MTXF_APPLY);
                    Matrix_RotateZ(gGfxMatrix, -3 * M_PI / 4, MTXF_APPLY);
                    break;
            }

            Matrix_SetGfxMtx(&gMasterDisp);
            Matrix_Push(&gGfxMatrix);

            if ((gGameFrameCount % 2) != 0) {
                Matrix_Scale(gGfxMatrix, var_fs1 * 2.5f, var_fs1 * 2.5f, var_fs1 * 2.5f, MTXF_APPLY);
            } else {
                Matrix_Scale(gGfxMatrix, var_fs1 * 3.0f, var_fs1 * 3.0f, var_fs1 * 3.0f, MTXF_APPLY);
            }

            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aBlueSphereDL);
            Matrix_Pop(&gGfxMatrix);
            Matrix_RotateX(gGfxMatrix, -M_PI / 6, MTXF_APPLY);

            if ((gGameFrameCount % 2) != 0) {
                Matrix_RotateY(gGfxMatrix, M_PI, MTXF_APPLY);
            }
            Matrix_Scale(gGfxMatrix, 2.0f, 2.0f * var_fs1, 2.0f, MTXF_APPLY);
            Matrix_Translate(gGfxMatrix, 0.0f, -20.0f, 0.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, D_102F5E0);
            Matrix_Pop(&gGfxMatrix);
        }
        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + 15, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if (this->fwork[11] > 15.0f) {
        var_fs1 = this->fwork[11] - 15.0f;
        gDPSetPrimColor(gMasterDisp++, 0, 0, 160, 255, 160, 255);
        Matrix_Push(&gGfxMatrix);
        Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, 1300.0f, MTXF_APPLY);
        if ((gGameFrameCount % 2) != 0) {
            Matrix_Scale(gGfxMatrix, var_fs1 * 0.8f, var_fs1 * 0.8f, var_fs1 * 0.8f, MTXF_APPLY);
        } else {
            Matrix_Scale(gGfxMatrix, var_fs1, var_fs1, var_fs1, MTXF_APPLY);
        }
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, aBlueSphereDL);
        Matrix_Pop(&gGfxMatrix);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    if ((this->state == 9) || (this->state == 10)) {
        var_fs1 = this->fwork[15];
        for (i = 0; i < 10; i++) {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + i + 16, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            var_fs1 += 0.3f;
            var_fs0 = var_fs1;
            if (var_fs1 <= 7.0f) {
                if (var_fs1 < 1.0f) {
                    var_fs0 = 1.0f;
                }

                gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, D_i2_80195554[i]);
                Matrix_Push(&gGfxMatrix);

                Matrix_Translate(gGfxMatrix, 700.0f, 0.0f, -(1235.0f - ((var_fs0 - 1.0f) * 89.2f)), MTXF_APPLY);

                if ((gGameFrameCount % 2) != 0) {
                    Matrix_Scale(gGfxMatrix, var_fs0, var_fs0, var_fs0, MTXF_APPLY);
                } else {
                    Matrix_Scale(gGfxMatrix, var_fs0 * 1.1f, var_fs0 * 1.1f, var_fs0 * 1.1f, MTXF_APPLY);
                }

                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_ME_6023810);
                Matrix_Pop(&gGfxMatrix);
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -700.0f, 0.0f, -(1235.0f - ((var_fs0 - 1.0f) * 89.2f)), MTXF_APPLY);

                if ((gGameFrameCount % 2) != 0) {
                    Matrix_Scale(gGfxMatrix, var_fs0, var_fs0, var_fs0, MTXF_APPLY);
                } else {
                    Matrix_Scale(gGfxMatrix, var_fs0 * 1.1f, var_fs0 * 1.1f, var_fs0 * 1.1f, MTXF_APPLY);
                }

                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_ME_6023810);
                Matrix_Pop(&gGfxMatrix);
            }

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }

    if (this->swork[0] > 0) {
        Matrix_Push(&gGfxMatrix);
        Matrix_Translate(gGfxMatrix, 0.0f, 650.0f, 600.0f, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + 30, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        Meteo_MeCrusherEngineGlow_Draw(this->swork[0] - 1);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        Matrix_Pop(&gGfxMatrix);
    }

    if (this->swork[1] > 0) {
        Matrix_Push(&gGfxMatrix);
        Matrix_Translate(gGfxMatrix, 0.0f, -650.0f, 600.0f, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + 31, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        Meteo_MeCrusherEngineGlow_Draw(this->swork[1] - 1);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        Matrix_Pop(&gGfxMatrix);
    }

    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
}
#endif

RECOMP_PATCH void Meteo_MeCrusherShield_Draw(MeCrusherShield* this) {
    s32 i;

    if ((this->timer_054 % 2) != 0) {
        RCP_SetupDL_27();
        gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 255, 0, 255);
    }

    Matrix_RotateZ(gGfxMatrix, (M_PI / 4), MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    gSPDisplayList(gMasterDisp++, D_ME_60240B0);
    RCP_SetupDL_64();

    for (i = 1; i < 4; i++) {
        if (this->swork[0] != 0) {
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, this->swork[0]);
            Matrix_Push(&gGfxMatrix);
            Matrix_RotateZ(gGfxMatrix, M_DTOR * 90.0f * i, MTXF_APPLY);
            Matrix_Translate(gGfxMatrix, 0.0f, 156.0f, 930.0f, MTXF_APPLY);

            if ((gGameFrameCount % 2) != 0) {
                Matrix_Scale(gGfxMatrix, 6.0f, 9.0f, 5.0f, MTXF_APPLY);
            } else {
                Matrix_Scale(gGfxMatrix, 4.0f, 7.0f, 5.0f, MTXF_APPLY);
            }

            Matrix_RotateZ(gGfxMatrix, 1.5707964f, MTXF_APPLY); // (M_PI / 2) does not match
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, D_ME_60263F0);
            Matrix_Pop(&gGfxMatrix);
        }
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

typedef struct {
    /* 0x00 */ f32 r[3];
    /* 0x0C */ f32 g[3];
    /* 0x18 */ f32 b[3];
    /* 0x24 */ f32 unk_24;
    /* 0x30 */ f32 unk_28[3];
    /* 0x3C */ f32 unk_34;
} UnkStruct_1C22F0;

extern UnkStruct_1C22F0 D_i3_801C22F0;

RECOMP_PATCH void Area6_8018BCD4(Vec3f* arg0, f32 arg1, f32 arg2, Vec3f* arg3, s32 arg4, f32 arg5, s32 arg6, f32 arg7) {
    s32 i;
    Vec3f sp90 = { 0.0f, 0.0f, 0.0f };

    if (D_i3_801C22F0.unk_24 != 0.0f) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_23);
        if (D_i3_801C22F0.unk_24 != 255.0f) {
            RCP_SetupDL(&gMasterDisp, SETUPDL_71);
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, (s32) D_i3_801C22F0.unk_24);
        } else if (arg4 != 0) {
            RCP_SetupDL(&gMasterDisp, SETUPDL_27);
            if (arg7 < 18.0f) {
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 0, 255, 255);
            } else {
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 0, 255);
            }
        }

        Matrix_Push(&gGfxMatrix);
        Matrix_Push(&gCalcMatrix);
        Matrix_Copy(gCalcMatrix, &gIdentityMatrix);
        Matrix_RotateZ(gCalcMatrix, M_DTOR * arg1, MTXF_APPLY);
        Matrix_Translate(gCalcMatrix, 0.0f, 100.0f, -223.0f, MTXF_APPLY);
        Matrix_RotateX(gCalcMatrix, M_DTOR * arg2, MTXF_APPLY);

        for (i = 0; i < arg6; i++) {
            Matrix_Push(&gGfxMatrix);
            Matrix_Push(&gCalcMatrix);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS_GORGON(arg0) | ((i << 16) & 0x00FF0000), G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

            if (i == 11) {
                Matrix_Scale(gCalcMatrix, 1.5f, 1.5f, 1.5f, MTXF_APPLY);
                Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
                gSPDisplayList(gMasterDisp++, D_A6_600F1F0);
                Matrix_MultVec3f(gCalcMatrix, &sp90, arg3);
            } else {
                Matrix_Scale(gCalcMatrix, 2.0f, 2.0f, 2.0f, MTXF_APPLY);
                Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);

                gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
                gSPDisplayList(gMasterDisp++, D_A6_6015EE0);

                if (arg6 != 12) {
                    Matrix_MultVec3f(gCalcMatrix, &sp90, arg3);
                }
            }
            Matrix_Pop(&gGfxMatrix);
            Matrix_Pop(&gCalcMatrix);
            Matrix_RotateY(gCalcMatrix, arg0[i].y * M_DTOR, MTXF_APPLY);
            Matrix_RotateX(gCalcMatrix, arg0[i].x * M_DTOR, MTXF_APPLY);
            Matrix_Translate(gCalcMatrix, 0.0f, 0.0f, arg5, MTXF_APPLY);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }

        Matrix_Pop(&gGfxMatrix);
    }
}

RECOMP_PATCH void SectorY_SyShogun_Draw(SyShogun* this) {
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;

    if ((this->index != 0) || ((this->swork[24] % 2) != 0) || ((this->timer_05C % 2) != 0)) {
        RCP_SetupDL_30(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    } else {
        RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    }

    if ((this->health > 0) || (this->swork[36] == 0)) {
        Animation_DrawSkeleton(2, D_SY_602D140, this->vwork, SectorY_SyShogun_OverrideLimbDraw,
                               SectorY_SyShogun_PostLimbDraw, this, gCalcMatrix);
        if (this->timer_054 != 0) {
            sp9C = D_i6_801A69AC[this->timer_054];
            RCP_SetupDL_49();
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
            gDPSetEnvColor(gMasterDisp++, 255, 48, 0, 255);
            Matrix_Pop(&gGfxMatrix);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + 1, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, this->fwork[1], this->fwork[2], this->fwork[3], MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, sp9C, sp9C, sp9C, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aOrbDL);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }

        if (this->fwork[34] != 0.0f) {
            RCP_SetupDL_49();
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
            gDPSetEnvColor(gMasterDisp++, 32, 255, 32, 255);
            Matrix_Pop(&gGfxMatrix);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + 2, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, this->fwork[31], this->fwork[32], this->fwork[33], MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, this->fwork[34], this->fwork[34], this->fwork[34], MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, -this->obj.rot.x * M_DTOR, MTXF_APPLY);
            Matrix_RotateZ(gGfxMatrix, -this->obj.rot.z * M_DTOR, MTXF_APPLY);
            sp98 = gPlayer[0].cam.eye.x - this->obj.pos.x;
            sp94 = gPlayer[0].cam.eye.y - this->obj.pos.y;
            sp90 = gPlayer[0].cam.eye.z - (this->obj.pos.z + gPathProgress);
            sp8C = -Math_Atan2F(sp98, sp90);
            sp9C = sqrtf(SQ(sp90) + SQ(sp98));
            sp88 = Math_Atan2F(sp94, sp9C);
            Matrix_RotateY(gGfxMatrix, -sp8C, MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, -sp88, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aOrbDL);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }

        if (this->index == 0) {
            if (this->fwork[41] != 0.0f) {
                RCP_SetupDL_49();
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                gDPSetEnvColor(gMasterDisp++, 32, 255, 32, 255);
                Matrix_Pop(&gGfxMatrix);

                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + 3, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, this->fwork[35], this->fwork[36], this->fwork[37], MTXF_APPLY);
                Matrix_RotateY(gGfxMatrix, this->obj.rot.y * M_DTOR, MTXF_APPLY);
                Matrix_RotateX(gGfxMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, this->obj.rot.z * M_DTOR, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->fwork[41] * 2.0f, this->fwork[41], this->fwork[41] * 2.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            }

            if (this->fwork[42] != 0.0f) {
                RCP_SetupDL_49();
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
                gDPSetEnvColor(gMasterDisp++, 32, 255, 32, 255);
                Matrix_Pop(&gGfxMatrix);

                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + 4, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, this->fwork[38], this->fwork[39], this->fwork[40], MTXF_APPLY);
                Matrix_RotateY(gGfxMatrix, this->obj.rot.y * M_DTOR, MTXF_APPLY);
                Matrix_RotateX(gGfxMatrix, this->obj.rot.x * M_DTOR, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, this->obj.rot.z * M_DTOR, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, this->fwork[42] * 2.0f, this->fwork[42], this->fwork[42] * 2.0f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
            }
        }

        if (this->fwork[43] != 0.0f) {
            RCP_SetupDL_49();
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 0, 0, 0, 255);
            gDPSetEnvColor(gMasterDisp++, 0, 0, 0, 0);
            Matrix_Pop(&gGfxMatrix);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + 5, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, this->obj.pos.x, 157.0f, this->obj.pos.z, MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, this->fwork[43], this->fwork[43], this->fwork[43], MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aOrbDL);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }

        if (this->fwork[46] != 0.0f) {
            RCP_SetupDL_49();
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, (s32) this->fwork[46], (s32) this->fwork[46], 0,
                            (s32) this->fwork[46]);
            gDPSetEnvColor(gMasterDisp++, 0, 0, 0, 0);
            Matrix_Pop(&gGfxMatrix);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this) + 6, G_EX_PUSH, G_MTX_MODELVIEW,
                                           G_EX_EDIT_ALLOW);

            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, this->obj.pos.x + 10.0f, this->obj.pos.y + 70.0f, this->obj.pos.z + 60.0f,
                             MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, 0.4f, 0.2f, 0.2f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aOrbDL);
            Matrix_Translate(gGfxMatrix, -46.0f, 0, 0, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aOrbDL);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }
    }
}

// TODO Tag Vertices
#if 0
RECOMP_PATCH void Aquas_AqBacoonMuscle_Draw(AqBacoonMuscle* this) {
    f32 var_fv0;
    u8 var_t1;
    u8 i;
    Vtx* var_t5;
    Vtx* temp_v1_2;

    if ((this->timer_0C6 != 0) && (this->state == 0) && (gPlayState != PLAY_PAUSE)) {
        this->iwork[1]++;
        this->iwork[1] %= 8;
    }

    temp_v1_2 = SEGMENTED_TO_VIRTUAL(D_AQ_6019078);
    if (this->iwork[1] < 4) {
        var_t5 = SEGMENTED_TO_VIRTUAL(D_AQ_6018C78);
    } else {
        var_t5 = SEGMENTED_TO_VIRTUAL(D_AQ_6018878);
    }

    var_t1 = this->iwork[1] % 4;
    if (var_t1 >= 3) {
        var_t1 = 4 - var_t1;
    }

    var_fv0 = this->timer_0C6 / 60.0f;
    if (var_fv0 < 0.1f) {
        var_fv0 = 0.1f;
    }

    for (i = 0; i < 28; i += 1) {
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.ob[0] =
            temp_v1_2[i].n.ob[0] + (s16) ((((var_t5[i].n.ob[0] - temp_v1_2[i].n.ob[0]) * var_t1) / 2) * var_fv0);
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.ob[1] =
            temp_v1_2[i].n.ob[1] + (((var_t5[i].n.ob[1] - temp_v1_2[i].n.ob[1]) * var_t1) / 2);
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.ob[2] =
            temp_v1_2[i].n.ob[2] + (((var_t5[i].n.ob[2] - temp_v1_2[i].n.ob[2]) * var_t1) / 2);
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.flag = temp_v1_2[i].n.flag;
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.tc[0] =
            temp_v1_2[i].n.tc[0] + (((var_t5[i].n.tc[0] - temp_v1_2[i].n.tc[0]) * var_t1) / 2);
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.tc[1] =
            temp_v1_2[i].n.tc[1] + (((var_t5[i].n.tc[1] - temp_v1_2[i].n.tc[1]) * var_t1) / 2);
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.n[0] =
            temp_v1_2[i].n.n[0] + (((var_t5[i].n.n[0] - temp_v1_2[i].n.n[0]) * var_t1) / 2);
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.n[1] =
            temp_v1_2[i].n.n[1] + (((var_t5[i].n.n[1] - temp_v1_2[i].n.n[1]) * var_t1) / 2);
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.n[2] =
            temp_v1_2[i].n.n[2] + (((var_t5[i].n.n[2] - temp_v1_2[i].n.n[2]) * var_t1) / 2);
        D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][i].n.a = temp_v1_2[i].n.a;
    }

    Matrix_Scale(gGfxMatrix, this->fwork[1] - 0.25f + ((1.5f - this->scale) * 0.5f), this->scale, this->fwork[1],
                 MTXF_APPLY);

    if (this->state != 0) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_32);
    } else if (gBosses[0].swork[0] == 1) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_4);
    } else if ((this->timer_0C6 % 2) == 0) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_29);
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_22);
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 0, 0, 255);
    }

    // @recomp Tag the transform.
            gEXMatrixGroupDecomposed(gMasterDisp++, 0x50123458, G_EX_PUSH, G_MTX_MODELVIEW,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                                     G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, G_EX_EDIT_ALLOW);

    Matrix_SetGfxMtx(&gMasterDisp);

    gDPSetTextureLUT(gMasterDisp++, G_TT_RGBA16);
    gDPLoadTLUT_pal256(gMasterDisp++, D_AQ_6008FC8);
    gDPLoadTextureBlock(gMasterDisp++, D_AQ_6008EC8, G_IM_FMT_CI, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, 4, 4, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(gMasterDisp++, D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2], 14, 0);
    gSP1Triangle(gMasterDisp++, 0, 1, 2, 0);
    gSP1Triangle(gMasterDisp++, 3, 1, 0, 0);
    gSP1Triangle(gMasterDisp++, 4, 5, 0, 0);
    gSP1Triangle(gMasterDisp++, 0, 6, 7, 0);
    gSP1Triangle(gMasterDisp++, 2, 6, 0, 0);
    gSP1Triangle(gMasterDisp++, 0, 5, 3, 0);
    gSP1Triangle(gMasterDisp++, 0, 8, 4, 0);
    gSP1Triangle(gMasterDisp++, 7, 8, 0, 0);
    gSP1Triangle(gMasterDisp++, 9, 4, 8, 0);
    gSP1Triangle(gMasterDisp++, 8, 7, 10, 0);
    gSP1Triangle(gMasterDisp++, 8, 11, 9, 0);
    gSP1Triangle(gMasterDisp++, 10, 11, 8, 0);
    gSP1Triangle(gMasterDisp++, 12, 4, 9, 0);
    gSP1Triangle(gMasterDisp++, 13, 5, 4, 0);
    gSP1Triangle(gMasterDisp++, 4, 12, 13, 0);
    gSP1Triangle(gMasterDisp++, 3, 5, 13, 0);
    gSP1Triangle(gMasterDisp++, 13, 6, 2, 0);
    gSP1Triangle(gMasterDisp++, 7, 6, 13, 0);
    gSP1Triangle(gMasterDisp++, 13, 12, 7, 0);
    gSP1Triangle(gMasterDisp++, 10, 7, 12, 0);
    gSPVertex(gMasterDisp++, &D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][1], 3, 0);
    gSPVertex(gMasterDisp++, &D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][9], 2, 3);
    gSPVertex(gMasterDisp++, &D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][12], 8, 5);
    gSP1Triangle(gMasterDisp++, 0, 2, 7, 0);
    gSP1Triangle(gMasterDisp++, 7, 2, 8, 0);
    gSP1Triangle(gMasterDisp++, 6, 8, 2, 0);
    gSP1Triangle(gMasterDisp++, 8, 1, 9, 0);
    gSP1Triangle(gMasterDisp++, 1, 8, 6, 0);
    gSP1Triangle(gMasterDisp++, 5, 10, 4, 0);
    gSP1Triangle(gMasterDisp++, 3, 10, 5, 0);
    gSP1Triangle(gMasterDisp++, 8, 12, 7, 0);
    gSP1Triangle(gMasterDisp++, 7, 11, 0, 0);
    gSP1Triangle(gMasterDisp++, 0, 11, 9, 0);
    gSP1Triangle(gMasterDisp++, 9, 1, 0, 0);
    gSP1Triangle(gMasterDisp++, 9, 12, 8, 0);
    gDPLoadTLUT_pal256(gMasterDisp++, D_AQ_6019338);
    gDPLoadTextureBlock(gMasterDisp++, D_AQ_6019238, G_IM_FMT_CI, G_IM_SIZ_8b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, 4, 4, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(gMasterDisp++, &D_i3_801C3A88[this->iwork[0]][gSysFrameCount % 2][20], 8, 0);
    gSP1Triangle(gMasterDisp++, 0, 1, 2, 0);
    gSP1Triangle(gMasterDisp++, 2, 3, 0, 0);
    gSP1Triangle(gMasterDisp++, 4, 5, 6, 0);
    gSP1Triangle(gMasterDisp++, 6, 7, 4, 0);
    gDPPipeSync(gMasterDisp++);
    gDPSetTextureLUT(gMasterDisp++, G_TT_NONE);

    // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}
#endif

#if 0

RECOMP_PATCH void Katina_KaFrontlineBase_Draw(KaFrontlineBase* this) {
    gSPFogPosition(gMasterDisp++, gFogNear, 1002);
    Matrix_Translate(gGfxMatrix, 0.0f, 20.0f, 0.0f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    if (this->state == 0) {
        gSPDisplayList(gMasterDisp++, aKaFLBaseDL);
    } else {
        RCP_SetupDL(&gMasterDisp, SETUPDL_57);
        gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
        gSPDisplayList(gMasterDisp++, aKaFLBaseDestroyedDL);
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void FoBase_Draw(Boss* this) {
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    RCP_SetupDL_29(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    gSPDisplayList(gMasterDisp++, aFoBaseDL2);
    RCP_SetupDL_34(gFogRed, gFogGreen, gFogBlue, gFogAlpha, gFogNear, gFogFar);
    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
    gDPSetTextureFilter(gMasterDisp++, G_TF_POINT);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 0, (s32) this->fwork[1], (s32) this->fwork[2], 255);
    gSPDisplayList(gMasterDisp++, aFoBaseDL1);
    gDPSetTextureFilter(gMasterDisp++, G_TF_BILERP);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Bolse_BoBase_Draw(BoBase* this) {
    s32 i;
    s32 alpha;

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

    Matrix_Scale(gGfxMatrix, this->scale, this->scale, this->scale, MTXF_APPLY);

    if (this->vwork[30].y >= 0.0f) {
        gSPDisplayList(gMasterDisp++, D_BO_6002020);
    }

    if ((gGameFrameCount % 2) != 0) {
        alpha = 128;
    } else {
        alpha = 30;
    }

    for (i = 0; i < 6; i++) {
        if (D_i4_801A0488[i].unk_18 == 0) {
            continue;
        }
        Matrix_Push(&gGfxMatrix);
        RCP_SetupDL(&gMasterDisp, SETUPDL_49);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, alpha);
        gDPSetEnvColor(gMasterDisp++, 255, 56, 56, alpha);
        Matrix_Translate(gGfxMatrix, D_i4_801A0488[i].unk_0C, D_i4_801A0488[i].unk_10, D_i4_801A0488[i].unk_14,
                         MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, D_i4_801A0488[i].unk_04 * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, D_i4_801A0488[i].unk_00 * M_DTOR, MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, M_PI / 2, MTXF_APPLY);
        Matrix_Translate(gGfxMatrix, 1.0f, 1.0f, D_i4_801A0488[i].unk_08 * 200.0f, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, 1.0f, 1.0f, D_i4_801A0488[i].unk_08, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, D_BO_600BF30);
        Matrix_Pop(&gGfxMatrix);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}

RECOMP_PATCH void Bolse_BoBaseShield_Draw(BoBaseShield* this) {
    s32 alpha;

    Matrix_Scale(gGfxMatrix, this->scale, this->scale, this->scale, MTXF_APPLY);

    alpha = this->fwork[0];
    if (alpha != 0) {
        if ((gGameFrameCount % 2) == 0) {
            alpha *= 1.7f;
        }
        RCP_SetupDL(&gMasterDisp, SETUPDL_41);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, alpha);
        Matrix_Scale(gGfxMatrix, 1.2f, 0.55f, 1.2f, MTXF_APPLY);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_BOSS(this), G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, aBoBaseShieldDL);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}

#endif