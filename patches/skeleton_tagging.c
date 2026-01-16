#include "patches.h"

/**
 * There are three versions of each function, the last two are used to solve specific problems:
 *
 * Animation_DrawSkeleton + peers: Tags interpolated matrices, used for most skeletons.
 * Animation_DrawSkeletonOriginal: Original function without tagging, RT64 handles matching, currently used in
 * Andross_AndPassage_Draw . Animation_DrawSkeleton_SkipInterpolation: Interpolation is skipped, currently unused.
 */

#if 1 // Global Scope

static s32 transform = 0;

bool Andross_801935B4(s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3f* rot, void* thisx);
bool Display_ArwingOverrideLimbDraw(s32 limbIndex, Gfx** gfxPtr, Vec3f* pos, Vec3f* rot, void* wingData);
extern ArwingInfoRecomp gActorTeamArwing_recomp;

RECOMP_PATCH void Animation_DrawSkeleton(s32 mode, Limb** skeletonSegment, Vec3f* jointTable,
                                         OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data,
                                         Matrix* transform) {
    bool override;
    Limb** skeleton;
    Limb* rootLimb;
    s32 rootIndex;
    Gfx* dList;
    Vec3f baseTrans;
    Vec3f baseRot;
    s32 overrideLimbIndex;
    s32 postLimbIndex;

    ArwingInfoRecomp* teamArwing = (ArwingInfoRecomp*) data;
    void* extraPtrData = NULL;

    // @recomp: Check if the incoming actor is an ActorTeamArwing
    // If it is, include it's actor address in the tag
    if (data == &gActorTeamArwing_recomp) {
        extraPtrData = teamArwing->actorPtr;
        // recomp_printf("Animation_DrawSkeleton\n");
        // recomp_printf("Team Arwing detected\n");
    }

    Matrix_Push(&gCalcMatrix);
    Matrix_Copy(gCalcMatrix, transform);

    skeleton = SEGMENTED_TO_VIRTUAL(skeletonSegment);
    rootLimb = SEGMENTED_TO_VIRTUAL(skeleton[0]);
    rootIndex = Animation_GetLimbIndex(skeleton[0], skeleton);
    baseRot = jointTable[rootIndex];

    overrideLimbIndex = (rootIndex - 1) != 0 ? rootIndex - 1 : 0;
    postLimbIndex = (rootIndex - 1) != 0 ? rootIndex - 1 : 0;

    if (mode & 1) {
        baseTrans.x = rootLimb->trans.x;
        baseTrans.y = rootLimb->trans.y;
        baseTrans.z = rootLimb->trans.z;
    } else {
        baseTrans.x = jointTable[0].x;
        baseTrans.y = jointTable[0].y;
        baseTrans.z = jointTable[0].z;
    }

    dList = rootLimb->dList;
    Matrix_Push(&gGfxMatrix);

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform of the rootLimb
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_LIMB(rootLimb, data) + rootIndex | (u32) extraPtrData,
                                        G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform of the rootLimb
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_LIMB(rootLimb, data) + rootIndex | (u32) extraPtrData,
                                       G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    }

    if (overrideLimbDraw == NULL) {
        override = false;
    } else {
        override = overrideLimbDraw(rootIndex - 1, &dList, &baseTrans, &baseRot, data);
    }
    if (!override) {
        Matrix_Translate(gCalcMatrix, baseTrans.x, baseTrans.y, baseTrans.z, MTXF_APPLY);
        Matrix_RotateZ(gCalcMatrix, baseRot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gCalcMatrix, baseRot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gCalcMatrix, baseRot.x * M_DTOR, MTXF_APPLY);
        if (dList != NULL) {
            Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);
        }
    }

    if (postLimbDraw != NULL) {
        postLimbDraw(rootIndex - 1, &baseRot, data);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    Matrix_Pop(&gGfxMatrix);

    if (rootLimb->child != NULL) {
        Animation_DrawLimb(mode, rootLimb->child, skeleton, jointTable, overrideLimbDraw, postLimbDraw, data);
    }

    Matrix_Pop(&gCalcMatrix);

    if (mode >= 2) {
        Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
    }
}

#if 1
RECOMP_PATCH void Animation_DrawLimb(s32 mode, Limb* limb, Limb** skeleton, Vec3f* jointTable,
                                     OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data) {
    bool override;
    s32 limbIndex;
    Gfx* dList;
    Vec3f trans;
    Vec3f rot;
    Vec3f pos;
    Vec3f origin = { 0.0f, 0.0f, 0.0f };
    Actor* actor = data;

    ArwingInfoRecomp* teamArwing = (ArwingInfoRecomp*) data;
    void* extraPtrData = NULL;

    // @recomp: Check if the incoming actor is an ActorTeamArwing
    // If it is, include it's actor address in the tag
    if (data == &gActorTeamArwing_recomp) {
        extraPtrData = teamArwing->actorPtr;
        // recomp_printf("Animation_DrawSkeleton\n");
        // recomp_printf("Team Arwing detected\n");
    }

    Matrix_Push(&gCalcMatrix);

    limbIndex = Animation_GetLimbIndex(limb, skeleton);

    limb = SEGMENTED_TO_VIRTUAL(limb);
    rot = jointTable[limbIndex];
    trans.x = limb->trans.x;
    trans.y = limb->trans.y;
    trans.z = limb->trans.z;
    dList = limb->dList;
    Matrix_Push(&gGfxMatrix);

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_LIMB(limb, data) + limbIndex | (u32) extraPtrData, G_EX_PUSH,
                                        G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_LIMB(limb, data) + limbIndex | (u32) extraPtrData, G_EX_PUSH,
                                       G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    }

    if (overrideLimbDraw == NULL) {
        override = false;
    } else {
        override = overrideLimbDraw(limbIndex - 1, &dList, &trans, &rot, data);
    }
    if (!override) {
        Matrix_Translate(gCalcMatrix, trans.x, trans.y, trans.z, MTXF_APPLY);
        Matrix_RotateZ(gCalcMatrix, rot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gCalcMatrix, rot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gCalcMatrix, rot.x * M_DTOR, MTXF_APPLY);
        if (dList != NULL) {
            if (mode >= 2) {
                Matrix_MultVec3f(gCalcMatrix, &origin, &pos);
                if (mode != 5) {
                    Display_SetSecondLight(&pos);
                }
            }
            Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);
        }
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    if (postLimbDraw != NULL) {
        postLimbDraw(limbIndex - 1, &rot, data);
    }
    Matrix_Pop(&gGfxMatrix);
    if (limb->child != NULL) {
        Animation_DrawLimb(mode, limb->child, skeleton, jointTable, overrideLimbDraw, postLimbDraw, data);
    }
    Matrix_Pop(&gCalcMatrix);
    if (limb->sibling != NULL) {
        Animation_DrawLimb(mode, limb->sibling, skeleton, jointTable, overrideLimbDraw, postLimbDraw, data);
    }
}

#endif

void Animation_DrawLimbEnding(s32 mode, Limb* limb, Limb** skeleton, Vec3f* jointTable,
                              OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data);

void Animation_DrawSkeletonEnding(s32 mode, Limb** skeletonSegment, Vec3f* jointTable,
                                  OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data,
                                  Matrix* transform) {
    bool override;
    Limb** skeleton;
    Limb* rootLimb;
    s32 rootIndex;
    Gfx* dList;
    Vec3f baseTrans;
    Vec3f baseRot;
    s32 overrideLimbIndex;
    s32 postLimbIndex;

    ArwingInfoRecomp* teamArwing = (ArwingInfoRecomp*) data;
    void* extraPtrData = NULL;

    // @recomp: Check if the incoming actor is an ActorTeamArwing
    // If it is, include it's actor address in the tag
    if (data == &gActorTeamArwing_recomp) {
        extraPtrData = teamArwing->actorPtr;
        // recomp_printf("Animation_DrawSkeleton\n");
        // recomp_printf("Team Arwing detected\n");
    }

    Matrix_Push(&gCalcMatrix);
    Matrix_Copy(gCalcMatrix, transform);

    skeleton = SEGMENTED_TO_VIRTUAL(skeletonSegment);
    rootLimb = SEGMENTED_TO_VIRTUAL(skeleton[0]);
    rootIndex = Animation_GetLimbIndex(skeleton[0], skeleton);
    baseRot = jointTable[rootIndex];

    overrideLimbIndex = (rootIndex - 1) != 0 ? rootIndex - 1 : 0;
    postLimbIndex = (rootIndex - 1) != 0 ? rootIndex - 1 : 0;

    if (mode & 1) {
        baseTrans.x = rootLimb->trans.x;
        baseTrans.y = rootLimb->trans.y;
        baseTrans.z = rootLimb->trans.z;
    } else {
        baseTrans.x = jointTable[0].x;
        baseTrans.y = jointTable[0].y;
        baseTrans.z = jointTable[0].z;
    }

    dList = rootLimb->dList;
    Matrix_Push(&gGfxMatrix);

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform of the rootLimb
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_LIMB(rootLimb, data) + rootIndex | (u32) extraPtrData,
                                        G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform of the rootLimb
        // gEXMatrixGroupSimpleNormal(gMasterDisp++, TAG_LIMB(rootLimb, data) + rootIndex| (u32) extraPtrData,
        // G_EX_PUSH, G_MTX_MODELVIEW,
        //                                G_EX_EDIT_ALLOW);
        // @recomp Tag the transform.
        gEXMatrixGroupSimple(gMasterDisp++, TAG_LIMB(rootLimb, data) + rootIndex | (u32) extraPtrData, G_EX_PUSH,
                             G_MTX_MODELVIEW, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);
    }

    if (overrideLimbDraw == NULL) {
        override = false;
    } else {
        override = overrideLimbDraw(rootIndex - 1, &dList, &baseTrans, &baseRot, data);
    }
    if (!override) {
        Matrix_Translate(gCalcMatrix, baseTrans.x, baseTrans.y, baseTrans.z, MTXF_APPLY);
        Matrix_RotateZ(gCalcMatrix, baseRot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gCalcMatrix, baseRot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gCalcMatrix, baseRot.x * M_DTOR, MTXF_APPLY);
        if (dList != NULL) {
            Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);
        }
    }

    if (postLimbDraw != NULL) {
        postLimbDraw(rootIndex - 1, &baseRot, data);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    Matrix_Pop(&gGfxMatrix);

    if (rootLimb->child != NULL) {
        Animation_DrawLimbEnding(mode, rootLimb->child, skeleton, jointTable, overrideLimbDraw, postLimbDraw, data);
    }

    Matrix_Pop(&gCalcMatrix);

    if (mode >= 2) {
        Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
    }
}

void Animation_DrawLimbEnding(s32 mode, Limb* limb, Limb** skeleton, Vec3f* jointTable,
                              OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data) {
    bool override;
    s32 limbIndex;
    Gfx* dList;
    Vec3f trans;
    Vec3f rot;
    Vec3f pos;
    Vec3f origin = { 0.0f, 0.0f, 0.0f };
    Actor* actor = data;

    ArwingInfoRecomp* teamArwing = (ArwingInfoRecomp*) data;
    void* extraPtrData = NULL;

    // @recomp: Check if the incoming actor is an ActorTeamArwing
    // If it is, include it's actor address in the tag
    if (data == &gActorTeamArwing_recomp) {
        extraPtrData = teamArwing->actorPtr;
        // recomp_printf("Animation_DrawSkeleton\n");
        // recomp_printf("Team Arwing detected\n");
    }

    Matrix_Push(&gCalcMatrix);

    limbIndex = Animation_GetLimbIndex(limb, skeleton);

    limb = SEGMENTED_TO_VIRTUAL(limb);
    rot = jointTable[limbIndex];
    trans.x = limb->trans.x;
    trans.y = limb->trans.y;
    trans.z = limb->trans.z;
    dList = limb->dList;
    Matrix_Push(&gGfxMatrix);

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_LIMB(limb, data) + limbIndex | (u32) extraPtrData, G_EX_PUSH,
                                        G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform
        gEXMatrixGroupSimple(gMasterDisp++, TAG_LIMB(limb, data) + limbIndex | (u32) extraPtrData, G_EX_PUSH,
                             G_MTX_MODELVIEW, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE,
                             G_EX_ORDER_LINEAR, G_EX_EDIT_NONE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_SKIP);
    }

    if (overrideLimbDraw == NULL) {
        override = false;
    } else {
        override = overrideLimbDraw(limbIndex - 1, &dList, &trans, &rot, data);
    }
    if (!override) {
        Matrix_Translate(gCalcMatrix, trans.x, trans.y, trans.z, MTXF_APPLY);
        Matrix_RotateZ(gCalcMatrix, rot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gCalcMatrix, rot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gCalcMatrix, rot.x * M_DTOR, MTXF_APPLY);
        if (dList != NULL) {
            if (mode >= 2) {
                Matrix_MultVec3f(gCalcMatrix, &origin, &pos);
                if (mode != 5) {
                    Display_SetSecondLight(&pos);
                }
            }
            Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);
        }
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    if (postLimbDraw != NULL) {
        postLimbDraw(limbIndex - 1, &rot, data);
    }
    Matrix_Pop(&gGfxMatrix);
    if (limb->child != NULL) {
        Animation_DrawLimbEnding(mode, limb->child, skeleton, jointTable, overrideLimbDraw, postLimbDraw, data);
    }
    Matrix_Pop(&gCalcMatrix);
    if (limb->sibling != NULL) {
        Animation_DrawLimbEnding(mode, limb->sibling, skeleton, jointTable, overrideLimbDraw, postLimbDraw, data);
    }
}

void Animation_DrawSkeletonOriginal(s32 mode, Limb** skeletonSegment, Vec3f* jointTable,
                                    OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data,
                                    Matrix* transform) {
    bool override;
    Limb** skeleton;
    Limb* rootLimb;
    s32 rootIndex;
    Gfx* dList;
    Vec3f baseTrans;
    Vec3f baseRot;

    Matrix_Push(&gCalcMatrix);
    Matrix_Copy(gCalcMatrix, transform);

    skeleton = SEGMENTED_TO_VIRTUAL(skeletonSegment);
    rootLimb = SEGMENTED_TO_VIRTUAL(skeleton[0]);
    rootIndex = Animation_GetLimbIndex(skeleton[0], skeleton);
    baseRot = jointTable[rootIndex];

    if (mode & 1) {
        baseTrans.x = rootLimb->trans.x;
        baseTrans.y = rootLimb->trans.y;
        baseTrans.z = rootLimb->trans.z;
    } else {
        baseTrans.x = jointTable[0].x;
        baseTrans.y = jointTable[0].y;
        baseTrans.z = jointTable[0].z;
    }

    dList = rootLimb->dList;
    Matrix_Push(&gGfxMatrix);

    if (overrideLimbDraw == NULL) {
        override = false;
    } else {
        override = overrideLimbDraw(rootIndex - 1, &dList, &baseTrans, &baseRot, data);
    }
    if (!override) {
        Matrix_Translate(gCalcMatrix, baseTrans.x, baseTrans.y, baseTrans.z, MTXF_APPLY);
        Matrix_RotateZ(gCalcMatrix, baseRot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gCalcMatrix, baseRot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gCalcMatrix, baseRot.x * M_DTOR, MTXF_APPLY);
        if (dList != NULL) {
            Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);
        }
    }
    if (postLimbDraw != NULL) {
        postLimbDraw(rootIndex - 1, &baseRot, data);
    }
    Matrix_Pop(&gGfxMatrix);

    if (rootLimb->child != NULL) {
        Animation_DrawLimbOriginal(mode, rootLimb->child, skeleton, jointTable, overrideLimbDraw, postLimbDraw, data);
    }
    Matrix_Pop(&gCalcMatrix);

    if (mode >= 2) {
        Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
    }
}

void Animation_DrawLimbOriginal(s32 mode, Limb* limb, Limb** skeleton, Vec3f* jointTable,
                                OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data) {
    bool override;
    s32 limbIndex;
    Gfx* dList;
    Vec3f trans;
    Vec3f rot;
    Vec3f pos;
    Vec3f origin = { 0.0f, 0.0f, 0.0f };

    Matrix_Push(&gCalcMatrix);

    limbIndex = Animation_GetLimbIndex(limb, skeleton);
    limb = SEGMENTED_TO_VIRTUAL(limb);
    rot = jointTable[limbIndex];
    trans.x = limb->trans.x;
    trans.y = limb->trans.y;
    trans.z = limb->trans.z;
    dList = limb->dList;
    Matrix_Push(&gGfxMatrix);

    if (overrideLimbDraw == NULL) {
        override = false;
    } else {
        override = overrideLimbDraw(limbIndex - 1, &dList, &trans, &rot, data);
    }
    if (!override) {
        Matrix_Translate(gCalcMatrix, trans.x, trans.y, trans.z, MTXF_APPLY);
        Matrix_RotateZ(gCalcMatrix, rot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gCalcMatrix, rot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gCalcMatrix, rot.x * M_DTOR, MTXF_APPLY);
        if (dList != NULL) {
            if (mode >= 2) {
                Matrix_MultVec3f(gCalcMatrix, &origin, &pos);
                if (mode != 5) {
                    Display_SetSecondLight(&pos);
                }
            }
            Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);
        }
    }

    if (postLimbDraw != NULL) {
        postLimbDraw(limbIndex - 1, &rot, data);
    }
    Matrix_Pop(&gGfxMatrix);
    if (limb->child != NULL) {
        Animation_DrawLimbOriginal(mode, limb->child, skeleton, jointTable, overrideLimbDraw, postLimbDraw, data);
    }
    Matrix_Pop(&gCalcMatrix);
    if (limb->sibling != NULL) {
        Animation_DrawLimbOriginal(mode, limb->sibling, skeleton, jointTable, overrideLimbDraw, postLimbDraw, data);
    }
}

void Animation_DrawLimb_SkipInterpolation(s32 mode, Limb* limb, Limb** skeleton, Vec3f* jointTable,
                                          OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data) {
    bool override;
    s32 limbIndex;
    Gfx* dList;
    Vec3f trans;
    Vec3f rot;
    Vec3f pos;
    Vec3f origin = { 0.0f, 0.0f, 0.0f };
    Actor* actor = data;

    Matrix_Push(&gCalcMatrix);

    limbIndex = Animation_GetLimbIndex(limb, skeleton);

    limb = SEGMENTED_TO_VIRTUAL(limb);
    rot = jointTable[limbIndex];
    trans.x = limb->trans.x;
    trans.y = limb->trans.y;
    trans.z = limb->trans.z;
    dList = limb->dList;
    Matrix_Push(&gGfxMatrix);

    // @recomp Tag the transform.
    gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_LIMB(limb, data) + limbIndex, G_EX_PUSH, G_MTX_MODELVIEW,
                                    G_EX_EDIT_ALLOW);

    if (overrideLimbDraw == NULL) {
        override = false;
    } else {
        override = overrideLimbDraw(limbIndex - 1, &dList, &trans, &rot, data);
    }
    if (!override) {
        Matrix_Translate(gCalcMatrix, trans.x, trans.y, trans.z, MTXF_APPLY);
        Matrix_RotateZ(gCalcMatrix, rot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gCalcMatrix, rot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gCalcMatrix, rot.x * M_DTOR, MTXF_APPLY);
        if (dList != NULL) {
            if (mode >= 2) {
                Matrix_MultVec3f(gCalcMatrix, &origin, &pos);
                if (mode != 5) {
                    Display_SetSecondLight(&pos);
                }
            }
            Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);
        }
    }
    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    if (postLimbDraw != NULL) {
        postLimbDraw(limbIndex - 1, &rot, data);
    }
    Matrix_Pop(&gGfxMatrix);
    if (limb->child != NULL) {
        Animation_DrawLimb_SkipInterpolation(mode, limb->child, skeleton, jointTable, overrideLimbDraw, postLimbDraw,
                                             data);
    }
    Matrix_Pop(&gCalcMatrix);
    if (limb->sibling != NULL) {
        Animation_DrawLimb_SkipInterpolation(mode, limb->sibling, skeleton, jointTable, overrideLimbDraw, postLimbDraw,
                                             data);
    }
}

void Animation_DrawSkeleton_SkipInterpolation(s32 mode, Limb** skeletonSegment, Vec3f* jointTable,
                                              OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* data,
                                              Matrix* transform) {
    bool override;
    Limb** skeleton;
    Limb* rootLimb;
    s32 rootIndex;
    Gfx* dList;
    Vec3f baseTrans;
    Vec3f baseRot;
    s32 overrideLimbIndex;
    s32 postLimbIndex;
    Actor* actor = (Actor*) data;

    Matrix_Push(&gCalcMatrix);
    Matrix_Copy(gCalcMatrix, transform);

    skeleton = SEGMENTED_TO_VIRTUAL(skeletonSegment);
    rootLimb = SEGMENTED_TO_VIRTUAL(skeleton[0]);
    rootIndex = Animation_GetLimbIndex(skeleton[0], skeleton);
    baseRot = jointTable[rootIndex];

    overrideLimbIndex = (rootIndex - 1) != 0 ? rootIndex - 1 : 0;
    postLimbIndex = (rootIndex - 1) != 0 ? rootIndex - 1 : 0;

    if (mode & 1) {
        baseTrans.x = rootLimb->trans.x;
        baseTrans.y = rootLimb->trans.y;
        baseTrans.z = rootLimb->trans.z;
    } else {
        baseTrans.x = jointTable[0].x;
        baseTrans.y = jointTable[0].y;
        baseTrans.z = jointTable[0].z;
    }

    dList = rootLimb->dList;
    Matrix_Push(&gGfxMatrix);

    // @recomp Tag the transform of the rootLimb
    gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_LIMB(rootLimb, data) + rootIndex, G_EX_PUSH, G_MTX_MODELVIEW,
                                    G_EX_EDIT_ALLOW);

    if (overrideLimbDraw == NULL) {
        override = false;
    } else {
        override = overrideLimbDraw(rootIndex - 1, &dList, &baseTrans, &baseRot, data);
    }
    if (!override) {
        Matrix_Translate(gCalcMatrix, baseTrans.x, baseTrans.y, baseTrans.z, MTXF_APPLY);
        Matrix_RotateZ(gCalcMatrix, baseRot.z * M_DTOR, MTXF_APPLY);
        Matrix_RotateY(gCalcMatrix, baseRot.y * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gCalcMatrix, baseRot.x * M_DTOR, MTXF_APPLY);
        if (dList != NULL) {
            Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
            Matrix_SetGfxMtx(&gMasterDisp);
            gSPDisplayList(gMasterDisp++, dList);
        }
    }

    if (postLimbDraw != NULL) {
        postLimbDraw(rootIndex - 1, &baseRot, data);
    }

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);

    Matrix_Pop(&gGfxMatrix);

    if (rootLimb->child != NULL) {
        Animation_DrawLimb_SkipInterpolation(mode, rootLimb->child, skeleton, jointTable, overrideLimbDraw,
                                             postLimbDraw, data);
    }

    Matrix_Pop(&gCalcMatrix);

    if (mode >= 2) {
        Matrix_Mult(gGfxMatrix, gCalcMatrix, MTXF_APPLY);
    }
}

#if 1
RECOMP_PATCH void Display_Arwing_Skel(ArwingInfo* arwing) {
    Vec3f frameTable[30];
    s32 drawFace;

    Matrix_Push(&gGfxMatrix);

    arwing->laserGunsXpos = 0.0f;
    if (arwing->laserGunsYpos < -7.0f) {
        arwing->laserGunsXpos = (-arwing->laserGunsYpos - 7.0f) * 2.5f;
    }

    if (gGameState == GSTATE_PLAY) {
        Animation_DrawSkeleton(1, aAwArwingSkel, gPlayer[0].jointTable, Display_ArwingOverrideLimbDraw, NULL, arwing,
                               &gIdentityMatrix);
    } else {
        if (gGameState == GSTATE_MENU) {
            Animation_GetFrameData(&aAwWingsHalfOpenAnim, 0, frameTable);
        } else {
            Animation_GetFrameData(&aAwWingsClosedAnim, 0, frameTable);
        }
        Animation_DrawSkeleton(1, aAwArwingSkel, frameTable, Display_ArwingOverrideLimbDraw, NULL, arwing,
                               &gIdentityMatrix);
    }

    D_display_800CA22C = false;

    drawFace = arwing->drawFace;
    if (D_display_800CA220 != 0) {
        drawFace = true;
    }

    if (gCamera1Skipped) {
        // Skip
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedSkipAll(gMasterDisp++, TAG_FACE + drawFace, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);
    } else {
        // @recomp Tag the transform
        gEXMatrixGroupDecomposedNormal(gMasterDisp++, TAG_FACE + drawFace, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_ALLOW);
    }

    if (drawFace != 0) {
        Matrix_Push(&gGfxMatrix);
        Matrix_Translate(gGfxMatrix, 0.0f, 6.4f, -16.5f, MTXF_APPLY);
        Matrix_RotateY(gGfxMatrix, arwing->teamFaceYrot * M_DTOR, MTXF_APPLY);
        Matrix_RotateX(gGfxMatrix, arwing->teamFaceXrot * M_DTOR, MTXF_APPLY);
        Matrix_Scale(gGfxMatrix, 1.0f / 70.925f, 1.0f / 70.925f, 1.0f / 70.925f, MTXF_APPLY);

        if (gGameState == GSTATE_ENDING) {
            Matrix_Scale(gGfxMatrix, 0.95f, 0.95f, 0.95f, MTXF_APPLY);
        }

        Matrix_SetGfxMtx(&gMasterDisp);

        if (gExpertMode) {
            gSPDisplayList(gMasterDisp++, sExpertFaceDL[drawFace - 1]);
        } else {
            gSPDisplayList(gMasterDisp++, sFaceDL[drawFace - 1]);
        }
        Matrix_Pop(&gGfxMatrix);
    }

    Matrix_Translate(gGfxMatrix, 0.0f, 17.2f, -25.8f, MTXF_APPLY);
    Matrix_RotateX(gGfxMatrix, arwing->cockpitGlassXrot * M_DTOR, MTXF_APPLY);
    Matrix_SetGfxMtx(&gMasterDisp);
    RCP_SetupDL_64_2();

    if ((gGameState == GSTATE_PLAY) && (gPlayer[0].state == PLAYERSTATE_LEVEL_INTRO) &&
        (gCurrentLevel == LEVEL_CORNERIA)) {
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 120);
        gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
        gSPDisplayList(gMasterDisp++, aAwCockpitGlassDL);
        RCP_SetupDL_46();
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 100);
        gSPDisplayList(gMasterDisp++, aAwCockpitGlassCsDL);
    } else {
        RCP_SetupDL_46();
        gDPSetPrimColor(gMasterDisp++, 0x00, 0x00, 255, 255, 255, 140);
        gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
        gSPDisplayList(gMasterDisp++, aAwCockpitGlassDL);
    }

    gSPSetGeometryMode(gMasterDisp++, G_CULL_BACK);
    Matrix_Pop(&gGfxMatrix);

    // @recomp Pop the transform id.
    gEXPopMatrixGroup(gMasterDisp++, G_MTX_MODELVIEW);
}
#endif

#if 1
RECOMP_PATCH void Andross_AndPassage_Draw(AndPassage* this) {
    Vec3f frameTable[20];

    Animation_GetFrameData(&aVe2AndrossGateAnim, this->unk_44, frameTable);
    if (gLevelMode == LEVELMODE_ALL_RANGE) {
        gSPClearGeometryMode(gMasterDisp++, G_CULL_BACK);
    }
    // @recomp: Call original function without matrix tagging for these doors so RT64 handles matching
    Animation_DrawSkeletonOriginal(1, aVe2AndrossGateSkel, frameTable, Andross_801935B4, NULL, NULL, &gIdentityMatrix);
}

#endif

#endif