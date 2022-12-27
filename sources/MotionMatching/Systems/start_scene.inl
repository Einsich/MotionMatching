#include <ecs/ecs.h>
#include <ecs/registration.h>
#include <render/debug_arrow.h>
#include <resources/resource_registration.h>
#include "Animation/AnimationDatabase/animation_database.h"
#include "Animation/animation_player.h"
#include "Animation/third_person_controller.h"
#include "Animation/person_controller.h"
#include "Animation/Test/animation_tester.h"
#include "Animation/Test/animation_test.h"
#include "Animation/settings.h"

ECS_REGISTER_TYPE(Asset<AnimationDataBase>, "AnimationDataBase", ecs::DefaultType)
ECS_REGISTER_TYPE(PersonController, "PersonController", ecs::DefaultType)
ECS_REGISTER_TYPE(AnimationPlayer, "AnimationPlayer", ecs::DefaultType)
ECS_REGISTER_TYPE(AnimationTester, "AnimationTester", ecs::DefaultType)
ECS_REGISTER_TYPE(ThirdPersonController, "ThirdPersonController", ecs::DefaultType)


EVENT(tags=game) init_anim_settings(const ecs::OnEntityCreated &,
  ecs::vector<AnimationTest> &tests,
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
  ecs::vector<AnimationTest> &tests,
  const Settings &settings,
  const SettingsContainer &settingsContainer)
{
  save_object(tests, "AnimationTests.bin");
  
  save_object(settings, "man_property.bin");
  
  save_object(settingsContainer, "settings.bin");
}
