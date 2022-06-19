#include <ecs.h>
#include <application/application.h>
#include <render/shader/shader.h>
#include <transform2d.h>
#include "game_structs.h"
#include "world_renderer.h"

EVENT() init_sprites_shaders_camera(
  const ecs::OnSceneCreated &,
  WorldRenderer &wr,
  SpriteFactory &sf)
{
  //feed in project_resources_path relative path from Arcada/Projects/ArcadeSample/Resources/**my_path**
  Texture2D *figuresTex = 
    wr.add_texture(new Texture2D(project_path("figures.png"), RGBA));
  Texture2D *arrowTex = 
    wr.add_texture(new Texture2D(project_path("arrow.png"), RGBA, UnsignedByte, Linear, ClampToBorder));
  Texture2D *bulletTex = 
    wr.add_texture(new Texture2D(project_path("bullet.png"), RGBA));
  
  float figureWidth = 1.f/FigureCount, figureHeight =  1.f/2.f;
  vec2 figureSize(figureWidth, figureHeight);
  for (int i = 0; i < FigureCount; i++)
    sf.figures[i] = Sprite(figuresTex, vec2(figureWidth*i, 0), figureSize);

  sf.arrow = Sprite(arrowTex);
  sf.bullet = Sprite(bulletTex);

  wr.spriteShader = get_shader("standart_sprite");
  
}

EVENT() load_scene_event(const LoadSceneEvent &)
{
  ecs::create_scene(root_path("resources/Arcade/Scenes/game_scene.blk").c_str());
}