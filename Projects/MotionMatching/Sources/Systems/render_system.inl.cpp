#include "render_system.inl"
//Code-generator production

ecs::QueryDescription find_light_descr("find_light", {
  {ecs::get_type_description<DirectionLight>("directionalLight"), false}
});

template<typename Callable>
void find_light(Callable lambda)
{
  for (ecs::QueryIterator begin = find_light_descr.begin(), end = find_light_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<DirectionLight>(0)
    );
  }
}


void set_global_render_data_func();

ecs::SystemDescription set_global_render_data_descr("set_global_render_data", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, set_global_render_data_func, ecs::SystemOrder::RENDER, (uint)(ecs::SystemTag::GameEditor));

void set_global_render_data_func()
{
  for (ecs::QueryIterator begin = set_global_render_data_descr.begin(), end = set_global_render_data_descr.end(); begin != end; ++begin)
  {
    set_global_render_data(
      *begin.get_component<MainCamera>(0)
    );
  }
}


void render_sky_box_func();

ecs::SystemDescription render_sky_box_descr("render_sky_box", {
  {ecs::get_type_description<SkyBox>("skyBox"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false}
}, render_sky_box_func, ecs::SystemOrder::RENDER+100, (uint)(ecs::SystemTag::GameEditor));

void render_sky_box_func()
{
  for (ecs::QueryIterator begin = render_sky_box_descr.begin(), end = render_sky_box_descr.end(); begin != end; ++begin)
  {
    render_sky_box(
      *begin.get_component<SkyBox>(0),
      *begin.get_component<MainCamera>(1),
      *begin.get_component<EditorRenderSettings>(2)
    );
  }
}


void lod_selector_func();

ecs::SystemDescription lod_selector_descr("lod_selector", {
  {ecs::get_type_description<MainCamera>("mainCamera"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<vector<Asset<Mesh>>>("lods_meshes"), false},
  {ecs::get_type_description<vector<float>>("lods_distances"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false}
}, lod_selector_func, ecs::SystemOrder::RENDER-1, (uint)(ecs::SystemTag::GameEditor));

void lod_selector_func()
{
  for (ecs::QueryIterator begin = lod_selector_descr.begin(), end = lod_selector_descr.end(); begin != end; ++begin)
  {
    lod_selector(
      *begin.get_component<MainCamera>(0),
      *begin.get_component<Transform>(1),
      *begin.get_component<vector<Asset<Mesh>>>(2),
      *begin.get_component<vector<float>>(3),
      *begin.get_component<Asset<Mesh>>(4)
    );
  }
}


void process_animation_func();

ecs::SystemDescription process_animation_descr("process_animation", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false}
}, process_animation_func, ecs::SystemOrder::RENDER, (uint)(ecs::SystemTag::GameEditor));

void process_animation_func()
{
  for (ecs::QueryIterator begin = process_animation_descr.begin(), end = process_animation_descr.end(); begin != end; ++begin)
  {
    process_animation(
      *begin.get_component<Asset<Mesh>>(0),
      *begin.get_component<Asset<Material>>(1),
      *begin.get_component<AnimationPlayer>(2)
    );
  }
}


void process_mesh_position_func();

ecs::SystemDescription process_mesh_position_descr("process_mesh_position", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<Transform>("transform"), false}
}, process_mesh_position_func, ecs::SystemOrder::RENDER, (uint)(ecs::SystemTag::GameEditor));

void process_mesh_position_func()
{
  for (ecs::QueryIterator begin = process_mesh_position_descr.begin(), end = process_mesh_position_descr.end(); begin != end; ++begin)
  {
    process_mesh_position(
      *begin.get_component<Asset<Mesh>>(0),
      *begin.get_component<Asset<Material>>(1),
      *begin.get_component<Transform>(2)
    );
  }
}


void render_skeleton_bones_func();

ecs::SystemDescription render_skeleton_bones_descr("render_skeleton_bones", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<Settings>("settings"), false}
}, render_skeleton_bones_func, ecs::SystemOrder::RENDER, (uint)(ecs::SystemTag::GameEditor));

void render_skeleton_bones_func()
{
  for (ecs::QueryIterator begin = render_skeleton_bones_descr.begin(), end = render_skeleton_bones_descr.end(); begin != end; ++begin)
  {
    render_skeleton_bones(
      *begin.get_component<AnimationPlayer>(0),
      *begin.get_component<Transform>(1),
      *begin.get_component<Settings>(2)
    );
  }
}


void main_render_func();

ecs::SystemDescription main_render_descr("main_render", {
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false}
}, main_render_func, ecs::SystemOrder::RENDER + 2, (uint)(ecs::SystemTag::GameEditor));

void main_render_func()
{
  for (ecs::QueryIterator begin = main_render_descr.begin(), end = main_render_descr.end(); begin != end; ++begin)
  {
    main_render(
      *begin.get_component<EditorRenderSettings>(0)
    );
  }
}


void render_debug_arrows_func();

ecs::SystemDescription render_debug_arrows_descr("render_debug_arrows", {
  {ecs::get_type_description<DebugArrow>("debugArrows"), false},
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false}
}, render_debug_arrows_func, ecs::SystemOrder::RENDER + 101, (uint)(ecs::SystemTag::GameEditor));

void render_debug_arrows_func()
{
  for (ecs::QueryIterator begin = render_debug_arrows_descr.begin(), end = render_debug_arrows_descr.end(); begin != end; ++begin)
  {
    render_debug_arrows(
      *begin.get_component<DebugArrow>(0),
      *begin.get_component<EditorRenderSettings>(1)
    );
  }
}



