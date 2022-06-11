#include <ecs.h>
#include <type_registration.h>
#include <render/debug_arrow.h>
#include <resources/resource_registration.h>
#include "Animation/AnimationDatabase/animation_database.h"
#include "Animation/animation_player.h"
#include "Animation/third_person_controller.h"
#include "Animation/person_controller.h"
#include "Animation/Test/animation_tester.h"
#include "Animation/Test/animation_test.h"
#include "Animation/settings.h"

#define CUSTOM_TYPES \
MACRO(PersonController)\
MACRO(AnimationPlayer)\
MACRO(AnimationTester)\
MACRO(ThirdPersonController)

#define MACRO(T) ECS_REGISTER_TYPE(T, T, false, false)
CUSTOM_TYPES
#undef MACRO
ECS_REGISTER_TYPE(AnimationDataBase, Asset<AnimationDataBase>, false, false)


EVENT(scene=game) init_anim_settings(const ecs::OnEntityCreated &,
  vector<AnimationTest> &tests,
  Settings &settings,
  SettingsContainer &settingsContainer)
{  
  load_object(tests, "AnimationTests.bin");
  load_object(settingsContainer, "settings.bin");
  settingsContainer.after_loading();

  load_object(settings, "man_property.bin");
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
