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

  shared_ptr<ArcballCamera> arcballCam = make_shared<ArcballCamera>(vec3(10.f,50.f,-150.f), 300, vec2());
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
  string manPath = join_recources_path("MocapOnlineMobilityStarterPack/MotusMan_v55/MotusMan_v55.fbx");
  importer.ReadFile(manPath, aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_FlipUVs | aiPostProcessSteps::aiProcess_LimitBoneWeights |
    aiPostProcessSteps::aiProcess_GenNormals);
  const aiScene* scene = importer.GetScene();
  
  aiNode* root = scene->mRootNode;
  root = root->mChildren[0]->mChildren[0];
  read_tree(root);

  Mesh mesh = Mesh(scene->mMeshes[0]);
  TexturePtr tex = make_texture2d("MocapOnlineMobilityStarterPack/MotusMan_v55/MCG_diff.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  TexturePtr floor = make_texture2d("Textures/ground.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);

  GameObjectPtr man = make_game_object(Transform(vec3(10.f, 0.f,-150.f)),
  make_mesh(scene->mMeshes[0]),
  standart_textured_material(tex),
  get_shader("animation_normal_uv"));
  man->get_material()->set_property(Property("Shininess", 100.f));
  
  arcballCam->set_target(&man->get_transform());

  GameObjectPtr plane = create_plane(Transform(vec3(10.f,0.f,-150.f), vec3(), vec3(1000,1,1000)), true);
  plane->set_shader(get_shader("standart_normal_uv"));
  plane->get_material()->set_property(Property("mainTex", floor));
  plane->get_material()->set_property(Property("uvScale", vec2(10,10)));
  plane->get_material()->set_property(Property("Specular", vec3(0.f)));
  gameObjects.emplace_back(plane);

  animPlayer = animation_preprocess(importer, man, root);
  
  
}
void Scene::update()
{ 
  main_camera()->update();
  animPlayer->PlayNextCadr();
  ImGui::Begin("FPS");
  ImGui::Text("%.1f", Time::fps());
  ImGui::End();
  ImGui::Begin("Debug");
  debug_show();
  ImGui::End();
}
void Scene::render()
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  CameraPtr mainCamera = main_camera();
  for (auto objects : gameObjects)
  {
    objects->get_shader().use();
    objects->render(*mainCamera, sun);
  }
  
  animPlayer->render(*mainCamera, sun);


  render_sky_box();
  glFlush();
}