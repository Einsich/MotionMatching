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
#include "../Animation/animation_debug.h"
#include "../Animation/AnimationDatabase/animation_preprocess.h"

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

  Mesh mesh = Mesh(scene->mMeshes[0]);
  TexturePtr tex = make_texture2d("MocapOnlineMobilityStarterPack/MotusMan_v55/MCG_diff.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  TexturePtr floor = make_texture2d("Textures/ground.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  TexturePtr tex1 = make_texture2d("Textures/screen.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);

  GameObjectPtr man = make_game_object(Transform(vec3(1.f, 0.f,-1.f), vec3(0.f), vec3(1,1,1)),
  make_mesh(scene->mMeshes[0]),
  standart_textured_material(tex),
  get_shader("animation_normal_uv"));
  man->get_material()->set_property(Property("Shininess", 100.f));
  
  arcballCam->set_target(&man->get_transform());

  GameObjectPtr plane = create_plane(Transform(vec3(0.f,0.f,0.f), vec3(), vec3(500,1,500)), true);
  plane->set_shader(get_shader("standart_normal_uv"));
  plane->get_material()->set_property(Property("mainTex", floor));
  plane->get_material()->set_property(Property("uvScale", vec2(80.f,80.f)));
  plane->get_material()->set_property(Property("Specular", vec3(0.f)));
  gameObjects.emplace_back(plane);

  GameObjectPtr cube = create_cube(Transform(vec3(2.f,1.f,4.f), vec3(), vec3(0.7f,0.4f,0.7f)), true);
  cube->set_shader(get_shader("standart_normal_uv"));
  cube->get_material()->set_property(Property("mainTex", tex1));
  cube->get_material()->set_property(Property("Specular", vec3(0.f)));
  gameObjects.emplace_back(cube);

GameObjectPtr sphere = create_sphere(Transform(vec3(-2.f,1.f,-2.f), vec3(), vec3(1.8f,1.8f,1.8f)), 20, true, true);
  sphere->set_shader(get_shader("standart_normal_uv"));
  sphere->get_material()->set_property(Property("mainTex", tex1));
  sphere->get_material()->set_property(Property("Specular", vec3(0.f)));
  gameObjects.emplace_back(sphere);

  auto dataBase = animation_preprocess(importer, root);
  animPlayer = make_shared<AnimationPlayer>(dataBase, man, 0);
  input.keyboard_event(KeyAction::Down, SDLK_LEFT) += createMethodEventHandler(*animPlayer, &AnimationPlayer::animation_selector);
  input.keyboard_event(KeyAction::Down, SDLK_RIGHT) += createMethodEventHandler(*animPlayer, &AnimationPlayer::animation_selector);

  personController = TestPersonController(man, animPlayer);
  input.keyboard_event(KeyAction::Down, SDLK_z) += createMethodEventHandler(personController, &TestPersonController::crouch);
  input.keyboard_event(KeyAction::Down, SDLK_SPACE) += createMethodEventHandler(personController, &TestPersonController::jump);
}
void Scene::update()
{ 
  main_camera()->update();
  animPlayer->update();
  personController.update();

}
void Scene::render()
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  CameraPtr mainCamera = main_camera();
  animPlayer->render(*mainCamera, sun);
  for (auto objects : gameObjects)
  {
    objects->get_shader().use();
    objects->render(*mainCamera, sun);
  }
  


  render_sky_box();
  glFlush(); 
}
void Scene::render_ui()
{
  ImGui::Begin("Debug");
  debug_show();
  ImGui::End();
  debug_pose_matching(animPlayer);
  ImGui::Begin("FPS");
  ImGui::Text("%.1f", Time::fps());
  ImGui::End();
}

void Scene::exit()
{
  animPlayer->get_current_animation().dataBase->save_runtime_parameters();
}