#include "motion_matching_scene.h"
#include "CommonCode/skybox.h"
#include "CommonCode/Camera/cameras.h"
#include "CommonCode/Application/application.h"
#include "CommonCode/Mesh/mesh.h"
#include "CommonCode/Texture/textures.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "CommonCode/Animation/AnimationDatabase/animation_preprocess.h"
#include "CommonCode/Components/MeshRender/mesh_render.h"
#include "CommonCode/Components/AnimationRender/animation_render.h"
#include "CommonCode/PersonController/person_controller.h"
#include "CommonCode/Animation/animation_player.h"
#include "CommonCode/Animation/animation_debug.h"
#include "CommonCode/Components/DebugTools/debug_arrow.h"
#include "CommonCode/Physics/physics.h"
#include "CommonCode/Application/config.h"
void read_tree(aiNode * node, int depth = 0)
{
  string tab(depth, ' ');
  debug_log("%s%s, mesh count %d",tab.c_str(), node->mName.C_Str(), node->mNumMeshes);
  
  for (uint i = 0; i < node->mNumChildren; i++)
    read_tree(node->mChildren[i], depth + 1);
}

void init_scene(vector<GameObjectPtr>&gameObjects, DirectionLight& sun)
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
  string manPath = join_recources_path("MocapOnline/MotusMan_v55/MotusMan_v55.fbx");
  importer.ReadFile(manPath, aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_FlipUVs | aiPostProcessSteps::aiProcess_LimitBoneWeights |
    aiPostProcessSteps::aiProcess_GenNormals | aiProcess_GlobalScale);
  const aiScene* scene = importer.GetScene();
  
  
  aiNode* root = scene->mRootNode;
  root = root->mChildren[0];
  //read_tree(root);

  MeshPtr mesh = make_mesh(scene->mMeshes[0]);
  AnimationDataBasePtr dataBase = animation_preprocess(importer, root);
  TexturePtr tex = make_texture2d("MocapOnline/MotusMan_v55/MCG_diff.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  TexturePtr floor = make_texture2d("Textures/ground.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  TexturePtr tex1 = make_texture2d("Textures/screen.jpg", TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  MaterialPtr material;
  {
    GameObjectPtr mmScene = make_game_object();
    mmScene->add_component<MotionMatchingScene>(dataBase);
    gameObjects.push_back(mmScene);
  }
  {
    GameObjectPtr man = make_game_object();
    man->add_component<Transform>(vec3(0.f, 0.f,-1.f), vec3(0.f), vec3(1,1,1));
    man->add_component<AnimationRender>(
      mesh,
      standart_textured_material(tex),
      get_shader("animation_normal_uv"), get_bool_config("showBones"));

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
  {
    GameObjectPtr plane = make_game_object();
    plane->add_component<Transform>(vec3(0.f,0.0f,0.0f), vec3(), vec3(500,1,500));
    plane->add_component<MeshRender>(create_plane(true));
    material = plane->get_component<MeshRender>()->get_material();
    material->set_property(Property("mainTex", floor));
    material->set_property(Property("uvScale", vec2(80.f,80.f)));
    material->set_property(Property("Specular", vec3(0.f)));
    gameObjects.push_back(plane);
    add_collider(plane, make_collider<PlaneCollider>());
  }
  {
    for (int i = 0; i < 0; i++)
    {
      vec3 rotation = radians(vec3(0, 0, 0));
      vec3 offset = vec3(0, i* 0.2f, i);
      GameObjectPtr cube = make_game_object();
      cube->add_component<Transform>(offset, rotation, vec3(0.7f,0.4f,0.7f));
      cube->add_component<MeshRender>(create_cube(true));
      material = cube->get_component<MeshRender>()->get_material();
      material->set_property(Property("mainTex", tex1));
      material->set_property(Property("Specular", vec3(0.f)));
      gameObjects.push_back(cube);
      add_collider(cube, make_collider<BoxCollider>());
    }
    for (int i = 0; i < 0; i++)
    {
      float rx = i % 2 ? (i / 2 ? 1 : -1) : 0;
      float ry = i % 2 ? 0 : (i / 2 ? 1 : -1);
      vec3 rotation = radians(vec3(0,ry, -rx) * 25.f);
      float scale = 15;
      vec3 offset = -vec3(rx * scale * 0.5f, 0.0f, ry * scale * 0.5f);
      GameObjectPtr plane = make_game_object();
      plane->add_component<Transform>(offset, rotation, vec3(scale,scale,scale));
      plane->add_component<MeshRender>(create_plane(true));
      material = plane->get_component<MeshRender>()->get_material();
      material->set_property(Property("mainTex", tex1));
      material->set_property(Property("Specular", vec3(0.f)));
      gameObjects.push_back(plane);
      add_collider(plane, make_collider<PlaneCollider>());
    }
  }
  {
    for (int i = 0; i < 8; i++)
    {
      vec3 offset = vec3(-1.f,0.f,-1.f) + vec3(-i, i*0.5, i)*0.4f;
      GameObjectPtr sphere = make_game_object();
      sphere->add_component<Transform>(offset, vec3(), vec3(0.4f));
      sphere->add_component<MeshRender>(create_sphere(20, true, true));
      material = sphere->get_component<MeshRender>()->get_material();
      material->set_property(Property("mainTex", tex1));
      material->set_property(Property("Specular", vec3(0.f)));
      gameObjects.push_back(sphere);
      add_collider(sphere, make_collider<SphereCollider>(0.4f));
    }
  }
  {
    GameObjectPtr debug_arrow = make_game_object();
    debug_arrow->add_component<DebugArrow>();
    gameObjects.push_back(debug_arrow);
  }
}


MotionMatchingScene::MotionMatchingScene(AnimationDataBasePtr dataBase) :
dataBase(dataBase){}
void MotionMatchingScene::save()
{
  dataBase->save_runtime_parameters();
}