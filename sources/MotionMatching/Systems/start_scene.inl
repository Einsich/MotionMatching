#include <ecs.h>
#include <render/debug_arrow.h>
#include <resources/resource_registration.h>
#include "Animation/AnimationDatabase/animation_database.h"
#include "Animation/animation_player.h"
#include "Animation/third_person_controller.h"
#include "Animation/person_controller.h"
#include "Animation/Test/animation_tester.h"
#include "Animation/Test/animation_test.h"
#include "Animation/settings.h"

#define CUSTOM_TYPE \
MACRO(PersonController)\
MACRO(DebugArrow)

#define MACRO(T) ECS_DECLARE_TYPE(T)

CUSTOM_TYPE

ECS_DECLARE_TYPE(AnimationPlayer)
ECS_DECLARE_TYPE(AnimationTester)
ECS_DECLARE_TYPE(ThirdPersonController)
ECS_DECLARE_TYPE(AnimationDataBasePtr)


EVENT(scene=game) init_anim_settings(const ecs::OnEntityCreated &,
  AnimationDataBasePtr dataBase,
  vector<AnimationTest> &tests,
  Settings &settings,
  SettingsContainer &settingsContainer)
{  
  load_object(tests, "AnimationTests.bin");
  load_object(settingsContainer, "settings.bin");
  settingsContainer.after_loading();

  load_object(settings, "man_property.bin");
  dataBase->acceleration_structs(true);
}

EVENT() scene_destroy(
  const ecs::OnEntityDestroyed &,
  vector<AnimationTest> &tests,
  const Settings &settings,
  const SettingsContainer &settingsContainer)
{
  save_object(tests, "AnimationTests.bin");
  
  save_object(settings, "man_property.bin");
  
  save_object(settingsContainer, "settings.bin");
}
