#include "user_input.inl"
//Code-generator production

ecs::QueryDescription is_game_started_descr("is_game_started", {
  {ecs::get_type_description<bool>("gameStarted"), false}
}, {
});

template<typename Callable>
void is_game_started(Callable lambda)
{
  ecs::perform_query<bool&>
  (is_game_started_descr, lambda);
}


ecs::QueryDescription select_map_query_descr("select_map_query", {
  {ecs::get_type_description<MapArrays>("map_arrays"), false}
}, {
});

template<typename Callable>
void select_map_query(Callable lambda)
{
  ecs::perform_query<MapArrays&>
  (select_map_query_descr, lambda);
}


void change_invasion_weight_func();

ecs::SystemDescription change_invasion_weight_descr("change_invasion_weight", {
  {ecs::get_type_description<float>("invasion_weight"), false},
  {ecs::get_type_description<uint>("forces"), false},
  {ecs::get_type_description<ecs::Tag>("isPlayer"), false}
}, {
}, {},
change_invasion_weight_func, ecs::SystemOrder::UI, ecs::SystemTag::Game,
{},
{});

void change_invasion_weight_func()
{
  ecs::perform_system(change_invasion_weight_descr, change_invasion_weight);
}

void check_mouse_over_ui_func();

ecs::SystemDescription check_mouse_over_ui_descr("check_mouse_over_ui", {
}, {
}, {},
check_mouse_over_ui_func, ecs::SystemOrder::UI, ecs::SystemTag::Game,
{},
{});

void check_mouse_over_ui_func()
{
  ecs::perform_system(check_mouse_over_ui_descr, check_mouse_over_ui);
}

void start_game_handler(const KeyDownEvent<SDLK_RETURN> &event);
void start_game_singl_handler(const KeyDownEvent<SDLK_RETURN> &event, ecs::EntityId eid);

ecs::EventDescription<KeyDownEvent<SDLK_RETURN>> start_game_descr("start_game", {
  {ecs::get_type_description<ecs::Tag>("isPlayer"), false}
}, {
}, {},
start_game_handler, start_game_singl_handler, ecs::SystemTag::Game);

void start_game_handler(const KeyDownEvent<SDLK_RETURN> &event)
{
  ecs::perform_event(event, start_game_descr, start_game);
}
void start_game_singl_handler(const KeyDownEvent<SDLK_RETURN> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, start_game_descr, eid, start_game);
}

void select_spawn_point_handler(const MouseButtonDownEvent<MouseButton::RightButton> &event);
void select_spawn_point_singl_handler(const MouseButtonDownEvent<MouseButton::RightButton> &event, ecs::EntityId eid);

ecs::EventDescription<MouseButtonDownEvent<MouseButton::RightButton>> select_spawn_point_descr("select_spawn_point", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<ecs::Tag>("player_spawning"), false}
}, {
}, {},
select_spawn_point_handler, select_spawn_point_singl_handler, ecs::SystemTag::Game);

void select_spawn_point_handler(const MouseButtonDownEvent<MouseButton::RightButton> &event)
{
  ecs::perform_event(event, select_spawn_point_descr, select_spawn_point);
}
void select_spawn_point_singl_handler(const MouseButtonDownEvent<MouseButton::RightButton> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, select_spawn_point_descr, eid, select_spawn_point);
}

void select_invasion_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event);
void select_invasion_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid);

ecs::EventDescription<MouseButtonDownEvent<MouseButton::LeftButton>> select_invasion_descr("select_invasion", {
  {ecs::get_type_description<uint>("landIndex"), false},
  {ecs::get_type_description<uint>("forces"), false},
  {ecs::get_type_description<vector<ecs::EntityId>>("neighbors"), false},
  {ecs::get_type_description<vector<uint>>("neighborsIdx"), false},
  {ecs::get_type_description<vector<Invasion>>("invasions"), false},
  {ecs::get_type_description<float>("invasion_weight"), false},
  {ecs::get_type_description<ecs::Tag>("isPlayer"), false}
}, {
}, {},
select_invasion_handler, select_invasion_singl_handler, ecs::SystemTag::Game);

void select_invasion_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event)
{
  ecs::perform_event(event, select_invasion_descr, select_invasion);
}
void select_invasion_singl_handler(const MouseButtonDownEvent<MouseButton::LeftButton> &event, ecs::EntityId eid)
{
  ecs::perform_event(event, select_invasion_descr, eid, select_invasion);
}


