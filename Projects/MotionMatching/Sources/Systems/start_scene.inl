#include <ecs/ecs.h>
#include <Engine/camera.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Application/application.h>
#include <Engine/Render/Texture/textures.h>
#include <Engine/Render/material.h>
#include <Engine/Render/skybox.h>
#include <Engine/Render/debug_arrow.h>
#include "Animation/AnimationDatabase/animation_database.h"
#include <Engine/Render/mesh_render.h>
#include "Animation/AnimationDatabase/animation_preprocess.h"
#include "Animation/animation_player.h"
#include "Animation/third_person_controller.h"
#include "Animation/person_controller.h"

#include "Animation/Test/animation_tester.h"
#include <Engine/input.h>
#include "Animation/settings.h"
#include <Engine/Resources/resources.h>
#include <Engine/imgui/imgui.h>

#define CUSTOM_TYPE \
MACRO(PersonController)\
MACRO(DebugArrow)

#define MACRO(T) REG_TYPE(T) EDIT_STUB(T)

CUSTOM_TYPE

REG_TYPE(AnimationPlayer)
REG_TYPE(AnimationTester)
REG_TYPE(ThirdPersonController)
REG_TYPE(AnimationDataBasePtr)

void write_tree(aiNode* root, int d = 1)
{
  debug_log("%*c%s",d, ' ', root->mName.C_Str());
  for (int i = 0, n = root->mNumChildren; i < n; i++)
    write_tree(root->mChildren[i], d + 1);
}

EVENT(ecs::SystemTag::Game) init_anim_settings(const ecs::OnSceneCreated &,
  Settings &settings,
  SettingsContainer &settingsContainer)
{
  load_object(settingsContainer, "settings.bin");
  settingsContainer.after_loading();

  load_object(settings, "man_property.bin");
}


EVENT() scene_destroy(
  const ecs::OnEntityDestroyed &,
  const AnimationDataBasePtr dataBase,
  const Settings &settings,
  const SettingsContainer &settingsContainer)
{
  dataBase->save_runtime_parameters();
  
  save_object(settings, "man_property.bin");
  
  save_object(settingsContainer, "settings.bin");
  std::fflush(stdout);
}
