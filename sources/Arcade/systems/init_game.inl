#include <ecs.h>
#include <transform2d.h>
#include "game_structs.h"


EVENT() init_game(const StartGameEvent &, const SpriteFactory &sf, const ScoreBoard &sb)
{
  int targetCount = 100 * (sb.curentLevel + 1);
  float areaRadius = 25.f;
  float minSize = 0.5f, maxSize = 1.5f;
  const ecs::BlkTemplate *targets = ecs::get_template("targets");
  for (int i = 0; i < targetCount; i++)
  {
    ecs::ComponentInitializerList list;
    list.set("sprite", sf.figures[rand_int(FigureCount)]);
    list.set("transform", Transform2D(rand_vec2() * areaRadius,
                                vec2(rand_float(minSize, maxSize)),
                                rand_float() * PITWO));
    list.set("velocity", rand_vec2());
    list.set("rotationVelocity", rand_float()*PI);
    list.set("color", vec4(rand_vec3(0.f, 1.f), 1));
                                
    ecs::create_entity(targets, std::move(list));
  }
  const ecs::BlkTemplate *mainHero = ecs::get_template("main_hero");
  ecs::ComponentInitializerList list;
  list.set("sprite", sf.arrow);
  ecs::create_entity(mainHero, std::move(list));

}