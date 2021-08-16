#include <ecs/ecs.h> 
#include <Engine/camera.h" 
#include <Engine/time.h" 
#include <Engine/Render/mesh_render.h"

template<typename Callable> 
void render_meshes(Callable); 
template<typename Callable> 
void render_flags(Callable); 
 
template<typename Callable> 
void render_skybox(Callable); 

template<typename Callable> 
void find_light(Callable); 
 
 
SYSTEM(ecs::SystemOrder::RENDER) main_render() 
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
  mat4 viewProjectionSkybox = camProjection *  mat4(mat3(viewTrasform)); 
 
  DirectionLight light; 
  find_light([&](const DirectionLight &directional_light){light = directional_light;});
  bool wire_frame = false;  
 
 
  QUERY() render_meshes([&]( 
    const MeshRender &meshRender, 
    const Transform &transform) 
  { 
    meshRender.render(transform, viewProjection, cameraPosition, light, wire_frame); 
  }); 
  QUERY() render_flags([&]( 
    const MeshRender &flagRender, 
    const Transform &transform) 
  { 
    glDisable(GL_CULL_FACE); 
    flagRender.get_shader().set_float("Time", Time::time()); 
    flagRender.render(transform, viewProjection, cameraPosition, light, wire_frame); 
    glEnable(GL_CULL_FACE); 
  }); 
 
  QUERY() render_skybox([&]( 
    SkyBox &skyBox) 
  { 
    skyBox.render(viewProjectionSkybox, cameraPosition, wire_frame); 
  }); 
}