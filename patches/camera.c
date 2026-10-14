#include "patches.h"

#define SQXYZ(vec) ((vec.x) * (vec.x) + (vec.y) * (vec.y) + (vec.z) * (vec.z))

void Math_Vec3f_Diff(Vec3f* l, Vec3f* r, Vec3f* dest) {
    dest->x = l->x - r->x;
    dest->y = l->y - r->y;
    dest->z = l->z - r->z;
}

void Math_Vec3f_Sum(Vec3f* l, Vec3f* r, Vec3f* dest) {
    dest->x = l->x + r->x;
    dest->y = l->y + r->y;
    dest->z = l->z + r->z;
}

f32 Math_Vec3f_DistXYZ(Vec3f* a, Vec3f* b) {
    Vec3f diff;
    Math_Vec3f_Diff(b, a, &diff);
    return sqrtf(SQXYZ(diff));
}

bool should_interpolate_perspective(Vec3f* eye, Vec3f* at) {
    static Vec3f prev_eye = { 0, 0, 0 };
    static Vec3f prev_at = { 0, 0, 0 };
    static Vec3f eye_velocity = { 0, 0, 0 };
    static Vec3f at_velocity = { 0, 0, 0 };

    Vec3f predicted_eye;
    Vec3f predicted_at;
    // Predict the new eye and at positions based on the previous velocity and positions.
    Math_Vec3f_Sum(&prev_eye, &eye_velocity, &predicted_eye);
    Math_Vec3f_Sum(&prev_at, &at_velocity, &predicted_at);

    // Calculate the current velocities from the previous and current positions.
    Math_Vec3f_Diff(eye, &prev_eye, &eye_velocity);
    Math_Vec3f_Diff(at, &prev_at, &at_velocity);

    // Compare the predicted positions to the real positions.
    float eye_dist = Math_Vec3f_DistXYZ(&predicted_eye, eye);
    float at_dist = Math_Vec3f_DistXYZ(&predicted_at, at);

    // Compare the velocities of the eye and at positions.
    float velocity_diff = Math_Vec3f_DistXYZ(&eye_velocity, &at_velocity);

    // Update the tracking for the previous positions with the new ones.
    prev_eye = *eye;
    prev_at = *at;

    // These numbers are all picked via testing.

    // If the velocity of both positions was the same, then they're moving together and should interpolate.
    if (velocity_diff <= 3.0f && eye_dist <= 100.0f && at_dist <= 100.0f) {
        return true;
    }

    // If the focus or position are basically the same across frames and the eye didn't move too far then it should
    // probably be interpolated.
    if (at_dist <= 20.0f && eye_dist <= 300.0f) {
        return true;
    }
    if (eye_dist <= 20.0f && at_dist <= 300.0f) {
        return true;
    }

    // Force camera interpolation during the all range transition
    if (gPlayer[0].state == PLAYERSTATE_START_360) {
        return true;
    }

    // Force camera interpolation during camera shake
    if (gCameraShake > 0 || gCameraShake > 0) {
        return true;
    }

    // Force camera interpolation during the first 2 robots explosion cutscenes
    // if ((gCurrentLevel == LEVEL_SECTOR_Y) && (gLevelMode == LEVELMODE_ALL_RANGE) &&
    //     ((gBosses[1].obj.status == OBJ_ACTIVE) || (gBosses[2].obj.status == OBJ_ACTIVE))) {
    //         recomp_printf("WONT SKIP BITCH\n");
    //     return true;
    // }

    switch (gCurrentLevel) {
        // For Robot destroy cutscenes
        case LEVEL_SECTOR_Y:
            if (velocity_diff > 100.0f || at_dist > 500.0f || eye_dist > 500.0f) {
                eye_velocity.x = 0.0f;
                eye_velocity.y = 0.0f;
                eye_velocity.z = 0.0f;
                at_velocity.x = 0.0f;
                at_velocity.y = 0.0f;
                at_velocity.z = 0.0f;

                // recomp_printf("velocity_diff: %f\n at_dist: %f\n eye_dist: %f\n", velocity_diff, at_dist, eye_dist);
                return false;
            }
            break;

        default:
            if (velocity_diff > 200.0f || at_dist > 150.0f || eye_dist > 300.0f) {
                eye_velocity.x = 0.0f;
                eye_velocity.y = 0.0f;
                eye_velocity.z = 0.0f;
                at_velocity.x = 0.0f;
                at_velocity.y = 0.0f;
                at_velocity.z = 0.0f;

                // recomp_printf("velocity_diff: %f\n at_dist: %f\n eye_dist: %f\n", velocity_diff, at_dist, eye_dist);
                return false;
            }
            break;
    }

    return true;
}