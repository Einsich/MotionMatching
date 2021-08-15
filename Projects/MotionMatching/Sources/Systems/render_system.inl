#include <ecs/ecs.h>
#include <algorithm>
#include "Animation/AnimationRender/animation_render.h"
#include "Animation/AnimationRender/bone_render.h"
#include "Animation/animation_player.h"
#include <Engine/Render/mesh_render.h>
#include <Engine/Render/render.h>
#include <Engine/Render/global_uniform.h>
#include <Engine/camera.h>
#include <Engine/Render/debug_arrow.h>
#include <Engine/Render/skybox.h>
#include <Engine/Render/direction_light.h>
#include "Animation/settings.h"
template<typename Callable>
void render_animation(Callable);

template<typename Callable>
void bone_render_animation(const ecs::EntityId&, Callable);

template<typename Callable>
void render_meshes(Callable);

template<typename Callable>
void render_arrows(Callable);

template<typename Callable>
void render_skybox(Callable);

template<typename Callable>
void render_debug_goal(Callable);
template<typename Callable>
void render_debug_goal_on_animplayer(Callable);

template<typename Callable> 
void find_light(Callable);

template<typename Callable> 
void lod_selector(Callable);

SYSTEM(ecs::SystemOrder::MIDDLE_RENDER,ecs::SystemTag::GameEditor)
main_render(DebugArrow &debugArrows, EditorRenderSettings &editorSettings)
{
  mat4 camTransform, camProjection;
  if(!main_camera(camTransform, camProjection))
  {
    debug_error("Need main camera");
    return;
  }

  vec3 cameraPosition = camTransform[3];
  mat4 viewTrasform = inverse(camTransform);

  mat4 viewProjection = camProjection *  viewTrasform;

  DirectionLight light; 
  QUERY() find_light([&](const DirectionLight &directionalLight){light = directionalLight;});

  get_buffer("GlobalRenderData").
  update_buffer_and_flush<GlobalRenderData>( 
  {viewProjection, cameraPosition, light.normalizedLightDirection});

  mat4 viewProjectionSkybox = camProjection *  mat4(mat3(viewTrasform));

  QUERY() lod_selector([&](
    const Transform &transform,
    const vector<Asset<Mesh>> &lods_meshes,
    const vector<float> &lods_distances,
    Asset<Mesh> &mesh)
  {
    float distToCamera = length(transform.get_position() - cameraPosition);
    uint lod = lods_meshes.size();
    for (uint i = 0; i < lods_distances.size() && i < lods_meshes.size(); ++i)
    {
      if (distToCamera < lods_distances[i])
      {
        lod = i;
        break;
      }
    }
    if (lod < lods_meshes.size())
      mesh = lods_meshes[lod];
    else
      mesh = Asset<Mesh>();//culled by dist

  });


  UniformBuffer &instanceData = get_buffer("InstanceData");
  bool wire_frame = editorSettings.wire_frame; 
  using renderStuff = pair<Asset<Material>, Asset<Mesh>>;
  static vector<renderStuff> renderQueue;
  renderQueue.clear();
  QUERY() render_animation([&](
    ecs::EntityId eid,
    AnimationRender &animationRender,
    const Asset<Mesh> &mesh,
    const AnimationPlayer &animationPlayer,
    const Transform &transform,
    const Settings &settings)
  {
    if (mesh)
    {
      renderQueue.emplace_back(animationRender.get_material(), mesh);
      animationRender.process(transform, animationPlayer.get_tree());
    }

    if (settings.debugBones)
    {
      QUERY() bone_render_animation(eid, [&](
        const BoneRender &boneRender)
      {
        boneRender.render(transform.get_transform(), animationPlayer.get_tree());

      });
    }
  });
  auto matComparer = [](const renderStuff &a, const renderStuff &b)->bool{
    int as = a.first ? a.first->get_shader().get_shader_program() : -1;
    int bs = b.first ? b.first->get_shader().get_shader_program() : -1;
    if (as < bs)
      return true;
    int av = a.second ? a.second->get_vao().vao() : -1;
    int bv = b.second ? b.second->get_vao().vao() : -1;
    return av < bv;
  };
  std::sort(renderQueue.begin(), renderQueue.end(), matComparer);

  
    //animationRender.get_material()->get_properties().set_data_to_buf(instanceData.get_buffer());
  if (renderQueue.size() > 0)
  {
    uint instanceCount = 0, instanceSize = renderQueue[0].first->buffer_size();
    renderStuff prevStuff = renderQueue[0];
    for (const auto &[material, mesh] : renderQueue)
    {
      if (matComparer(prevStuff, {material, mesh})) // prevStuff < p
      {
        material->get_shader().use();
        material->bind_textures_to_shader();
        instanceData.flush_buffer(instanceCount * instanceSize);
        mesh->get_vao().render_instances(instanceCount, wire_frame);
        instanceCount = 0;
        instanceSize = material->buffer_size(); // new size of instance
      }
      if (instanceData.size() < (instanceCount + 1) * instanceSize)
        debug_log("reallocate buffer space");
      material->set_data_to_buf(instanceData.get_buffer(instanceCount * instanceSize, instanceSize));
      instanceCount++;
      prevStuff = {material, mesh};
    }
    prevStuff.first->get_shader().use();
    prevStuff.first->bind_textures_to_shader();
    instanceData.flush_buffer(instanceCount * instanceSize);
    prevStuff.second->get_vao().render_instances(instanceCount, wire_frame);
  }
  QUERY() render_meshes([&](
    const MeshRender &meshRender,
    const Transform &transform)
  {
    meshRender.render(transform, wire_frame);
  });



  QUERY() render_debug_goal([&](Transform &debugTransform, const MeshRender &debugGoalSphere)
  {
    QUERY() render_debug_goal_on_animplayer([&](
      const AnimationPlayer &animationPlayer,
      const Transform &transform,
      const Settings &settings)
    {
      AnimationLerpedIndex index = animationPlayer.get_motion_matching() ? animationPlayer.get_motion_matching()->get_index() : animationPlayer.get_index();

      mat4 transformation = transform.get_transform();


      Asset<Material> material = debugGoalSphere.get_material();
      const auto& feature = index.current_index().get_feature();
      
      /*vec3 man = transformation * vec4(feature.features[(int)AnimationFeaturesNode::Hips], 1.f);
      Ray ray(man, vec3(0,-1,0), 100);
      Collision collision = ray_cast(ray);
      draw_arrow(ray.from, collision.collisionPoint, vec3(10,0,0), 0.04f, false);*/

      if (settings.debugNodes)
      {
        #define DEBUG_NODE(node)\
        {\
          vec3 p = vec3(transformation * vec4(feature.nodes[(int)node], 1.f));\
          vec3 v = vec3(transformation * vec4(feature.nodesVelocity[(int)node], 0.f));\
          debugTransform.get_position() = p;\
          debugGoalSphere.render(debugTransform, true);\
          draw_arrow(p, p + v * 0.5f, vec3(1,0,0), 0.02f, false);\
        }

        material->set_property(Property("Ambient", vec3(1,1,1)));
        //debugTransform.set_scale(vec3(0.1f));
        DEBUG_NODE(AnimationFeaturesNode::LeftToeBase)
        DEBUG_NODE(AnimationFeaturesNode::LeftHand)
        DEBUG_NODE(AnimationFeaturesNode::RightToeBase)
        DEBUG_NODE(AnimationFeaturesNode::RightHand)
        DEBUG_NODE(AnimationFeaturesNode::Hips)
      }

      if (false)
      {
        u8 onGround = index.current_index().get_clip().onGround[index.current_index().get_cadr_index()];
        onGround = animationPlayer.onGround;
        if (onGround & 1)
        {
          material->set_property(Property("Ambient", vec3(1,0,0)));
          debugTransform.set_scale(vec3(0.11f));
          
          debugTransform.get_position() = transformation * vec4(feature.nodes[(int)AnimationFeaturesNode::LeftToeBase], 1.f);
          debugGoalSphere.render(debugTransform, true);
        }
        if (onGround & 2)
        {
          material->set_property(Property("Ambient", vec3(1,0,0)));
          debugTransform.set_scale(vec3(0.11f));
          
          debugTransform.get_position() = transformation * vec4(feature.nodes[(int)AnimationFeaturesNode::RightToeBase], 1.f);
          debugGoalSphere.render(debugTransform, true);
        }
      }
      if (settings.debugTrajectory)
      {
        constexpr float dirLength = 0.3f;
        constexpr vec3 colors[2] = {vec3(0,1,0), vec3(1,0,0)};
        constexpr float lenghts[2] = {0.3f, 0.3f};
        const AnimationTrajectory &trajectory = index.current_index().get_trajectory();

        const std::array<TrajectoryPoint,AnimationTrajectory::PathLength> *trajectories[2] = {&trajectory.trajectory, &animationPlayer.inputGoal.path.trajectory};
        for(int i = 0; i < 2; i++)
        {
          for (const TrajectoryPoint &p: *trajectories[i])
          {
            vec3 v = vec3(transformation * vec4(p.point, 1.f));
            vec3 w = vec3(transformation * vec4(quat(vec3(0, p.rotation, 0)) * vec3(0, 0, dirLength * lenghts[i]), 0.f));
            draw_arrow(v, v + w, colors[i], 0.02f, false);
          }
        }
      }
    });
  });

  debugArrows.render(wire_frame);



  QUERY() render_skybox([&](
    SkyBox &skyBox)
  {
    skyBox.render(viewProjectionSkybox, wire_frame);
  });
}
EVENT() debug_goal_copy_mat(const ecs::OnEntityCreated &, MeshRender &debugGoalSphere)
{
  debugGoalSphere.get_material() = debugGoalSphere.get_material().copy();
}