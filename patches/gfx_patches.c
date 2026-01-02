#define RECOMP_PATCH __attribute__((section(".recomp_patch")))
#define END_OF_ARRAY(array) (&(array)[0] + ARRAY_COUNT(array))
#define SCREEN_MARGIN_RECOMP 0

#define osSendMesg osSendMesg_recomp
#define osViSwapBuffer osViSwapBuffer_recomp
#define osRecvMesg osRecvMesg_recomp
#define osWritebackDCacheAll osWritebackDCacheAll_recomp

#include "../lib/sf64decomp/include/PR/ultratypes.h"
#include "../lib/sf64decomp/include/PR/os_message.h"
#include "../lib/sf64decomp/include/sf64thread.h"
#include "../lib/sf64decomp/include/gfx.h"
#include "../lib/sf64decomp/include/sys.h"
#include "../lib/sf64decomp/include/sf64audio_external.h"
#include "../lib/rt64/include/rt64_extended_gbi.h"
#include "misc_funcs.h"
#include "patches.h"

typedef struct AssetInfo {
    /* 0x00 */ void* unk_00;
    /* 0x04 */ void* unk_04;
    /* 0x08 */ s16 unk_08;
    /* 0x0C */ u32 unk_0C;
    /* 0x10 */ u32 unk_10;
    /* 0x14 */ f32 unk_14;
    /* 0x18 */ Vec3f unk_18;
    /* 0x24 */ Vec3f unk_24;
    /* 0x30 */ Vec3f unk_30;
    /* 0x3C */ Vec3f unk_3C;
    /* 0x48 */ Vec3f unk_48;
    /* 0x54 */ Vec3f unk_54;
    /* 0x60 */ Color_RGBA32 fog;
    /* 0x64 */ s16 fogNear;
    /* 0x66 */ s16 fogFar;
    /* 0x68 */ Color_RGBA32 env;
    /* 0x6C */ Color_RGBA32 prim;
    /* 0x70 */ u8 unk_70;
    /* 0x71 */ u8 unk_71;
} AssetInfo; // size = 0x72

// @recomp make GfxPool larger
typedef struct {
    /* 0x00000 */ SPTask task;
    /* 0x000500 */ Vp viewports[0x10 * 0x10];
    /* 0x001500 */ Mtx mtx[0x480 * 0x10];
    /* 0x121500 */ Gfx unkDL1[0x180 * 0x10];
    /* 0x12D500 */ Gfx masterDL[0x1380 * 0x10];
    /* 0x1C9500 */ Gfx unkDL2[0xD80 * 0x10];
    /* 0x235500 */ Lightsn lights[0x100 * 0x10];
} ExGfxPool; // size = 0x2AD500, 0x8 aligned

extern OSMesgQueue gControllerMesgQueue;
extern OSContPad gControllerPress[4];
extern FrameBuffer gFrameBuffers[3];
extern Gfx gRcpInitDL[];

ExGfxPool gExGfxPools[2];
ExGfxPool* gExGfxPool;

extern void Graphics_InitializeTask(u32 frameCount);
extern void Graphics_SetTask(void);
extern void Main_SetVIMode(void);
extern int recomp_printf(const char* fmt, ...);
extern void DrawBorders(void);

// @recomp FPS fix, pass Vi's per frame to RT64 for interpolated frames
RECOMP_PATCH void Graphics_ThreadEntry(void* arg0) {
    u8 i;
    u8 visPerFrame;
    u8 validVIsPerFrame;

    Game_Initialize();
    osSendMesg(&gSerialThreadMesgQueue, (OSMesg) SI_READ_CONTROLLER, OS_MESG_NOBLOCK);
    Graphics_InitializeTask(gSysFrameCount);
    {
        gSPSegment(gUnkDisp1++, 0, 0);
        gSPDisplayList(gMasterDisp++, gExGfxPool->unkDL1); // @recomp
        Game_Update();
        gSPEndDisplayList(gUnkDisp1++);
        gSPEndDisplayList(gUnkDisp2++);
        gSPDisplayList(gMasterDisp++, gExGfxPool->unkDL2); // @recomp

        gDPFullSync(gMasterDisp++);
        gSPEndDisplayList(gMasterDisp++);
    }

    // @recomp Crash the game if any GfxPool goes out of bounds.
    if ((gViewport > END_OF_ARRAY(gExGfxPool->viewports)) || (gGfxMtx > END_OF_ARRAY(gExGfxPool->mtx)) ||
        (gUnkDisp1 > END_OF_ARRAY(gExGfxPool->unkDL1)) || (gMasterDisp > END_OF_ARRAY(gExGfxPool->masterDL)) ||
        (gUnkDisp2 > END_OF_ARRAY(gExGfxPool->unkDL2)) || (gLight > END_OF_ARRAY(gExGfxPool->lights))) {
        // CRASH
        *(volatile int*) 0 = 0;
    }

    Graphics_SetTask();

    while (true) {
        gSysFrameCount++;
        Graphics_InitializeTask(gSysFrameCount);
        MQ_WAIT_FOR_MESG(&gControllerMesgQueue, NULL);
        osSendMesg(&gSerialThreadMesgQueue, (OSMesg) SI_RUMBLE, OS_MESG_NOBLOCK);
        Controller_UpdateInput();
        osSendMesg(&gSerialThreadMesgQueue, (OSMesg) SI_READ_CONTROLLER, OS_MESG_NOBLOCK);
        if (gControllerPress[3].button & U_JPAD) {
            Main_SetVIMode();
        }

        {
            gEXEnable(gMasterDisp++); // @recomp

            gSPSegment(gUnkDisp1++, 0, 0);
            gSPDisplayList(gMasterDisp++, gExGfxPool->unkDL1); // @recomp
            Game_Update();
            if (gStartNMI == 1) {
                Graphics_NMIWipe();
            }
            gSPEndDisplayList(gUnkDisp1++);
            gSPEndDisplayList(gUnkDisp2++);
            gSPDisplayList(gMasterDisp++, gExGfxPool->unkDL2); // @recomp

            visPerFrame = MIN(gVIsPerFrame, 4);                                  // @recomp
            validVIsPerFrame = MAX(visPerFrame, gGfxVImesgQueue.validCount + 1); // @recomp
            gEXSetRefreshRate(gMasterDisp++, 60 / validVIsPerFrame);             // @recomp

#if 1
            // Noise
            if (recomp_get_film_grain_enabled()) {
                // gDPSetAlphaDither(gMasterDisp++, G_AD_NOISE);
                gDPSetColorDither(gMasterDisp++, G_CD_NOISE);
                // Fill the screen with a White rectangle
                gDPSetRenderMode(gMasterDisp++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
                gDPSetCombineMode(gMasterDisp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
                gDPSetPrimColor(gMasterDisp++, 0, 0, 175, 175, 175, 2); // White with 100 alpha (semi-transparent)
                gDPFillRectangle(gMasterDisp++, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            }
#endif
            gDPFullSync(gMasterDisp++);
            gSPEndDisplayList(gMasterDisp++);
        }

        MQ_WAIT_FOR_MESG(&gGfxTaskMesgQueue, NULL);

        // @recomp Crash the game if any GfxPool goes out of bounds.
        if ((gViewport > END_OF_ARRAY(gExGfxPool->viewports)) || (gGfxMtx > END_OF_ARRAY(gExGfxPool->mtx)) ||
            (gUnkDisp1 > END_OF_ARRAY(gExGfxPool->unkDL1)) || (gMasterDisp > END_OF_ARRAY(gExGfxPool->masterDL)) ||
            (gUnkDisp2 > END_OF_ARRAY(gExGfxPool->unkDL2)) || (gLight > END_OF_ARRAY(gExGfxPool->lights))) {
            // CRASH
            recomp_printf("GfxPool out of bounds!\n");
            *(volatile int*) 0 = 0;
        }
        Graphics_SetTask();

        if (!gFillScreen) {
            osViSwapBuffer(&gFrameBuffers[(gSysFrameCount - 1) % 3]);
        }

        Fault_SetFrameBuffer(&gFrameBuffers[(gSysFrameCount - 1) % 3], SCREEN_WIDTH, 16);

        visPerFrame = MIN(gVIsPerFrame, 4);
        validVIsPerFrame = MAX(visPerFrame, gGfxVImesgQueue.validCount + 1);

        for (i = 0; i < validVIsPerFrame; i++) {
            MQ_WAIT_FOR_MESG(&gGfxVImesgQueue, NULL);
        }

        Audio_Update();
    }
}

// @recomp use gExGfxPool instead of the original GfxPool
RECOMP_PATCH void Graphics_InitializeTask(u32 frameCount) {
    gExGfxPool = &gExGfxPools[frameCount % 2]; // @recomp

    gGfxTask = &gExGfxPool->task;       // @recomp
    gViewport = gExGfxPool->viewports;  // @recomp
    gGfxMtx = gExGfxPool->mtx;          // @recomp
    gUnkDisp1 = gExGfxPool->unkDL1;     // @recomp
    gMasterDisp = gExGfxPool->masterDL; // @recomp
    gUnkDisp2 = gExGfxPool->unkDL2;     // @recomp
    gLight = gExGfxPool->lights;

    gFrameBuffer = &gFrameBuffers[frameCount % 3];
    gTextureRender = &gTextureRenderBuffer[0];

    gGfxMatrix = &sGfxMatrixStack[0];
    gCalcMatrix = &sCalcMatrixStack[0];

    D_80178710 = &D_80178580[0];
}

#define BOOTSTRAP_COMMAND_COUNT 3

typedef struct {
    Gfx data[2][BOOTSTRAP_COMMAND_COUNT];
} BootstrapDLs;

// @recomp use gExGfxPool instead of the original GfxPool
RECOMP_PATCH void Graphics_SetTask(void) {
    // Unused memory between the recomp variables and the patch memory
    BootstrapDLs* bootstrapDLs = (BootstrapDLs*) (0x80800800);
    Gfx* bootstrapDL = bootstrapDLs->data[gSysFrameCount % 2];
    Gfx* bootstrapDLHead = bootstrapDL;

    // Initialize the bootstrap DL to enable the extended gbi and extended rdram
    gEXEnable(bootstrapDLHead++);
    gEXSetRDRAMExtended(bootstrapDLHead++, 1);
    gSPBranchList(bootstrapDLHead++, gExGfxPool->masterDL);

    gGfxTask->mesgQueue = &gGfxTaskMesgQueue;
    gGfxTask->msg = (OSMesg) TASK_MESG_2;
    gGfxTask->task.t.type = M_GFXTASK;
    gGfxTask->task.t.flags = 0;
    gGfxTask->task.t.ucode_boot = (u64*) rspbootTextStart; // @recomp (cast)
    gGfxTask->task.t.ucode_boot_size = (uintptr_t) rspbootTextEnd - (uintptr_t) rspbootTextStart;
    gGfxTask->task.t.ucode = (u64*) gspF3DEX_fifoTextStart; // @recomp (cast)
    gGfxTask->task.t.ucode_size = SP_UCODE_SIZE;
    gGfxTask->task.t.ucode_data = (u64*) &gspF3DEX_fifoDataStart; // @recomp (cast)
    gGfxTask->task.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    gGfxTask->task.t.dram_stack = gDramStack;
    gGfxTask->task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    gGfxTask->task.t.output_buff = (u64*) gTaskOutputBuffer;
    gGfxTask->task.t.output_buff_size = (u64*) gAudioHeap;
    // gGfxTask->task.t.data_ptr = (u64*) gExGfxPool->masterDL;                         // @recomp
    gGfxTask->task.t.data_ptr = (u64*) bootstrapDL;                                  // @recomp
    gGfxTask->task.t.data_size = (gMasterDisp - gExGfxPool->masterDL) * sizeof(Gfx); // @recomp
    gGfxTask->task.t.yield_data_ptr = (u64*) &gOSYieldData;
    gGfxTask->task.t.yield_data_size = OS_YIELD_DATA_SIZE;
    osWritebackDCacheAll();
    osSendMesg(&gTaskMesgQueue, gGfxTask, OS_MESG_NOBLOCK);
}

// @recomp Remove screen margin & gEXSetScissor
RECOMP_PATCH void Game_InitMasterDL(Gfx** dList) {
    gSPDisplayList((*dList)++, gRcpInitDL);
    // gDPSetScissor((*dList)++, G_SC_NON_INTERLACE, SCREEN_MARGIN_RECOMP, SCREEN_MARGIN_RECOMP,// @recomp
    //               SCREEN_WIDTH - SCREEN_MARGIN_RECOMP, SCREEN_HEIGHT - SCREEN_MARGIN_RECOMP);
    gEXSetScissor(gMasterDisp++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0,
                  SCREEN_HEIGHT); // @recomp
    gDPSetDepthImage((*dList)++, &gZBuffer);
    gDPSetColorImage((*dList)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, &gZBuffer);
    gDPSetFillColor((*dList)++, FILL_COLOR(GPACK_ZDZ(G_MAXFBZ, 0)));
    gDPFillRectangle((*dList)++, SCREEN_MARGIN_RECOMP, SCREEN_MARGIN_RECOMP, SCREEN_WIDTH - SCREEN_MARGIN_RECOMP - 1,
                     SCREEN_HEIGHT - SCREEN_MARGIN_RECOMP - 1);
    gDPSetColorImage((*dList)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, gFrameBuffer);

    if (gBlurAlpha < 255) {
        gDPPipeSync((*dList)++);
        gDPSetCycleType((*dList)++, G_CYC_1CYCLE);
        gDPSetCombineMode((*dList)++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPSetRenderMode((*dList)++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor((*dList)++, 0x00, 0x00, RGBA16_RED(gBgColor) * 8, RGBA16_GRN(gBgColor) * 8,
                        RGBA16_BLU(gBgColor) * 8, gBlurAlpha);
    } else {
        gDPSetFillColor((*dList)++, FILL_COLOR(gBgColor | 1));
    }
    gDPFillRectangle((*dList)++, SCREEN_MARGIN_RECOMP, SCREEN_MARGIN_RECOMP, SCREEN_WIDTH - SCREEN_MARGIN_RECOMP - 1,
                     SCREEN_HEIGHT - SCREEN_MARGIN_RECOMP);
    gDPPipeSync((*dList)++);
    gDPSetColorDither((*dList)++, G_CD_MAGICSQ);
}

// @recomp Remove screen margin
RECOMP_PATCH void Ending_8018D398(u32 arg0, AssetInfo* asset) {
    u8 alpha = 255;

    if ((asset->unk_0C + asset->fogNear) > arg0) {
        alpha = (arg0 - asset->unk_0C) * 255 / asset->fogNear;
    }

    if ((asset->unk_0C + asset->unk_10 - asset->fogFar) < arg0) {
        alpha = (asset->unk_0C + asset->unk_10 - arg0) * 255 / asset->fogFar;
    }

    gFillScreenRed = gFillScreenGreen = gFillScreenBlue = gFillScreenAlpha = gFillScreenAlphaTarget =
        gFillScreenAlphaStep = 0;
    // @recomp use of SCREEN_MARGIN_RECOMP instead of SCREEN_MARGIN
    Graphics_FillRectangle(&gMasterDisp, SCREEN_MARGIN_RECOMP, SCREEN_MARGIN_RECOMP,
                           SCREEN_WIDTH - SCREEN_MARGIN_RECOMP, SCREEN_HEIGHT - SCREEN_MARGIN_RECOMP, asset->prim.r,
                           asset->prim.g, asset->prim.b, alpha);
}

RECOMP_PATCH void Game_InitStandbyDL(Gfx** dList) {
    gSPDisplayList((*dList)++, gRcpInitDL);
    // gDPSetScissor((*dList)++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 3);
    gEXSetScissor(gMasterDisp++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, SCREEN_HEIGHT);
    gDPSetFillColor((*dList)++, FILL_COLOR(0x0001));
    gDPSetColorImage((*dList)++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, gFrameBuffers[0].data);
    gDPFillRectangle((*dList)++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT * 3 - 1);
    gDPPipeSync((*dList)++);
    gDPSetColorDither((*dList)++, G_CD_MAGICSQ);
}