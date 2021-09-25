#include "ecs/ecs.h"
#include "Engine/camera.h"
#include "Engine/Render/direction_light.h"
#include "Engine/Render/global_uniform.h"
#include "Engine/Render/render.h"
#include <Engine/Render/skybox.h>
#include <Engine/Render/debug_arrow.h>
#include "Engine/Render/Texture/textures.h"
#include "Engine/Resources/resources.h"
#include <Engine/imgui/imgui.h>

REGISTER_TYPE_NAMED(Asset<Mesh>, mesh)
REGISTER_TYPE_NAMED(Asset<Texture2D>, Texture2D)
REGISTER_TYPE_NAMED(Asset<Material>, material)

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

SYSTEM(ecs::SystemOrder::RENDER,ecs::SystemTag::GameEditor) process_mesh_position(
  const Asset<Mesh> &mesh,
  Asset<Material> &material,
  const Transform &transform,
  RenderQueue &render)
{
  if (material && mesh)
  {
    render.queue.emplace_back(RenderStuff{material, mesh});
    //material->set_property("Model", transform.get_transform());
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
main_instanced_render(EditorRenderSettings &editorSettings, RenderQueue &render)
{
  UniformBuffer &instanceData = get_buffer("InstanceData");
  bool wire_frame = editorSettings.wire_frame; 

  render.queue.erase(std::remove_if(render.queue.begin(), render.queue.end(), emptyRenderStuff), render.queue.end());
  std::sort(render.queue.begin(), render.queue.end(), matComparer);

  if (render.queue.size() > 0)
  {
    uint instanceCount = 0, instanceSize = render.queue[0].material->buffer_size();
    RenderStuff prevStuff = render.queue[0];
    uint sp = 0;
    for (const RenderStuff &stuff : render.queue)
    {
      if (matComparer(prevStuff, stuff)) // prevStuff < p
      {
        if (prevStuff.material->get_shader().get_shader_program() != sp)
        {
          prevStuff.material->get_shader().use();
          sp = prevStuff.material->get_shader().get_shader_program();
        }
        prevStuff.material->bind_textures_to_shader();
        instanceData.flush_buffer(instanceCount * instanceSize);
        prevStuff.mesh->get_vao().render_instances(instanceCount, wire_frame);
        instanceCount = 0;
        instanceSize = stuff.material->buffer_size(); // new size of instance
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
  render.queue.clear();
}