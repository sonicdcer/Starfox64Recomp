#ifndef __PATCHES_H__
#define __PATCHES_H__

#define RECOMP_EXPORT __attribute__((section(".recomp_export")))
#define RECOMP_PATCH __attribute__((section(".recomp_patch")))
#define RECOMP_FORCE_PATCH __attribute__((section(".recomp_force_patch")))
#define RECOMP_DECLARE_EVENT(func)                                                          \
    _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"") \
        __attribute__((noinline, weak, used, section(".recomp_event"))) void func {         \
    }                                                                                       \
    _Pragma("GCC diagnostic pop")

// TODO fix renaming symbols in patch recompilation
#define osCreateMesgQueue osCreateMesgQueue_recomp
#define osRecvMesg osRecvMesg_recomp
#define osSendMesg osSendMesg_recomp
#define osViGetCurrentFramebuffer osViGetCurrentFramebuffer_recomp
#define osFlashWriteArray osFlashWriteArray_recomp
#define osFlashWriteBuffer osFlashWriteBuffer_recomp
#define osWritebackDCache osWritebackDCache_recomp
#define osInvalICache osInvalICache_recomp
#define osGetTime osGetTime_recomp

#define osContStartReadData osContStartReadData_recomp
#define osContGetReadData osContGetReadData_recomp
#define osContStartQuery osContStartQuery_recomp
#define osContGetQuery osContGetQuery_recomp

#define __sinf __sinf_recomp
#define __cosf __cosf_recomp
#define sqrtf sqrtf_recomp
#define bzero bzero_recomp
#define gRandFloat sRandFloat

#define INVERT_Y_AXIS_COND_RES (recomp_get_invert_y_axis_mode() == 1 ? 1 : -1)

#include "PR/ultratypes.h"
#include "PR/gbi.h"
#include "gfx.h"
#include "sf64thread.h"
#include "sf64object.h"
#include "context.h"
#include "functions.h"
#include "rt64_extended_gbi.h"
#include "graphics.h"
#include "PR/abi.h"
#include "audioseq_cmd.h"
#include "macros.h"
#include "variables.h"
#include "debug.h"
#include "sfx.h"
#include "sf64event.h"
#include "sf64_tagging.h"
#include "assets.h"
#include "bgm.h"
#include "input.h"
#include "functions.h"

typedef struct ArwingInfoRecomp {
    u8 rightWingState;
    u8 leftWingState;
    f32 upperRightFlapYrot;
    f32 bottomRightFlapYrot;
    f32 upperLeftFlapYrot;
    f32 bottomLeftFlapYrot;
    f32 laserGunsYpos;
    f32 laserGunsXpos;
    f32 wingsXrot;
    f32 wingsYrot;
    f32 wingsZrot;
    f32 unk_28;
    u8 drawFace;
    f32 teamFaceXrot;
    f32 teamFaceYrot;
    f32 cockpitGlassXrot;
    void* actorPtr;
} ArwingInfoRecomp; // size = 0x40

extern s32 gCamera1Skipped;

#ifndef gEXFillRectangle
#define gEXFillRectangle(cmd, lorigin, rorigin, ulx, uly, lrx, lry)                         \
    G_EX_COMMAND2(cmd, PARAM(RT64_EXTENDED_OPCODE, 8, 24) | PARAM(G_EX_FILLRECT_V1, 24, 0), \
                  PARAM(lorigin, 12, 0) | PARAM(rorigin, 12, 12),                           \
                                                                                            \
                  PARAM((ulx) * 4, 16, 16) | PARAM((uly) * 4, 16, 0),                       \
                  PARAM((lrx) * 4, 16, 16) | PARAM((lry) * 4, 16, 0))
#endif

#define gEXMatrixGroupNoInterpolation(cmd, push, proj, edit)                                                           \
    gEXMatrixGroup(cmd, G_EX_ID_IGNORE, G_EX_INTERPOLATE_SIMPLE, push, proj, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, \
                   G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,                 \
                   G_EX_COMPONENT_SKIP, G_EX_ORDER_LINEAR, edit, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP)

#define gEXMatrixGroupInterpolateOnlyTiles(cmd, push, proj, edit)                                                      \
    gEXMatrixGroup(cmd, G_EX_ID_IGNORE, G_EX_INTERPOLATE_SIMPLE, push, proj, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, \
                   G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,                 \
                   G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP)

#define gEXMatrixGroupDecomposedNormal(cmd, id, push, proj, edit)                                                \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,        \
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit,           \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP)

#define gEXMatrixGroupDecomposedNormal2(cmd, id, push, proj, edit)                                               \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_AUTO,               \
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit,           \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP)

#define gEXMatrixGroupDecomposedSkipRot(cmd, id, push, proj, edit)                                               \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP,               \
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit,           \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP)

#define gEXMatrixGroupDecomposedSkipPosRot(cmd, id, push, proj, edit)                                            \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,                      \
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit,           \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP)

#define gEXMatrixGroupDecomposedSkipAll(cmd, id, push, proj, edit)                                               \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP,                      \
                             G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit, G_EX_COMPONENT_SKIP,           \
                             G_EX_COMPONENT_SKIP)

#define gEXMatrixGroupDecomposedVerts(cmd, id, push, proj, edit)                                                 \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,        \
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, \
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit,    \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP)

#define gEXMatrixGroupDecomposedVertsOrderAuto(cmd, id, push, proj, edit)                                        \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,        \
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, \
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_AUTO, edit,      \
                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP)

int recomp_printf(const char* fmt, ...);
float recomp_powf(float, float);
f32 __sinf(f32);
f32 __cosf(f32);
float sqrtf(float f);
void Game_InitFullViewport(void);
void Gfx_Texture_UV_Scroll(Gfx* displayList, u16 width, u16 height, s32 mode, bool segmented);
void* memcpy2(void* dest, const void* src, size_t n);

#define INCBIN(identifier, filename)         \
    asm(".pushsection .rodata\n"             \
        "\t.local " #identifier "\n"         \
        "\t.type " #identifier ", @object\n" \
        "\t.balign 8\n" #identifier ":\n"    \
        "\t.incbin \"" filename "\"\n\n"     \
                                             \
        "\t.balign 8\n"                      \
        "\t.popsection\n");                  \
    extern u8 identifier[]

// void View_ApplyInterpolate(View* view, s32 mask, bool reset_interpolation_state);

// void set_camera_skipped(bool skipped);
void clear_camera_skipped();
float recomp_get_target_aspect_ratio(float);
// bool camera_was_skipped();

void recomp_crash(const char* err);

#endif
