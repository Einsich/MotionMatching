#include <ecs/ecs.h>
#include <algorithm>
#include "Animation/animation_player.h"
#include <Engine/Render/mesh.h>
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

SYSTEM(ecs::SystemOrder::RENDER,ecs::SystemTag::GameEditor)
set_global_render_data(const MainCamera &mainCamera)
{
  DirectionLight light; 
  QUERY() find_light([&](const DirectionLight &directionalLight){light = directionalLight;});
  get_buffer("GlobalRenderData").
  update_buffer_and_flush<GlobalRenderData>( 
  {mainCamera.projection * mainCamera.view, mainCamera.position, light.normalizedLightDirection});
}

SYSTEM(ecs::SystemOrder::RENDER+100,ecs::SystemTag::GameEditor)
render_sky_box(SkyBox &skyBox, const MainCamera &mainCamera, const EditorRenderSettings &editorSettings)
{
  mat4 viewProjectionSkybox = mainCamera.projection *  mat4(mat3(mainCamera.view));
  skyBox.render(viewProjectionSkybox, editorSettings.wire_frame);
}

SYSTEM(ecs::SystemOrder::RENDER-1,ecs::SystemTag::GameEditor) lod_selector(
  const MainCamera &mainCamera,
  const Transform &transform,
  const vector<Asset<Mesh>> &lods_meshes,
  const vector<float> &lods_distances,
  Asset<Mesh> &mesh)
{
  float distToCamera = length(transform.get_position() - mainCamera.position);
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
}
struct RenderStuff
{
  Asset<Material> material;
  Asset<Mesh> mesh;
};
static vector<RenderStuff> renderQueue;

  
SYSTEM(ecs::SystemOrder::RENDER,ecs::SystemTag::GameEditor) process_animation(
  const Asset<Mesh> &mesh,
  Asset<Material> &material,
  const AnimationPlayer &animationPlayer)
{
  if (mesh)
  {
    static vector<mat4> curTransform;
    const AnimationTree &tree = animationPlayer.get_tree();
  
    curTransform.resize(mesh->bonesMap.size());
    for (uint i = 0; i < tree.nodes.size(); i++)
    {
      auto it2 = mesh->bonesMap.find(tree.nodes[i].get_name());
      if (it2 != mesh->bonesMap.end())
      {
        curTransform[it2->second] = tree.get_bone_transform(i);
      }
    }
    material->set_property("Bones[0]", curTransform);
  }
}

SYSTEM(ecs::SystemOrder::RENDER,ecs::SystemTag::GameEditor) process_mesh_position(
  const Asset<Mesh> &mesh,
  Asset<Material> &material,
  const Transform &transform)
{
  renderQueue.emplace_back(RenderStuff{material, mesh});
  material->set_property("Model", transform.get_transform());
}

SYSTEM(ecs::SystemOrder::RENDER,ecs::SystemTag::GameEditor) render_skeleton_bones(
  const AnimationPlayer &animationPlayer,
  const Transform &transform,
  const Settings &settings)
{
  if (settings.debugBones)
  {
    mat4 t = transform.get_transform();
    const AnimationTree &tree = animationPlayer.get_tree();
    static vector<vec3> boneOffsets;
    if (boneOffsets.size() != tree.nodes.size())
      boneOffsets.resize(tree.nodes.size());

    for (uint i = 0; i < tree.nodes.size(); i++)
    {
      boneOffsets[i] = vec3(tree.get_transform(i)[3]);
    }
    for (uint i = 0; i < tree.nodes.size(); i++)
    {
      vec3 p = tree.nodes[i].parent() >= 0 ? boneOffsets[tree.nodes[i].parent()] : vec3(0.f);
      float width = std::min(0.5f, length(boneOffsets[i] - p) * 0.05f);
      draw_arrow(t, p, boneOffsets[i], vec3(0,0.8f,0), width);
    }
  }
}
bool matComparer (const RenderStuff &a, const RenderStuff &b)
{
    int as = a.material->get_shader().get_shader_program();
    int bs = b.material->get_shader().get_shader_program();
    if (as != bs)
      return as < bs;
    int av = a.mesh->get_vao().vao();
    int bv = b.mesh->get_vao().vao();
    return av < bv;
};
bool emptyRenderStuff(const RenderStuff &a)
{
  return !(a.material && a.mesh);
};
SYSTEM(ecs::SystemOrder::RENDER + 2,ecs::SystemTag::GameEditor)
main_render(EditorRenderSettings &editorSettings)
{
  UniformBuffer &instanceData = get_buffer("InstanceData");
  bool wire_frame = editorSettings.wire_frame; 

  renderQueue.erase(std::remove_if(renderQueue.begin(), renderQueue.end(), emptyRenderStuff), renderQueue.end());
  std::sort(renderQueue.begin(), renderQueue.end(), matComparer);

  if (renderQueue.size() > 0)
  {
    uint instanceCount = 0, instanceSize = renderQueue[0].material->buffer_size();
    RenderStuff prevStuff = renderQueue[0];
    for (const RenderStuff &stuff : renderQueue)
    {
      if (matComparer(prevStuff, stuff)) // prevStuff < p
      {
        prevStuff.material->get_shader().use();
        prevStuff.material->bind_textures_to_shader();
        instanceData.flush_buffer(instanceCount * instanceSize);
        prevStuff.mesh->get_vao().render_instances(instanceCount, wire_frame);
        instanceCount = 0;
        instanceSize = prevStuff.material->buffer_size(); // new size of instance
      }
      if (instanceData.size() < (instanceCount + 1) * instanceSize)
        debug_log("reallocate buffer space");
      stuff.material->set_data_to_buf(instanceData.get_buffer(instanceCount * instanceSize, instanceSize));
      instanceCount++;
      prevStuff = stuff;
    }
    prevStuff.material->get_shader().use();
    prevStuff.material->bind_textures_to_shader();
    instanceData.flush_buffer(instanceCount * instanceSize);
    prevStuff.mesh->get_vao().render_instances(instanceCount, wire_frame);
  }
  renderQueue.clear();

/* 

  QUER() render_debug_goal([&](Transform &debugTransform, const MeshRender &debugGoalSphere)
  {
    QUER() render_debug_goal_on_animplayer([&](
      const AnimationPlayer &animationPlayer,
      const Transform &transform,
      const Settings &settings)
    {
      AnimationLerpedIndex index = animationPlayer.get_motion_matching() ? animationPlayer.get_motion_matching()->get_index() : animationPlayer.get_index();

      mat4 transformation = transform.get_transform();


      Asset<Material> material = debugGoalSphere.get_material();
      const auto& feature = index.current_index().get_feature();
      
      vec3 man = transformation * vec4(feature.features[(int)AnimationFeaturesNode::Hips], 1.f);
      Ray ray(man, vec3(0,-1,0), 100);
      Collision collision = ray_cast(ray);
      draw_arrow(ray.from, collision.collisionPoint, vec3(10,0,0), 0.04f, false);

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
 */

}

SYSTEM(ecs::SystemOrder::RENDER + 101,ecs::SystemTag::GameEditor)
render_debug_arrows(DebugArrow &debugArrows, EditorRenderSettings &editorSettings)
{
  debugArrows.render(editorSettings.wire_frame);

}
/* EVEN() debug_goal_copy_mat(const ecs::OnEntityCreated &, Asset<Mesh> &debugGoalSphere)
{
  debugGoalSphere.get_material() = debugGoalSphere.get_material().copy();
} */