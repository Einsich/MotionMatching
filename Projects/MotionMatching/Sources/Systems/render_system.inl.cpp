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


ecs::QueryDescription lod_selector_descr("lod_selector", {
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<vector<Asset<Mesh>>>("lods_meshes"), false},
  {ecs::get_type_description<vector<float>>("lods_distances"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false}
});

template<typename Callable>
void lod_selector(Callable lambda)
{
  for (ecs::QueryIterator begin = lod_selector_descr.begin(), end = lod_selector_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<Transform>(0),
      *begin.get_component<vector<Asset<Mesh>>>(1),
      *begin.get_component<vector<float>>(2),
      *begin.get_component<Asset<Mesh>>(3)
    );
  }
}


ecs::QueryDescription render_animation_descr("render_animation", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<AnimationRender>("animationRender"), false},
  {ecs::get_type_description<Asset<Mesh>>("mesh"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<Settings>("settings"), false}
});

template<typename Callable>
void render_animation(Callable lambda)
{
  for (ecs::QueryIterator begin = render_animation_descr.begin(), end = render_animation_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<AnimationRender>(1),
      *begin.get_component<Asset<Mesh>>(2),
      *begin.get_component<AnimationPlayer>(3),
      *begin.get_component<Transform>(4),
      *begin.get_component<Settings>(5)
    );
  }
}


ecs::QueryDescription render_meshes_descr("render_meshes", {
  {ecs::get_type_description<MeshRender>("meshRender"), false},
  {ecs::get_type_description<Transform>("transform"), false}
});

template<typename Callable>
void render_meshes(Callable lambda)
{
  for (ecs::QueryIterator begin = render_meshes_descr.begin(), end = render_meshes_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<MeshRender>(0),
      *begin.get_component<Transform>(1)
    );
  }
}


ecs::QueryDescription render_debug_goal_descr("render_debug_goal", {
  {ecs::get_type_description<Transform>("debugTransform"), false},
  {ecs::get_type_description<MeshRender>("debugGoalSphere"), false}
});

template<typename Callable>
void render_debug_goal(Callable lambda)
{
  for (ecs::QueryIterator begin = render_debug_goal_descr.begin(), end = render_debug_goal_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<Transform>(0),
      *begin.get_component<MeshRender>(1)
    );
  }
}


ecs::QueryDescription render_debug_goal_on_animplayer_descr("render_debug_goal_on_animplayer", {
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false},
  {ecs::get_type_description<Settings>("settings"), false}
});

template<typename Callable>
void render_debug_goal_on_animplayer(Callable lambda)
{
  for (ecs::QueryIterator begin = render_debug_goal_on_animplayer_descr.begin(), end = render_debug_goal_on_animplayer_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<AnimationPlayer>(0),
      *begin.get_component<Transform>(1),
      *begin.get_component<Settings>(2)
    );
  }
}


ecs::QueryDescription render_skybox_descr("render_skybox", {
  {ecs::get_type_description<SkyBox>("skyBox"), false}
});

template<typename Callable>
void render_skybox(Callable lambda)
{
  for (ecs::QueryIterator begin = render_skybox_descr.begin(), end = render_skybox_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<SkyBox>(0)
    );
  }
}


ecs::SingleQueryDescription bone_render_animation_descr("bone_render_animation", {
  {ecs::get_type_description<BoneRender>("boneRender"), false}
});

template<typename Callable>
void bone_render_animation(const ecs::EntityId &eid, Callable lambda)
{
  ecs::QueryIterator begin;
  if (ecs::get_iterator(bone_render_animation_descr, eid, begin))
  {
    lambda(
      *begin.get_component<BoneRender>(0)
    );
  }
}


void main_render_func();

ecs::SystemDescription main_render_descr("main_render", {
  {ecs::get_type_description<DebugArrow>("debugArrows"), false},
  {ecs::get_type_description<EditorRenderSettings>("editorSettings"), false},
  {ecs::get_type_description<MainCamera>("mainCamera"), false}
}, main_render_func, ecs::SystemOrder::MIDDLE_RENDER + 2, (uint)(ecs::SystemTag::GameEditor));

void main_render_func()
{
  for (ecs::QueryIterator begin = main_render_descr.begin(), end = main_render_descr.end(); begin != end; ++begin)
  {
    main_render(
      *begin.get_component<DebugArrow>(0),
      *begin.get_component<EditorRenderSettings>(1),
      *begin.get_component<MainCamera>(2)
    );
  }
}


void debug_goal_copy_mat_handler(const ecs::OnEntityCreated &event);

ecs::EventDescription<ecs::OnEntityCreated> debug_goal_copy_mat_descr("debug_goal_copy_mat", {
  {ecs::get_type_description<MeshRender>("debugGoalSphere"), false}
}, debug_goal_copy_mat_handler, (uint)(ecs::SystemTag::Game));

void debug_goal_copy_mat_handler(const ecs::OnEntityCreated &event)
{
  for (ecs::QueryIterator begin = debug_goal_copy_mat_descr.begin(), end = debug_goal_copy_mat_descr.end(); begin != end; ++begin)
  {
    debug_goal_copy_mat(
      event,
      *begin.get_component<MeshRender>(0)
    );
  }
}


void debug_goal_copy_mat_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin);

ecs::SingleEventDescription<ecs::OnEntityCreated> debug_goal_copy_mat_singl_descr("debug_goal_copy_mat", {
  {ecs::get_type_description<MeshRender>("debugGoalSphere"), false}
}, debug_goal_copy_mat_singl_handler, (uint)(ecs::SystemTag::Game));

void debug_goal_copy_mat_singl_handler(const ecs::OnEntityCreated &event, ecs::QueryIterator &begin)
{
  debug_goal_copy_mat(
    event,
      *begin.get_component<MeshRender>(0)
  );
}



