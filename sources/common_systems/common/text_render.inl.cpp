#include "text_render.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription gather_text_descr("gather_text", {
  {ecs::get_type_hash<ecs::string>(), ecs::get_name_hash("text"), false},
  {ecs::get_type_hash<ecs::vector<vec4>>(), ecs::get_name_hash("glyph_buffer"), false},
  {ecs::get_type_hash<ivec2>(), ecs::get_name_hash("position"), false},
  {ecs::get_type_hash<float>(), ecs::get_name_hash("scale"), false},
  {ecs::get_type_hash<vec4>(), ecs::get_name_hash("color"), false}
}, {
});

template<typename Callable>
void gather_text(Callable lambda)
{
  ecs::perform_query<ecs::string&, ecs::vector<vec4>&, ivec2, float, vec4&>
  (gather_text_descr, lambda);
}


void text_render_func();

ecs::SystemDescription text_render_descr("text_render", {
  {ecs::get_type_hash<EditorRenderSettings>(), ecs::get_name_hash("editorSettings"), false},
  {ecs::get_type_hash<TextRender>(), ecs::get_name_hash("textRender"), false}
}, {
},
{},
{},
text_render_func, "render_ui", {}, false);

void text_render_func()
{
  ecs::perform_system(text_render_descr, text_render);
}

void text_changed_handler(const ecs::Event &event);
void text_changed_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription text_changed_descr(
  ecs::get_mutable_event_handlers<OnEntityEdited>(), "text_changed", {
  {ecs::get_type_hash<ecs::vector<vec4>>(), ecs::get_name_hash("glyph_buffer"), false},
  {ecs::get_type_hash<ecs::string>(), ecs::get_name_hash("text"), false},
  {ecs::get_type_hash<TextAlignment>(), ecs::get_name_hash("aligment"), false},
  {ecs::get_type_hash<vec2>(), ecs::get_name_hash("pivot"), false},
  {ecs::get_type_hash<TextRender>(), ecs::get_name_hash("textRender"), false}
}, {
},
{},
{},
text_changed_handler, text_changed_singl_handler, {});

void text_changed_handler(const ecs::Event &event)
{
  ecs::perform_event((const OnEntityEdited&)event, text_changed_descr, text_changed);
}
void text_changed_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const OnEntityEdited&)event, text_changed_descr, eid, text_changed);
}

void text_appear_handler(const ecs::Event &event);
void text_appear_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription text_appear_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "text_appear", {
  {ecs::get_type_hash<ecs::vector<vec4>>(), ecs::get_name_hash("glyph_buffer"), false},
  {ecs::get_type_hash<ecs::string>(), ecs::get_name_hash("text"), false},
  {ecs::get_type_hash<TextAlignment>(), ecs::get_name_hash("aligment"), false},
  {ecs::get_type_hash<vec2>(), ecs::get_name_hash("pivot"), false},
  {ecs::get_type_hash<TextRender>(), ecs::get_name_hash("textRender"), false}
}, {
},
{},
{},
text_appear_handler, text_appear_singl_handler, {});

void text_appear_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, text_appear_descr, text_appear);
}
void text_appear_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, text_appear_descr, eid, text_appear);
}


