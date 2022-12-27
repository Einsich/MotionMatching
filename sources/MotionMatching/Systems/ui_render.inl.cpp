#include "ui_render.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache briefing_ui__cache__;

static ecs::QueryCache motion_matching_statistic__cache__;

static ecs::QueryCache current_anim_index__cache__;

static ecs::QueryCache menu_ui__cache__;

static ecs::QueryCache mm_early_text_perf__cache__;

static void briefing_ui_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), briefing_ui__cache__, briefing_ui);
}

static void briefing_ui_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), briefing_ui__cache__, briefing_ui);
}

static void motion_matching_statistic_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), motion_matching_statistic__cache__, motion_matching_statistic);
}

static void motion_matching_statistic_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), motion_matching_statistic__cache__, motion_matching_statistic);
}

static void current_anim_index_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), current_anim_index__cache__, current_anim_index);
}

static void current_anim_index_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), current_anim_index__cache__, current_anim_index);
}

static void menu_ui_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiMenuRender &>(event), menu_ui__cache__, menu_ui);
}

static void menu_ui_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiMenuRender &>(event), menu_ui__cache__, menu_ui);
}

static void mm_early_text_perf_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), mm_early_text_perf__cache__, mm_early_text_perf);
}

static void mm_early_text_perf_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), mm_early_text_perf__cache__, mm_early_text_perf);
}

static void registration_pull_ui_render()
{
  ecs::register_event(ecs::EventDescription(
  "",
  "briefing_ui",
  &briefing_ui__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &briefing_ui_handler, &briefing_ui_single_handler),
  ecs::EventIndex<ImguiRender>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "motion_matching_statistic",
  &motion_matching_statistic__cache__,
  {
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<AnimationPlayer>()},
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Settings>()},
    {"updateMMStatistic", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()}
  },
  {
    {"thirdPersonController", ecs::TypeIndex<ThirdPersonController>::value}
  },
  {},
  {},
  {},
  {},
  &motion_matching_statistic_handler, &motion_matching_statistic_single_handler),
  ecs::EventIndex<ImguiRender>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "current_anim_index",
  &current_anim_index__cache__,
  {
    {"animationPlayer", ecs::get_type_index<AnimationPlayer>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<AnimationPlayer>()}
  },
  {
    {"thirdPersonController", ecs::TypeIndex<ThirdPersonController>::value}
  },
  {},
  {},
  {},
  {},
  &current_anim_index_handler, &current_anim_index_single_handler),
  ecs::EventIndex<ImguiRender>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "menu_ui",
  &menu_ui__cache__,
  {
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Settings>()},
    {"settingsContainer", ecs::get_type_index<SettingsContainer>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SettingsContainer>()}
  },
  {},
  {},
  {},
  {},
  {},
  &menu_ui_handler, &menu_ui_single_handler),
  ecs::EventIndex<ImguiMenuRender>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "mm_early_text_perf",
  &mm_early_text_perf__cache__,
  {
    {"settings", ecs::get_type_index<Settings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Settings>()}
  },
  {},
  {},
  {},
  {},
  {},
  &mm_early_text_perf_handler, &mm_early_text_perf_single_handler),
  ecs::EventIndex<ImguiRender>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_ui_render)
