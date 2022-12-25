#include <ecs/ecs.h>
#include <transform2d.h>
#include "game_structs.h"


EVENT() init_game(const StartGameEvent &, const SpriteFactory &sf, const ScoreBoard &sb)
{
  int targetCount = 100 * (sb.curentLevel + 1);
  float areaRadius = 25.f;
  float minSize = 0.5f, maxSize = 1.5f;
  const ecs::prefab_id targets = ecs::get_prefab_id("targets");
  for (int i = 0; i < targetCount; i++)
  {                                
    ecs::create_entity(targets, {
      {"sprite", sf.figures[rand_int(FigureCount)]},
      {"transform", Transform2D(rand_vec2() * areaRadius,
                                vec2(rand_float(minSize, maxSize)),
                                rand_float() * PITWO)},
      {"velocity", rand_vec2()},
      {"rotationVelocity", rand_float()*PI},
      {"color", vec4(rand_vec3(0.f, 1.f), 1)}
    });
  }
  ecs::create_entity("main_hero", {{"sprite", sf.arrow}});

}