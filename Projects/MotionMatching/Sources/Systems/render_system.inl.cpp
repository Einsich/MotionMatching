#include "render_system.inl"
//Code-generator production

ecs::QueryDescription render_animation_descr("render_animation", {
  {ecs::get_type_description<ecs::EntityId>("eid"), false},
  {ecs::get_type_description<AnimationRender>("animationRender"), false},
  {ecs::get_type_description<AnimationPlayer>("animationPlayer"), false},
  {ecs::get_type_description<Transform>("transform"), false}
});

template<typename Callable>
void render_animation(Callable lambda)
{
  for (ecs::QueryIterator begin = render_animation_descr.begin(), end = render_animation_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<ecs::EntityId>(0),
      *begin.get_component<AnimationRender>(1),
      *begin.get_component<AnimationPlayer>(2),
      *begin.get_component<Transform>(3)
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


ecs::QueryDescription render_arrows_descr("render_arrows", {
  {ecs::get_type_description<DebugArrow>("debugArrow"), false}
});

template<typename Callable>
void render_arrows(Callable lambda)
{
  for (ecs::QueryIterator begin = render_arrows_descr.begin(), end = render_arrows_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<DebugArrow>(0)
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
  {ecs::get_type_description<Transform>("transform"), false}
});

template<typename Callable>
void render_debug_goal_on_animplayer(Callable lambda)
{
  for (ecs::QueryIterator begin = render_debug_goal_on_animplayer_descr.begin(), end = render_debug_goal_on_animplayer_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<AnimationPlayer>(0),
      *begin.get_component<Transform>(1)
    );
  }
}


ecs::QueryDescription render_skybox_descr("render_skybox", {
  {ecs::get_type_description<SkyBox>("skybox"), false}
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
  {ecs::get_type_description<SceneRender>("sceneRender"), false}
}, main_render_func, ecs::SystemOrder::MIDDLE_RENDER);

void main_render_func()
{
  for (ecs::QueryIterator begin = main_render_descr.begin(), end = main_render_descr.end(); begin != end; ++begin)
  {
    main_render(
      *begin.get_component<SceneRender>(0)
    );
  }
}



