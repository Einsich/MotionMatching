#include "render.inl"
//Code-generator production

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


ecs::QueryDescription render_flags_descr("render_flags", {
  {ecs::get_type_description<MeshRender>("flagRender"), false},
  {ecs::get_type_description<Transform>("transform"), false}
});

template<typename Callable>
void render_flags(Callable lambda)
{
  for (ecs::QueryIterator begin = render_flags_descr.begin(), end = render_flags_descr.end(); begin != end; ++begin)
  {
    lambda(
      *begin.get_component<MeshRender>(0),
      *begin.get_component<Transform>(1)
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



