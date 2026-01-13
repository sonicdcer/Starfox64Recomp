
#include "patches.h"
#include "rt64_extended_gbi.h"
#include "sfx.h"

#if 1 // enable/disable all patches

#if 1 // Hud_PlayerShieldGauge_Draw

extern f32 sShieldGaugeCurrentScale;
extern f32 sShieldFillAmount;
extern s32 sShieldBorderColorR;
extern s32 sShieldBorderColorG;
extern s32 sShieldBorderColorB;

void HUD_ShieldGaugeBars_Draw(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
void HUD_ShieldGaugeEdgeLeft_Draw(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
void HUD_ShieldGaugeEdgeRight_Draw(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
void HUD_ShieldGaugeFrame_Draw(f32 x, f32 y, f32 arg2, f32 arg3);
void HUD_BoostGaugeFrame_Draw(f32 arg0, f32 arg1, f32 arg2, f32 arg3);
void HUD_RadarMarks_Setup(void);
void HUD_RadarMark_Draw(s32 type);
void HUD_RadarWindow_Draw(f32 x, f32 y);
void HUD_RadarMissileAlarm_Draw(void);

RECOMP_PATCH void HUD_PlayerShieldGauge_Draw(f32 x, f32 y) {

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);

    RCP_SetupDL(&gMasterDisp, SETUPDL_75);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
    HUD_ShieldGaugeBars_Draw(x + 8.0f, y + 2.0f, sShieldGaugeCurrentScale, 1.0f, sShieldFillAmount);
    // White Outiline
    RCP_SetupDL(&gMasterDisp, SETUPDL_76);
    gDPSetPrimColor(gMasterDisp++, 0, 0, sShieldBorderColorR, sShieldBorderColorG, sShieldBorderColorB, 255);
    HUD_ShieldGaugeEdgeLeft_Draw(x, y, 1.0f, 1.0f);
    HUD_ShieldGaugeEdgeRight_Draw(x + 7.0f + (sShieldGaugeCurrentScale * 6.0f * 8.0f), y, 1.0f, 1.0f);
    HUD_ShieldGaugeFrame_Draw(x + 7.0f, y, sShieldGaugeCurrentScale * 6.0f, 1.0f);

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
}
#endif

#if 1 // Hud_Hitpoints_Draw

extern u8 aLargeText_0[];
extern u8 aLargeText_1[];
extern u8 aLargeText_2[];
extern u8 aLargeText_3[];
extern u8 aLargeText_4[];
extern u8 aLargeText_5[];
extern u8 aLargeText_6[];
extern u8 aLargeText_7[];
extern u8 aLargeText_8[];
extern u8 aLargeText_9[];

RECOMP_PATCH void HUD_Score_Draw(f32 x, f32 y) {
    u8* D_800D24DC[] = { aLargeText_0, aLargeText_1, aLargeText_2, aLargeText_3, aLargeText_4,
                         aLargeText_5, aLargeText_6, aLargeText_7, aLargeText_8, aLargeText_9 };
    s32 D_800D2504[] = { 100, 10, 1 };
    bool boolTemp;
    s32 i;
    s32 j;
    s32 temp;
    s32 temp2;
    s32 temp3;
    s32 temp4;
    f32 x1;
    f32 y1;
    f32 xScale;

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);

    if (gHitCount > gDisplayedHitCount) {
        temp3 = gDisplayedHitCount + 1;
        temp4 = gDisplayedHitCount;
    } else {
        temp3 = gHitCount;
        temp4 = gDisplayedHitCount;
    }

    boolTemp = false;
    i = 1000;
    temp3 %= i;
    temp4 %= i;

    for (i /= 10, j = 0; i != 1; i /= 10, j++) {
        xScale = 1.0f;
        x1 = x;
        y1 = y + 3.0f;

        temp = temp3 / i;
        temp2 = temp4 / i;

        if ((temp != 0) || (boolTemp == true)) {
            if (temp != temp2) {
                D_hud_80161720[j] += 0.4f;
                if (D_hud_80161720[j] <= 0.9f) {
                    xScale = D_hud_80161720[j];
                    x1 += 8.0f * xScale;
                    xScale = 1.0f - xScale;
                }

                if ((D_hud_80161720[j] > 0.9f) && (D_hud_80161720[j] < 1.1f)) {
                    xScale = 0.0f;
                }

                if (D_hud_80161720[j] >= 2.0f) {
                    temp2++;
                    if (temp2 >= 10) {
                        temp2 = 0;
                    }
                    D_hud_80161720[j] = 0.0f;
                }

                if ((D_hud_80161720[j] < 2.0f) && (D_hud_80161720[j] >= 1.1f)) {
                    temp2++;
                    if (temp2 >= 10) {
                        temp2 = 0;
                    }
                    xScale = 2.0f - D_hud_80161720[j];
                    x1 += 8.0f * xScale;
                    xScale = 1.0f - xScale;
                }
                temp = temp2;
            }

            if (xScale != 0.0f) {
                Lib_TextureRect_IA8(&gMasterDisp, D_800D24DC[temp], 16, 15, x1, y1, xScale, 1.0f);
            }
            boolTemp = true;
        }

        if (!boolTemp && (xScale != 0.0f)) {
            Lib_TextureRect_IA8(&gMasterDisp, D_800D24DC[0], 16, 15, x1, y1, xScale, 1.0f);
        }

        x += 13.0f;
        temp3 %= i;
        temp4 %= i;
    }

    xScale = 1.0f;
    x1 = x;
    y1 = y + 3.0f;

    if (temp3 != temp4) {
        D_hud_80161720[j] += 0.4f;
        if (D_hud_80161720[j] <= 0.9f) {
            xScale = D_hud_80161720[j];
            x1 += 8.0f * xScale;
            xScale = 1.0f - xScale;
        }

        if ((D_hud_80161720[j] > 0.9f) && (D_hud_80161720[j] < 1.1f)) {
            xScale = 0.0f;
        }

        if (D_hud_80161720[j] >= 2.0f) {
            temp4++;
            if (temp4 >= 10) {
                temp4 = 0;
            }
            D_hud_80161720[j] = 0.0f;
        }

        if ((D_hud_80161720[j] < 2.0f) && (D_hud_80161720[j] >= 1.1f)) {
            temp4++;
            if (temp4 >= 10) {
                temp4 = 0;
            }
            xScale = 2.0f - D_hud_80161720[j];
            x1 += 8.0f * xScale;
            xScale = 1.0f - xScale;
        }
        temp3 = temp4;
    }

    if (xScale != 0.0f) {
        Lib_TextureRect_IA8(&gMasterDisp, D_800D24DC[temp3], 16, 15, x1, y1, xScale, 1.0f);
    }

    if ((gHitCount != gDisplayedHitCount) && (D_hud_80161720[0] == 0.0f) && (D_hud_80161720[1] == 0.0f) &&
        (D_hud_80161720[2] == 0.0f)) {
        gDisplayedHitCount++;

        if ((gDisplayedHitCount == 4) || (gDisplayedHitCount == 9) || (gDisplayedHitCount == 14) ||
            (gDisplayedHitCount == 19) || (gDisplayedHitCount == 24) || (gDisplayedHitCount == 29)) {
            gDropHitCountItem = gDisplayedHitCount;
        }
    }

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
}
#endif

#if 1 // Hud_GoldRings_Draw

static f32 tempx = 0;
static f32 tempy = 0;

RECOMP_PATCH void HUD_GoldRings_Draw(void) {
    Gfx* sGoldRingDLs[] = { aGoldRingFrame1DL, aGoldRingFrame2DL,  aGoldRingFrame3DL,  aGoldRingFrame4DL,
                            aGoldRingFrame5DL, aGoldRingFrame6DL,  aGoldRingFrame7DL,  aGoldRingFrame8DL,
                            aGoldRingFrame9DL, aGoldRingFrame10DL, aGoldRingFrame11DL, aGoldRingFrame12DL };
    s32 i;
    s32 j;
    f32 D_800D1AC4[] = { 0.0f, -30.0f, -26.0f, -22.0f, -18.0f };
    f32 D_800D1AD8[] = { 0.0f, 28.0f, 28.0f, 28.0f, 28.0f };
    f32 scale;
    f32 x;
    f32 y;
    s32 temp;

    // @recomp
    gEXViewport(gMasterDisp++, G_EX_ORIGIN_LEFT, gViewport); // Force the viewport update

    sGoldRingsFwork[6] += 0.7f;
    if (sGoldRingsFwork[6] >= 12.0f) {
        sGoldRingsFwork[6] = 0.0f;
    }

    if (D_80161900[4]) {
        D_80161900[4]--;
    }

    for (i = 0; i < 4; i++) {
        if (D_80161900[i]) {
            D_80161900[i]--;
        }
    }

    if ((D_80161900[4] == 0) && (gGoldRingCount[0] > gGoldRingCount[1])) {
        gGoldRingCount[1] += 1; // can't be ++

        if ((i = gGoldRingCount[1] % 3) == 0) {
            i = 3;
        }

        i--;

        sGoldRingsIwork[1 + i] = 1;
        D_80161900[0 + i] = 14;
        sGoldRingsFwork[2 + i] = 0.0f;

        if (i == 2) {
            D_80161900[4] = 28;
        }

        if (gGoldRingCount[1] == 3) {
            D_80161900[4] += 28;
        }
    }

    for (i = 0; i < 3; i++) {

        // @recomp Tag the transform
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_HUD_GOLD_RING + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        switch (sGoldRingsIwork[i + 1]) {
            case 0: // transparent gold rings

                RCP_SetupDL(&gMasterDisp, SETUPDL_62);
                Matrix_Push(&gGfxMatrix);

                x = D_800D1AC4[i + 1] + tempx;
                y = D_800D1AD8[i + 1] + tempy;

                scale = 0.28f;

                if (gFovYMode == 2) {
                    x -= 7.00f;
                    y += 7.00f;
                    scale += 0.06f;
                }

                Matrix_Translate(gGfxMatrix, x, y, -100.0f, MTXF_NEW);
                Matrix_Scale(gGfxMatrix, scale, scale, scale, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);

                gDPSetPrimColor(gMasterDisp++, 0, 0, 180, 180, 0, 50);
                gSPDisplayList(gMasterDisp++, aGoldRingEmptySlotDL);

                Matrix_Pop(&gGfxMatrix);

                break;

            case 1:
            case 2:
                if (D_80161900[i]) {
                    if (D_80161900[i + 0] >= 7) {
                        sGoldRingsFwork[i + 2] += 0.15f;
                    } else {
                        sGoldRingsFwork[i + 2] -= 0.15f;
                    }

                    RCP_SetupDL(&gMasterDisp, SETUPDL_62);
                    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

                    // @recomp Tag the transform
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_HUD_GOLD_RING + i + 3, G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);

                    Matrix_Push(&gGfxMatrix);
                    Matrix_Translate(gGfxMatrix, D_800D1AC4[i + 1], D_800D1AD8[i + 1], -100.0f, MTXF_NEW);
                    Matrix_RotateZ(gGfxMatrix, M_DTOR * sGoldRingsFwork[0], MTXF_APPLY);
                    Matrix_Scale(gGfxMatrix, sGoldRingsFwork[i + 2], sGoldRingsFwork[i + 2], sGoldRingsFwork[i + 2],
                                 MTXF_APPLY);

                    Matrix_SetGfxMtx(&gMasterDisp);
                    gSPDisplayList(gMasterDisp++, aStarDL);
                    Matrix_Pop(&gGfxMatrix);

                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                }

                if (D_80161900[i] < 7) {
                    if (sGoldRingsIwork[i + 1] == 2) {
                        RCP_SetupDL(&gMasterDisp, SETUPDL_62);
                    } else {
                        RCP_SetupDL(&gMasterDisp, SETUPDL_36);
                    }

                    // @recomp Tag the transform
                    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_HUD_GOLD_RING + i + 6, G_EX_PUSH, G_MTX_MODELVIEW,
                                                   G_EX_EDIT_ALLOW);

                    Matrix_Push(&gGfxMatrix);

                    x = D_800D1AC4[i + 1];
                    y = D_800D1AD8[i + 1];
                    scale = 0.28f;

                    if (gFovYMode == 2) {
                        x -= 7.00f;
                        y += 7.00f;
                        scale += 0.06f;
                    }

                    Matrix_Translate(gGfxMatrix, x, y, -100.0f, MTXF_NEW);
                    Matrix_Scale(gGfxMatrix, scale, scale, scale, MTXF_APPLY);
                    Matrix_SetGfxMtx(&gMasterDisp);

                    if (sGoldRingsIwork[i + 1] == 2) {
                        gDPSetPrimColor(gMasterDisp++, 0, 0, 180, 180, 0, 50);
                        gSPDisplayList(gMasterDisp++, aGoldRingEmptySlotDL);
                    } else {
                        temp = sGoldRingsFwork[6];
                        gSPDisplayList(gMasterDisp++, sGoldRingDLs[temp]);
                    }
                    Matrix_Pop(&gGfxMatrix);

                    // @recomp Pop the transform id.
                    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
                }
                break;

            default:
                break;
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }

    if ((gGoldRingCount[1] && ((gGoldRingCount[1] % 3) == 0)) && (D_80161900[4] == 1)) {
        for (j = 0; j < 4; j++) {
            if (sGoldRingsIwork[j + 1] == 1) {
                sGoldRingsIwork[j + 1] = 2;
                D_80161900[j + 0] = 14;
                sGoldRingsFwork[j + 2] = 0.0f;
                D_80161900[4] = 28;
            } else {
                sGoldRingsIwork[j + 1] = 0;
                D_80161900[j + 0] = 14;
                sGoldRingsFwork[j + 2] = 0.0f;
            }
        }
    }
    sGoldRingsFwork[0] += 35.0f;
    sGoldRingsFwork[1] += 10.0f;

    gEXViewport(gMasterDisp++, G_EX_ORIGIN_NONE, gViewport);
}
#endif

#if 1 // HUD_DrawBossHealth

extern f32 D_801616BC;
extern f32 D_801616C0;
extern u8 D_1011A40[];
extern u16 D_1011AB0[];
extern u8 D_1002040[];
extern u8 D_10129C0[];
extern u16 D_1013090[];
extern f32 D_801616C4;
extern f32 D_801616C8;

RECOMP_PATCH void HUD_DrawBossHealth(void) {
    f32 sp3C;
    f32 temp1;
    f32 temp2;
    f32 temp3;
    f32 temp4;
    f32 temp5;
    f32 temp6;
    f32 temp7;
    f32 var_fv0;

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);

    if ((gShowBossHealth == 1) && (gTeamShields[TEAM_ID_SLIPPY] > 0)) {
        if ((gBossHealthBar >= 0) && (D_801616BC == -1.0f)) {
            AUDIO_PLAY_SFX(NA_SE_BOSS_GAUGE_OPEN, gDefaultSfxSource, 4);
            D_801616BC = 255.0f;
        }

        Math_SmoothStepToF(&D_801616C0, 25.0f, 0.3f, 4.0f, 4.0f);

        sp3C = D_801616C0;
        temp1 = 52.0f;
        temp2 = sp3C + 5.0f;
        temp3 = temp1 + 3.0f;
        temp4 = sp3C + 6.0f;
        temp5 = temp1 + 10.0f;

        RCP_SetupDL(&gMasterDisp, SETUPDL_78);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
        Lib_TextureRect_CI4(&gMasterDisp, D_1011A40, D_1011AB0, 32, 7, temp2, temp3, 1.0f, 1.0f);

        RCP_SetupDL(&gMasterDisp, SETUPDL_76);
        Lib_TextureRect_IA8(&gMasterDisp, D_1002040, 40, 12, sp3C, temp1, 1.0f, 1.0f);

        if (sp3C >= 25.0f) {
            Math_SmoothStepToF(&D_801616C4, 0.88f, 0.3f, 0.2f, 0.1f);
        }

        if ((D_801616C4 >= 0.1f) && (sp3C >= 25.0f)) {
            RCP_SetupDL(&gMasterDisp, SETUPDL_78);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
            Lib_TextureRect_CI8(&gMasterDisp, D_10129C0, D_1013090, 16, 109, temp4, temp5, 1.0f, D_801616C4);
        }

        RCP_SetupDL(&gMasterDisp, SETUPDL_76);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 0, 0, 255);

        if (D_801616BC > 0.0f) {
            var_fv0 = (D_801616BC - gBossHealthBar) * (2.76f / D_801616BC);
            if (var_fv0 > 2.76f) {
                var_fv0 = 2.76f;
            }
            if (var_fv0 < 0.00f) {
                var_fv0 = 0.00f;
            }
            Math_SmoothStepToF(&D_801616C8, var_fv0, 0.2f, 1000.0f, 0.01f);
        } else {
            D_801616C8 = 2.76f;
        }

        if (D_801616C4 >= 0.88f) {
            temp6 = sp3C + 8.0f;
            temp7 = 101.0f - ((2200.0f / 69.0f) * D_801616C8) + temp1;
            if (D_801616C8 > 0.0f) {
                Lib_TextureRect_RGBA16(&gMasterDisp, D_Tex_800D99F8, 32, 32, temp6, temp7, 0.2f, D_801616C8);
            }
        }
    } else {
        D_801616C0 = 0.0f;
        D_801616C4 = 0.0f;
        D_801616C8 = 0.0f;
        D_801616BC = -1.0f;
    }

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
}
#endif

#if 1 // Hud_LivesCount2_Draw (player)
extern u8 aAwArwingLifeIconTex[];
extern u8 aBlueMarineLifeIconTex[];
extern u8 aLandmasterLifeIconTex[];
extern u16 aAwArwingLifeIconTLUT[];
extern u16 aBlueMarineLifeIconTLUT[];
extern u16 aLandmasterLifeIconTLUT[];
extern u8 aXTex[];
extern u16 aXTLUT[];

RECOMP_PATCH void HUD_LivesCount2_Draw(f32 x, f32 y, s32 number) {
    u8* sLivesCounterTexs[] = { aAwArwingLifeIconTex, aBlueMarineLifeIconTex, aLandmasterLifeIconTex };
    u16* sLivesCounterTLUTs[] = { aAwArwingLifeIconTLUT, aBlueMarineLifeIconTLUT, aLandmasterLifeIconTLUT };
    Player* player = &gPlayer[0];
    f32 x0;
    f32 x1;
    f32 x2;
    f32 y0;
    f32 y1;
    f32 y2;
    s32 var_v1;
    s32 i;
    s32 form;

    if (gPlayer[0].state != PLAYERSTATE_LEVEL_COMPLETE) {
        gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                        -(SCREEN_WIDTH) * 4, 0);
        gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, -SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4);
    }

    RCP_SetupDL(&gMasterDisp, SETUPDL_78);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

    form = FORM_ARWING;

    switch (player->form) {
        case FORM_ARWING:
            form = FORM_ARWING;
            break;

        case FORM_LANDMASTER:
            form = FORM_LANDMASTER;
            break;

        case FORM_BLUE_MARINE:
            form = FORM_BLUE_MARINE;
            break;

        default:
            break;
    }

    x0 = x;
    y0 = y;
    x1 = x0 + 16.0f;
    y1 = y0 + 9.0f;
    x2 = x1 + 11.0f;
    y2 = y1 - 1.0f;

    if (number < 0) {
        number = 0;
    }
    if (number > 99) {
        number = 99;
    }

    var_v1 = 10;
    for (i = 1; var_v1 <= number; i++) {
        var_v1 *= 10;
    }
    x2 += (2 - i) * 4;

    Lib_TextureRect_CI4(&gMasterDisp, sLivesCounterTexs[form], sLivesCounterTLUTs[form], 16, 16, x0, y0, 1.0f, 1.0f);
    Lib_TextureRect_CI4(&gMasterDisp, aXTex, aXTLUT, 16, 7, x1, y1, 1.0f, 1.0f);

    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplayHUDNumber(x2, y2, number);

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
    gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_NONE, 0, 0);
}
#endif

#if 1 // Boost Bar

extern f32 D_800D19AC[];

RECOMP_PATCH void HUD_BoostGauge_Draw(f32 arg0, f32 arg1) {
    f32 D_800D2108[] = {
        110.0f, 270.0f, 110.0f, 270.0f, 0.0f,
    };
    f32 D_800D211C[] = {
        16.0f, 16.0f, 136.0f, 136.0f, 0.0f,
    };
    f32 temp_fv0;
    f32 temp2;
    f32 temp_fs0;
    f32 sp68;
    f32 temp;
    f32 sp60;
    s32 temp_t9;
    s32 var_v1;

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0, -(SCREEN_WIDTH) * 4,
                    0);
    gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0);

    if (gVersusMode) {
        temp = 2.0f;
        sp60 = 1.0f;
        var_v1 = gPlayerNum;
        sp68 = 24.0f;
    } else {
        temp = 2.0f;
        sp60 = 2.0f;
        sp68 = 40.0f;
        var_v1 = 4;
        D_800D2108[var_v1] = arg0;
        D_800D211C[var_v1] = arg1;
    }

    temp_fs0 = gPlayer[gPlayerNum].boostMeter * (1.0f / 90.0f);

    temp_fv0 = 1.0f - temp_fs0;
    temp2 = sp68 * temp_fs0;

    if (!gPlayer[gPlayerNum].boostCooldown) {
        Math_SmoothStepToF(&D_800D19AC[var_v1], 255.0f, 0.4f, 100.0f, 0.01f);
    } else {
        Math_SmoothStepToF(&D_800D19AC[var_v1], 100.0f, 0.4f, 100.0f, 0.01f);
    }

    temp_t9 = D_800D19AC[var_v1];

    RCP_SetupDL(&gMasterDisp, SETUPDL_78);

    gDPSetPrimColor(gMasterDisp++, 0, 0, temp_t9, temp_t9, temp_t9, 255);

    if (temp_fv0 > 0.01f) {
        HUD_BoostGaugeCool_Draw(D_800D2108[var_v1] + temp + temp2, D_800D211C[var_v1] + sp60, temp_fv0, 1.0f);
    }
    if (temp_fs0 > 0.01f) {
        HUD_BoostGaugeOverheat_Draw(D_800D2108[var_v1] + temp + 1.0f, D_800D211C[var_v1] + sp60, temp_fs0, 1.0f);
    }

    RCP_SetupDL(&gMasterDisp, SETUPDL_76);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
    HUD_BoostGaugeFrame_Draw(D_800D2108[var_v1], D_800D211C[var_v1], 1.0f, 1.0f);

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
    gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_NONE, 0, 0);
}
#endif

#if 1 // Hud_BombCounter_Draw

void HUD_VS_X_Draw(f32 arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4);
void HUD_VS_LivesCount_Draw(f32 arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

extern s32 sBombCounterState;
extern s32 sHudBombCount;
extern f32 D_800D19F0;
extern s32 D_80161770;
extern s32 D_80161774;
extern s32 D_80161778;
extern s32 D_8016177C;
extern s32 D_80161780;
extern s32 D_80161784;
extern s32 D_8016177C;
extern u8 aVsBombIconTex[];
extern u16 aVsBombIconTLUT[];
extern u8 aBlueMarineBombCountTex[];
extern u16 aBlueMarineBombCountTLUT[];

RECOMP_PATCH void HUD_BombCounter_Draw(f32 x, f32 y) {
    s32 i;
    s32 sp68;
    f32 temp_fv0;
    f32 temp;

    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0, -(SCREEN_WIDTH) * 4,
                    0);
    gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0);

    if (gBombCount[gPlayerNum] > 9) {
        gBombCount[gPlayerNum] = 9;
    }

    switch (sBombCounterState) {
        case 0:
            sHudBombCount = gBombCount[gPlayerNum];
            sp68 = 0;
            sBombCounterState = 1;
            D_80161770 = D_80161774 = D_80161778 = 255.0f;
            D_8016177C = D_80161780 = D_80161784 = 255.0f;
            break;

        case 1:
            temp_fv0 = gBombCount[gPlayerNum] - sHudBombCount;
            if (temp_fv0 > 0.0f) {
                sHudBombCount++;
                if (sHudBombCount > 5) {
                    sp68 = 0;
                    break;
                }
                if (sHudBombCount == 5) {
                    D_800D19F0 = 0.0f;
                    sp68 = 4;
                    sBombCounterState = 6;
                    break;
                } else {
                    D_800D19F0 = 0.0f;
                    sp68 = 3;
                    sBombCounterState = 5;
                }
                break;
            }

            if (temp_fv0 < 0.0f) {
                sBombCounterState = 2;
            }
            sp68 = 0;
            break;

        case 2:
            sHudBombCount--;
            sp68 = 0;
            sBombCounterState = 1;
            if (sHudBombCount <= 3) {
                D_800D19F0 = 0.0f;
                sp68 = 2;
                sBombCounterState = 4;
                break;
            }

            if (sHudBombCount == 4) {
                D_800D19F0 = 0.0f;
                sp68 = 1;
                sBombCounterState = 3;
                break;
            }
            break;

        case 3:
            sp68 = 1;
            if (Math_SmoothStepToF(&D_800D19F0, 10.0f, 0.3f, 10.0f, 0.1f) == 0.0f) {
                sBombCounterState = 1;
                D_800D19F0 = 10.0f;
                sp68 = 1;
            }
            break;

        case 4:
            sp68 = 2;
            if (Math_SmoothStepToF(&D_800D19F0, 10.0f, 0.3f, 10.0f, 0.1f) == 0.0f) {
                sBombCounterState = 1;
                D_800D19F0 = 10.0f;
                sp68 = 2;
            }
            break;

        case 5:
            sp68 = 3;
            if (Math_SmoothStepToF(&D_800D19F0, 20.0f + ((sHudBombCount - 1) * 10), 0.3f, 10.0f, 0.001f) == 0.0f) {
                sBombCounterState = 1;
                D_800D19F0 = 20.0f + ((sHudBombCount - 1) * 10);
                sp68 = 3;
            }
            break;

        case 6:
            sp68 = 4;
            if (Math_SmoothStepToF(&D_800D19F0, 10.0f, 0.3f, 10.0f, 0.1f) == 0.0f) {
                sBombCounterState = 7;
                D_800D19F0 = 0.0f;
                sp68 = 5;
            }
            break;

        case 7:
            sp68 = 5;
            if (0.0f == Math_SmoothStepToF(&D_800D19F0, 12.0f, 0.3f, 10.0f, 5.0f)) {
                sBombCounterState = 1;
                D_800D19F0 = 0.0f;
                sp68 = 0;
            }
            break;

        default:
            break;
    }

    if (gPlayerShots[15].obj.status == SHOT_FREE) {
        D_80161770 = D_80161774 = D_80161778 = 255.0f;
        D_8016177C = D_80161780 = 255.0f;
        D_80161784 = 0.0f;
    } else {
        D_80161770 = 100.0f;
        D_80161774 = D_80161778 = 0.0f;
        D_8016177C = D_80161780 = 40.0f;
        D_80161784 = 0.0f;
    }

    RCP_SetupDL_78();
    gDPSetPrimColor(gMasterDisp++, 0, 0, (s32) D_80161770, (s32) D_80161774, (s32) D_80161778, D_hud_80161708);

    if (gCurrentLevel == LEVEL_AQUAS) {
        sp68 = 6;
    }

    switch (sp68) {
        case 0:
            if (sHudBombCount >= 5) {
                Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x, y, 1.0f, 1.0f);
                HUD_VS_X_Draw(x + 14.0f, y + 2.0f, D_8016177C, D_80161780, D_80161784);
                HUD_VS_LivesCount_Draw(x + 29.0f, y + 1.0f, sHudBombCount, D_8016177C, D_80161780, D_80161784);
            } else {
                for (i = (sHudBombCount - 1); i >= 0; i--) {
                    Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + (30.0f - (i * 10)),
                                        y, 1.0f, 1.0f);
                }
            }
            break;

        case 1:
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + (D_800D19F0 * 3.0f), y, 1.0f,
                                1.0f);
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + (D_800D19F0 * 2.0f), y, 1.0f,
                                1.0f);
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + (D_800D19F0 * 1.0f), y, 1.0f,
                                1.0f);
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + (D_800D19F0 * 0.0f), y, 1.0f,
                                1.0f);
            break;

        case 2:
            for (i = sHudBombCount; i >= 0; i--) {
                if (i == 0) {
                    temp = D_800D19F0 * 4.0f;
                } else {
                    temp = D_800D19F0;
                }
                Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16,
                                    x + (30.0f - (i * 10)) + temp, y, 1.0f, 1.0f);
            }
            break;

        case 3:
            for (i = (sHudBombCount - 2); i >= 0; i--) {
                Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + (30.0f - (i * 10)), y,
                                    1.0f, 1.0f);
            }
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + 50.0f - D_800D19F0, y, 1.0f,
                                1.0f);
            break;

        case 4:
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + 30.0f - (D_800D19F0 * 3.0f),
                                y, 1.0f, 1.0f);
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + 20.0f - (D_800D19F0 * 2.0f),
                                y, 1.0f, 1.0f);
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x + 10.0f - (D_800D19F0 * 1.0f),
                                y, 1.0f, 1.0f);
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x, y, 1.0f, 1.0f);
            break;

        case 5:
            HUD_VS_X_Draw(x + D_800D19F0, y + 2.0f, D_8016177C, D_80161780, D_80161784);
            HUD_VS_LivesCount_Draw(x + 15.0f + D_800D19F0, y + 1.0f, sHudBombCount, D_8016177C, D_80161780, D_80161784);
            Lib_TextureRect_CI4(&gMasterDisp, aVsBombIconTex, aVsBombIconTLUT, 16, 16, x, y, 1.0f, 1.0f);
            break;

        case 6:
            RCP_SetupDL(&gMasterDisp, SETUPDL_78);
            gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
            Lib_TextureRect_CI4(&gMasterDisp, aBlueMarineBombCountTex, aBlueMarineBombCountTLUT, 32, 9, x + 1.0f, y,
                                1.0f, 1.0f);
            break;
    }
    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
    gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_NONE, 0, 0);
}
#endif

#if 1
RECOMP_PATCH void HUD_EdgeArrows_Draw(s32 idx, bool arg1) {
    static const f32 D_800D1EF8[] = { 0.0f, 0.0f, -9.0f, 9.0f, 10.0f, 10.0f, 10.0f, 10.0f, 0.0f, 0.0f, -8.0f, 8.0f };
    static const f32 D_800D1F28[] = { -7.0f, 7.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f, -8.0f, 0.0f, 0.0f };
    static const f32 D_800D1F58[] = { -22.0f, -22.0f, -22.0f, -22.0f, -28.0f, -28.0f,
                                      -28.0f, -28.0f, -28.0f, -28.0f, -28.0f, -28.0f };
    static const f32 D_800D1F88[] = { 0.0f, 0.0f, 0.0f, 0.0f, 495.0f, 405.0f, 585.0f, 675.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    static const f32 D_800D1FB8[] = { 180.0f, 0.0f,   270.0f, 90.0f,  270.0f, 270.0f,
                                      270.0f, 270.0f, 0.0f,   180.0f, 90.0f,  270.0f };
    static const f32 D_800D1FE8[] = { 0.0f, 0.0f, 2.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f, 0.0f, 0.0f, 2.0f, -2.0f };
    static const f32 D_800D2018[] = { 2.0f, -2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -2.0f, 2.0f, 0.0f, 0.0f };

    Matrix_Push(&gGfxMatrix);

    if (gPlayer[0].alternateView) {
        Matrix_RotateZ(gGfxMatrix, M_DTOR * gPlayer[0].camRoll, MTXF_APPLY);
    }

    if (D_800D1F88[idx]) {
        Matrix_RotateZ(gGfxMatrix, M_DTOR * D_800D1F88[idx], MTXF_APPLY);
    }

    // Simplified viewport alignment based on X position
    f32 xPos = D_800D1EF8[idx];

    if (xPos < 0.0f) {
        // Align viewport to the left if X is negative
        gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, 0, 0);
    } else if (xPos > 0.0f) {
        // Align viewport to the right if X is positive
        gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, -SCREEN_WIDTH * 4, 0);
    }
    // Otherwise, no special alignment (centered)

    if (arg1) {
        Matrix_Translate(gGfxMatrix, D_800D1EF8[idx] + D_800D1FE8[idx], D_800D1F28[idx] + D_800D2018[idx],
                         D_800D1F58[idx], MTXF_APPLY);
    } else {
        Matrix_Translate(gGfxMatrix, D_800D1EF8[idx], D_800D1F28[idx], D_800D1F58[idx], MTXF_APPLY);
    }

    gSPViewport(gMasterDisp++, gViewport);

    Matrix_RotateZ(gGfxMatrix, M_DTOR * D_800D1FB8[idx], MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 0.026f, 0.026f, 0.026f, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);

    RCP_SetupDL(&gMasterDisp, SETUPDL_62);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 150);
    gSPDisplayList(gMasterDisp++, aArrowDL);

    Matrix_Pop(&gGfxMatrix);
}
#endif

#if 1 // Radar

void HUD_VS_DrawHUD(void);
s32 HUD_RadarMarks_Update(void);

RECOMP_PATCH void HUD_Radar(void) {
    if (!gVersusMode) {
        gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                        -(SCREEN_WIDTH) * 4, 0);
        gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, -SCREEN_WIDTH * 4, 0);
        gSPViewport(gMasterDisp++, gViewport);
    }

    if (gVersusMode) {
        HUD_VS_DrawHUD();
    }
    HUD_RadarMarks_Update();

    if (!gVersusMode) {
        gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_NONE, SCREEN_WIDTH, SCREEN_HEIGHT);
        gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
    }
}
#endif

#if 0
void HUD_MsgWindowBg_Draw(f32 xPos, f32 yPos, f32 xScale, f32 yScale);
void HUD_RadarWindowFrame_Draw(f32 xPos, f32 yPos, f32 xScale, f32 yScale);

RECOMP_PATCH void HUD_RadarWindow_Draw(f32 x, f32 y) {
    f32 sVsRadarWindowXpos[] = { 20.0f, 180.0f, 20.0f, 180.0f };
    f32 sVsRadarWindowYpos[] = { 72.0f, 72.0f, 192.0f, 192.0f };
    f32 xPos;
    f32 yPos;
    f32 xScale;
    f32 yScale;
    f32 xScale1;
    f32 yScale1;

    if (gCamCount != 1) { // Versus mode
        xPos = sVsRadarWindowXpos[gPlayerNum];
        yPos = sVsRadarWindowYpos[gPlayerNum];
        xScale = 1.21f;
        yScale = 1.69f;
        xScale1 = 0.70f;
        yScale1 = 0.70f;
    } else { // Simple player mode
        xPos = x - 32.0f;
        yPos = y - 14.0f;
        xScale = 2.98f;
        yScale = 4.24f;
        xScale1 = 1.70f;
        yScale1 = 1.70f;
    }
    recomp_printf("Orig xPos: %f\n", xPos);
    f32 vpAspect = ((f32) gViewport->vp.vscale[0] / 2.0f) / ((f32) gViewport->vp.vscale[1] / 2.0f);
    recomp_printf("viewport aspect ratio: %f\n", vpAspect);
    xPos = xPos * vpAspect;
    recomp_printf("xPos: %f\n", xPos);
    RCP_SetupDL(&gMasterDisp, SETUPDL_78);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 60, 60, 255, 170);
    HUD_MsgWindowBg_Draw(xPos + 1.0f, yPos + 1.0f, xScale, yScale);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
    HUD_RadarWindowFrame_Draw(xPos, yPos, xScale1, yScale1);
}
#endif

#if 0
RECOMP_PATCH s32 HUD_RadarMarks_Update(void) {
    s32 i;
    f32 scale;
    f32 x1 = 0;
    f32 y1 = 0;
    f32 z1 = 0;
    f32 x = 0;
    f32 y = 0;
    s32 pad;
    f32 temp;
    f32 temp2;
    f32 temp3;

    if (!gVersusMode) {
        if (gLevelMode != LEVELMODE_ALL_RANGE) {
            return 0;
        }

        if (gDrawBackdrop >= 5) {
            return 0;
        }

        if (gLevelStartStatusScreenTimer != 0) {
            D_800D1E10 = 60.0f;
        } else {
            Math_SmoothStepToF(&D_800D1E10, 0.0f, 0.3f, 10.0f, 0.1f);
        }

        if (D_800D1E10 == 60.0f) {
            return 0;
        }

        switch (gCurrentLevel) {
            case LEVEL_SECTOR_Z:
                temp2 = 20000.0f;
                y1 = -360.0f;
                x1 = 542.0f;
                z1 = -1584.0f;
                temp3 = 7.5f;
                scale = 0.02f;
                break;

            case LEVEL_CORNERIA:
                temp2 = 8000.0f;
                y1 = -142.0f;
                x1 = 214.0f;
                z1 = -626.0f;
                temp3 = 3.0f;
                scale = 0.008f;
                break;

            case LEVEL_BOLSE:
                temp2 = 10000.0f;
                y1 = -178.0f;
                x1 = 268.0f;
                z1 = -784.0f;
                temp3 = 3.7f;
                scale = 0.01f;
                break;

            default:
                temp2 = 12500.0f;
                y1 = -220.0f;
                x1 = 330.0f;
                z1 = -970.0f;
                temp3 = 4.7f;
                scale = 0.013f;
                break;
        }

        x = 254.000f + D_800D1E10;
        y = 162.000f;
        x1 += D_800D1E10 * temp3;
    } else { // Versus mode
        if (!gVsMatchStart || gVsMatchOver) {
            return 0;
        }
        temp2 = 13000.00f;

        scale = 0.03f;
        z1 = -885.00f;
        x1 = -274.00f;
        y1 = -166.00f;
    }

    HUD_RadarMarks_Setup();
    HUD_RadarWindow_Draw(x, y);

    if (!gVersusMode &&
        ((gCurrentLevel == LEVEL_SECTOR_Z) || (gCurrentLevel == LEVEL_FORTUNA) || (gCurrentLevel == LEVEL_VENOM_2) ||
         (gCurrentLevel == LEVEL_BOLSE) || (gCurrentLevel == LEVEL_SECTOR_Y) || (gCurrentLevel == LEVEL_KATINA))) {

        RCP_SetupDL(&gMasterDisp, SETUPDL_76);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 128, 128, 128, 255);

        switch (gCurrentLevel) {
            case LEVEL_SECTOR_Z:
                Lib_TextureRect_IA8(&gMasterDisp, D_SZ_60012D0, 16, 9, 251.0f + D_800D1E10, 181.0f, 1.00f, 1.00f);
                break;

            case LEVEL_FORTUNA:
                Lib_TextureRect_IA8(&gMasterDisp, D_FO_6001260, 16, 16, 251.0f + D_800D1E10, 178.0f, 1.00f, 1.00f);
                break;

            case LEVEL_BOLSE:
                Lib_TextureRect_IA8(&gMasterDisp, D_BO_6000C80, 16, 16, 251.0f + D_800D1E10, 178.0f, 1.00f, 1.00f);
                break;

            case LEVEL_SECTOR_Y:
                if ((fabsf(gScenery360[0].obj.pos.x) < temp2 + 1000.0f) &&
                    (fabsf(gScenery360[0].obj.pos.z) < temp2 + 1000.0f)) {
                    temp = 150.0f + ((12500.0f + gScenery360[0].obj.pos.z) / 446.42f);

                    if ((y < 150.0f) || (y > 206.0f)) {
                        break;
                    }
                    Lib_TextureRect_IA8(&gMasterDisp, D_SY_6000840, 64, 64, 250.0f + D_800D1E10, temp, 0.25f, 0.25f);
                }
                break;

            case LEVEL_KATINA:
                Lib_TextureRect_IA8(&gMasterDisp, D_KA_6001260, 8, 8, 254.0f + D_800D1E10, 182.0f, 1.00f, 1.00f);
                break;

            case LEVEL_VENOM_2:
                Lib_TextureRect_IA8(&gMasterDisp, D_VE2_6002890, 16, 16, 251.0f + D_800D1E10, 178.0f, 1.00f, 1.00f);
                break;
        }
    }

    Matrix_Push(&gGfxMatrix);

    if (gVersusMode) {
        f32 tempX = ((f32) gViewport->vp.vscale[0] / 2.0f) / ((f32) gViewport->vp.vscale[1] / 2.0f);
        // recomp_printf("Orig X1 pos: %f\n", x1);
        // x1 = x1 * +tempX;
        Matrix_Translate(gGfxMatrix, x1, y1, z1, MTXF_APPLY);
        // recomp_printf("X1 pos: %f\n", x1);
    } else {
        Matrix_Translate(gGfxMatrix, x1, y1, z1, MTXF_APPLY);
    }

    if ((gCurrentLevel == LEVEL_SECTOR_Z) && (gRadarMissileAlarmTimer != 0)) {
        Matrix_Push(&gGfxMatrix);
        HUD_RadarMissileAlarm_Draw();
        Matrix_Pop(&gGfxMatrix);
        gRadarMissileAlarmTimer--;
    }

    for (i = ARRAY_COUNT(gRadarMarks) - 1; i >= 0; i--) {
        if ((gRadarMarks[i].enabled == 0) || (fabsf(gRadarMarks[i].pos.x) >= (temp2 + 1000.0f)) ||
            (fabsf(gRadarMarks[i].pos.z) >= (temp2 + 1000.0f))) {
            continue;
        }

        Matrix_Push(&gGfxMatrix);
        Matrix_Translate(gGfxMatrix, gRadarMarks[i].pos.x * 0.008f, -gRadarMarks[i].pos.z * 0.008f, 0.0f, MTXF_APPLY);

        if (gRadarMarks[i].type == 103) {
            gRadarMarks[i].yRot = 45.0f;
        }

        Matrix_RotateZ(gGfxMatrix, M_DTOR * gRadarMarks[i].yRot, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, scale, scale, 1.0f, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);

        HUD_RadarMark_Draw(gRadarMarks[i].type);
        Matrix_Pop(&gGfxMatrix);

        gRadarMarks[i].enabled = false;
    }

    Matrix_Pop(&gGfxMatrix);
    return 0;
}
#endif

#if 1 // Team HELP! Arrows

extern Gfx D_1023700[];

RECOMP_PATCH void Display_DrawHelpAlert(void) {
    bool sp7C;
    f32 sp78;
    f32 sp74;
    Vec3f sp68;
    Vec3f sp5C;

    if ((gPlayState == PLAY_PAUSE) || (gTeamHelpActor == NULL)) {
        return;
    }

    if ((gTeamHelpActor->obj.status != OBJ_ACTIVE) || (gPlayer[0].state != PLAYERSTATE_ACTIVE)) {
        gTeamHelpActor = NULL;
        gTeamHelpTimer = 0;
        return;
    }

    if (gTeamHelpTimer != 0) {
        gTeamHelpTimer--;
        if (gTeamHelpTimer == 0) {
            gTeamHelpActor = NULL;
            return;
        }
    }

    if ((gTeamHelpTimer & 4) == 0) {
        Matrix_RotateY(gCalcMatrix, gPlayer[0].camYaw, MTXF_NEW);
        Matrix_RotateX(gCalcMatrix, gPlayer[0].camPitch, MTXF_APPLY);

        sp68.x = gTeamHelpActor->obj.pos.x - gPlayer[0].cam.eye.x;
        sp68.y = gTeamHelpActor->obj.pos.y - gPlayer[0].cam.eye.y;
        sp68.z = gTeamHelpActor->obj.pos.z + gPathProgress - gPlayer[0].cam.eye.z;

        Matrix_MultVec3f(gCalcMatrix, &sp68, &sp5C);

        sp7C = false;
        if ((sp5C.z < 0.0f) && (sp5C.z > -12000.0f) && (fabsf(sp5C.x) < fabsf(sp5C.z * 0.4f))) {
            sp7C = true;
        }

        RCP_SetupDL(&gMasterDisp, SETUPDL_12);

        switch (gTeamHelpActor->aiType) {
            case AI360_PEPPY:
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 30, 0, 255);
                break;
            case AI360_SLIPPY:
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 00, 179, 67, 255);
                break;
            case AI360_FALCO:
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 30, 30, 255, 255);
                break;
        }

        switch (sp7C) {
            case false:
                if (gTeamHelpActor->sfxSource[0] > 0.0f) {
                    sp78 = 20.0f;
                    sp74 = M_PI / 2;

                    // Align to right (since sp78 is positive)
                    gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, -SCREEN_WIDTH * 4, 0);
                    gSPViewport(gMasterDisp++, gViewport);
                } else {
                    sp78 = -20.0f;
                    sp74 = -M_PI / 2;

                    // Align to left (since sp78 is negative)
                    gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, 0, 0);
                    gSPViewport(gMasterDisp++, gViewport);
                }
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, sp78, 0.0f, -50.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, sp74, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.03f, 0.03f, 0.03f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, D_1023700);
                Matrix_Pop(&gGfxMatrix);
                break;

            case true:
                // Default case: align to both sides (up and down arrows, neutral position)

                // Draw the right arrow
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, 20.0f, 0.0f, -50.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, -M_PI / 2, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.03f, 0.03f, 0.03f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, -SCREEN_WIDTH * 4, 0);
                gSPViewport(gMasterDisp++, gViewport);
                gSPDisplayList(gMasterDisp++, D_1023700);
                Matrix_Pop(&gGfxMatrix);

                // Draw the left arrow
                sp78 = -20.0f;
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, -20.0f, 0.0f, -50.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, M_PI / 2, MTXF_APPLY);
                Matrix_Scale(gGfxMatrix, 0.03f, 0.03f, 0.03f, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, 0, 0);
                gSPViewport(gMasterDisp++, gViewport);
                gSPDisplayList(gMasterDisp++, D_1023700);
                Matrix_Pop(&gGfxMatrix);
                break;
        }

        gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_NONE, 0, 0);
        gSPViewport(gMasterDisp++, gViewport);

        switch (sp7C) {
            case false:
                RCP_SetupDL(&gMasterDisp, SETUPDL_76);
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 0, 255);
                if (sp78 < 0.0f) {
                    // Align help message to left
                    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);
                    Graphics_DisplaySmallText(43 - 19, 106, 1.0f, 1.0f, "HELP!!");
                    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
                } else {
                    // Align help message to right
                    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                                    -(SCREEN_WIDTH) * 4, 0);
                    Graphics_DisplaySmallText(SCREEN_WIDTH - 43 - 19, 106, 1.0f, 1.0f, "HELP!!");
                    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
                }
                break;

            case true:
                RCP_SetupDL(&gMasterDisp, SETUPDL_76);
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 0, 255);
                // Display help on both sides
                gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);
                Graphics_DisplaySmallText(43 - 19, 106, 1.0f, 1.0f, "HELP!!");
                gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                                -(SCREEN_WIDTH) * 4, 0);
                Graphics_DisplaySmallText(SCREEN_WIDTH - 43 - 19, 106, 1.0f, 1.0f, "HELP!!");
                gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
                break;
        }
    }
}
#endif

#if 1 // Radio and text

void Audio_PlayVoice(s32 msgId);
void Audio_ClearVoice(void);
s32 Audio_GetCurrentVoice(void);
s32 Audio_GetCurrentVoiceStatus(void);
void func_radio_800BAAE8(void);
void func_radio_800BA760(void);

s32 D_radio_80178748; // set to 1, never used
s32 sRadioCheckMouthFlag;

RECOMP_PATCH void Radio_Draw(void) {
    s32 idx;
    RadioCharacterId radioCharId;
    u32 ret;
    s32 fakeTemp;

    Game_InitFullViewport();
    if ((gGameState != GSTATE_ENDING) && (recomp_get_radio_comm_box_mode() == 1)) {
        gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);
        gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, 0, 0);
        gSPViewport(gMasterDisp++, gViewport);
    }

    if ((gPlayState == PLAY_PAUSE) && (gGameState != GSTATE_ENDING)) {
        return;
    }

    if (gRadioStateTimer > 0) {
        gRadioStateTimer--;
    }

    if (gRadioMouthTimer > 0) {
        gRadioMouthTimer--;
    }

    switch (gRadioState) {
        case 100:
            D_radio_80178748 = 1;
            gCurrentRadioPortrait = RCID_1000;
            gRadioState = 1;
            gRadioMsgCharIndex = 0;
            gRadioPortraitScaleY = 0.0f;
            gRadioTextBoxScaleY = 0.0f;
            sRadioCheckMouthFlag = 0;
            if (gCamCount != 1) {
                gRadioState = 0;
            }
            break;

        case 1:
            gRadioPortraitScaleY += 0.25f;
            if (gRadioPortraitScaleY == 1.0f) {
                gRadioState++;
                gRadioStateTimer = 10;
            }
            gCurrentRadioPortrait = RCID_STATIC;
            if ((gGameFrameCount % 2) != 0) {
                gCurrentRadioPortrait = RCID_STATIC + 1;
            }
            break;

        case 2:
            if (gRadioStateTimer == 0) {
                gRadioState++;
                gRadioStateTimer = 10;
            }
            gCurrentRadioPortrait = RCID_STATIC;
            if ((gGameFrameCount % 2) != 0) {
                gCurrentRadioPortrait = RCID_STATIC + 1;
            }
            break;

        case 3:
            if (gRadioStateTimer == 0) {
                gRadioState++;
                ret = Message_GetWidth(gRadioMsg);
                if (gVIsPerFrame == 3) {
                    gRadioStateTimer = ret + 16;
                } else {
                    gRadioStateTimer = (2 * ret) + 16;
                }
                if ((gGameState == GSTATE_TITLE) || (gGameState == GSTATE_ENDING)) {
                    gRadioStateTimer = ret * 2;
                }
            }
            gCurrentRadioPortrait = (s32) gRadioMsgRadioId;
            gRadioTextBoxScaleY += 0.26f;
            if (gRadioTextBoxScaleY > 1.3f) {
                gRadioTextBoxScaleY = 1.3f;
            }
            break;

        case 31:
            gRadioState++;
            gRadioStateTimer = 80 - gRadioStateTimer;
            break;

        case 32:
            if (Audio_GetCurrentVoice() == 0) {
                gRadioMsgListIndex++;
                gRadioMsg = gRadioMsgList[gRadioMsgListIndex];
                Audio_PlayVoice(Message_IdFromPtr(gRadioMsg));
                gRadioMsgCharIndex = 0;
                sRadioCheckMouthFlag = 0;
                gRadioStateTimer = 80;
                gRadioStateTimer = Message_GetWidth(gRadioMsg) * 2;
                gRadioState = 4;
            }
            break;

        case 4:
            if ((Audio_GetCurrentVoice() == 0) && (gRadioStateTimer == 0)) {
                gRadioStateTimer = 10;
                gCurrentRadioPortrait = (s32) gRadioMsgRadioId;
                gRadioState = 6;
            }
            gCurrentRadioPortrait = (s32) gRadioMsgRadioId;
            if (gRadioMouthTimer > 0) {
                gCurrentRadioPortrait = (s32) gRadioMsgRadioId + 1;
            }

            if (!gVIsPerFrame) {}

            if (1) {
                fakeTemp = 0;
            }

            if (!(fakeTemp)) {
                ret = Audio_GetCurrentVoiceStatus();

                if (gRadioMsgCharIndex < 60) {
                    if (gRadioMsg[gRadioMsgCharIndex + 1] == MSGCHAR_NXT) {
                        if (ret == 0) {
                            gRadioState = 31;
                        }
                    } else {
                        gRadioMsgCharIndex++;
                    }
                }

                if (sRadioCheckMouthFlag) {
                    if ((gRadioMsgId >= 23000) && (gRadioMsgId < 23033)) {
                        if (gMsgCharIsPrinting) {
                            gRadioMouthTimer = 2;
                            AUDIO_PLAY_SFX(NA_SE_MESSAGE_MOVE, gDefaultSfxSource, 4);
                        }
                    } else if (ret == 1) {
                        gRadioMouthTimer = 2;
                    } else {
                        gRadioMouthTimer = 0;
                    }
                }
            }
            sRadioCheckMouthFlag ^= 1;
            break;

        case 5:
            if (gRadioStateTimer == 0) {
                gRadioState++;
                gRadioStateTimer = 10;
            }
            gCurrentRadioPortrait = (s32) gRadioMsgRadioId;
            break;

        case 6:
            if (gRadioStateTimer == 0) {
                if (gGameState == GSTATE_ENDING) {
                    Audio_ClearVoice();
                } else {
                    Audio_PlayVoice(0);
                }
                gRadioState++;
            }
            gCurrentRadioPortrait = RCID_STATIC;
            if ((gGameFrameCount % 2) != 0) {
                gCurrentRadioPortrait = RCID_STATIC + 1;
            }
            gRadioTextBoxScaleY -= 0.26f;
            if (gRadioTextBoxScaleY < 0.0f) {
                gRadioTextBoxScaleY = 0.0f;
            }
            break;

        case 7:
            gRadioPortraitScaleY -= 0.25f;
            if (gRadioPortraitScaleY == 0) {
                gHideRadio = false;
                gRadioMsgPri = 0;
                gRadioState = 0;
            }
            gCurrentRadioPortrait = RCID_STATIC;
            if ((gGameFrameCount % 2) != 0) {
                gCurrentRadioPortrait = RCID_STATIC + 1;
            }
            break;

        case 8:
            gCurrentRadioPortrait = (s32) gRadioMsgRadioId;
            gRadioTextBoxScaleY = 1.3f;
            gRadioPortraitScaleY = 1.0f;
            break;

        case 0:
            break;
    }

    if (((gRadioState > 0) && (gRadioState != 100)) && !gHideRadio) {
        func_radio_800BAAE8();
        func_radio_800BB388();

        radioCharId = (s32) gRadioMsgRadioId;

        if (((radioCharId == RCID_FALCO) || (radioCharId == RCID_SLIPPY)) || (radioCharId == RCID_PEPPY)) {
            if (radioCharId == RCID_FALCO) {
                idx = TEAM_ID_FALCO;
            }
            if (radioCharId == RCID_SLIPPY) {
                idx = TEAM_ID_SLIPPY;
            }
            if (radioCharId == RCID_PEPPY) {
                idx = TEAM_ID_PEPPY;
            }
            if ((gTeamShields[idx] <= 0) && (gGameFrameCount & 4) && (gTeamShields[idx] != -2) &&
                (gCurrentRadioPortrait != RCID_STATIC) && (gCurrentRadioPortrait != RCID_STATIC + 1) &&
                (gCurrentRadioPortrait != RCID_1000)) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_76);
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 0, 255);
                Graphics_DisplaySmallText(31, 167, 1.0f, 1.0f, "DOWN");
                HUD_TeamDownWrench_Draw(1);
            }
            if (((gCurrentRadioPortrait != RCID_STATIC) && (gCurrentRadioPortrait != RCID_STATIC + 1)) &&
                (gCurrentRadioPortrait != RCID_1000)) {
                HUD_TeamShields_Draw(22.0f, 165.0f, gTeamShields[idx]);
            }
        }

        radioCharId = (s32) gRadioMsgRadioId;

        if ((radioCharId == RCID_WOLF) || (radioCharId == RCID_PIGMA) || (radioCharId == RCID_LEON) ||
            (radioCharId == RCID_ANDREW) || (radioCharId == RCID_WOLF_2) || (radioCharId == RCID_PIGMA_2) ||
            (radioCharId == RCID_LEON_2) || (radioCharId == RCID_ANDREW_2)) {
            switch (radioCharId) {
                case RCID_WOLF:

                case RCID_WOLF_2:
                    idx = 4;
                    break;

                case RCID_LEON:

                case RCID_LEON_2:
                    idx = 5;
                    break;

                case RCID_PIGMA:

                case RCID_PIGMA_2:
                    idx = 6;
                    break;

                case RCID_ANDREW:

                case RCID_ANDREW_2:
                    idx = 7;
                    break;

                default:
                    idx = 0;
                    break;
            }

            if ((gActors[idx].obj.status != OBJ_ACTIVE) && (gGameFrameCount & 4) &&
                (gPlayer[0].state == PLAYERSTATE_ACTIVE) && (gCurrentRadioPortrait != RCID_STATIC) &&
                (gCurrentRadioPortrait != RCID_STATIC + 1) && (gCurrentRadioPortrait != RCID_1000)) {
                RCP_SetupDL(&gMasterDisp, SETUPDL_76);
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 0, 255);
                Graphics_DisplaySmallText(31, 167, 1.0f, 1.0f, "DOWN");
            }
            if (((gCurrentRadioPortrait != RCID_STATIC) && (gCurrentRadioPortrait != RCID_STATIC + 1)) &&
                (gCurrentRadioPortrait != RCID_1000)) {
                HUD_TeamShields_Draw(22.0f, 165.0f, gActors[idx].health * 2.55f);
            }
        }
        if (((gCurrentRadioPortrait != RCID_STATIC) && (gCurrentRadioPortrait != RCID_STATIC + 1)) &&
            (gCurrentRadioPortrait != RCID_1000)) {
            HUD_RadioCharacterName_Draw();
        }
    }

    if (gHideRadio == true) {
        func_radio_800BA760();
    }
    gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
    gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_NONE, 0, 0);
    gSPViewport(gMasterDisp++, gViewport);
}
#endif

#if 1 // Red Damage

void HUD_RadioDamage_Update(void);
void HUD_RadioDamage_Draw(void);

RECOMP_PATCH void HUD_RadioDamage(void) {
    if (gPlayState != PLAY_PAUSE) {
        HUD_RadioDamage_Update();
        Game_InitFullViewport();
        if (recomp_get_radio_comm_box_mode() == 1) {
            gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, 0, 0);
        } else {
            gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_NONE, 0, 0);
        }
        gSPViewport(gMasterDisp++, gViewport);
        HUD_RadioDamage_Draw();
        gEXSetViewportAlign(gMasterDisp++, G_EX_ORIGIN_NONE, 0, 0);
        gSPViewport(gMasterDisp++, gViewport);
    }
}
#endif

#if 1 // Hit Counter

extern u8 aTextStatusOfTeam[];
extern u8 aTextAccumTotal[];
extern u8 aTextEnemiesDown[];
extern s32 D_801617C0[10];
extern s32 D_801617E8[10];

void HUD_MsgWindowBg_Draw2(f32 xPos, f32 yPos, f32 xScale, f32 yScale);
void HUD_TeammateStatus_Draw(void);

RECOMP_PATCH void HUD_LevelClearStatusScreen_Draw(void) {
    s32 i;
    s32 temp;
    f32 x0;
    f32 x1;
    f32 x2;
    f32 x3;
    f32 x4;
    f32 x5;
    f32 x6;
    f32 y0;
    f32 y1;
    f32 y2;
    f32 y3;
    f32 y4;
    f32 y5;
    f32 y6;

    if (gShowLevelClearStatusScreen == 0) {
        Audio_KillSfxById(NA_SE_TEAM_SHIELD_UP);
        D_801617C0[0] = 0;
    }

    if ((gPlayState != PLAY_PAUSE) && (gShowLevelClearStatusScreen == 1) && (D_801617E8[0] == 0)) {
        switch (D_801617C0[0]) {
            case 0:
                D_801617C0[5] = gHitCount;
                D_801617C0[1] = gHitCount;
                D_801617C0[2] = gTotalHits;
                gTotalHits += gHitCount;
                D_801617C0[3] = gLifeCount[gPlayerNum];
                gLifeCount[gPlayerNum] += ((D_801617C0[2] % 100) + gHitCount) / 100;

                if (gLifeCount[gPlayerNum] > 99) {
                    gLifeCount[gPlayerNum] = 99;
                }

                D_801617E8[0] = 10;
                D_801617C0[0] = 1;
                D_801617C0[4] = gHitCount / 2;
                D_801617C0[6] = 0;

                HUD_SetMissionTeamStatus();
                break;

            case 1:
                if (((gTeamShields[TEAM_ID_FALCO] > 0) && (gTeamShields[TEAM_ID_FALCO] < 255)) &&
                    ((gTeamShields[TEAM_ID_SLIPPY] > 0) && (gTeamShields[TEAM_ID_SLIPPY] < 255)) &&
                    ((gTeamShields[TEAM_ID_PEPPY] > 0) && (gTeamShields[TEAM_ID_PEPPY] < 255))) {
                    AUDIO_PLAY_SFX(NA_SE_TEAM_SHIELD_UP, gDefaultSfxSource, 4);
                }
                D_801617C0[0] = 2;

            case 2:
                if ((D_801617C0[5] == 0) && (D_801617C0[4] == 0)) {
                    Audio_KillSfxById(NA_SE_TEAM_SHIELD_UP);
                    D_801617C0[0] = 3;
                    D_801617E8[0] = 30;
                    break;
                }

                if (D_801617C0[5] > 0) {
                    AUDIO_PLAY_SFX(NA_SE_COUNT_UP, gDefaultSfxSource, 4);

                    if (D_801617C0[5] >= 100) {
                        D_801617C0[5] -= 100;
                        D_801617C0[2] += 100;
                    } else {
                        D_801617C0[5]--;
                        D_801617C0[2]++;
                    }
                }

                if (D_801617C0[4] > 0) {
                    for (i = TEAM_ID_FALCO, temp = 0; i <= TEAM_ID_PEPPY; i++) {
                        if (gTeamShields[i] > 0) {
                            if (D_801617C0[4] >= 4) {
                                gTeamShields[i] += 4;
                            } else {
                                gTeamShields[i]++;
                            }
                            if (gTeamShields[i] >= 255) {
                                gTeamShields[i] = 255;
                            } else {
                                temp++;
                            }
                        }
                    }

                    if (D_801617C0[4] >= 4) {
                        D_801617C0[4] -= 4;
                    } else {
                        D_801617C0[4]--;
                    }

                    if (D_801617C0[4] <= 0) {
                        D_801617C0[i] = 0;
                    }

                    if ((D_801617C0[i] == 0) || (temp == 0)) {
                        D_801617C0[4] = 0;
                        Audio_KillSfxById(NA_SE_TEAM_SHIELD_UP);
                    }
                }
                break;

            case 3:
            case 4:
                if (D_801617C0[3] < gLifeCount[gPlayerNum]) {
                    D_801617C0[6] = 30;
                }
                D_801617C0[0] = 5;

            case 5:
                if (D_801617C0[3] >= gLifeCount[gPlayerNum]) {
                    gLifeCount[gPlayerNum] = D_801617C0[3];
                    D_801617E8[0] = 10;
                    D_801617C0[0]++;
                } else {
                    if (((gGameFrameCount % 2) == 0)) {
                        AUDIO_PLAY_SFX(NA_SE_ONE_UP, gDefaultSfxSource, 4);
                        D_801617C0[3]++;
                    }
                }
                break;

            case 6:
            case 7:
                D_801617C0[0]++;
                D_801617C0[1] = gHitCount;
                break;

            default:
                break;
        }
    }

    if (D_801617C0[6] > 0) {
        D_801617C0[6]--;
    }

    if (gShowLevelClearStatusScreen == 1) {
        x0 = 128.0f;
        y0 = 30.0f;

        x1 = x0 + 8.0f + 4.0f;
        y1 = y0 + 19.0f + 4.0f;

        x2 = x1 + 13.0f;
        y2 = y1 + 18.0f + 6.0f;

        x3 = x0 - 84.0f;
        y3 = y2 + 19.0f;

        x4 = x3 + 103.0f + 24.0f;
        y4 = y3 - 6.0f;

        x5 = x4 + 60.0f;
        y5 = y4 + 10.0f;

        x6 = x2 - 56.0f + 16.0f;
        y6 = y3 + 18.0f;

        HUD_MsgWindowBg_Draw2(x0 - 4.0f, y0 - 4.0f, 2.9f, 3.6f);
        HUD_MsgWindowBg_Draw2(x3 - 4.0f, y3 - 4.0f, 10.0f, 1.0f);
        HUD_MsgWindowBg_Draw2(x6 - 12.0f, y6 + 8.0f, 5.2f, 1.0f);

        if (D_801617C0[1] < 0) {
            D_801617C0[1] = 0;
        }
        if (D_801617C0[1] > 999) {
            D_801617C0[1] = 999;
        }
        if (D_801617C0[2] < 0) {
            D_801617C0[2] = 0;
        }
        if (D_801617C0[2] > 9999) {
            D_801617C0[2] = 9999;
        }

        temp = 10;
        for (i = 1; temp <= D_801617C0[1]; i++) {
            temp *= 10;
        }
        x1 += (3 - i) * 8;

        temp = 10;
        for (i = 1; temp <= D_801617C0[2]; i++) {
            temp *= 10;
        }
        x4 += (4 - i) * 8;

        RCP_SetupDL(&gMasterDisp, SETUPDL_76);

        gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT, 0, 0, 0, 0);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 90, 160, 200, 255);
        HUD_Number_Draw(24.0f, 30.0f + 3.0f, D_801617C0[5], 1.0f, 0, 999);
        gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);

        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
        Lib_TextureRect_IA8(&gMasterDisp, aTextEnemiesDown, 64, 25, x0, y0 + 4.0f, 1.0f, 1.0f);

        HUD_Number_Draw(x1, y1 + 12.0f, D_801617C0[1], 1.0f, 1, 999);

        Lib_TextureRect_IA8(&gMasterDisp, aTextAccumTotal, 128, 10, x3, y3, 1.0f, 1.0f);

        HUD_Number_Draw(x4 + 4.0f, y4 + 3.0f, D_801617C0[2], 1.00f, 1, 9999);

        if ((D_801617C0[6] % 2) == 0) {
            HUD_LivesCount2_Draw(232.0f, 90.0f, D_801617C0[3]);
        }

        RCP_SetupDL(&gMasterDisp, SETUPDL_76);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);

        Lib_TextureRect_IA8(&gMasterDisp, aTextStatusOfTeam, 120, 12, x6 - 8.0f, y6 + 10.0f, 1.0f, 1.0f);

        HUD_TeammateStatus_Draw();
        HUD_TeamDownWrench_Draw(0);
        HUD_PlayerShieldGauge_Draw(20.0f, 18.0f);
    }
}
#endif

#if 1 // Training_RingPassCount_Draw
RECOMP_PATCH void Training_RingPassCount_Draw(void) {
    if (gRingPassCount != 0) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_83);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 255, 255);
        gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_RIGHT, G_EX_ORIGIN_RIGHT, -(SCREEN_WIDTH) * 4, 0,
                        -(SCREEN_WIDTH) * 4, 0);
        HUD_Number_Draw(250.0f, 50.0f, gRingPassCount, 1.0f, 0, 999);
        gEXSetRectAlign(gMasterDisp++, G_EX_ORIGIN_NONE, G_EX_ORIGIN_NONE, 0, 0, 0, 0);
    }
}
#endif

#if 0
RECOMP_PATCH void Ending_80191234(s32 arg0, s32 arg1) {
    gLastGameState = GSTATE_ENDING;
    gGameState = GSTATE_MENU;
    gNextGameStateTimer = 2;
    gOptionMenuStatus = OPTION_WAIT;
    gDrawMode = DRAW_NONE;
    gBgColor = 0;
    gStarCount = 0;
    gControllerLock = 10;

    // Set up rendering state for opaque black rectangles
    gDPSetRenderMode(gMasterDisp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCombineMode(gMasterDisp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 0, 0, 0, 255);

    // Draw left black rectangle
    gEXTextureRectangle(gMasterDisp++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_CENTER, (-SCREEN_WIDTH) * 2, 0,
                        (-SCREEN_WIDTH * 2), SCREEN_WIDTH * 3, 0, 0, 0, 0, 0);

    // Draw right black rectangle
    gEXTextureRectangle(gMasterDisp++, G_EX_ORIGIN_CENTER, G_EX_ORIGIN_RIGHT, SCREEN_WIDTH * 2, 0, SCREEN_WIDTH * 2,
                        SCREEN_WIDTH * 3, 0, 0, 0, 0, 0);
    gDPSetScissor(gMasterDisp++, G_SC_NON_INTERLACE, (SCREEN_WIDTH - 320) / 2, 0, (SCREEN_WIDTH + 320) / 2, 240);
}
#endif

#endif
