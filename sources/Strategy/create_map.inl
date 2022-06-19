#include <ecs.h> 
#include <render/render.h> 
#include <transform.h> 
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
  return Asset<Mesh>("flag mesh", indices, poses, normes, uves); 
} 
EVENT() scene_created(const ecs::OnSceneCreated&) 
{ 
  return;
  Asset<Mesh> flag = create_flag(1, 2, 1.5f, 0.02f); 
  Asset<Material> flag_mat = get_resource<Material>("flag");
  const ecs::Template *flagTemplate = ecs::create_template("flag", {
      {"transform", Transform()},
      {"mesh", flag},
      {"material", flag_mat}
  });

  uint N = 5; 
  for (uint i = 0; i < N; i++) 
    for (uint j = 0; j < N; j++) 
    {
      ecs::create_entity(flagTemplate, {{"transform", Transform(vec3(j,0,i)* 5.f)}});
    } 
 
} 

EVENT(scene=game, editor) spawn_buildings(const ecs::OnEntityCreated&,
  const ecs::vector<ecs::string> &items_templates,
  vec3 center,
  float step_length,
  int row_count
)
{
  return;
  row_count = row_count <= 0 ? 1 : row_count;
  int i = 0;
  for (const ecs::string &template_name : items_templates)
  {
    ecs::ComponentInitializerList list;
    const ecs::Template *t = ecs::get_template(template_name.c_str());
    Transform transform;
    for (const auto &component : t->components)
    {
      
      if (component.typeNameHash == ecs::get_type_description<Transform>("transform"))
      {
        transform = *(const Transform *)component.get_data();
        break;
      }
    }
    int x = i / row_count;
    int y = i % row_count;
    transform.set_position(center + vec3(x, 0, y) * step_length);
    ecs::create_entity(t, {{"transform", transform}});
    i++;
  }
}