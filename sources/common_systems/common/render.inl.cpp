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


void render_submenu_func();

ecs::SystemDescription render_submenu_descr("render_submenu", {
  {ecs::get_type_description<EditorRenderSettings>("settings"), false}
}, {
},
{},
{},
render_submenu_func, "ui_menu", {"editor"}, false);

void render_submenu_func()
{
  ecs::perform_system(render_submenu_descr, render_submenu);
}

void set_global_render_data_func();

ecs::SystemDescription set_global_render_data_descr("set_global_render_data", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, {
},
{},
{},
set_global_render_data_func, "render", {}, false);

void set_global_render_data_func()
{
  ecs::perform_system(set_global_render_data_descr, set_global_render_data);
}

void lod_selector_func();

ecs::SystemDescription lod_selector_descr("lod_selector", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<ecs::vector<Asset<Mesh>>>("lods_meshes"), false},
  {ecs::get_type_description<ecs::vector<float>>("lods_distances"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<vec3>("lod_selector_axis"), true},
  {ecs::get_type_description<bool>("is_enabled"), false}
}, {
},
{"frustum_culling"},
{},
lod_selector_func, "render", {}, true);

void lod_selector_func()
{
  ecs::perform_job_system(lod_selector_descr, lod_selector);
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
},
{"process_mesh_position"},
{},
frustum_culling_func, "render", {}, true);

void frustum_culling_func()
{
  ecs::perform_job_system(frustum_culling_descr, frustum_culling);
}

void process_mesh_position_func();

ecs::SystemDescription process_mesh_position_descr("process_mesh_position", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<RenderQueue>("render"), false},
  {ecs::get_type_description<bool>("is_visible"), false},
  {ecs::get_type_description<bool>("is_enabled"), false}
}, {
},
{"main_instanced_render"},
{},
process_mesh_position_func, "render", {}, false);

void process_mesh_position_func()
{
  ecs::perform_system(process_mesh_position_descr, process_mesh_position);
}

void render_sky_box_func();

ecs::SystemDescription render_sky_box_descr("render_sky_box", {
  {ecs::get_type_description<SkyBox>("skyBox"), false}
}, {
},
{},
{"main_instanced_render"},
render_sky_box_func, "render", {}, false);

void render_sky_box_func()
{
  ecs::perform_system(render_sky_box_descr, render_sky_box);
}

void render_debug_arrows_func();

ecs::SystemDescription render_debug_arrows_descr("render_debug_arrows", {
  {ecs::get_type_description<DebugArrow>("debugArrows"), false},
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false}
}, {
},
{},
{"render_sky_box"},
render_debug_arrows_func, "render", {}, false);

void render_debug_arrows_func()
{
  ecs::perform_system(render_debug_arrows_descr, render_debug_arrows);
}

void main_instanced_render_func();

ecs::SystemDescription main_instanced_render_descr("main_instanced_render", {
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false},
  {ecs::get_type_description<RenderQueue>("render"), false}
}, {
},
{},
{},
main_instanced_render_func, "render", {}, false);

void main_instanced_render_func()
{
  ecs::perform_system(main_instanced_render_descr, main_instanced_render);
}

void render_collision_func();

ecs::SystemDescription render_collision_descr("render_collision", {
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false}
}, {
},
{"render_sky_box"},
{"process_mesh_position"},
render_collision_func, "render", {}, false);

void render_collision_func()
{
  ecs::perform_system(render_collision_descr, render_collision);
}

void add_global_uniform_handler(const ecs::Event &event);
void add_global_uniform_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription add_global_uniform_descr(
  ecs::get_mutable_event_handlers<ecs::OnSceneCreated>(), "add_global_uniform", {
}, {
},
{},
{},
add_global_uniform_handler, add_global_uniform_singl_handler, {});

void add_global_uniform_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, add_global_uniform_descr, add_global_uniform);
}
void add_global_uniform_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnSceneCreated&)event, add_global_uniform_descr, eid, add_global_uniform);
}

void mesh_loader_handler(const ecs::Event &event);
void mesh_loader_singl_handler(const ecs::Event &event, ecs::EntityId eid);

ecs::EventDescription mesh_loader_descr(
  ecs::get_mutable_event_handlers<ecs::OnEntityCreated>(), "mesh_loader", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false}
}, {
},
{},
{},
mesh_loader_handler, mesh_loader_singl_handler, {});

void mesh_loader_handler(const ecs::Event &event)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, mesh_loader_descr, mesh_loader);
}
void mesh_loader_singl_handler(const ecs::Event &event, ecs::EntityId eid)
{
  ecs::perform_event((const ecs::OnEntityCreated&)event, mesh_loader_descr, eid, mesh_loader);
}


