#include "text_render.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache gather_text__cache__;

static ecs::QueryCache text_changed__cache__;

static ecs::QueryCache text_appear__cache__;

static ecs::QueryCache text_render__cache__;

template<typename Callable>
static void gather_text(Callable lambda)
{
  ecs::perform_query<const ecs::string&, const ecs::vector<vec4>&, ivec2, float, const vec4&>(gather_text__cache__, lambda);
}

static void text_changed_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const OnEntityEdited &>(event), text_changed__cache__, text_changed);
}

static void text_changed_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const OnEntityEdited &>(event), text_changed__cache__, text_changed);
}

static void text_appear_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), text_appear__cache__, text_appear);
}

static void text_appear_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), text_appear__cache__, text_appear);
}

static void text_render_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ImguiRender &>(event), text_render__cache__, text_render);
}

static void text_render_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ImguiRender &>(event), text_render__cache__, text_render);
}

static void registration_pull_text_render()
{
  ecs::register_query(ecs::QueryDescription(
  "",
  "gather_text",
  &gather_text__cache__,
  {
    {"text", ecs::get_type_index<ecs::string>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::string>()},
    {"glyph_buffer", ecs::get_type_index<ecs::vector<vec4>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<vec4>>()},
    {"position", ecs::get_type_index<ivec2>(), ecs::AccessType::Copy, false, ecs::is_singleton<ivec2>()},
    {"scale", ecs::get_type_index<float>(), ecs::AccessType::Copy, false, ecs::is_singleton<float>()},
    {"color", ecs::get_type_index<vec4>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<vec4>()}
  },
  {},
  {}
  ));

  ecs::register_event(ecs::EventDescription(
  "",
  "text_changed",
  &text_changed__cache__,
  {
    {"glyph_buffer", ecs::get_type_index<ecs::vector<vec4>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<vec4>>()},
    {"text", ecs::get_type_index<ecs::string>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::string>()},
    {"aligment", ecs::get_type_index<TextAlignment>(), ecs::AccessType::Copy, false, ecs::is_singleton<TextAlignment>()},
    {"pivot", ecs::get_type_index<vec2>(), ecs::AccessType::Copy, false, ecs::is_singleton<vec2>()},
    {"textRender", ecs::get_type_index<TextRender>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<TextRender>()}
  },
  {},
  {},
  {},
  {},
  {},
  &text_changed_handler, &text_changed_single_handler),
  ecs::EventIndex<OnEntityEdited>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "text_appear",
  &text_appear__cache__,
  {
    {"glyph_buffer", ecs::get_type_index<ecs::vector<vec4>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<ecs::vector<vec4>>()},
    {"text", ecs::get_type_index<ecs::string>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::string>()},
    {"aligment", ecs::get_type_index<TextAlignment>(), ecs::AccessType::Copy, false, ecs::is_singleton<TextAlignment>()},
    {"pivot", ecs::get_type_index<vec2>(), ecs::AccessType::Copy, false, ecs::is_singleton<vec2>()},
    {"textRender", ecs::get_type_index<TextRender>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<TextRender>()}
  },
  {},
  {},
  {},
  {},
  {},
  &text_appear_handler, &text_appear_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "",
  "text_render",
  &text_render__cache__,
  {
    {"editorSettings", ecs::get_type_index<EditorRenderSettings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorRenderSettings>()},
    {"textRender", ecs::get_type_index<TextRender>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<TextRender>()}
  },
  {},
  {},
  {},
  {},
  {},
  &text_render_handler, &text_render_single_handler),
  ecs::EventIndex<ImguiRender>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_text_render)
