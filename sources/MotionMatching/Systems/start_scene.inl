#include <ecs.h>
#include <render/debug_arrow.h>
#include <resources/resource_registration.h>
#include "Animation/AnimationDatabase/animation_database.h"
#include "Animation/animation_player.h"
#include "Animation/third_person_controller.h"
#include "Animation/person_controller.h"
#include "Animation/Test/animation_tester.h"
#include "Animation/settings.h"

#define CUSTOM_TYPE \
MACRO(PersonController)\
MACRO(DebugArrow)

#define MACRO(T) REG_TYPE(T) EDIT_STUB(T)

CUSTOM_TYPE

REG_TYPE(AnimationPlayer)
REG_TYPE(AnimationTester)
REG_TYPE(ThirdPersonController)
REG_TYPE(AnimationDataBasePtr)


EVENT(ecs::SystemTag::Game) init_anim_settings(const ecs::OnSceneCreated &,
  Settings &settings,
  SettingsContainer &settingsContainer)
{
  load_object(settingsContainer, "settings.bin");
  settingsContainer.after_loading();

  load_object(settings, "man_property.bin");
}

EVENT(ecs::SystemTag::GameEditor) init_blk_scene(const ecs::OnSceneCreated &)
{
  ecs::create_scene(root_path("resources/MotionMatching/Scenes/test_scene.blk"), false);
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
