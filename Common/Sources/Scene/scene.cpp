#include "scene.h"

#include "glad/glad.h"
#include "skybox.h"
#include "Camera/camera.h"
#include "Time/time.h"
#include "imgui/imgui.h"


void Scene::start_scene()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}
void Scene::init(void init_f(vector<GameObjectPtr>&, DirectionLight&))
{
  if (init_f != nullptr)
    init_f(gameObjects, sun);
}
void Scene::update_logic()
{   
  for (auto objects : gameObjects)
  {
    for (auto component : objects->get_components())
    {
      IUpdatable *updatable = dynamic_cast<IUpdatable*>(component.get());
      if (updatable)
        updatable->update();
    }
  }
}
void Scene::update_render()
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (auto objects : gameObjects)
  {
    for (auto component : objects->get_components())
    {
      IRenderable *renderable = dynamic_cast<IRenderable*>(component.get());
      if (renderable)
        renderable->render(Camera(), sun);
    }
  }

  glFlush(); 
}
void Scene::update_ui()
{
  ImGui::Begin("Debug");
  debug_show();
  ImGui::End();
  for (auto objects : gameObjects)
  {
    for (auto component : objects->get_components())
    {
      IUIRenderable *UIrenderable = dynamic_cast<IUIRenderable*>(component.get());
      if (UIrenderable)
        UIrenderable->ui_render();
    }
  }
  ImGui::Begin("FPS");
  ImGui::Text("%.1f", Time::fps());
  ImGui::End();
}
void Scene::process_events()
{
  
}
void Scene::destroy_scene()
{ 
  for (auto objects : gameObjects)
  {
    for (auto component : objects->get_components())
    {
      ISaveable *saveable = dynamic_cast<ISaveable*>(component.get());
      if (saveable)
        saveable->save();
    }
  }
}