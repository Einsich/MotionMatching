#pragma once
#include "3dmath.h"
#include "AnimationTree/animation_tree.h"

void process_IK(AnimationTree &tree, AnimationCadr &cadr, const mat4 &toWorld, vec3 target_point, vec3 target_normal, int node,  int node_static_node);
