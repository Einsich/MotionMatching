#pragma once
#include "CommonCode/common.h"
#include "animation_channel.h"
#include "animation.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "CommonCode/GameObject/game_object.h"

AnimationPlayerPtr animation_preprocess(Assimp::Importer& importer, GameObjectPtr go, aiNode *root);