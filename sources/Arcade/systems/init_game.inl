#include <ecs.h>
#include <transform2d.h>
#include "game_structs.h"


EVENT() init_game(const StartGameEvent &, const SpriteFactory &sf, const ScoreBoard &sb)
{
  int targetCount = 100 * (sb.curentLevel + 1);
  float areaRadius = 25.f;
  float minSize = 0.5f, maxSize = 1.5f;
  for (int i = 0; i < targetCount; i++)
  {
    ecs::create_entity<Sprite, Transform2D, vec2, float, vec4, bool, ecs::Tag>(
      {"sprite", sf.figures[rand_int(FigureCount)]},
      {"transform", Transform2D(rand_vec2() * areaRadius,
                                vec2(rand_float(minSize, maxSize)),
                                rand_float() * PITWO)},
      {"velocity", rand_vec2()},
      {"rotationVelocity", rand_float()*PI},
      {"color", vec4(rand_vec3(0.f, 1.f), 1)},
      {"destroyed", false},
      {"target", {}}
    );
  }
  ecs::create_entity<Sprite, Transform2D, vec2, vec4, int, bool, ecs::Tag>(
    {"sprite", sf.arrow},
    {"transform", Transform2D(vec2(0.f), vec2(1.f))},
    {"velocity", vec2(0.f)},
    {"color", vec4(1, 0.84f, 0, 1)},
    {"killsCount", 0},
    {"isWinner", false},
    {"mainHero", {}}
  );

}