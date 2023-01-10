#include "render.inl"
#include <ecs/ecs_perform.h>
//Code-generator production

static ecs::QueryCache find_light__cache__;

static ecs::QueryCache find_collidable_entity__cache__;

static ecs::QueryCache find_matrices__cache__;

static ecs::QueryCache render_submenu__cache__;

static ecs::QueryCache set_global_render_data__cache__;

static ecs::QueryCache lod_selector__cache__;

static ecs::QueryCache frustum_culling__cache__;

static ecs::QueryCache process_mesh_position__cache__;

static ecs::QueryCache render_sky_box__cache__;

static ecs::QueryCache render_debug_arrows__cache__;

static ecs::QueryCache main_instanced_render__cache__;

static ecs::QueryCache render_collision__cache__;

static ecs::QueryCache add_global_uniform__cache__;

static ecs::QueryCache mesh_loader__cache__;

template<typename Callable>
static void find_light(Callable lambda)
{
  ecs::perform_query<const DirectionLight&>(find_light__cache__, lambda);
}

template<typename Callable>
static void find_collidable_entity(Callable lambda)
{
  ecs::perform_query<const Transform&, const Asset<Mesh>&>(find_collidable_entity__cache__, lambda);
}

template<typename Callable>
static void find_matrices(ecs::EntityId eid, Callable lambda)
{
  ecs::perform_query<const ecs::vector<mat3x4>&>(find_matrices__cache__, eid, lambda);
}

static void render_submenu_implementation()
{
  ecs::perform_system(render_submenu__cache__, render_submenu);
}

static void set_global_render_data_implementation()
{
  ecs::perform_system(set_global_render_data__cache__, set_global_render_data);
}

static void lod_selector_implementation()
{
  ecs::perform_system(lod_selector__cache__, lod_selector);
}

static void frustum_culling_implementation()
{
  ecs::perform_system(frustum_culling__cache__, frustum_culling);
}

static void process_mesh_position_implementation()
{
  ecs::perform_system(process_mesh_position__cache__, process_mesh_position);
}

static void render_sky_box_implementation()
{
  ecs::perform_system(render_sky_box__cache__, render_sky_box);
}

static void render_debug_arrows_implementation()
{
  ecs::perform_system(render_debug_arrows__cache__, render_debug_arrows);
}

static void main_instanced_render_implementation()
{
  ecs::perform_system(main_instanced_render__cache__, main_instanced_render);
}

static void render_collision_implementation()
{
  ecs::perform_system(render_collision__cache__, render_collision);
}

static void add_global_uniform_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnSceneCreated &>(event), add_global_uniform__cache__, add_global_uniform);
}

static void add_global_uniform_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnSceneCreated &>(event), add_global_uniform__cache__, add_global_uniform);
}

static void mesh_loader_handler(const ecs::Event &event)
{
  ecs::perform_event(reinterpret_cast<const ecs::OnEntityCreated &>(event), mesh_loader__cache__, mesh_loader);
}

static void mesh_loader_single_handler(ecs::EntityId eid, const ecs::Event &event)
{
  ecs::perform_event(eid, reinterpret_cast<const ecs::OnEntityCreated &>(event), mesh_loader__cache__, mesh_loader);
}

static void registration_pull_render()
{
  ecs::register_query(ecs::QueryDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:52",
  "find_light",
  &find_light__cache__,
  {
    {"directionalLight", ecs::get_type_index<DirectionLight>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<DirectionLight>()}
  },
  {},
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:334",
  "find_collidable_entity",
  &find_collidable_entity__cache__,
  {
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform>()},
    {"mesh", ecs::get_type_index<Asset<Mesh>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Mesh>>()}
  },
  {},
  {}
  ));

  ecs::register_query(ecs::QueryDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:193",
  "find_matrices",
  &find_matrices__cache__,
  {
    {"bones_matrices", ecs::get_type_index<ecs::vector<mat3x4>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<mat3x4>>()}
  },
  {},
  {}
  ));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:35",
  "render_submenu",
  &render_submenu__cache__,
  {
    {"settings", ecs::get_type_index<EditorRenderSettings>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<EditorRenderSettings>()}
  },
  {},
  {},
  "imgui_menu",
  {},
  {},
  {"editor"},
  &render_submenu_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:48",
  "set_global_render_data",
  &set_global_render_data__cache__,
  {
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MainCamera>()}
  },
  {},
  {},
  "render",
  {},
  {},
  {},
  &set_global_render_data_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:65",
  "lod_selector",
  &lod_selector__cache__,
  {
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MainCamera>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform>()},
    {"lods_meshes", ecs::get_type_index<ecs::vector<Asset<Mesh>>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<Asset<Mesh>>>()},
    {"lods_distances", ecs::get_type_index<ecs::vector<float>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::vector<float>>()},
    {"mesh", ecs::get_type_index<Asset<Mesh>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Mesh>>()},
    {"lod_selector_axis", ecs::get_type_index<vec3>(), ecs::AccessType::ReadOnly, true, ecs::is_singleton<vec3>()},
    {"is_enabled", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  "render",
  {"frustum_culling"},
  {},
  {},
  &lod_selector_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:98",
  "frustum_culling",
  &frustum_culling__cache__,
  {
    {"mainCamera", ecs::get_type_index<MainCamera>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<MainCamera>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform>()},
    {"mesh", ecs::get_type_index<Asset<Mesh>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Mesh>>()},
    {"is_visible", ecs::get_type_index<bool>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<bool>()},
    {"is_enabled", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {
    {"useFrustumCulling", ecs::TypeIndex<ecs::Tag>::value}
  },
  {},
  "render",
  {"process_mesh_position"},
  {},
  {},
  &frustum_culling_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:140",
  "process_mesh_position",
  &process_mesh_position__cache__,
  {
    {"mesh", ecs::get_type_index<Asset<Mesh>>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Asset<Mesh>>()},
    {"material", ecs::get_type_index<Asset<Material>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Material>>()},
    {"transform", ecs::get_type_index<Transform>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<Transform>()},
    {"eid", ecs::get_type_index<ecs::EntityId>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<ecs::EntityId>()},
    {"render", ecs::get_type_index<RenderQueue>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<RenderQueue>()},
    {"is_visible", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()},
    {"is_enabled", ecs::get_type_index<bool>(), ecs::AccessType::Copy, false, ecs::is_singleton<bool>()}
  },
  {},
  {},
  "render",
  {"main_instanced_render"},
  {},
  {},
  &process_mesh_position_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:155",
  "render_sky_box",
  &render_sky_box__cache__,
  {
    {"skyBox", ecs::get_type_index<SkyBox>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<SkyBox>()}
  },
  {},
  {},
  "render",
  {},
  {"main_instanced_render"},
  {},
  &render_sky_box_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:165",
  "render_debug_arrows",
  &render_debug_arrows__cache__,
  {
    {"debugArrows", ecs::get_type_index<DebugArrow>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<DebugArrow>()},
    {"editorSettings", ecs::get_type_index<EditorRenderSettings>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<EditorRenderSettings>()}
  },
  {},
  {},
  "render",
  {},
  {"render_sky_box"},
  {},
  &render_debug_arrows_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:211",
  "main_instanced_render",
  &main_instanced_render__cache__,
  {
    {"editorSettings", ecs::get_type_index<EditorRenderSettings>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<EditorRenderSettings>()},
    {"render", ecs::get_type_index<RenderQueue>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<RenderQueue>()}
  },
  {},
  {},
  "render",
  {},
  {},
  {},
  &main_instanced_render_implementation));

  ecs::register_system(ecs::SystemDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:319",
  "render_collision",
  &render_collision__cache__,
  {
    {"editorSettings", ecs::get_type_index<EditorRenderSettings>(), ecs::AccessType::ReadOnly, false, ecs::is_singleton<EditorRenderSettings>()}
  },
  {},
  {},
  "render",
  {"render_sky_box"},
  {"process_mesh_position"},
  {},
  &render_collision_implementation));

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:26",
  "add_global_uniform",
  &add_global_uniform__cache__,
  {},
  {},
  {},
  {},
  {},
  {},
  &add_global_uniform_handler, &add_global_uniform_single_handler),
  ecs::EventIndex<ecs::OnSceneCreated>::value);

  ecs::register_event(ecs::EventDescription(
  "C:/Users/Lord/workDirectory/MotionMatching/sources/common_systems/common/render.inl:60",
  "mesh_loader",
  &mesh_loader__cache__,
  {
    {"mesh", ecs::get_type_index<Asset<Mesh>>(), ecs::AccessType::ReadWrite, false, ecs::is_singleton<Asset<Mesh>>()}
  },
  {},
  {},
  {},
  {},
  {},
  &mesh_loader_handler, &mesh_loader_single_handler),
  ecs::EventIndex<ecs::OnEntityCreated>::value);

}
ECS_FILE_REGISTRATION(&registration_pull_render)
ECS_PULL_DEFINITION(variable_pull_render)
