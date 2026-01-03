#include "patches.h"

#define MEM_ARRAY_ALLOCATE(arr, count) ((arr) = Memory_Allocate((count) * sizeof(*(arr))))

#if 1 // NEW STARFIELD TESTING

// Define constants for starfield multipliers
#define STARFIELD_WIDTH_MULTIPLIER 1.0f
#define STARFIELD_HEIGHT_MULTIPLIER 1.0f

// Declare global variables for screen dimensions
float gCurrentScreenWidth = (float) SCREEN_WIDTH * 3;   // Default width
float gCurrentScreenHeight = (float) SCREEN_HEIGHT * 3; // Default height

// New global variables for storing the previoous positions
f32 gStarPrevX[3000];
f32 gStarPrevY[3000];

// Custom floating-point modulo function (replaces fmodf)
float FloatMod(float a, float b) {
    float result = a - ((int) (a / b)) * b;
    if (result < 0.0f) {
        result += b;
    }
    return result;
}

// Define a single 1x1 star as two triangles
static Vtx starVerts[4] = {
    // Format: VTX(x, y, z, s, t, r, g, b, a)
    VTX(0, 0, 0, 0, 0, 255, 255, 255, 255), // Bottom-left
    VTX(0, 1, 0, 0, 0, 255, 255, 255, 255), // Top-left
    VTX(1, 0, 0, 0, 0, 255, 255, 255, 255), // Bottom-right
    VTX(1, 1, 0, 0, 0, 255, 255, 255, 255), // Top-right
};

#if 1
// @recomp: Starfield drawn with triangles, re-engineered by @Tharo
RECOMP_PATCH void Background_DrawStarfield(void) {
    f32 by;
    f32 bx;
    s32 i;
    s32 starCount;
    f32 zCos;
    f32 zSin;
    f32 xField;
    f32 yField;
    f32* xStar;
    f32* yStar;
    u32* color;
    float currentScreenWidth;
    float currentScreenHeight;
    float starfieldWidth;
    float starfieldHeight;
    float vx;
    float vy;
    const float STAR_MARGIN = 10.0f; // Margin to hide seam stars

    // Set projection to orthographic before drawing stars
    Lib_InitOrtho(&gMasterDisp);

    // Setup render state for stars
    static Gfx starSetupDL[] = {
        gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF), // Disable texturing
        gsSPClearGeometryMode(G_ZBUFFER | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_CULL_BACK |
                              G_SHADING_SMOOTH),
        gsDPPipeSync(),
        gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE), // Use shade color
        gsDPSetOtherMode(G_AD_NOTPATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                             G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                         G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2),
        gsSPEndDisplayList(),
    };
    gSPDisplayList(gMasterDisp++, starSetupDL);

    // Get current screen dimensions
    currentScreenWidth = gCurrentScreenWidth;
    currentScreenHeight = gCurrentScreenHeight;
    starfieldWidth = STARFIELD_WIDTH_MULTIPLIER * currentScreenWidth;
    starfieldHeight = STARFIELD_HEIGHT_MULTIPLIER * currentScreenHeight;

    starCount = gStarCount;

    if (starCount != 0) {
        // Wrapping logic for starfield positions
        if (gStarfieldX >= starfieldWidth) {
            gStarfieldX -= starfieldWidth;
        }
        if (gStarfieldY >= starfieldHeight) {
            gStarfieldY -= starfieldHeight;
        }
        if (gStarfieldX < 0.0f) {
            gStarfieldX += starfieldWidth;
        }
        if (gStarfieldY < 0.0f) {
            gStarfieldY += starfieldHeight;
        }
        xField = gStarfieldX;
        yField = gStarfieldY;

        xStar = gStarOffsetsX;
        yStar = gStarOffsetsY;
        color = gStarFillColors;

        if (gGameState != GSTATE_PLAY) {
            starCount = 1000;
        }

        starCount = starCount * 3; // Adjust multiplier as needed

        zCos = __cosf(gStarfieldRoll);
        zSin = __sinf(gStarfieldRoll);

        for (i = 0; i < starCount; i++, yStar++, xStar++, color++) {
            // Adjust star positions with field offsets
            bx = *xStar + xField;
            by = *yStar + yField;

            // Wrapping logic for individual stars along X-axis
            if (bx >= starfieldWidth) {
                bx -= starfieldWidth;
            }
            if (bx < 0.0f) {
                bx += starfieldWidth;
            }

            // Wrapping logic for individual stars along Y-axis
            if (by >= starfieldHeight) {
                by -= starfieldHeight;
            }
            if (by < 0.0f) {
                by += starfieldHeight;
            }

            // Center the positions
            bx -= starfieldWidth / 2.0f;
            by -= starfieldHeight / 2.0f;

            // Apply rotation
            vx = (zCos * bx) + (zSin * by) + currentScreenWidth / 2.0f;
            vy = (-zSin * bx) + (zCos * by) + currentScreenHeight / 2.0f;

            // Check if the star is within the visible screen area with margin
            if ((vx >= STAR_MARGIN) && (vx < currentScreenWidth - STAR_MARGIN) && (vy >= STAR_MARGIN) &&
                (vy < currentScreenHeight - STAR_MARGIN)) {
                // @recomp Tag the transform.
                u8 skipInterpolation = (fabsf(vx - gStarPrevX[i]) > starfieldWidth / 2.0f) ||
                                       (fabsf(vy - gStarPrevY[i]) > starfieldHeight / 2.0f);

                u8 interpolateComponent = skipInterpolation ? G_EX_COMPONENT_SKIP : G_EX_COMPONENT_INTERPOLATE;

                if (gCamera1Skipped) {
                    // Skip
                    // @recomp Tag the transform

                    gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_STARFIELD + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                                    G_EX_EDIT_NONE);
                } else {
                    // @recomp Tag the transform.
                    gEXMatrixGroupDecomposed(gMasterDisp++, TAG_STARFIELD + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                             interpolateComponent, interpolateComponent, interpolateComponent,
                                             interpolateComponent, interpolateComponent, G_EX_COMPONENT_SKIP,
                                             interpolateComponent, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW,
                                             G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);
                }

                // Translate to (vx, vy) in ortho coordinates
                Matrix_Push(&gGfxMatrix);
                Matrix_Translate(gGfxMatrix, vx - (currentScreenWidth / 2.0f), -(vy - (currentScreenHeight / 2.0f)),
                                 0.0f, MTXF_NEW);
                Matrix_SetGfxMtx(&gMasterDisp);
                Matrix_Pop(&gGfxMatrix);

                // Convert color from fill color (assuming RGB5A1) to RGBA8
                u32 r = ((*color >> 11) & 0x1F);
                r = (r << 3) | (r >> 2); // Convert 5-bit to 8-bit
                u32 g = ((*color >> 6) & 0x1F);
                g = (g << 3) | (g >> 2); // Convert 5-bit to 8-bit
                u32 b = ((*color >> 1) & 0x1F);
                b = (b << 3) | (b >> 2); // Convert 5-bit to 8-bit
                u32 a = 255;             // Fully opaque
                u32 colorRGBA32 = a | (b << 8) | (g << 16) | (r << 24);

                gSPVertex(gMasterDisp++, starVerts, ARRAY_COUNT(starVerts), 0);
                gSPModifyVertex(gMasterDisp++, 1, G_MWO_POINT_RGBA, colorRGBA32);
                gSP2Triangles(gMasterDisp++, 1, 2, 0, 0, 1, 2, 3, 0);

                // Pop the transform id
                gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

                gStarPrevX[i] = vx;
                gStarPrevY[i] = vy;
            }
        }
    }

    // Restore original perspective after drawing stars
    Lib_InitPerspective(&gMasterDisp);

    // Finalize rendering state
    gDPPipeSync(gMasterDisp++);
    gDPSetColorDither(gMasterDisp++, G_CD_MAGICSQ);
}
#endif

#if 1
RECOMP_PATCH void Play_GenerateStarfield(void) {
    u32 i;
    float currentScreenWidth = gCurrentScreenWidth;
    float currentScreenHeight = gCurrentScreenHeight;
    float starfieldWidth = STARFIELD_WIDTH_MULTIPLIER * currentScreenWidth;
    float starfieldHeight = STARFIELD_HEIGHT_MULTIPLIER * currentScreenHeight;

    MEM_ARRAY_ALLOCATE(gStarOffsetsX, 3000);
    MEM_ARRAY_ALLOCATE(gStarOffsetsY, 3000);
    MEM_ARRAY_ALLOCATE(gStarFillColors, 3000);
    // MEM_ARRAY_ALLOCATE(gStarPrevX, 3000);
    // MEM_ARRAY_ALLOCATE(gStarPrevY, 3000);

    Rand_SetSeed(1, 29000, 9876);

    for (i = 0; i < 3000; i++) {
        gStarOffsetsX[i] = RAND_FLOAT_SEEDED(starfieldWidth);
        gStarOffsetsY[i] = RAND_FLOAT_SEEDED(starfieldHeight);
        gStarFillColors[i] = FILL_COLOR(gStarColors[i % ARRAY_COUNT(gStarColors)]);
        gStarPrevX[i] = 0.0f;
        gStarPrevY[i] = 0.0f;
    }
}
#endif

RECOMP_PATCH void Camera_SetStarfieldPos(f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt) {
    f32 sp34;
    f32 sp30;
    f32 pitch;
    f32 yaw;
    f32 tempf;
    f32 sp20;

    // Get current screen dimensions
    float currentScreenWidth = gCurrentScreenWidth;
    float currentScreenHeight = gCurrentScreenHeight;
    float starfieldWidth = STARFIELD_WIDTH_MULTIPLIER * currentScreenWidth;
    float starfieldHeight = STARFIELD_HEIGHT_MULTIPLIER * currentScreenHeight;

    yaw = -Math_Atan2F(xEye - xAt, zEye - zAt);
    tempf = sqrtf(SQ(zEye - zAt) + SQ(xEye - xAt));
    pitch = -Math_Atan2F(yEye - yAt, tempf);

    // Adjust yaw to stay within the range [-π/2, π/2]
    if (yaw >= M_PI / 2) {
        yaw -= M_PI;
    }
    if (yaw <= -M_PI / 2) {
        yaw += M_PI;
    }

    tempf = 0.0f;
    if (gCurrentLevel == LEVEL_UNK_15) {
        tempf = gPlayer[0].cam.eye.y * 0.03f;
    }

    // Calculate new starfield positions
    sp30 = (-pitch * (-8.0f / 3.0f * M_RTOD) * 2.0f) + 3000.0f + gStarfieldScrollY + tempf;
    sp34 = (yaw * (-8.0f / 3.0f * M_RTOD) * 2.0f) + 3000.0f + gStarfieldScrollX;
    sp20 = gStarfieldX;

    // Wrap the starfield positions within the starfield dimensions
    gStarfieldX = FloatMod(sp34, starfieldWidth);
    gStarfieldY = FloatMod(sp30, starfieldHeight);

    // Special case handling for specific game state and level
    if ((gGameState == GSTATE_PLAY) && (gPlayer[0].state == PLAYERSTATE_LEVEL_INTRO) &&
        (gCurrentLevel == LEVEL_METEO)) {
        if (fabsf(gStarfieldX - sp20) < 50.0f) {
            D_bg_8015F96C = 0.0f;
            if (fabsf(gStarfieldX - sp20) > 3.0f) {
                D_bg_8015F96C = fabsf(gStarfieldX - sp20 - 3.0f) * 0.5f;
            }
        }
    }
}

RECOMP_PATCH void Play_SetupStarfield(void) {
    // Get current screen dimensions
    float currentScreenWidth = gCurrentScreenWidth;
    float currentScreenHeight = gCurrentScreenHeight;
    float baseAspectRatio = 4.0f / 3.0f; // Original aspect ratio
    float baseScreenWidth = gCurrentScreenHeight * baseAspectRatio;
    float baseArea = baseScreenWidth * gCurrentScreenHeight;
    float currentArea = currentScreenWidth * currentScreenHeight;
    float areaRatio = currentArea / baseArea;

    Play_GenerateStarfield();
    gGroundHeight = -25000.0f;

    // Base star count adjusted for screen area
    gStarCount = (s32) (600 * areaRatio);
    if (gStarCount > 1000) {
        gStarCount = 1000; // Cap the star count to 1000
    }

    // Adjust star count based on the current level
    if (gCurrentLevel == LEVEL_AREA_6) {
        gStarCount = (s32) (300 * areaRatio);
        if (gStarCount > 1000) {
            gStarCount = 1000;
        }
    }
    if (gCurrentLevel == LEVEL_UNK_15) {
        gStarCount = (s32) (400 * areaRatio);
        if (gStarCount > 1000) {
            gStarCount = 1000;
        }
    }
    if (gGameState != GSTATE_PLAY) {
        gStarCount = (s32) (800 * areaRatio);
        if (gStarCount > 1000) {
            gStarCount = 1000;
        }
    }
    if (gCurrentLevel == LEVEL_FORTUNA) {
        gStarCount = (s32) (500 * areaRatio);
        if (gStarCount > 1000) {
            gStarCount = 1000;
        }
    }
    if (gVersusMode) {
        gStarCount = 0; // No stars in versus mode
    }
    if (gCurrentLevel == LEVEL_BOLSE) {
        gStarCount = (s32) (300 * areaRatio);
        if (gStarCount > 1000) {
            gStarCount = 1000;
        }
        gGroundHeight = -0.0f;
    }
    if (gCurrentLevel == LEVEL_TRAINING) {
        gStarCount = (s32) (800 * areaRatio);
        if (gStarCount > 1000) {
            gStarCount = 1000;
        }
        gGroundHeight = -0.0f;
    }

    // Initialize starfield position with dynamic screen dimensions
    gStarfieldX = currentScreenWidth;
    gStarfieldY = currentScreenHeight;
}

#if 1
// Background_DrawPartialStarfield
// Stars that are in the Epilogue
RECOMP_PATCH void Background_DrawPartialStarfield(s32 yMin, s32 yMax) {
    f32 by;
    f32 bx;
    f32 vy;
    f32 vx;
    s32 i;
    s32 starCount;
    f32 cos;
    f32 sin;
    f32 xField;
    f32 yField;
    f32* xStar;
    f32* yStar;
    u32* color;

    yMin += 245;
    yMax += 245;

    // Get current screen dimensions
    float currentScreenWidth = gCurrentScreenWidth;
    float currentScreenHeight = gCurrentScreenHeight;
    float starfieldWidth = STARFIELD_WIDTH_MULTIPLIER * currentScreenWidth;
    float starfieldHeight = STARFIELD_HEIGHT_MULTIPLIER * currentScreenHeight;

    // Set projection to orthographic before drawing stars
    Lib_InitOrtho(&gMasterDisp);

    // Setup render state for stars
    static Gfx starSetupDL[] = {
        gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF), // Disable texturing
        gsSPClearGeometryMode(G_ZBUFFER | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_CULL_BACK |
                              G_SHADING_SMOOTH),
        gsDPPipeSync(),
        gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE), // Use shade color
        gsDPSetOtherMode(G_AD_NOTPATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                             G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                         G_AC_NONE | G_ZS_PIXEL | G_RM_OPA_SURF | G_RM_OPA_SURF2),
        gsSPEndDisplayList(),
    };
    gSPDisplayList(gMasterDisp++, starSetupDL);

    // Wrapping logic for starfield positions
    if (gStarfieldX >= starfieldWidth) {
        gStarfieldX -= starfieldWidth;
    }
    if (gStarfieldY >= starfieldHeight) {
        gStarfieldY -= starfieldHeight;
    }
    if (gStarfieldX < 0.0f) {
        gStarfieldX += starfieldWidth;
    }
    if (gStarfieldY < 0.0f) {
        gStarfieldY += starfieldHeight;
    }

    xField = gStarfieldX;
    yField = gStarfieldY;

    xStar = gStarOffsetsX;
    yStar = gStarOffsetsY;
    color = gStarFillColors;
    starCount = 500;

    starCount = starCount * 3; // Adjust multiplier as needed

    cos = __cosf(gStarfieldRoll);
    sin = __sinf(gStarfieldRoll);

    for (i = 0; i < starCount; i++, yStar++, xStar++, color++) {
        bx = *xStar + xField;
        by = *yStar + yField;

        // Wrapping logic for individual stars along X-axis
        if (bx >= starfieldWidth) {
            bx -= starfieldWidth;
        }
        if (bx < 0.0f) {
            bx += starfieldWidth;
        }

        // Wrapping logic for individual stars along Y-axis
        if (by >= starfieldHeight) {
            by -= starfieldHeight;
        }
        if (by < 0.0f) {
            by += starfieldHeight;
        }

        // Center the positions
        bx -= starfieldWidth / 2.0f;
        by -= starfieldHeight / 2.0f;

        // Apply rotation
        vx = (cos * bx) + (sin * by) + currentScreenWidth / 2.0f;
        vy = (-sin * bx) + (cos * by) + currentScreenHeight / 2.0f;

        // Check if the star is within the visible screen area
        if ((vx >= 0) && (vx < currentScreenWidth) && (yMin < vy) && (vy < yMax)) {
            // Tag the transform. Assuming TAG_STARFIELD is a defined base tag value
            // @recomp Tag the transform.
            u8 skipInterpolation = (fabsf(vx - gStarPrevX[i]) > starfieldWidth / 2.0f) ||
                                   (fabsf(vy - gStarPrevY[i]) > starfieldHeight / 2.0f);

            u8 interpolateComponent = skipInterpolation ? G_EX_COMPONENT_SKIP : G_EX_COMPONENT_INTERPOLATE;

            if (gCamera1Skipped) {
                // Skip
                // @recomp Tag the transform
                gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_STARFIELD + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                                G_EX_EDIT_NONE);
            } else {
                // @recomp Tag the transform.
                gEXMatrixGroupDecomposed(gMasterDisp++, TAG_STARFIELD + i, G_EX_PUSH, G_MTX_MODELVIEW,
                                         interpolateComponent, interpolateComponent, interpolateComponent,
                                         interpolateComponent, interpolateComponent, G_EX_COMPONENT_SKIP,
                                         interpolateComponent, G_EX_ORDER_LINEAR, G_EX_EDIT_ALLOW, G_EX_COMPONENT_SKIP,
                                         G_EX_COMPONENT_SKIP);
            }

            // Translate to (vx, vy) in ortho coordinates
            Matrix_Push(&gGfxMatrix);
            Matrix_Translate(gGfxMatrix, vx - (currentScreenWidth / 2.0f), -(vy - (currentScreenHeight / 2.0f)), 0.0f,
                             MTXF_NEW);
            Matrix_SetGfxMtx(&gMasterDisp);
            Matrix_Pop(&gGfxMatrix);

            // Convert color from fill color (assuming RGB5A1) to RGBA8
            u8 r = ((*color >> 11) & 0x1F);
            r = (r << 3) | (r >> 2); // Convert 5-bit to 8-bit
            u8 g = ((*color >> 6) & 0x1F);
            g = (g << 3) | (g >> 2); // Convert 5-bit to 8-bit
            u8 b = ((*color >> 1) & 0x1F);
            b = (b << 3) | (b >> 2); // Convert 5-bit to 8-bit
            u8 a = 255;              // Fully opaque
            u32 colorRGBA32 = a | (b << 8) | (g << 16) | (r << 24);

            gSPVertex(gMasterDisp++, starVerts, ARRAY_COUNT(starVerts), 0);
            gSPModifyVertex(gMasterDisp++, 1, G_MWO_POINT_RGBA, colorRGBA32);
            gSP2Triangles(gMasterDisp++, 1, 2, 0, 0, 1, 2, 3, 0);

            // Pop the transform id
            gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

            gStarPrevX[i] = vx;
            gStarPrevY[i] = vy;
        }
    }
    // Restore original perspective after drawing stars
    Lib_InitPerspective(&gMasterDisp);

    // Finalize rendering state
    gDPPipeSync(gMasterDisp++);
    gDPSetColorDither(gMasterDisp++, G_CD_MAGICSQ);
}
#endif // Background_DrawPartialStarfield

#endif // End of NEW STARFIELD TESTING block
