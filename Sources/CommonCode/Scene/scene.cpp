#include "scene.h"

#include "glad/glad.h"
#include "CommonCode/skybox.h"
#include "CommonCode/Camera/cameras.h"
#include "CommonCode/Application/application.h"
#include "CommonCode/Mesh/mesh.h"
#include "CommonCode/Texture/textures.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "imgui/imgui.h"
#include "../Animation/AnimationDatabase/animation_preprocess.h"
#include "CommonCode/Components/MeshRender/mesh_render.h"
#include "CommonCode/Components/AnimationRender/animation_render.h"
#include "../PersonController/person_controller.h"
#include "CommonCode/Animation/animation_player.h"
#include "../Animation/animation_debug.h"

void read_tree(aiNode * node, int depth = 0)
{
  string tab(depth, ' ');
  debug_log("%s%s, mesh count %d",tab.c_str(), node->mName.C_Str(), node->mNumMeshes);
  
  for (uint i = 0; i < node->mNumChildren; i++)
    read_tree(node->mChildren[i], depth + 1);
}

void Scene::init()
{
  Context &context = Application::get_context();
  Input &input = Application::get_input();

  //create_sky_box("Textures/Skyboxes/Village");
  //create_sky_box("Textures/Skyboxes/DesertSky");
  create_sky_box("Textures/Skyboxes/CloudSky");
  sun = DirectionLight(vec3(0.1f, -0.5f, 0.1f));

  shared_ptr<ArcballCamera> arcballCam = make_shared<ArcballCamera>(vec3(10.f,50.f,-150.f), 40, vec2(0.f, 30.f*DegToRad));
  arcballCam->set_perspective(70.f * DegToRad, context.get_width(), context.get_height(), 0.01f, 5000.f);
  add_camera(arcballCam);
  input.mouse_move_event() += createMethodEventHandler(*arcballCam, &ArcballCamera::mouse_move_handler);
  input.mouse_click_event() += createMethodEventHandler(*arcballCam, &ArcballCamera::mouse_click_handler);

  shared_ptr<FreeCamera> freeCam = make_shared<FreeCamera>(vec3(0,100,0), vec2(0,0));
  freeCam->set_perspective(70.f * DegToRad, context.get_width(), context.get_height(), 0.01f, 5000.f);
  add_camera(freeCam);
  input.mouse_move_event() += createMethodEventHandler(*freeCam, &FreeCamera::mouse_move_handler);
  input.keyboard_event(KeyAction::Down, SDLK_SPACE) += createMethodEventHandler(*freeCam, &FreeCamera::space_button_handler);
  
  Assimp::Importer importer;
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
  importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
  string manPath = join_recources_path("MocapOnlineMobilityStarterPack/MotusMan_v55/MotusMan_v55.fbx");
  importer.ReadFile(manPath, aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_FlipUVs | aiPostProcessSteps::aiProcess_LimitBoneWeights |
    aiPostProcessSteps::aiProcess_GenNormals | aiProcess_GlobalScale);
  const aiScene* scene = importer.GetScene();
  
  
  aiNode* root = scene->mRootNode;
  root = root->mChildren[0];
  //read_tree(root);

  MeshPtr mesh = make_mesh(scene->mMeshes[0]);
  dataBase = animation_preprocess(importer, root);
  TexturePtr tex = make_texture2d("MocapOnlineMobilityStarterPack/MotusMan_v55/MCG_diff.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  TexturePtr floor = make_texture2d("Textures/ground.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  TexturePtr tex1 = make_texture2d("Textures/screen.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  MaterialPtr material;
  {
    GameObject* man = new GameObject();
    man->add_component<Transform>(vec3(1.f, 0.f,-1.f), vec3(0.f), vec3(1,1,1));
    man->add_component<AnimationRender>(
      mesh,
      standart_textured_material(tex),
      get_shader("animation_normal_uv"), true);

    man->get_component<AnimationRender>()->get_material()->set_property(Property("Shininess", 100.f));


    AnimationPlayer *animPlayer = man->add_component<AnimationPlayer>(dataBase, 0, AnimationPlayerType::MotionMatching);
    input.keyboard_event(KeyAction::Down, SDLK_LEFT) += createMethodEventHandler(*animPlayer, &AnimationPlayer::animation_selector);
    input.keyboard_event(KeyAction::Down, SDLK_RIGHT) += createMethodEventHandler(*animPlayer, &AnimationPlayer::animation_selector);

    TestPersonController *personController = man->add_component<TestPersonController>();
    input.keyboard_event(KeyAction::Down, SDLK_z) += createMethodEventHandler(*personController, &TestPersonController::crouch);
    input.keyboard_event(KeyAction::Down, SDLK_SPACE) += createMethodEventHandler(*personController, &TestPersonController::jump);
    input.keyboard_event(KeyAction::Down, SDLK_a) += createMethodEventHandler(*personController, &TestPersonController::rotate);
    input.keyboard_event(KeyAction::Down, SDLK_d) += createMethodEventHandler(*personController, &TestPersonController::rotate);

    man->add_component<AnimationDebugRender>();

    gameObjects.push_back(man);

    arcballCam->set_target(man->get_component<Transform>());
  }
  GameObject* plane = new GameObject();
  plane->add_component<Transform>(vec3(0.f,0.f,0.f), vec3(), vec3(500,1,500));
  plane->add_component<MeshRender>(create_plane(true));
  material = plane->get_component<MeshRender>()->get_material();
  material->set_property(Property("mainTex", floor));
  material->set_property(Property("uvScale", vec2(80.f,80.f)));
  material->set_property(Property("Specular", vec3(0.f)));
  gameObjects.push_back(plane);

  GameObject* cube = new GameObject();
  cube->add_component<Transform>(vec3(2.f,1.f,4.f), vec3(), vec3(0.7f,0.4f,0.7f));
  cube->add_component<MeshRender>(create_cube(true));
  material = cube->get_component<MeshRender>()->get_material();
  material->set_property(Property("mainTex", tex1));
  material->set_property(Property("Specular", vec3(0.f)));
  gameObjects.push_back(cube);

  GameObject* sphere = new GameObject();
  sphere->add_component<Transform>(vec3(-6.f,1.f,-6.f), vec3(), vec3(1.8f,1.8f,1.8f));
  sphere->add_component<MeshRender>(create_sphere(20, true, true));
  material = sphere->get_component<MeshRender>()->get_material();

  material->set_property(Property("mainTex", tex1));
  material->set_property(Property("Specular", vec3(0.f)));
  gameObjects.push_back(sphere);

  
}
void Scene::update()
{ 
  main_camera()->update();
  
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
void Scene::render()
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  CameraPtr mainCamera = main_camera();
  for (auto objects : gameObjects)
  {
    for (auto component : objects->get_components())
    {
      IRenderable *renderable = dynamic_cast<IRenderable*>(component.get());
      if (renderable)
        renderable->render(*mainCamera, sun);
    }
  }

  render_sky_box();
  glFlush(); 
}
void Scene::render_ui()
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

void Scene::exit()
{
  dataBase->save_runtime_parameters();
}