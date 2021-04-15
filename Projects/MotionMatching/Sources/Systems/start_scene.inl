#include "ecs/ecs.h"
#include "Camera/camera.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Application/application.h"
#include "Texture/textures.h"
#include "Material/material.h"
#include "skybox.h"
#include "Components/DebugTools/debug_arrow.h"
#include "Animation/AnimationDatabase/animation_database.h"
#include "Components/MeshRender/mesh_render.h"
#include "Animation/AnimationDatabase/animation_preprocess.h"
#include "motion_matching_scene.h"
#include "Animation/AnimationRender/animation_render.h"
#include "Animation/animation_player.h"
#include "PersonController/third_person_controller.h"
#include "PersonController/person_controller.h"
#include "Application/config.h"
#include "Animation/Test/animation_tester.h"
#include "Event/input.h"
#include "Animation/AnimationRender/bone_render.h"
EVENT() start_scene(const ecs::OnSceneCreated &)
{
  ecs::EntityId attachedCamera;
  {
    create_camera_manager();
    attachedCamera = create_camera();
    create_free_camera(vec3(0, 5, -5),radians(vec2(-270,0)));
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
    list.add<DebugArrow>("debugArrows") = DebugArrow();
    ecs::create_entity(list);
  }
  {
    ecs::ComponentInitializerList list;
    list.add<SkyBox>("skyBox") = SkyBox(common_resources_path("Textures/Skyboxes/CloudSky"));
    ecs::create_entity(list);
  }
  Assimp::Importer importer;
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
  importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
  string manPath = project_resources_path("MocapOnline/MotusMan_v55/MotusMan_v55.fbx");
  importer.ReadFile(manPath, aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_FlipUVs | aiPostProcessSteps::aiProcess_LimitBoneWeights |
    aiPostProcessSteps::aiProcess_GenNormals | aiProcess_GlobalScale | aiProcess_FlipWindingOrder);
  const aiScene* scene = importer.GetScene();
  
  
  aiNode* root = scene->mRootNode;
  root = root->mChildren[0];

  MeshPtr mesh = make_mesh(scene->mMeshes[0]);
  AnimationDataBasePtr dataBase = animation_preprocess(importer, root);
  TexturePtr tex = make_texture2d(project_resources_path("MocapOnline/MotusMan_v55/MCG_diff.jpg"), TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  TexturePtr floor = make_texture2d(common_resources_path("Textures/ground.jpg"), TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  TexturePtr tex1 = make_texture2d(common_resources_path("Textures/screen.jpg"), TexturePixelFormat::Linear, TextureWrappFormat::Repeat, true);
  MaterialPtr material;
  {
    ecs::ComponentInitializerList list;
    list.add<MotionMatchingScene>("motionMatchingScene") = MotionMatchingScene(dataBase);
    ecs::create_entity(list);
  }
  {
    ecs::ComponentInitializerList list;
    vec2 rotation(0,0);
    vec3 pos(0.f, 0.f,-12.f);
    list.add<Transform>("transform") = Transform(pos, vec3(rotation.x, rotation.y ,0), vec3(1));
    list.add<PersonController>("personController") = PersonController(pos);
    list.add<AnimationRender>("animationRender") = AnimationRender(
      mesh,
      standart_textured_material(tex),
      get_shader("animation_normal_uv"));
    if (get_bool_config("showBones"))
      list.add<BoneRender>("boneRender") = BoneRender();

     if (get_bool_config("showMatchingStatistic"))
     {
      list.add<bool>("showMatchingStatistic") = true;
     }

    list.get<AnimationRender>("animationRender").get_material()->set_property(Property("Shininess", 100.f));
    list.add<AnimationPlayer>("animationPlayer") =  AnimationPlayer(dataBase, "MOB1_Stand_Relaxed_Idle_v2", AnimationPlayerType::MotionMatching);
    list.add<ThirdPersonController>("thirdPersonController") =  ThirdPersonController(rotation + vec2(PI*0.5f,0), 3.f);
    list.add<ecs::EntityId>("attachedCamera") = attachedCamera;
    ecs::create_entity(list);
  }
  if (dataBase->test.size() > 0)
  {
    int testN = 9;
    int testK = (int)sqrt(testN);
    float testSq = 5;
    for (int i = 0; i < testN; i++)
    {
      ecs::ComponentInitializerList list;
      vec2 rotation = vec2(0,0);
      vec3 pos = vec3(i / testK, 0, i % testK) * testSq;
      list.add<Transform>("transform") = Transform(pos, vec3(rotation.x, rotation.y ,0), vec3(1));
      list.add<PersonController>("personController") = PersonController(pos);
      list.add<AnimationRender>("animationRender") = AnimationRender(
      mesh,
      standart_textured_material(tex),
      get_shader("animation_normal_uv"));

      if (get_bool_config("showBones"))
        list.add<BoneRender>("boneRender") = BoneRender();
      
      list.get<AnimationRender>("animationRender").get_material()->set_property(Property("Shininess", 100.f));

      list.add<AnimationPlayer>("animationPlayer") =  AnimationPlayer(dataBase, "MOB1_Stand_Relaxed_Idle_v2", AnimationPlayerType::MotionMatching);
      list.add<AnimationTester>("animationTester") = AnimationTester(dataBase, i * (dataBase->test.size() / testN), pos);

      ecs::create_entity(list);
    }
  }
  if (get_bool_config("showGoal"))
  {
    ecs::ComponentInitializerList list; 
    list.add<Transform>("debugTransform") = Transform();
    list.add<MeshRender>("debugGoalSphere") = create_sphere(10);
    material = list.get<MeshRender>("debugGoalSphere").get_material();
    material->set_property(Property("Ambient", vec3(1,1,1)));
    material->set_property(Property("Diffuse", vec3(0,0,0)));
    material->set_property(Property("Specular", vec3(0,0,0)));
    ecs::create_entity(list);
  }
  {
    ecs::ComponentInitializerList list; 
    list.add<Transform>("transform") = Transform(vec3(0.f,0.0f,0.0f), vec3(), vec3(500,1,500));
    list.add<MeshRender>("meshRender") = create_plane(true);
    material = list.get<MeshRender>("meshRender").get_material();
    material->set_property(Property("mainTex", floor));
    material->set_property(Property("uvScale", vec2(80.f,80.f)));
    material->set_property(Property("Specular", vec3(0.f)));
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
      list.add<MeshRender>("meshRender") = create_cube(true);
      material = list.get<MeshRender>("meshRender").get_material();
      material->set_property(Property("mainTex", tex1));
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
      list.add<MeshRender>("meshRender") = create_plane(true);
      material = list.get<MeshRender>("meshRender").get_material();
      material->set_property(Property("mainTex", tex1));
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
      list.add<MeshRender>("meshRender") = create_sphere(20, true, true);
      material = list.get<MeshRender>("meshRender").get_material();
      material->set_property(Property("mainTex", tex1));
      material->set_property(Property("Specular", vec3(0.f)));
      //add_collider(sphere, make_collider<SphereCollider>(0.4f));
      ecs::create_entity(list);
    }
  }
  ecs::system_statistic();

}


EVENT() scene_destroy(
  const ecs::OnEntityDestroyed &,
  const MotionMatchingScene &motionMatchingScene)
{
  motionMatchingScene.dataBase->save_runtime_parameters();
}
