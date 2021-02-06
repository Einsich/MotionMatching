#pragma once
#include "CommonCode/common.h"
#include "animation_database.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

AnimationDataBasePtr animation_preprocess(Assimp::Importer& importer, aiNode *root);