#define RECOMP_PATCH __attribute__((section(".recomp_patch")))

#include "patches.h"
#include "debug.h"
#include "i3.h"
#include "i5.h"
#include "i6.h"
#include "sf64_record.h"

#define __sinf __sinf_recomp

extern bool sDrawCockpit;
extern u8 sPlayersVisible[];
extern f32 sReticleScales[4];
extern f32 D_i3_801C41B8[30];
extern s32 D_i3_801C4190[10];
extern bool gBackToMap;

void Display_CsLevelCompleteHandleCamera(Player* player);
void Display_Player_Update(Player* player, s32 reflectY);
void Display_PlayerFeatures(Player* player);
void Display_ArwingWingTrail_Draw(Player* player);
void Display_ArwingWingTrail_Update(Player* player);
void Display_PlayerShadow_Update(Player* player);
void Display_ActorMarks(void);
void Display_LockOnIndicator(void);
void Display_CockpitGlass(void);
void Display_Reticle(Player* player);
void Display_LandmasterMuzzleFlash(Player* player);
void Display_OnFootMuzzleFlash(Player* player);
void Display_DrawHelpAlert(void);

bool should_interpolate_perspective(Vec3f* eye, Vec3f* at);

#if DEBUG_SPAWNER == 1
void Spawner(void);
#endif
#if DEBUG_BOSS_KILLER == 1
void KillBoss(void);
#endif

// for draw distance tests
#if 0
f32 segataSanshiro;
int gXoffsetBounds = 0;
int gYoffsetBounds = 0;
int gBackZdist = 0;
int gFrontZdist = 0;
#endif

// for background tests
#if 0
int xTemp;
int yTemp;
int xTest = 0;
#endif

// for texture scroll debugging
#if 0
int gUvOn = 0;
#endif

s32 gCamera1Skipped = 0;
s32 gCamera2Skipped = 0;
s32 gCamera3Skipped = 0;
s32 gCamera4Skipped = 0;

RECOMP_PATCH void Display_Update(void) {
    s32 i;
    Vec3f tempVec;
    Vec3f playerCamUp;
    Vec3f playerPos;
    s32 pad;
    Player* player;
    Player* camPlayer = &gPlayer[gPlayerNum];

    sDrawCockpit = false;

    // @recomp: Display player's face at all times.
    gPlayer[0].arwing.drawFace = true;

// @recomp set hit count cap to 999 to restore US/JP 1.0 behaviour
#if 1
    // 999 hit count cap (511 in 1.1 US)
    if (gHitCount > 999) {
        gHitCount = 999;
    }
#endif

    Matrix_Push(&gGfxMatrix);

    if ((gCurrentLevel == LEVEL_AQUAS) && (gPlayer[0].state == PLAYERSTATE_ACTIVE)) {
        Math_SmoothStepToF(&gCamDistortion, 0.01f, 0.2f, 0.002f, 0.0f);
    } else {
        Math_SmoothStepToF(&gCamDistortion, 0.0f, 0.2f, 0.002f, 0.0f);
    }

    Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 10.0f * M_DTOR, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f + gCamDistortion, 1.0f - gCamDistortion, 1.0f, MTXF_APPLY);
    Matrix_RotateZ(gGfxMatrix, -(f32) gGameFrameCount * 10.0f * M_DTOR, MTXF_APPLY);
    Matrix_Scale(gGfxMatrix, 1.0f + gCamDistortion, 1.0f - gCamDistortion, 1.0f, MTXF_APPLY);
    Matrix_Push(&gGfxMatrix);
    Camera_SetupLights(camPlayer);
    Lights_SetOneLight(&gMasterDisp, gLight1x, gLight1y, gLight1z, gLight1R, gLight1G, gLight1B, gAmbientR, gAmbientG,
                       gAmbientB);

    if (gLevelMode == LEVELMODE_ON_RAILS) {
        Matrix_RotateY(gCalcMatrix, camPlayer->yRot_114 * M_DTOR, MTXF_NEW);
        Matrix_RotateX(gCalcMatrix, camPlayer->xRot_120 * M_DTOR, MTXF_APPLY);

        tempVec.x = camPlayer->cam.eye.x - camPlayer->pos.x;
        tempVec.y = camPlayer->cam.eye.y - camPlayer->pos.y;
        tempVec.z = camPlayer->cam.eye.z - (camPlayer->trueZpos + camPlayer->zPath);
        Matrix_MultVec3f(gCalcMatrix, &tempVec, &gPlayCamEye);
        gPlayCamEye.x += camPlayer->pos.x;
        gPlayCamEye.y += camPlayer->pos.y;
        gPlayCamEye.z += camPlayer->trueZpos + camPlayer->zPath;

        tempVec.x = camPlayer->cam.at.x - camPlayer->pos.x;
        tempVec.y = camPlayer->cam.at.y - camPlayer->pos.y;
        tempVec.z = camPlayer->cam.at.z - (camPlayer->trueZpos + camPlayer->zPath);
        Matrix_MultVec3f(gCalcMatrix, &tempVec, &gPlayCamAt);
        gPlayCamAt.x += camPlayer->pos.x;
        gPlayCamAt.y += camPlayer->pos.y;
        gPlayCamAt.z += camPlayer->trueZpos + camPlayer->zPath;

        if (camPlayer->alternateView && (camPlayer->boostSpeed > 5.0f)) {
            gPlayCamAt.x += SIN_DEG(gGameFrameCount * 150.0f) * camPlayer->boostSpeed * 0.2f;
        }
    } else if (camPlayer->state == PLAYERSTATE_LEVEL_COMPLETE) {
        Display_CsLevelCompleteHandleCamera(camPlayer);
    } else {
        gPlayCamEye.x = camPlayer->cam.eye.x;
        gPlayCamEye.y = camPlayer->cam.eye.y;
        gPlayCamEye.z = camPlayer->cam.eye.z;
        gPlayCamAt.x = camPlayer->cam.at.x;
        gPlayCamAt.y = camPlayer->cam.at.y;
        gPlayCamAt.z = camPlayer->cam.at.z;
    }

    static PlayState prevPlayState = 0;
    static int camSkipTimes = 0;

    bool bigJump = !should_interpolate_perspective(&gPlayCamEye, &gPlayCamAt);

    // @recomp: Force interpolation camera skip if we're transitioning to or from a pause state.
    if (((prevPlayState == PLAY_PAUSE) && (gPlayState == PLAY_UPDATE)) ||
        ((prevPlayState == PLAY_UPDATE) && (gPlayState == PLAY_PAUSE))) {
        bigJump = true;
    }

    if (bigJump) {
        // Skip interpolation for this frame.
        gEXMatrixGroupSimple(gMasterDisp++, 0xFFFFAAAA, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);
        recomp_printf("CAMERA 1 SKIPED: %d\n", camSkipTimes++);
        gCamera1Skipped = true;
    } else {
        // Simple interpolation works much better for cameras because they orbit around a focus.
        gEXMatrixGroupSimple(gMasterDisp++, 0xFFFFAAAA, G_EX_NOPUSH, G_MTX_PROJECTION, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, G_EX_EDIT_NONE, G_EX_COMPONENT_SKIP,
                             G_EX_COMPONENT_SKIP);
        gCamera1Skipped = false;
    }

    prevPlayState = gPlayState;

    camPlayer->camYaw = -Math_Atan2F(gPlayCamEye.x - gPlayCamAt.x, gPlayCamEye.z - gPlayCamAt.z);
    camPlayer->camPitch = -Math_Atan2F(gPlayCamEye.y - gPlayCamAt.y,
                                       sqrtf(SQ(gPlayCamEye.z - gPlayCamAt.z) + SQ(gPlayCamEye.x - gPlayCamAt.x)));
    Matrix_RotateY(gCalcMatrix, -camPlayer->camYaw, MTXF_NEW);
    Matrix_RotateX(gCalcMatrix, camPlayer->camPitch, MTXF_APPLY);
    Matrix_RotateZ(gCalcMatrix, -camPlayer->camRoll * M_DTOR, MTXF_APPLY);
    tempVec.x = 0.0f;
    tempVec.y = 100.0f;
    tempVec.z = 0.0f;
    Matrix_MultVec3f(gCalcMatrix, &tempVec, &playerCamUp);

    if (gStarCount != 0) {
        gStarfieldRoll = DEG_TO_RAD(gPlayer[0].camRoll);
        Camera_SetStarfieldPos(gPlayCamEye.x, gPlayCamEye.y, gPlayCamEye.z, gPlayCamAt.x, gPlayCamAt.y, gPlayCamAt.z);
        Background_DrawStarfield();
    }

    Background_DrawBackdrop();
    Background_DrawSun();
    Matrix_Push(&gGfxMatrix);
    Matrix_LookAt(gGfxMatrix, gPlayCamEye.x, gPlayCamEye.y, gPlayCamEye.z, gPlayCamAt.x, gPlayCamAt.y, gPlayCamAt.z,
                  playerCamUp.x, playerCamUp.y, playerCamUp.z, MTXF_APPLY);

    if ((gLevelType == LEVELTYPE_PLANET) || (gCurrentLevel == LEVEL_BOLSE)) {
        if ((gCurrentLevel == LEVEL_TITANIA) &&
            ((gPlayer[0].state != PLAYERSTATE_LEVEL_INTRO) || (gPlayer[0].unk_19C != 0))) {
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, 0.0f, gCameraShakeY, 0.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            Ground_801B58AC(&gMasterDisp, gPathGroundScroll);
            gPathGroundScroll = 0.0f;
            Matrix_Pop(&gGfxMatrix);
        } else if (gGroundSurface != SURFACE_WATER) {
            gDrawAquasSurfaceWater = false;
            Background_DrawGround();
        }
    }

    Lights_SetOneLight(&gMasterDisp, gLight2x, gLight2y, gLight2z, gLight2R, gLight2G, gLight2B, gAmbientR, gAmbientG,
                       gAmbientB);

    for (i = 0, player = &gPlayer[0]; i < gCamCount; i++, player++) {
        playerPos.x = player->pos.x;
        playerPos.y = player->pos.y;
        playerPos.z = player->trueZpos;
        Display_SetSecondLight(&playerPos);
        Display_Player_Update(player, 0);
        Display_SetupPlayerSfxPos(player);
    }

    if ((gGroundSurface == SURFACE_WATER) && (gPlayer[0].state != PLAYERSTATE_LEVEL_INTRO)) {
        Lights_SetOneLight(&gMasterDisp, gLight2x, -1 * gLight2y, gLight2z, gLight2R, gLight2G, gLight2B, gAmbientR,
                           gAmbientG, gAmbientB);
        Matrix_Push(&gGfxMatrix);
        Matrix_Scale(gGfxMatrix, 1.0f, -1.0f, 1.0f, MTXF_APPLY);
        for (i = 0, player = &gPlayer[0]; i < gCamCount; i++, player++) {
            playerPos.x = player->pos.x;
            playerPos.y = player->pos.y;
            playerPos.z = player->trueZpos;
            Display_Player_Update(player, 1);
        }
        Matrix_Pop(&gGfxMatrix);
    }

    Lights_SetOneLight(&gMasterDisp, gLight1x, gLight1y, gLight1z, gLight1R, gLight1G, gLight1B, gAmbientR, gAmbientG,
                       gAmbientB);
    Object_Draw(1);
    TexturedLine_Draw();
    gReflectY = 1;
    PlayerShot_DrawAll();

    if ((gGroundSurface == SURFACE_WATER) && (gPlayer[0].state != PLAYERSTATE_LEVEL_INTRO)) {
        Matrix_Push(&gGfxMatrix);
        Matrix_Scale(gGfxMatrix, 1.0f, -1.0f, 1.0f, MTXF_APPLY);
        gReflectY = -1;
        PlayerShot_DrawAll();
        Matrix_Pop(&gGfxMatrix);
    }

    gReflectY = -1;

    if ((gGroundSurface == SURFACE_WATER) && (gPlayer[0].state != PLAYERSTATE_LEVEL_INTRO)) {
        Matrix_Push(&gGfxMatrix);
        Matrix_Scale(gGfxMatrix, 1.0f, -1.0f, 1.0f, MTXF_APPLY);
        for (i = 0, player = &gPlayer[0]; i < gCamCount; i++, player++) {
            if (sPlayersVisible[i]) {
                Display_PlayerFeatures(player);
                Display_ArwingWingTrail_Update(player);
            }
            Matrix_Pop(&gGfxMatrix);
        }
    }

    if (gCurrentLevel == LEVEL_AQUAS) {
        Effect_Draw(0);
    }

    if ((gGroundSurface == SURFACE_WATER) || (gAqDrawMode != 0)) {
        gDrawAquasSurfaceWater = true;
        Effect_Draw(1);
        Background_DrawGround();
    }

    if ((gCurrentLevel != LEVEL_AQUAS) &&
        (((gCurrentLevel != LEVEL_CORNERIA) && (gCurrentLevel != LEVEL_VENOM_ANDROSS)) ||
         ((gPlayer[0].state != PLAYERSTATE_LEVEL_COMPLETE) && (gPlayer[0].state != PLAYERSTATE_LEVEL_INTRO)))) {
        Effect_Draw(0);
    }

    gReflectY = 1;

    for (i = 0, player = &gPlayer[0]; i < gCamCount; i++, player++) {
        if (sPlayersVisible[i]) {
            Display_PlayerShadow_Update(player);
            Display_PlayerFeatures(player);
            Display_ArwingWingTrail_Update(player);
        }
    }

    if ((gCurrentLevel == LEVEL_AQUAS) && (gPlayer[0].state == PLAYERSTATE_ACTIVE)) {
        Aquas_BlueMarineReticle_Draw();
    }

    if (((gCurrentLevel == LEVEL_CORNERIA) || (gCurrentLevel == LEVEL_VENOM_ANDROSS)) &&
        ((gPlayer[0].state == PLAYERSTATE_LEVEL_COMPLETE) || (gPlayer[0].state == PLAYERSTATE_LEVEL_INTRO))) {
        Effect_Draw(0);
    }

    BonusText_DrawAll();
    Matrix_Pop(&gGfxMatrix);
    Display_ActorMarks();
    Display_LockOnIndicator();

    if (sDrawCockpit) {
        Display_CockpitGlass();
    }

    for (i = 0, player = &gPlayer[0]; i < gCamCount; i++, player++) {
        if (sPlayersVisible[i]) {
            if (gShowReticles[i]) {
                Display_Reticle(player);
            }
            if (player->form == FORM_LANDMASTER) {
                Display_LandmasterMuzzleFlash(player);
            } else if (player->form == FORM_ON_FOOT) {
                Display_OnFootMuzzleFlash(player);
            }
        }
    }

    if ((gLevelMode == LEVELMODE_TURRET) && (gPlayer[0].state == PLAYERSTATE_ACTIVE)) {
        Turret_Draw(gPlayer);
    }

    Background_DrawLensFlare();

    if ((gCamCount != 1) && ((camPlayer->state == PLAYERSTATE_ACTIVE) || (camPlayer->state == PLAYERSTATE_U_TURN))) {
        HUD_Draw();
        HUD_EdgeArrows_Update();
    }
    Matrix_Pop(&gGfxMatrix);
    Display_DrawHelpAlert();
    sPlayersVisible[gPlayerNum] = false;
    Matrix_Pop(&gGfxMatrix);

    // @recomp: Warpzone cutscene recording
    if (gPlayer[0].state == PLAYERSTATE_ENTER_WARP_ZONE) {
        if (gPlayer[0].csState == 0) {
            gWarpzoneCsFrameCount = 0;
        }
        gWarpzoneCsFrameCount++;
        UpdateVisPerFrameFromRecording_Warpzone(gWarpzoneCsRecord, ARRAY_COUNT(gWarpzoneCsRecord));
    }

// @recomp DEBUG SECTION:
#if DEBUG_CHEATS == 1
    if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
        return;
    }
    gBombCount[0] = 9;
    gLaserStrength[0] = 2;
#endif

#if DEBUG_SPAWNER == 1
    Spawner();
#endif

#if DEBUG_BOSS_KILLER == 1
    KillBoss();
#endif
#if DEBUG_L_TO_ALL_RANGE == 1
    {
        Player* pl2 = &gPlayer[0];

        if (gControllerPress[0].button & L_TRIG) {
            pl2->state = PLAYERSTATE_START_360;
        }
    }
#endif
#if DEBUG_L_TO_LEVEL_COMPLETE == 1
    {
        Player* pl = &gPlayer[0];

        if (gControllerPress[0].button & L_TRIG) {
            pl->state = PLAYERSTATE_LEVEL_COMPLETE;
            gMissionStatus = MISSION_ACCOMPLISHED;
        }
    }
#endif
#if DEBUG_Z_R_START_TO_RESET == 1
    {
        Player* pl = &gPlayer[0];
        static int resetTimer = 0;

        if ((gControllerHold[0].button & Z_TRIG) && (gControllerHold[0].button & R_TRIG) &&
            (gControllerPress[0].button & START_BUTTON)) {
            resetTimer = 40;
            // pl->state = PLAYERSTATE_NEXT;
            pl->csTimer = 0;
            gFadeoutType = 4;
        }
        if (resetTimer == 10) {
            gGameState = GSTATE_BOOT;
        }

        if (resetTimer > 0) {
            resetTimer--;
        }
    }
#endif
#if DEBUG_BACK_TO_MAP == 1
    {
        Player* pl = &gPlayer[0];

        if ((gControllerHold[0].button & Z_TRIG) && (gControllerHold[0].button & R_TRIG) &&
            (gControllerPress[0].button & U_CBUTTONS)) {
            gShowLevelClearStatusScreen = false;
            gLevelStartStatusScreenTimer = 0;
            gStarCount = 0;
            gGameState = GSTATE_MAP;
            gNextGameStateTimer = 2;
            gMapState = 0;
            gLastGameState = GSTATE_NONE;
            gDrawMode = DRAW_NONE;
            gControllerLock = 3;
            gBackToMap = true;
        }
    }
#endif
#if DEBUG_L_TO_WARP_ZONE == 1
    if ((gGameState != GSTATE_PLAY) || (gPlayState <= PLAY_INIT)) {
        return;
    }
    if (gControllerPress[0].button & L_TRIG) {
        if ((gCurrentLevel != LEVEL_SECTOR_X) && (gCurrentLevel != LEVEL_METEO)) {
            return;
        }
        if (gCurrentLevel == LEVEL_SECTOR_X) {
            gRingPassCount++;
            gPlayer[0].state = PLAYERSTATE_ENTER_WARP_ZONE;
            gPlayer[0].csState = 0;
            gSceneSetup = 1;
            AUDIO_PLAY_SFX(NA_SE_WARP_HOLE, gDefaultSfxSource, 0);
            gMissionStatus = MISSION_WARP;
            gLeveLClearStatus[gCurrentLevel] = 1;
        } else {
            gPlayer[0].state = PLAYERSTATE_ENTER_WARP_ZONE;
            gPlayer[0].csState = 0;
            AUDIO_PLAY_SFX(NA_SE_WARP_HOLE, gDefaultSfxSource, 0);
            gMissionStatus = MISSION_WARP;
            gLeveLClearStatus[gCurrentLevel] = 1;
        }
    }
#endif
#if DEBUG_SPEED_CONTROL == 1 // baseSpeed control
    {
        Player* player = gPlayer;
        static s32 prevSpeed;
        static bool debugFreeze = false;

        if (gControllerPress[0].button & L_JPAD) {
            player->baseSpeed -= 50;
        } else if (gControllerPress[0].button & R_JPAD) {
            player->baseSpeed += 50;
        }

        if ((!debugFreeze) && (gControllerPress[0].button & D_JPAD)) {
            prevSpeed = player->baseSpeed;
            player->baseSpeed = 0;
            debugFreeze = true;
        } else if ((debugFreeze) && (gControllerPress[0].button & D_JPAD)) {
            player->baseSpeed = prevSpeed;
            debugFreeze = false;
        }
    }
#endif
#if 0
    RCP_SetupDL(&gMasterDisp, SETUPDL_83);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplaySmallText(10, 210, 1.0f, 1.0f, "STICKX:");
    Graphics_DisplaySmallNumber(60, 210, (int) ABS(gInputPress->stick_x));
    Graphics_DisplaySmallText(10, 220, 1.0f, 1.0f, "STICKY:");
    Graphics_DisplaySmallNumber(60, 220, (int) ABS(gInputPress->stick_y));
    if (gInputPress->stick_x < 0.0f) Graphics_DisplaySmallText(110, 210, 1.0f, 1.0f, "NEG:");
    if (gInputPress->stick_y < 0.0f) Graphics_DisplaySmallText(110, 220, 1.0f, 1.0f, "NEG:");
#endif
#if DEBUG_NO_COLLISION == 1
    gPlayer->mercyTimer = 1000;
#endif

// background testing
#if 0
    RCP_SetupDL(&gMasterDisp, SETUPDL_83);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplaySmallText(10, 210, 1.0f, 1.0f, "CSFR:");
    Graphics_DisplaySmallNumber(80, 210, (int)gCsFrameCount);
#endif
// for draw distance tests
#if 0
    {

        RCP_SetupDL(&gMasterDisp, SETUPDL_83);
        gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);

        if (xTemp >= 0) {
            Graphics_DisplaySmallText(10, 190, 1.0f, 1.0f, "XTEMP:");
        } else {
            Graphics_DisplaySmallText(10, 190, 1.0f, 1.0f, "XTEMP:");
        }
        Graphics_DisplaySmallNumber(80, 190, ABS(xTemp));

        if (yTemp >= 0) {
            Graphics_DisplaySmallText(10, 200, 1.0f, 1.0f, "YTEMP:");
        } else {
            Graphics_DisplaySmallText(10, 200, 1.0f, 1.0f, "YTEMP:");
        }
        Graphics_DisplaySmallNumber(80, 200, ABS(yTemp));

        Graphics_DisplaySmallText(10, 210, 1.0f, 1.0f, "XTEST:");
        Graphics_DisplaySmallNumber(80, 210, ABS(xTest));
        Graphics_DisplaySmallText(10, 220, 1.0f, 1.0f, "CSFRAME:");
        Graphics_DisplaySmallNumber(80, 220, ABS(gCsFrameCount));
        if (gControllerPress[0].button & Z_TRIG) {
            xTest -= 10;
        } else if (gControllerPress[0].button & R_TRIG) {
            xTest += 10;
        }
        /*
        Graphics_DisplaySmallText(10, 200, 1.0f, 1.0f, "BACK:");
        Graphics_DisplaySmallNumber(80, 200, ABS(gBackZdist));
        Graphics_DisplaySmallText(10, 190, 1.0f, 1.0f, "FRONT:");
        Graphics_DisplaySmallNumber(80, 190, ABS(gFrontZdist));

        if (gControllerPress[0].button & L_JPAD) {
            gXoffsetBounds -= 1000;
        } else if (gControllerPress[0].button & R_JPAD) {
            gXoffsetBounds += 1000;
        } else if (gControllerPress[0].button & U_JPAD) {
            gYoffsetBounds += 1000;
        } else if (gControllerPress[0].button & D_JPAD) {
            gYoffsetBounds -= 1000;
        } else if (gControllerPress[0].button & Z_TRIG) {
            gFrontZdist -= 1000;
        } else if (gControllerPress[0].button & R_TRIG) {
            gFrontZdist += 1000;
        }


        Player* player = gPlayer;
        static s32 prevSpeed;
        static bool debugFreeze = false;

        if ((!debugFreeze) && (gControllerPress[0].button & L_TRIG)) {
            prevSpeed = player->baseSpeed;
            player->baseSpeed = 0;
            debugFreeze = true;
        } else if ((debugFreeze) && (gControllerPress[0].button & L_TRIG)) {
            player->baseSpeed = prevSpeed;
            debugFreeze = false;
        }
        */
    }
#endif
#if 0
    RCP_SetupDL(&gMasterDisp, SETUPDL_83);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplaySmallText(10, 210, 1.0f, 1.0f, "STICK_X:");
    Graphics_DisplaySmallNumber(60, 210, (int) ABS(gInputPress->stick_x));
    Graphics_DisplaySmallText(10, 220, 1.0f, 1.0f, "STICK_Y:");
    Graphics_DisplaySmallNumber(60, 220, (int) ABS(gInputPress->stick_y));
    if (gInputPress->stick_x < 0) Graphics_DisplaySmallText(110, 210, 1.0f, 1.0f, "NEG:");
    if (gInputPress->stick_y < 0) Graphics_DisplaySmallText(110, 220, 1.0f, 1.0f, "NEG:");
#endif

#if 0
    RCP_SetupDL(&gMasterDisp, SETUPDL_83);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplaySmallText(10, 210, 1.0f, 1.0f, "STICK_X:");
    Graphics_DisplaySmallNumber(60, 210, (int) gPlayer->csState);
#endif

// For texture scroll debugging
#if 0
    RCP_SetupDL(&gMasterDisp, SETUPDL_83);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplaySmallText(10, 210, 1.0f, 1.0f, "UV:");
    Graphics_DisplaySmallNumber(80, 210, (int) gUvOn);
    if (gControllerPress[0].button & L_TRIG) {
        gUvOn ^= 1;
    }
#endif

    // D_display_800CA220 = 2;

    // For debugging cutscene timings
#if 0
    RCP_SetupDL(&gMasterDisp, SETUPDL_83);
    gDPSetPrimColor(gMasterDisp++, 0, 0, 255, 255, 0, 255);
    Graphics_DisplaySmallText(10 + 210, 180, 1.0f, 1.0f, "VIS:");
    Graphics_DisplaySmallNumber(60 + 210, 180, (int) gVIsPerFrame);
    Graphics_DisplaySmallText(10 + 210, 190, 1.0f, 1.0f, "CSFMS:");
    Graphics_DisplaySmallNumber(60 + 210, 190, (int) gCsFrameCount);
    Graphics_DisplaySmallText(10 + 210, 200, 1.0f, 1.0f, "PLTIM:");
    Graphics_DisplaySmallNumber(60 + 220, 200, (int) gPlayer->csTimer);
    Graphics_DisplaySmallText(10 + 210, 210, 1.0f, 1.0f, "CSSTATE:");
    Graphics_DisplaySmallNumber(60 + 220, 210, (int) gPlayer->csState);
#endif
}

// for draw distance tests
#if 0
RECOMP_PATCH void SectorZ_Missile_Update(ActorAllRange* this) {
    return;
}
#endif

Gfx SETUPDL_63_POINT[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE, ENVIRONMENT,
                       TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ALPHACOMPARE, 3, G_AC_NONE | G_ZS_PIXEL),
    gsDPSetRenderMode(G_RM_CLD_SURF, G_RM_CLD_SURF2),
    gsSPSetOtherModeHi(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                       G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE),
    gsSPEndDisplayList(),
};

Gfx SETUPDL_36_POINT[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPSetGeometryMode(0),
    gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ALPHACOMPARE, 3, G_AC_NONE | G_ZS_PIXEL),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPSetOtherModeHi(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                       G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE),
    gsSPEndDisplayList(),
};

RECOMP_PATCH void Display_Reticle(Player* player) {
    Vec3f* translate;
    s32 i;

    if ((gPlayerNum == player->num) && ((player->form == FORM_ARWING) || (player->form == FORM_LANDMASTER)) &&
        player->draw &&
        (((gGameState == GSTATE_PLAY) && (player->state == PLAYERSTATE_ACTIVE)) || (gGameState == GSTATE_MENU))) {
        for (i = 0; i < 2; i++) {
            translate = &D_display_801613E0[i];
            Matrix_Push(&gGfxMatrix);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_RETICLE + i, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

            Matrix_Translate(gGfxMatrix, translate->x, translate->y, translate->z, MTXF_APPLY);

            if (gChargeTimers[player->num] >= 20) {
                // RCP_SetupDL(&gMasterDisp, SETUPDL_63);
                gSPDisplayList(gMasterDisp++, SETUPDL_63_POINT);
                if (i == 1) {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 0, 0, 255);
                    gDPSetEnvColor(gMasterDisp++, 255, 0, 0, 255);
                    Math_SmoothStepToF(&sReticleScales[player->num], 2.0f, 1.0f, 0.4f, 0.0f);
                } else {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 0, 255);
                    gDPSetEnvColor(gMasterDisp++, 255, 255, 0, 255);
                }
            } else {
                // RCP_SetupDL_36();
                gSPDisplayList(gMasterDisp++, SETUPDL_36_POINT);
            }

            if (i == 1) {
                Matrix_Scale(gGfxMatrix, sReticleScales[player->num], sReticleScales[player->num], 1.0f, MTXF_APPLY);
                Math_SmoothStepToF(&sReticleScales[player->num], 1.0f, 1.0f, 0.2f, 0.0f);
            }
            Matrix_Scale(gGfxMatrix, 4.0f, 4.0f, 4.0f, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, D_1024F60);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            Matrix_Pop(&gGfxMatrix);
        }
    }
}

Gfx SETUPDL_67_POINT[] = {
    /* SETUPDL_67_POINT */
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN |
                          G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE, ENVIRONMENT,
                       TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH),
    gsSPSetOtherMode(G_SETOTHERMODE_L, G_MDSFT_ALPHACOMPARE, 3, G_AC_NONE | G_ZS_PIXEL),
    gsDPSetRenderMode(G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2),
    gsSPSetOtherModeHi(G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                       G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE),
    gsSPEndDisplayList(),
};

RECOMP_PATCH void Display_LockOnIndicator(void) {
    s32 i;
    s32 j;
    f32 var_fs0;

    for (i = 0; i < gCamCount; i++) {
        if (gLockOnTargetViewPos[i].z < 0.0f) {
            var_fs0 = VEC3F_MAG(&gLockOnTargetViewPos[i]);
            if (var_fs0 < 20000.0f) {
                var_fs0 *= 0.0015f;
                if (var_fs0 > 100.0f) {
                    var_fs0 = 100.0f;
                }
                if (var_fs0 < 1.2f) {
                    var_fs0 = 1.2f;
                }

                Matrix_Push(&gGfxMatrix);

                // @recomp Tag the transform.
                gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_RETICLE + 2 + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                               G_EX_EDIT_ALLOW);

                Matrix_Translate(gGfxMatrix, gLockOnTargetViewPos[i].x, gLockOnTargetViewPos[i].y,
                                 gLockOnTargetViewPos[i].z, MTXF_APPLY);
                if ((gPlayState != PLAY_PAUSE) && (i == gPlayerNum)) {
                    Math_SmoothStepToF(&D_display_801615A8[i], 0.0f, 0.5f, 20.0f, 0);
                    Math_SmoothStepToF(&D_display_801615B8[i], 1.0, 0.5f, 0.2f, 0);
                }
                var_fs0 *= D_display_801615B8[i];
                Matrix_Scale(gGfxMatrix, var_fs0 * 1.5f, var_fs0 * 1.5f, 1.0f, MTXF_APPLY);
                Matrix_RotateZ(gGfxMatrix, D_display_801615A8[i] * M_DTOR, MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                // RCP_SetupDL(&gMasterDisp, SETUPDL_67);
                gSPDisplayList(gMasterDisp++, SETUPDL_67_POINT);
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 0, 0, 255);
                gDPSetEnvColor(gMasterDisp++, 255, 0, 0, 255);
                gSPDisplayList(gMasterDisp++, D_1024F60);

                // @recomp Pop the transform id.
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

                Matrix_Pop(&gGfxMatrix);
            }
        }
    }

    for (j = 0; j < gCamCount; j++) {
        gLockOnTargetViewPos[j].x = gLockOnTargetViewPos[j].y = 0.f;
        gLockOnTargetViewPos[j].z = 100.0f;
    }
}

RECOMP_PATCH void Display_LandmasterEngineGlow_Draw(Player* player) {
    RCP_SetupDL_64();
    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 100);
    Matrix_Push(&gGfxMatrix);
    Matrix_RotateZ(gGfxMatrix, player->bankAngle * M_DTOR, MTXF_APPLY);

    if (player->form == FORM_LANDMASTER) {
        if (player->unk_194 <= 0.0f) {
            Matrix_Pop(&gGfxMatrix);
            return;
        }
        Matrix_Scale(gGfxMatrix, player->unk_194, player->unk_194, 1.0f, MTXF_APPLY);
    } else {
        Matrix_Scale(gGfxMatrix, player->unk_194, player->unk_194, 1.0f, MTXF_APPLY);
    }

    if ((gGameFrameCount % 2) != 0) {
        Matrix_Scale(gGfxMatrix, 0.9f, 0.63f, 1.0f, MTXF_APPLY);
    } else {
        Matrix_Scale(gGfxMatrix, 0.9f * 0.9f, 0.9f * 0.63f, 1.0f, MTXF_APPLY);
    }
    Matrix_SetGfxMtx(&gMasterDisp);
    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ADDRESS(player) | 0x50, G_EX_PUSH, G_MTX_MODELVIEW,
                                   G_EX_EDIT_ALLOW);
    Display_DrawEngineGlow(gLevelType);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    Matrix_Pop(&gGfxMatrix);
}

RECOMP_PATCH void Actor_DrawEngineAndContrails(Actor* this) {
    f32 sp5C;
    f32 temp1;
    f32 sp54;
    s32 pad[5];

    if ((this->iwork[11] != 0) && (this->obj.status == OBJ_ACTIVE)) {
        temp1 = 652.5f * 0.001f; // 0.65250003f;
        if (this->iwork[11] >= 2) {
            temp1 = 1.3050001f;
        }

        Math_SmoothStepToF(&this->fwork[29], temp1, 0.3f, 5.0f, 0.0f);

        sp5C = this->fwork[29];
        if ((gGameFrameCount % 2) != 0) {
            sp5C *= 1.111f;
        }
        Matrix_Push(&gGfxMatrix);
        Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -60.0f, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, sp5C, sp5C * 0.7f, sp5C, MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, -this->obj.rot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, -this->obj.rot.x * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, -this->obj.rot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, -gPlayer[gPlayerNum].camYaw, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, gPlayer[gPlayerNum].camPitch, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ACTOR(this) | 0x50, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        Display_DrawEngineGlow(gLevelType);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        Matrix_Pop(&gGfxMatrix);
    }

    sp5C = this->fwork[21];
    if ((sp5C != 0.0f) && (gLevelType == LEVELTYPE_PLANET)) {
        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, (TAG_ACTOR(this) | 0x50) + 1, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);
        sp54 = 0.0f;
        if ((gGameFrameCount % 2) != 0) {
            sp54 = 180.0f;
        }
        RCP_SetupDL_64_2();
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 100);
        Matrix_Push(&gGfxMatrix);
        Matrix_Translate(gGfxMatrix, 70.0f, -10.0f, -100.0f, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, sp5C, 1.0f, 50.0f, MTXF_APPLY);
        Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -17.5f, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, M_DTOR * sp54, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, aBallDL);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        Matrix_Pop(&gGfxMatrix);

        Matrix_Push(&gGfxMatrix);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, (TAG_ACTOR(this) | 0x50) + 2, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        Matrix_Translate(gGfxMatrix, -70.0f, -10.0f, -100.0f, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, sp5C, 1.0f, 50.0f, MTXF_APPLY);
        Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -17.5f, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, M_DTOR * sp54, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, aBallDL);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        Matrix_Pop(&gGfxMatrix);
    }
}

RECOMP_PATCH void Actor_DrawEngineGlow(Actor* actor, EngineGlowColor color) {
    f32 scale;

    if ((actor->iwork[11] != 0) && (actor->obj.status == OBJ_ACTIVE)) {
        scale = 0.63f;
        if (actor->iwork[11] >= 2) {
            scale = D_edisplay_800CFCA0[actor->iwork[11] - 2] * 0.45f;
        }
        if ((gGameFrameCount % 2) != 0) {
            scale *= 1.2f;
        }
        Matrix_Push(&gGfxMatrix);
        Matrix_Scale(gGfxMatrix, scale, scale, scale, MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, -actor->obj.rot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, -actor->obj.rot.x * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, -actor->obj.rot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, -gPlayer[gPlayerNum].camYaw, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_ACTOR(actor) | 0x50, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        Display_DrawEngineGlow(color);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        Matrix_Pop(&gGfxMatrix);
    }
}

// Blue Marine Reticle
RECOMP_PATCH void Aquas_BlueMarineReticle_Draw(void) {
    s32 i;
    f32 x;
    f32 y;

    if (gPlayer[0].draw) {
        Matrix_Push(&gGfxMatrix);
        Math_SmoothStepToF(&D_i3_801C41B8[5], 3.0f, 1.0f, 4.0f, 0.0001f);
        RCP_SetupDL(&gMasterDisp, SETUPDL_61);

        if (D_i3_801C4190[3] == 0) {
            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 255);
            x = -10.0f;
            y = 10.0f;

            if (D_i3_801C4190[0] != 0) {
                x = y = 0.0f;
            }

            Matrix_Translate(gGfxMatrix, D_i3_801C41B8[0] + x, D_i3_801C41B8[1] + y,
                             D_i3_801C41B8[2] + gPlayer[0].zPath, MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, D_i3_801C41B8[5], D_i3_801C41B8[5], D_i3_801C41B8[5], MTXF_APPLY);

            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_RETICLE + 6, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

            for (i = 0; i < 4; i++) {
                Matrix_RotateZ(gGfxMatrix, M_PI / 2, MTXF_APPLY);
                Matrix_Translate(gGfxMatrix, x, y, 0.0f, MTXF_APPLY);
                Matrix_Push(&gGfxMatrix);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aBlueMarineReticleDL);
                Matrix_Pop(&gGfxMatrix);
            }

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        } else {
            // @recomp Tag the transform.
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_RETICLE + 7, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

            gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 0, 0, 255);
            Matrix_Translate(gGfxMatrix, D_i3_801C41B8[0], D_i3_801C41B8[1], D_i3_801C41B8[2] + gPlayer[0].zPath,
                             MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, D_i3_801C41B8[5], D_i3_801C41B8[5], D_i3_801C41B8[5], MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aBlueMarineReticleTargetDL);

            // @recomp Pop the transform id.
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
        }

        Matrix_Pop(&gGfxMatrix);
    }
}

#if 1
u32 bolse_ult = 0, bolse_lrt = 63, bolse_uls = 0, bolse_lrs = 63;

RECOMP_PATCH void Bolse_BoBaseShield_Update(BoBaseShield* this) {
    D_i4_801A0530 = 0;

    Math_SmoothStepToF(&this->fwork[0], D_BO_801A03DC * 9.0f + 10.0f, 1.0f, 10.0f, 0.0f);

    // Lib_Texture_Scroll(aBoBaseShieldTex, 16, 16, 0);
    bolse_ult = (bolse_ult - 4) & 0x3F;
    bolse_lrt = (bolse_ult - 63) & 0xFFF;
    bolse_lrt = (bolse_ult - 63) & 0xFFF;
    // pointer to the SetTileSize cmd
    Gfx* cmd = (Gfx*) SEGMENTED_TO_VIRTUAL((void*) ((Gfx*) (aBoBaseShieldDL + 2)));
    // upper left coords
    cmd->words.w0 = (G_SETTILESIZE << 24) | (bolse_uls << 12) | bolse_ult;
    // lower right coords
    cmd->words.w1 = (cmd->words.w1 & 0x07000000) | (bolse_lrs << 12) | bolse_lrt;

    switch (this->state) {
        case 2:
            break;

        case 0:
            if (D_BO_801A03DC == 0) {
                this->timer_052 = 130;
                this->state = 1;
                Radio_PlayMessage(gMsg_ID_11050, RCID_FOX);
                SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM, 1);
                SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_FANFARE, 1);
            }
            break;

        case 1:
            Math_SmoothStepToF(&gBosses[0].fwork[1], 0.0f, 1.0f, 0.01f, 0.001f);
            if (this->timer_052 == 0) {
                this->state = 2;
                AUDIO_PLAY_SFX(NA_SE_OB_BARRIER_RELEASE, this->sfxSource, 0);
            }
            break;
    }

    if (gGroundClipMode == 0) {
        if ((gGameFrameCount % 2) == 0) {
            gLight3Brightness = 0.0f;
        } else {
            gLight3Brightness = 0.5f;
        }
        gLight3x = this->obj.pos.x;
        gLight3y = this->obj.pos.y;
        gLight3z = this->obj.pos.z;
        gLight3R = 255;
        gLight3G = 128;
        gLight3B = 128;
    }
}
#endif

#if 1
RECOMP_PATCH void Display_ArwingLaserCharge(Player* player) {
    Vec3f spC4 = { 0.0f, -5.0f, 60.0f };
    Vec3f spB8 = { 0.0f, -14.0f, 60.0f };
    Vec3f spAC = { 30.0f, -10.0f, 30.0f };
    Vec3f spA0 = { -30.0f, -10.0f, 30.0f };
    Vec3f sp94;
    Vec3f sp88;
    u8 laserStrength;
    f32 sp80;

    if (gChargeTimers[player->num] > 10) {
        RCP_SetupDL(&gMasterDisp, SETUPDL_67);
        Matrix_Copy(gCalcMatrix, &D_display_80161418[player->num]);

        if (player->alternateView && (gLevelMode == LEVELMODE_ON_RAILS)) {
            Matrix_MultVec3f(gCalcMatrix, &spB8, &sp94);
        } else {
            Matrix_MultVec3f(gCalcMatrix, &spC4, &sp94);
        }

        if (gCamera1Skipped) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, ((u32) player << 16) | TAG_CHARGED_SHOT, G_EX_PUSH,
                                            G_MTX_MODELVIEW, G_EX_EDIT_NONE);
        } else {
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, ((u32) player << 16) | TAG_CHARGED_SHOT, G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        }

        Matrix_Push(&gGfxMatrix);

        sp80 = gChargeTimers[player->num] / 20.0f;

        Matrix_Translate(gGfxMatrix, sp94.x, sp94.y, sp94.z, MTXF_NEW);
        Matrix_Scale(gGfxMatrix, sp80, sp80, 1.0f, MTXF_APPLY);
        Matrix_Push(&gGfxMatrix);

        if (player->alternateView && (gLevelMode == LEVELMODE_ON_RAILS)) {
            Matrix_Scale(gGfxMatrix, 3.0f, 3.0f, 3.0f, MTXF_APPLY);
        } else {
            Matrix_Scale(gGfxMatrix, 10.0f, 10.0f, 10.0f, MTXF_APPLY);
        }

        RCP_SetupDL(&gMasterDisp, SETUPDL_49);
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 128);

        if (gVersusMode) {
            switch (player->num) {
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
            gDPSetEnvColor(gMasterDisp++, 0, 255, 0, 128);
        }

        Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 53.0f * M_DTOR, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, aStarDL);
        Matrix_RotateZ(gGfxMatrix, gGameFrameCount * -53.0f * 2.0f * M_DTOR, MTXF_APPLY);
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, aStarDL);
        Matrix_Pop(&gGfxMatrix);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        if (gCamera1Skipped) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, ((u32) player << 16) | TAG_CHARGED_SHOT + 1, G_EX_PUSH,
                                            G_MTX_MODELVIEW, G_EX_EDIT_NONE);
        } else {
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, ((u32) player << 16) | TAG_CHARGED_SHOT + 1, G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        }

        if (player->alternateView && (gLevelMode == LEVELMODE_ON_RAILS)) {
            Matrix_Scale(gGfxMatrix, 0.3f, 0.3f, 0.3f, MTXF_APPLY);
        }

        Matrix_Scale(gGfxMatrix, 0.5f, 0.5f, 1.0f, MTXF_APPLY);

        if ((gGameFrameCount % 2) == 0) {
            Matrix_Scale(gGfxMatrix, 1.7f, 1.7f, 1.0f, MTXF_APPLY);
        } else {
            Matrix_Scale(gGfxMatrix, 1.3f, 1.3f, 1.0f, MTXF_APPLY);
        }
        Matrix_SetGfxMtx(&gMasterDisp);
        gSPDisplayList(gMasterDisp++, aOrbDL);
        Matrix_Pop(&gGfxMatrix);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }

    if (gMuzzleFlashScale[player->num] > 0.1f) {

        if (gCamera1Skipped) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, ((u32) player << 16) | TAG_CHARGED_SHOT + 2, G_EX_PUSH,
                                            G_MTX_MODELVIEW, G_EX_EDIT_NONE);
        } else {
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, ((u32) player << 16) | TAG_CHARGED_SHOT + 2, G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        }

        Matrix_Push(&gGfxMatrix);
        RCP_SetupDL(&gMasterDisp, SETUPDL_67);
        Matrix_Copy(gCalcMatrix, &D_display_80161418[player->num]);

        laserStrength = gLaserStrength[player->num];
        if (player->arwing.laserGunsYpos > -8.0f) {
            laserStrength = LASERS_SINGLE;
        }

        switch (laserStrength) {
            case LASERS_SINGLE:
                gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 192, 255, 192, 128);
                gDPSetEnvColor(gMasterDisp++, 64, 255, 64, 128);

                if (player->alternateView && (gLevelMode == LEVELMODE_ON_RAILS)) {
                    Matrix_MultVec3f(gCalcMatrix, &spB8, &sp94);
                } else {
                    Matrix_MultVec3f(gCalcMatrix, &spC4, &sp94);
                }

                Matrix_Push(&gGfxMatrix);

                Matrix_Translate(gGfxMatrix, sp94.x, sp94.y, sp94.z, MTXF_NEW);
                Matrix_Scale(gGfxMatrix, gMuzzleFlashScale[player->num], gMuzzleFlashScale[player->num], 1.0f,
                             MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);

                Matrix_Pop(&gGfxMatrix);
                break;

            case LASERS_TWIN:
            case LASERS_HYPER:
                if (laserStrength == LASERS_TWIN) {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 192, 255, 192, 128);
                    gDPSetEnvColor(gMasterDisp++, 64, 255, 64, 128);
                } else {
                    gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 128, 255, 255, 160);
                    gDPSetEnvColor(gMasterDisp++, 128, 128, 255, 160);
                }
                Matrix_MultVec3f(gCalcMatrix, &spAC, &sp94);
                Matrix_MultVec3f(gCalcMatrix, &spA0, &sp88);
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, sp94.x, sp94.y, sp94.z, MTXF_NEW);
                Matrix_Scale(gGfxMatrix, gMuzzleFlashScale[player->num], gMuzzleFlashScale[player->num], 1.0f,
                             MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);
                Matrix_Pop(&gGfxMatrix);

                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, sp88.x, sp88.y, sp88.z, MTXF_NEW);
                Matrix_Scale(gGfxMatrix, gMuzzleFlashScale[player->num], gMuzzleFlashScale[player->num], 1.0f,
                             MTXF_APPLY);
                Matrix_SetGfxMtx(&gMasterDisp);
                gSPDisplayList(gMasterDisp++, aOrbDL);

                Matrix_Pop(&gGfxMatrix);
                break;
        }
        Matrix_Pop(&gGfxMatrix);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void Display_BarrelRollShield(Player* player) {
    f32 zRotDirection;
    Vec3f src;
    Vec3f dest;

    if (player->barrelRollAlpha != 0) {
        Matrix_RotateY(gCalcMatrix, (player->yRot_114 + player->rot.y + player->damageShake + 180.0f) * M_DTOR,
                       MTXF_NEW);
        Matrix_RotateX(gCalcMatrix,
                       -((player->xRot_120 + player->rot.x + player->damageShake + player->aerobaticPitch) * M_DTOR),
                       MTXF_APPLY);
        Matrix_RotateZ(gCalcMatrix, -((player->bankAngle + player->rockAngle + player->damageShake) * M_DTOR),
                       MTXF_APPLY);

        Matrix_Translate(gCalcMatrix, player->xShake, player->yBob, 0.0f, MTXF_APPLY);

        src.x = 0.0f;
        src.y = 0.0f;
        src.z = -30.0f;

        Matrix_MultVec3f(gCalcMatrix, &src, &dest);

        zRotDirection = 1.0f;
        if (player->baseRollRate < 0) {
            zRotDirection = -1.0f;
        }

        if (gCamera1Skipped) {
            // Skip
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, ((u32) player << 16) | TAG_BARREL_ROLL, G_EX_PUSH,
                                            G_MTX_MODELVIEW, G_EX_EDIT_NONE);
        } else {
            // @recomp Tag the transform
            gEXMatrixGroupDecomposedNormal(gMasterDisp++, ((u32) player << 16) | TAG_BARREL_ROLL, G_EX_PUSH,
                                           G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
        }

        Matrix_Push(&gGfxMatrix);
        Matrix_Translate(gGfxMatrix, player->pos.x + dest.x, player->pos.y + dest.y,
                         player->trueZpos + player->zPath + dest.z, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, -gPlayer[gPlayerNum].camYaw, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, gPlayer[gPlayerNum].camPitch, MTXF_APPLY);
        Matrix_RotateZ(gGfxMatrix, gGameFrameCount * 20.0f * zRotDirection * M_DTOR, MTXF_APPLY);

        if (player->form == FORM_ARWING) {
            Matrix_Scale(gGfxMatrix, 2.0f, 2.0f, 2.0f, MTXF_APPLY);
        } else {
            Matrix_Scale(gGfxMatrix, 1.2f, 1.2f, 1.2f, MTXF_APPLY);
        }

        if (player->baseRollRate < 0) {
            Matrix_RotateX(gGfxMatrix, M_PI, MTXF_APPLY);
        }

        Matrix_SetGfxMtx(&gMasterDisp);
        RCP_SetupDL(&gMasterDisp, SETUPDL_67);
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, player->barrelRollAlpha);
        gDPSetEnvColor(gMasterDisp++, 0, 0, 160, player->barrelRollAlpha);
        gSPDisplayList(gMasterDisp++, aBarrelRollTex);
        Matrix_Pop(&gGfxMatrix);

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}
#endif

#if 1
RECOMP_PATCH void Display_ArwingWingTrail_Draw(Player* player) {
    f32 sp5C = 70.0f;
    f32 sp58 = -18.0f;
    f32 sp54;
    f32 sp50;
    f32 yRot;

    if (player->wingPosition == 2) {
        sp5C = 108.0f;
        sp58 = -22.0f;
    }

    if (player->contrailScale != 0.0f) {
        sp54 = 0.0f;
        if ((gGameFrameCount % 2) != 0) {
            sp54 = 180.0f;
        }

        yRot = player->rot.y;
        if (yRot > 90.0f) {
            yRot -= 180.0f;
        }

        yRot *= 0.25f;
        sp50 = player->rot.x * 0.25f;

        if (player->state == PLAYERSTATE_LEVEL_COMPLETE) {
            yRot = 0.0f;
            sp50 = 0.0f;
        }

        RCP_SetupDL_64();
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 100);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_PLAYER_TRAIL, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);

        if (player->arwing.leftWingState == WINGSTATE_INTACT) {
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, sp5C, sp58, -100.0f, MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, M_DTOR * sp50, MTXF_APPLY);
            Matrix_RotateY(gGfxMatrix, -(M_DTOR * yRot), MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, player->contrailScale, 1.0f, 50.0f, MTXF_APPLY);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -17.5f, MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
            Matrix_RotateY(gGfxMatrix, M_DTOR * sp54, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aBallDL);
            Matrix_Pop(&gGfxMatrix);
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

        // @recomp Tag the transform.
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_PLAYER_TRAIL + 1, G_EX_PUSH, G_MTX_MODELVIEW,
                                       G_EX_EDIT_ALLOW);

        if (player->arwing.rightWingState == WINGSTATE_INTACT) {
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, -sp5C, sp58, -100.0f, MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, M_DTOR * sp50, MTXF_APPLY);
            Matrix_RotateY(gGfxMatrix, -(M_DTOR * yRot), MTXF_APPLY);
            Matrix_Scale(gGfxMatrix, player->contrailScale, 1.0f, 50.0f, MTXF_APPLY);
            Matrix_Translate(gGfxMatrix, 0.0f, 0.0f, -17.5f, MTXF_APPLY);
            Matrix_RotateX(gGfxMatrix, M_PI / 2, MTXF_APPLY);
            Matrix_RotateY(gGfxMatrix, M_DTOR * sp54, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, aBallDL);
            Matrix_Pop(&gGfxMatrix);
        }

        // @recomp Pop the transform id.
        gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
    }
}
#endif
