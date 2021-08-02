#include "ecs/ecs.h"
#include "Engine/camera.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Application/application.h"
#include "Engine/Render/Texture/textures.h"
#include "Engine/Render/material.h"
#include "Engine/Render/skybox.h"
#include "Engine/Render/debug_arrow.h"
#include "Animation/AnimationDatabase/animation_database.h"
#include "Engine/Render/mesh_render.h"
#include "Animation/AnimationDatabase/animation_preprocess.h"
#include "motion_matching_scene.h"
#include "Animation/AnimationRender/animation_render.h"
#include "Animation/animation_player.h"
#include "Animation/third_person_controller.h"
#include "Animation/person_controller.h"
#include "config.h"
#include "Animation/Test/animation_tester.h"
#include "Engine/input.h"
#include "Animation/AnimationRender/bone_render.h"
#include "Animation/settings.h"
#include "Engine/Resources/resources.h"
#include "Engine/imgui/imgui.h"

#define CUSTOM_TYPE \
MACRO(PersonController)\
MACRO(DebugArrow)\
MACRO(BoneRender)\
MACRO(SceneRender)\
MACRO(MotionMatchingScene)

#define MACRO(T) REG_TYPE(T) EDIT_STUB(T)

CUSTOM_TYPE
REG_TYPE(MeshRender)
REG_TYPE(AnimationRender)
REG_TYPE(AnimationPlayer)
REG_TYPE(AnimationTester)
REG_TYPE(ThirdPersonController)

void write_tree(aiNode* root, int d = 1)
{
  debug_log("%*c%s",d, ' ', root->mName.C_Str());
  for (int i = 0, n = root->mNumChildren; i < n; i++)
    write_tree(root->mChildren[i], d + 1);
}

EVENT(ecs::SystemTag::Game) init_anim_settings(const ecs::OnSceneCreated &)
{

  SettingsContainer::instance = new SettingsContainer();
  load_object(*SettingsContainer::instance, "settings.bin");
  SettingsContainer::instance->after_loading();

  Settings::instance = new Settings();
  load_object(*Settings::instance, "man_property.bin");
}
#define EVEN(...) void
EVEN(ecs::SystemTag::Editor) start_scene(const ecs::OnSceneCreated &)
{

  fflush(stdout);
  const char* animData[2] = {"", "-AnimData -hUnity"};
  add_configs(2, animData);

  ecs::EntityId attachedCamera;
  {
    attachedCamera = create_camera();
    create_free_camera(vec3(0, 5, -5),radians(vec2(-270,0)));
    create_arcball_camera(10.f, radians(vec2(-270,0)), vec3(0, 5, -5));
    ecs::send_event_immediate(OnSetMainCamera(attachedCamera));
  }
  {
    ecs::ComponentInitializerList list;
    list.add<SceneRender>("sceneRender") = SceneRender();
    list.get<SceneRender>("sceneRender").sun = vec3(0.1f, -0.5f, 0.1f);
    ecs::create_entity(list);
  }

  {
    ecs::ComponentInitializerList list;
    list.add<SkyBox>("skyBox") = SkyBox(common_resources_path("Textures/Skyboxes/CloudSky"));
    ecs::create_entity(list);
  }
  {
    ecs::ComponentInitializerList list;
    list.add<float>("fps") = float(0);
    ecs::create_entity(list);
  }
  Assimp::Importer importer;
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
  importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
  string manPath = project_resources_path("MocapOnline/MotusMan_v55/MotusMan_v55.fbx");
  importer.ReadFile(manPath, aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_LimitBoneWeights |
    aiPostProcessSteps::aiProcess_GenNormals | aiProcess_GlobalScale | aiProcess_FlipWindingOrder);
  //const aiScene* scene = importer.GetScene();
  
  Asset<Texture2D> tex1 = get_resource<Texture2D>("screen.jpg");
  Asset<Material> material;
  /*
  aiNode* root = scene->mRootNode;
  root = root->mChildren[0];
  //write_tree(root);
  Asset<Mesh> mesh = Asset<Mesh>("motusman", false, scene->mMeshes[0]);
  AnimationDataBasePtr dataBase = animation_preprocess(importer, root);

  {
    ecs::ComponentInitializerList list;
    list.add<MotionMatchingScene>("motionMatchingScene") = MotionMatchingScene(dataBase);
    list.add<int>("recordedTest") = -1;
    list.add<int>("recordedState") = 0;
    list.add<float>("recorderStartTime") = 0;
    ecs::create_entity(list);
  }
  float sectionSize = 10.f;
  {
    ecs::ComponentInitializerList list;
    vec2 rotation(0,0);
    vec3 pos = vec3(-0.5f, 0.f,-0.5f) * sectionSize;
    list.add<Transform>("transform") = Transform(pos, vec3(rotation.x, rotation.y ,0), vec3(1));
    list.add<PersonController>("personController") = PersonController(pos);
    list.add<AnimationRender>("animationRender") = AnimationRender(
      mesh,
      get_resource<Material>("man material"),
      get_shader("animation_normal_uv"));
    
    list.add<BoneRender>("boneRender") = BoneRender();


    list.get<AnimationRender>("animationRender").get_material()->set_property(Property("Shininess", 100.f));
    list.add<AnimationPlayer>("animationPlayer") =  AnimationPlayer(dataBase, "MOB1_Stand_Relaxed_Idle_v2", AnimationPlayerType::MotionMatching);
    list.add<ThirdPersonController>("thirdPersonController") =  ThirdPersonController(rotation + vec2(PI*0.5f,-0.5), 3.f);
    list.add<ecs::EntityId>("attachedCamera") = attachedCamera;
    list.add<int>("controllerIndex") = 0;
    list.add<int>("mmIndex") = 0;
    list.add<int>("mmOptimisationIndex") = 0;
    ecs::create_entity(list);
  }
  if (dataBase->tests.size() > 0)
  {
    int testN = 5;
    Settings::testCount = testN;
    int testK = (int)sqrt(testN);
    for (int i = 0; i < testN; i++)
    {
      ecs::ComponentInitializerList list;
      vec2 rotation = vec2(0,0);
      vec3 pos = vec3(i / testK + 0.5f, 0, i % testK + 0.5f) * sectionSize;
      list.add<Transform>("transform") = Transform(pos, vec3(rotation.x, rotation.y ,0), vec3(1));
      list.add<PersonController>("personController") = PersonController(pos);
      list.add<AnimationRender>("animationRender") = AnimationRender(
      mesh,
      get_resource<Material>("man material"),
      get_shader("animation_normal_uv"));

      list.add<BoneRender>("boneRender") = BoneRender();
      

      list.add<AnimationPlayer>("animationPlayer") =  AnimationPlayer(dataBase, "MOB1_Stand_Relaxed_Idle_v2", AnimationPlayerType::MotionMatching);
      list.add<AnimationTester>("animationTester") = AnimationTester();
      list.add<vec3>("testOffset") = pos;

      ecs::create_entity(list);
    }
  }
  
  {
    ecs::ComponentInitializerList list; 
    list.add<Transform>("debugTransform") = Transform();
    list.add<MeshRender>("debugGoalSphere") =
     MeshRender(sphere_mesh(5, false), get_resource<Material>("debug sphere"),get_shader("standart_normal"));
    ecs::create_entity(list);
  }
  */
  {
    ecs::ComponentInitializerList list; 
    list.add<Transform>("transform") = Transform(vec3(0.f,0.0f,0.0f), vec3(), vec3(500,1,500));
    list.add<MeshRender>("meshRender") = 
     MeshRender(plane_mesh(true), get_resource<Material>("floor_ground"));

    //add_collider(plane, make_collider<PlaneCollider>());
    ecs::create_entity(list);
  }
  {
    for (int i = 0; i < 0; i++)
    {
      ecs::ComponentInitializerList list; 
      vec3 rotation = radians(vec3(0, 0, 0));
      vec3 offset = vec3(0, i* 0.2f, i);
      list.add<Transform>("transform") = Transform(offset, rotation, vec3(0.7f,0.4f,0.7f));
      list.add<MeshRender>("meshRender") = 
      MeshRender(cube_mesh(true), standart_textured_material(tex1));
      material = list.get<MeshRender>("meshRender").get_material();
      material->set_property(Property("Specular", vec3(0.f)));
      //add_collider(cube, make_collider<BoxCollider>());
      ecs::create_entity(list);
    }
    for (int i = 0; i < 0; i++)
    {
      ecs::ComponentInitializerList list; 
      float rx = i % 2 ? (i / 2 ? 1 : -1) : 0;
      float ry = i % 2 ? 0 : (i / 2 ? 1 : -1);
      vec3 rotation = radians(vec3(0,ry, -rx) * 25.f);
      float scale = 15;
      vec3 offset = -vec3(rx * scale * 0.5f, 0.0f, ry * scale * 0.5f);
      list.add<Transform>("transform") = Transform(offset, rotation, vec3(scale,scale,scale));
      list.add<MeshRender>("meshRender") = 
      MeshRender(plane_mesh(true), standart_textured_material(tex1));
      material = list.get<MeshRender>("meshRender").get_material();
      material->set_property(Property("Specular", vec3(0.f)));
      //add_collider(plane, make_collider<PlaneCollider>());
      ecs::create_entity(list);
    }
  }
  {
    for (int i = 0; i < 0; i++)
    {
      ecs::ComponentInitializerList list; 
      vec3 offset = vec3(-1.f,0.f,-1.f) + vec3(-i, i*0.5, i)*0.4f;
      list.add<Transform>("transform") = Transform(offset, vec3(), vec3(0.4f));
      list.add<MeshRender>("meshRender") = 
      MeshRender(sphere_mesh(20, true), standart_textured_material(tex1));
      material = list.get<MeshRender>("meshRender").get_material();
      material->set_property(Property("Specular", vec3(0.f)));
      //add_collider(sphere, make_collider<SphereCollider>(0.4f));
      ecs::create_entity(list);
    }
  }
  //ecs::system_statistic();

}


EVENT() scene_destroy(
  const ecs::OnEntityDestroyed &,
  const MotionMatchingScene &motionMatchingScene)
{
  motionMatchingScene.dataBase->save_runtime_parameters();
  
  save_object(*Settings::instance, "man_property.bin");
  
  save_object(*SettingsContainer::instance, "settings.bin");
  std::fflush(stdout);
}
