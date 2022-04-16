#include "render.inl"
#include <ecs_perform.h>
//Code-generator production

ecs::QueryDescription find_light_descr("find_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
}, {
});

template<typename Callable>
void find_light(Callable lambda)
{
  ecs::perform_query<DirectionLight&>
  (find_light_descr, lambda);
}


ecs::QueryDescription find_collidable_entity_descr("find_collidable_entity", {
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false}
}, {
});

template<typename Callable>
void find_collidable_entity(Callable lambda)
{
  ecs::perform_query<Transform&, Asset<Mesh>&>
  (find_collidable_entity_descr, lambda);
}


ecs::QueryDescription find_matrices_descr("find_matrices", {
  {ecs::get_type_description<Transform>("transform"), true}
}, {
});

template<typename Callable>
void find_matrices(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<Transform*>
  (find_matrices_descr, eid, lambda);
}


void render_submenu_func();

ecs::SystemDescription render_submenu_descr("render_submenu", {
  {ecs::get_type_description<EditorRenderSettings>("settings"), false}
}, {
}, {"editor"},
render_submenu_func, ecs::stage::ui_menu, ecs::tags::all,
{},
{});

void render_submenu_func()
{
  ecs::perform_system(render_submenu_descr, render_submenu);
}

void set_global_render_data_func();

ecs::SystemDescription set_global_render_data_descr("set_global_render_data", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, {
}, {"game","editor"},
set_global_render_data_func, ecs::stage::render, ecs::tags::all,
{},
{});

void set_global_render_data_func()
{
  ecs::perform_system(set_global_render_data_descr, set_global_render_data);
}

void lod_selector_func();

ecs::SystemDescription lod_selector_descr("lod_selector", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<vector<Asset<Mesh>>>("lods_meshes"), false},
  {ecs::get_type_description<vector<float>>("lods_distances"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<vec3>("lod_selector_axis"), true},
  {ecs::get_type_description<bool>("is_enabled"), false}
}, {
}, {"game","editor"},
lod_selector_func, ecs::stage::render, ecs::tags::all,
{"frustum_culling"},
{});

void lod_selector_func()
{
  ecs::perform_system(lod_selector_descr, lod_selector);
}

void frustum_culling_func();

ecs::SystemDescription frustum_culling_descr("frustum_culling", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<bool>("is_visible"), false},
  {ecs::get_type_description<bool>("is_enabled"), false},
  {ecs::get_type_description<ecs::Tag>("useFrustumCulling"), false}
}, {
}, {"game","editor"},
frustum_culling_func, ecs::stage::render, ecs::tags::all,
{"process_mesh_position"},
{});

void frustum_culling_func()
{
  ecs::perform_system(frustum_culling_descr, frustum_culling);
}

void process_mesh_position_func();

ecs::SystemDescription process_mesh_position_descr("process_mesh_position", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<RenderQueue>("render"), false},
  {ecs::get_type_description<bool>("is_visible"), false},
  {ecs::get_type_description<bool>("is_enabled"), false}
}, {
}, {"game","editor"},
process_mesh_position_func, ecs::stage::render, ecs::tags::all,
{"main_instanced_render"},
{});

void process_mesh_position_func()
{
  ecs::perform_system(process_mesh_position_descr, process_mesh_position);
}

void render_sky_box_func();

ecs::SystemDescription render_sky_box_descr("render_sky_box", {
  {ecs::get_type_description<SkyBox>("skyBox"), false}
}, {
}, {"game","editor"},
render_sky_box_func, ecs::stage::render, ecs::tags::all,
{},
{"main_instanced_render"});

void render_sky_box_func()
{
  ecs::perform_system(render_sky_box_descr, render_sky_box);
}

void render_debug_arrows_func();

ecs::SystemDescription render_debug_arrows_descr("render_debug_arrows", {
  {ecs::get_type_description<DebugArrow>("debugArrows"), false},
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false}
}, {
}, {"game","editor"},
render_debug_arrows_func, ecs::stage::render, ecs::tags::all,
{},
{"render_sky_box"});

void render_debug_arrows_func()
{
  ecs::perform_system(render_debug_arrows_descr, render_debug_arrows);
}

void main_instanced_render_func();

ecs::SystemDescription main_instanced_render_descr("main_instanced_render", {
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false},
  {ecs::get_type_description<RenderQueue>("render"), false}
}, {
}, {"game","editor"},
main_instanced_render_func, ecs::stage::render, ecs::tags::all,
{},
{});

void main_instanced_render_func()
{
  ecs::perform_system(main_instanced_render_descr, main_instanced_render);
}

void render_collision_func();

ecs::SystemDescription render_collision_descr("render_collision", {
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false}
}, {
}, {"game","editor"},
render_collision_func, ecs::stage::render, ecs::tags::all,
{"render_sky_box"},
{"process_mesh_position"});

void render_collision_func()
{
  ecs::perform_system(render_collision_descr, render_collision);
}

void add_global_uniform_handler(const ecs::OnSceneCreated &event);
void add_global_uniform_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnSceneCreated> add_global_uniform_descr("add_global_uniform", {
}, {
}, {"game","editor"},
add_global_uniform_handler, add_global_uniform_singl_handler, ecs::tags::all);

void add_global_uniform_handler(const ecs::OnSceneCreated &event)
{
  ecs::perform_event(event, add_global_uniform_descr, add_global_uniform);
}
void add_global_uniform_singl_handler(const ecs::OnSceneCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, add_global_uniform_descr, eid, add_global_uniform);
}

void mesh_loader_handler(const ecs::OnEntityCreated &event);
void mesh_loader_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid);

ecs::EventDescription<ecs::OnEntityCreated> mesh_loader_descr("mesh_loader", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false}
}, {
}, {"game","editor"},
mesh_loader_handler, mesh_loader_singl_handler, ecs::tags::all);

void mesh_loader_handler(const ecs::OnEntityCreated &event)
{
  ecs::perform_event(event, mesh_loader_descr, mesh_loader);
}
void mesh_loader_singl_handler(const ecs::OnEntityCreated &event, ecs::EntityId eid)
{
  ecs::perform_event(event, mesh_loader_descr, eid, mesh_loader);
}


