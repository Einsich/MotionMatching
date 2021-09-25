#include "render.inl"
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


ecs::SingleQueryDescription find_matrices_descr("find_matrices", {
  {ecs::get_type_description<Transform>("transform"), true}
});

template<typename Callable>
void find_matrices(const ecs::EntityId &eid, Callable lambda)
{
  ecs::QueryIterator begin;
  if (ecs::get_iterator(find_matrices_descr, eid, begin))
  {
    lambda(
       begin.get_component<Transform>(0)
    );
  }
}


void render_submenu_func();

ecs::SystemDescription render_submenu_descr("render_submenu", {
  {ecs::get_type_description<EditorRenderSettings>("settings"), false}
}, render_submenu_func, ecs::SystemOrder::UIMENU, (uint)( ecs::SystemTag::Editor));

void render_submenu_func()
{
  for (ecs::QueryIterator begin = render_submenu_descr.begin(), end = render_submenu_descr.end(); begin != end; ++begin)
  {
    render_submenu(
      *begin.get_component<EditorRenderSettings>(0)
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


void process_mesh_position_func();

ecs::SystemDescription process_mesh_position_descr("process_mesh_position", {
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<Asset<Material>>("material"), false},
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<RenderQueue>("render"), false}
}, process_mesh_position_func, ecs::SystemOrder::RENDER, (uint)(ecs::SystemTag::GameEditor));

void process_mesh_position_func()
{
  for (ecs::QueryIterator begin = process_mesh_position_descr.begin(), end = process_mesh_position_descr.end(); begin != end; ++begin)
  {
    process_mesh_position(
      *begin.get_component<Asset<Mesh>>(0),
      *begin.get_component<Asset<Material>>(1),
      *begin.get_component<ecs::EntityId>(2),
      *begin.get_component<RenderQueue>(3)
    );
  }
}


void render_sky_box_func();

ecs::SystemDescription render_sky_box_descr("render_sky_box", {
  {ecs::get_type_description<SkyBox>("skyBox"), false}
}, render_sky_box_func, ecs::SystemOrder::RENDER+100, (uint)(ecs::SystemTag::GameEditor));

void render_sky_box_func()
{
  for (ecs::QueryIterator begin = render_sky_box_descr.begin(), end = render_sky_box_descr.end(); begin != end; ++begin)
  {
    render_sky_box(
      *begin.get_component<SkyBox>(0)
    );
  }
}


void render_debug_arrows_func();

ecs::SystemDescription render_debug_arrows_descr("render_debug_arrows", {
  {ecs::get_type_description<DebugArrow>("debugArrows"), false},
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false}
}, render_debug_arrows_func, ecs::SystemOrder::RENDER+101, (uint)(ecs::SystemTag::GameEditor));

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


void main_instanced_render_func();

ecs::SystemDescription main_instanced_render_descr("main_instanced_render", {
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false},
  {ecs::get_type_description<RenderQueue>("render"), false}
}, main_instanced_render_func, ecs::SystemOrder::RENDER + 2, (uint)(ecs::SystemTag::GameEditor));

void main_instanced_render_func()
{
  for (ecs::QueryIterator begin = main_instanced_render_descr.begin(), end = main_instanced_render_descr.end(); begin != end; ++begin)
  {
    main_instanced_render(
      *begin.get_component<EditorRenderSettings>(0),
      *begin.get_component<RenderQueue>(1)
    );
  }
}


void add_global_uniform_handler(const ecs::OnSceneCreated &event);

ecs::EventDescription<ecs::OnSceneCreated> add_global_uniform_descr("add_global_uniform", {
}, add_global_uniform_handler, (uint)(ecs::SystemTag::GameEditor));

void add_global_uniform_handler(const ecs::OnSceneCreated &event)
{
  for (ecs::QueryIterator begin = add_global_uniform_descr.begin(), end = add_global_uniform_descr.end(); begin != end; ++begin)
  {
    add_global_uniform(
      event
    );
  }
}


void add_global_uniform_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnSceneCreated> add_global_uniform_singl_descr("add_global_uniform", {
}, add_global_uniform_singl_handler, (uint)(ecs::SystemTag::GameEditor));

void add_global_uniform_singl_handler(const ecs::OnSceneCreated &event, ecs::QueryIterator &)
{
  add_global_uniform(
    event
  );
}



