#include <ecs.h> 
#include "application/application.h" 
#include <render/render.h> 
#include <camera.h> 
#include "resources/resource_registration.h"
#include <resources/resources.h>
 
Asset<Mesh> create_flag(float flag_h, float flag_w, float stick_h, float stick_w) 
{ 
  vector<uint> indices; 
  vector<vec3> poses, normes; 
  vector<vec2> uves; 
   
  uint N = 20; 
  for (uint i = 0; i <= N; i++) 
    for (uint j = 0; j <= N; j++) 
    { 
      float y = i * 1.f / N, x = j * 1.f / N; 
      poses.push_back(vec3(x * flag_w, stick_h + y * flag_h, 0.f)); 
      normes.push_back(vec3(0.f, 0.f, 1.f)); 
      uves.push_back(vec2(x,y)); 
    } 
  for (uint i = 0; i < N; i++) 
    for (uint j = 0; j < N; j++) 
    { 
      uint a = i * (N+1) + j, b = a + 1, c = a + N+1, d = c + 1; 
      indices.insert(indices.end(),{a,b,c, b,d,c}); 
    } 
  N = 8; 
  uint k = poses.size(); 
  stick_h += flag_h; 
  for (uint i = 0; i <= N; i++) 
    for (uint j = 0; j <= N; j++) 
    { 
      float y = i * 1.f / N, x = j * 1.f / N; 
      float nx = cos(x*PITWO), ny = sin(x*PITWO); 
      poses.push_back(vec3(nx  * stick_w, y * stick_h, ny * stick_w)); 
      normes.push_back(vec3(nx, 0.f, ny)); 
      uves.push_back(vec2(x,y)); 
    } 
  for (uint i = 0; i < N; i++) 
    for (uint j = 0; j < N; j++) 
    { 
      uint a = k + i * (N+1) + j, b = a + 1, c = a + N+1, d = c + 1; 
      indices.insert(indices.end(),{a,b,c, b,d,c}); 
    } 
  return Asset<Mesh>("flag mesh", VertexArrayObject(indices, poses, normes, uves)); 
} 
EVENT() scene_created(const ecs::OnSceneCreated&) 
{ 
  Asset<Mesh> flag = create_flag(1, 2, 1.5f, 0.02f); 
  Asset<Material> flag_mat = get_resource<Material>("flag");
   
  uint N = 5; 
  for (uint i = 0; i < N; i++) 
    for (uint j = 0; j < N; j++) 
    {
      ecs::create_entity<Transform, Asset<Mesh>, Asset<Material>>
       ({"transform", Transform(vec3(j,0,i)* 5.f)}, {"mesh", flag}, {"material", flag_mat}); 
    } 
 
} 