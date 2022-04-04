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

ECS_DECLARE_NAMED_TYPE_EXT(Asset<Mesh>, Mesh)
ECS_DECLARE_NAMED_TYPE_EXT(Asset<Texture2D>, Texture2D)
ECS_DECLARE_NAMED_TYPE_EXT(Asset<Texture2DArray>, Texture2DArray)
ECS_DECLARE_NAMED_TYPE_EXT(Asset<CubeMap>, CubeMap)
ECS_DECLARE_NAMED_TYPE_EXT(Asset<Material>, Material)

EVENT(ecs::SystemTag::GameEditor) add_global_uniform(const ecs::OnSceneCreated &)
{
  add_uniform_buffer<GlobalRenderData>("GlobalRenderData", 0);
  add_storage_buffer("InstanceData", 0, 1);
}

SYSTEM(ecs::SystemOrder::UIMENU, ecs::SystemTag::Editor) render_submenu(EditorRenderSettings &settings)
{
  if (ImGui::BeginMenu("Render"))
  {
    ImGui::Checkbox("wire frame", &settings.wire_frame);
    ImGui::EndMenu();
  }
}

template<typename Callable> 
void find_light(Callable);

SYSTEM(ecs::SystemOrder::RENDER,ecs::SystemTag::GameEditor)
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
EVENT(ecs::SystemTag::GameEditor) mesh_loader(const ecs::OnEntityCreated&, Asset<Mesh> &mesh)
{
  if (mesh)
    mesh.load();
}
SYSTEM(ecs::SystemOrder::RENDER-1,ecs::SystemTag::GameEditor) lod_selector(
  const MainCamera &mainCamera,
  const Transform &transform,
  const vector<Asset<Mesh>> &lods_meshes,
  const vector<float> &lods_distances,
  Asset<Mesh> &mesh,
  const vec3 *lod_selector_axis)
{
  vec3 delta = transform.get_position() - mainCamera.position;
  if (lod_selector_axis)
    delta *= *lod_selector_axis;
  float distToCameraSq = length2(delta);
  uint lod = lods_meshes.size();
  for (uint i = 0; i < lods_distances.size() && i < lods_meshes.size(); ++i)
  {
    if (distToCameraSq < lods_distances[i]*lods_distances[i])
    {
      lod = i;
      break;
    }
  }
  if (lod < lods_meshes.size())
    mesh = lods_meshes[lod];
  else
    mesh = Asset<Mesh>();//culled by dist
  if (mesh)
    mesh.load();
}

struct RenderStuff
{
  ecs::EntityId eid;
  Asset<Material> material;
  Asset<Mesh> mesh;
};
struct RenderQueue : ecs::Singleton
{
  vector<RenderStuff> queue;
};

SYSTEM(ecs::SystemOrder::RENDER,ecs::SystemTag::GameEditor) process_mesh_position(
  const Asset<Mesh> &mesh,
  Asset<Material> &material,
  const ecs::EntityId &eid,
  RenderQueue &render)
{
  if (material && mesh)
  {
    render.queue.emplace_back(RenderStuff{eid, material, mesh});
  }
}

SYSTEM(ecs::SystemOrder::RENDER+100,ecs::SystemTag::GameEditor)
render_sky_box(SkyBox &skyBox)
{
  skyBox.render();
}
// after skybox
SYSTEM(ecs::SystemOrder::RENDER+101,ecs::SystemTag::GameEditor) 
render_debug_arrows(DebugArrow &debugArrows, EditorRenderSettings &editorSettings)
{
  UniformBuffer &instanceData = get_buffer("InstanceData");
  debugArrows.render(instanceData, editorSettings.wire_frame);

}

bool matComparer(const RenderStuff &a, const RenderStuff &b)
{
  int ad = a.material->draw_order();
  int bd = b.material->draw_order();
  if (ad != bd)
    return ad < bd;
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
  return !(a.material && a.material->get_shader() && a.mesh);
};

template<typename Callable> 
void find_matrices(ecs::EntityId, Callable);

void set_matrices_to_buffer(ecs::EntityId eid, const ShaderBuffer &buffer, char *data)
{
  QUERY() find_matrices(eid, [&](const Transform *transform)
  {
    if (transform)
    {
      if (buffer.Model.type)
        copy_buffer_field(transform->get_transform(), data, buffer.Model);
      if (buffer.Bones.type && !transform->get_bones().empty())
        copy_buffer_field(transform->get_bones(), data, buffer.Bones);
    }
  });
}

SYSTEM(ecs::SystemOrder::RENDER + 2,ecs::SystemTag::GameEditor)
main_instanced_render(EditorRenderSettings &editorSettings, RenderQueue &render)
{
  UniformBuffer &instanceData = get_buffer("InstanceData");
  bool wire_frame = editorSettings.wire_frame; 

  render.queue.erase(std::remove_if(render.queue.begin(), render.queue.end(), emptyRenderStuff), render.queue.end());
  std::sort(render.queue.begin(), render.queue.end(), matComparer);

  if (render.queue.size() > 0)
  {
    uint instanceCount = 0;
    uint sp = 0;
    bool startTransparentPass = false;
    instanceData.bind();
    //debug_log("start");
    for (uint i = 0; i < render.queue.size(); i++)
    {
      const RenderStuff &stuff = render.queue[i];
      uint instanceSize = stuff.material->buffer_size();
      char *buffer = instanceData.get_buffer(instanceCount * instanceSize, instanceSize);
      memset(buffer, 0, instanceSize);
      stuff.material->set_data_to_buf(buffer);
      set_matrices_to_buffer(stuff.eid, stuff.material->get_shader().get_instance_data(), buffer);
      instanceCount++;
      bool needRender = i + 1 == render.queue.size() || matComparer(stuff, render.queue[i + 1]); // stuff < next stuff
      if (needRender)
      {
        if (stuff.material->get_shader().get_shader_program() != sp)//need use new shader
        {
          stuff.material->get_shader().use();
          sp = stuff.material->get_shader().get_shader_program();
          if (stuff.material->is_transparent() && !startTransparentPass)
          {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            startTransparentPass = true;
          }
        }
        stuff.material->bind_textures_to_shader();
        instanceData.flush_buffer(instanceCount * instanceSize);
        stuff.mesh->get_vao().render_instances(instanceCount, wire_frame);
        //debug_log("draw instance = %d, instance size = %d, %s",
        //    instanceCount, instanceSize, stuff.material->get_shader().get_name().c_str());
        instanceCount = 0;
      }
    }
    if (startTransparentPass)
    {
      glDisable(GL_BLEND);
    }
  }
  render.queue.clear();
}