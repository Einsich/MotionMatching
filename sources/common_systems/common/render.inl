#include "ecs.h"
#include <application/time.h>
#include "camera.h"
#include "render/shader/copy_buffer_field.h"
#include "render/direction_light.h"
#include "render/global_uniform.h"
#include "render/render.h"
#include <render/skybox.h>
#include <render/debug_arrow.h>
#include "render/texture/textures.h"
#include "resources/resource_registration.h"
#include "resources/resources.h"
#include <imgui.h>
#include <profiler/profiler.h>
#include <render/frustum.h>
#include <parallel/thread_pool.h>

ECS_DECLARE_NAMED_TYPE_EXT(Asset<Mesh>, Mesh)
ECS_DECLARE_NAMED_TYPE_EXT(Asset<Texture2D>, Texture2D)
ECS_DECLARE_NAMED_TYPE_EXT(Asset<Texture2DArray>, Texture2DArray)
ECS_DECLARE_NAMED_TYPE_EXT(Asset<CubeMap>, CubeMap)
ECS_DECLARE_NAMED_TYPE_EXT(Asset<Material>, Material)

EVENT(scene=game, editor) add_global_uniform(const ecs::OnSceneCreated &)
{
  add_uniform_buffer<GlobalRenderData>("GlobalRenderData", 0);
  add_storage_buffer("InstanceData", 0, 1);
  add_storage_buffer("DynamicTransforms", 0, 2);
  add_storage_buffer("StaticTransforms", 0, 3);
}

SYSTEM(stage=ui_menu; scene=editor) render_submenu(EditorRenderSettings &settings)
{
  if (ImGui::BeginMenu("Render"))
  {
    ImGui::Checkbox("wire frame", &settings.wire_frame);
    ImGui::Checkbox("Render collision", &settings.render_collision);
    ImGui::EndMenu();
  }
}

template<typename Callable> 
void find_light(Callable);

SYSTEM(stage=render;scene=game, editor)
set_global_render_data(const MainCamera &mainCamera)
{
  DirectionLight light; 
  QUERY() find_light([&](const DirectionLight &directionalLight){light = directionalLight;});
  float t = Time::time(), dt = Time::delta_time();
  get_buffer("GlobalRenderData").
  update_buffer_and_flush<GlobalRenderData>( 
    {mainCamera.projection * mainCamera.view, mainCamera.projection * mat4(mat3(mainCamera.view)),
    mainCamera.position, light.normalizedLightDirection, 
    light.ambient, light.lightColor, vec4(t , t * 2.f, t * 4.f, dt)});
}
EVENT(scene=game, editor) mesh_loader(const ecs::OnEntityCreated&, Asset<Mesh> &mesh)
{
  if (mesh)
    mesh.load();
}
SYSTEM(stage=render; before=frustum_culling; scene=game, editor;job=true) lod_selector(
  const MainCamera &mainCamera,
  const Transform &transform,
  const vector<Asset<Mesh>> &lods_meshes,
  const vector<float> &lods_distances,
  Asset<Mesh> &mesh,
  const vec3 *lod_selector_axis,
  bool is_enabled)
{
  if (!is_enabled)
    return;
  vec3 delta = transform.get_position() - mainCamera.position;
  if (lod_selector_axis)
    delta *= *lod_selector_axis;
  float distToCameraSq = length2(delta);
  uint lod = lods_meshes.size();
  for (uint i = 0, n = glm::min(lods_distances.size(), lods_meshes.size()); i < n; ++i)
  {
    if (distToCameraSq < lods_distances[i]*lods_distances[i])
    {
      lod = i;
      break;
    }
  }
  if (lod < lods_meshes.size())
  {
    mesh = lods_meshes[lod];
    if (mesh)
      mesh.load();
  }
  else
    mesh = Asset<Mesh>();//culled by dist
}
SYSTEM(stage=render; before=process_mesh_position; scene=game, editor; require=ecs::Tag useFrustumCulling;job=true)
frustum_culling(
  const MainCamera &mainCamera,
  const Transform &transform,
  const Asset<Mesh> &mesh,
  bool &is_visible,
  bool is_enabled)
{
  if (mesh && mesh.loaded() && is_enabled)
    is_visible = isOnFrustum(mainCamera.mainFrustum, transform.get_position(), 
        (transform.get_scale() * mesh->get_bounding_box().diagonal()).length() * 0.5f);
  else
    is_visible = false;
}
struct RenderStuff
{
  ecs::EntityId eid;
  const Material *material;
  const char *label;
  const Mesh *mesh;
  const Transform *transform;
  RenderStuff(ecs::EntityId eid,
    const Material *material,
    const char *label,
    const Mesh *mesh,
    const Transform *transform):
  eid(eid), 
  material(material),
  label(label),
  mesh(mesh),
  transform(transform)
  {}
};
struct RenderQueue : ecs::Singleton
{
  vector<RenderStuff> queue;
};

SYSTEM(stage=render;before=main_instanced_render; scene=game, editor) process_mesh_position(
  const Asset<Mesh> &mesh,
  Asset<Material> &material,
  const Transform &transform,
  const ecs::EntityId &eid,
  RenderQueue &render,
  bool is_visible,
  bool is_enabled)
{
  if (is_enabled && is_visible && material && material->get_shader() && mesh)
  {
    render.queue.emplace_back(eid, material.get(), material.asset_name().c_str(), mesh.get(), &transform);
  }
}

SYSTEM(stage=render; after=main_instanced_render; scene=game, editor)
render_sky_box(SkyBox &skyBox)
{
  ProfilerLabelGPU label("skybox");
  skyBox.render();
}
// after skybox
SYSTEM(stage=render; after=render_sky_box; scene=game, editor) 
render_debug_arrows(DebugArrow &debugArrows, EditorRenderSettings &editorSettings)
{
  UniformBuffer &instanceData = get_buffer("InstanceData");
  debugArrows.render(instanceData, editorSettings.wire_frame);

}

static bool matComparer(const RenderStuff &a, const RenderStuff &b)
{
  int ad = a.material->draw_order();
  int bd = b.material->draw_order();
  if (ad != bd)
    return ad < bd;
  int as = a.material->get_shader().get_shader_program();
  int bs = b.material->get_shader().get_shader_program();
  if (as != bs)
    return as < bs;
  uint av = a.mesh->get_vao();
  uint bv = b.mesh->get_vao();
  return av < bv;
};

template<typename Callable> 
void find_matrices(ecs::EntityId, Callable);

void set_matrices_to_buffer(const Transform &transform, const ShaderBuffer &buffer, char *data)
{
  if (buffer.Bones.type && !transform.get_bones().empty())
    copy_buffer_field(transform.get_bones(), data, buffer.Bones);
}

SYSTEM(stage=render; scene=game, editor)
main_instanced_render(EditorRenderSettings &editorSettings, RenderQueue &render)
{
  UniformBuffer &instanceData = get_buffer("InstanceData");
  bool wire_frame = editorSettings.wire_frame; 

  std::sort(render.queue.begin(), render.queue.end(), matComparer);

  if (!render.queue.empty())
  {
    //debug_log("start");
    {
      ProfilerLabel transforms_copy("transforms copy");
      UniformBuffer &dynamicTransforms = get_buffer("DynamicTransforms");
      const uint instanceSize = sizeof(mat3x4);
      const uint n = render.queue.size();
      dynamicTransforms.bind();
      mat3x4 *buffer = (mat3x4*)dynamicTransforms.get_buffer(0, n * instanceSize);
      for (uint i = 0; i < n; i++)
      {
        const mat3x4 &tm = render.queue[i].transform->get_3x4transform();
        memcpy(buffer + i, &tm, instanceSize);
      }
      dynamicTransforms.flush_buffer(n * instanceSize);
    }
    uint instanceCount = 0, matrixOffset = 0;
    uint sp = 0;
    bool startTransparentPass = false;
    bool zTestEnabled = true;
    instanceData.bind();
    //debug_log("start");
    for (uint i = 0, n = render.queue.size(); i < n; i++)
    {
      const RenderStuff &stuff = render.queue[i];
      const Material &material = *render.queue[i].material;
      const Shader &shader = material.get_shader();
      uint instanceSize = material.buffer_size();
      char *buffer = instanceData.get_buffer(instanceCount * instanceSize, instanceSize);
      material.set_data_to_buf(buffer);
      set_matrices_to_buffer(*stuff.transform, shader.get_instance_data(), buffer);
      instanceCount++;
      bool needRender = i + 1 == n || matComparer(stuff, render.queue[i + 1]); // stuff < next stuff
      if (needRender)
      {
        ProfilerLabelGPU label(stuff.label);
        if (shader.get_shader_program() != sp)//need use new shader
        {
          shader.use();
          sp = shader.get_shader_program();
          if (startTransparentPass != material.is_transparent())
          {
            startTransparentPass = !startTransparentPass;
            if (startTransparentPass)
            {
              glEnable(GL_BLEND);
              glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            else
            {
              glDisable(GL_BLEND);
            }
          }
          if (zTestEnabled != material.need_z_test())
          {
            zTestEnabled = !zTestEnabled;
            if (zTestEnabled)
              glEnable(GL_DEPTH_TEST);
            else
              glDisable(GL_DEPTH_TEST);
          }
        }
        shader.set_int("dynamicTransformsOffset", matrixOffset);
        material.bind_textures_to_shader();
        instanceData.flush_buffer(instanceCount * instanceSize);
        stuff.mesh->render_instances(instanceCount, wire_frame);
        //debug_log("draw instance = %d, instance size = %d, %s",
        //    instanceCount, instanceSize, material.get_shader().get_name().c_str());
        matrixOffset += instanceCount;
        instanceCount = 0;
      }
    }
    if (startTransparentPass)
    {
      glDisable(GL_BLEND);
      glEnable(GL_DEPTH_TEST);
    }
  }
  render.queue.clear();
}



template<typename Callable> 
void find_collidable_entity(Callable);

SYSTEM(stage=render; after=process_mesh_position; before=render_sky_box; scene=game, editor)
render_collision(const EditorRenderSettings &editorSettings)
{
  if (!editorSettings.render_collision)
    return;
  Asset<Mesh> cube = cube_mesh(false);
  Asset<Mesh> sphere = sphere_mesh(4, false);
  Asset<Material> collisionMat = get_resource<Material>("collision");
  if (!cube || !collisionMat || !sphere)
    return;


  UniformBuffer &dynamicTransforms = get_buffer("DynamicTransforms");
  const uint instanceSize = sizeof(mat3x4);
  uint instanceCount = 0;
  QUERY()find_collidable_entity([&](const Transform &transform, const Asset<Mesh> &mesh)
  {
    if (!mesh)
      return;
    const BoundingBox &box = mesh->get_bounding_box();
    Transform tm = transform;
    tm.get_position() += tm.get_scale() * box.center();
    tm.set_scale(tm.get_scale() * box.diagonal() * 0.5f);
    mat3x4 *buffer = (mat3x4*)dynamicTransforms.get_buffer(instanceCount * instanceSize, instanceSize);
    *buffer = tm.get_transform();
    instanceCount++;
  });
  if (instanceCount == 0)
    return;
  ProfilerLabelGPU label("collision");
  dynamicTransforms.bind();
  collisionMat->get_shader().use();
  dynamicTransforms.flush_buffer(instanceCount * instanceSize);
  cube->render_instances(instanceCount, true);
  sphere->render_instances(instanceCount, true);
}