#include <ecs.h>
#include <application/time.h>
#include <transform2d.h>
#include <input.h>
#include "world_renderer.h"
#include "game_structs.h"


SYSTEM(stage=act; require=ecs::Tag target) controll_targets(
  Transform2D &transform,
  vec2 &velocity
)
{
  float dt = Time::delta_time();
  vec2 acceleration = rand_vec2() * 15.f;

  constexpr float arenaRadius = 5.f;
  constexpr float additionalForce = 1.5f;
  if (length(transform.position) > arenaRadius)
  {
    acceleration -= normalize(transform.position) * additionalForce;
  }
  velocity += acceleration * dt;
}



EVENT(require=ecs::Tag mainHero) look_at_mouse_position_when_mouse_moves(
  const MouseMoveEvent &event,
  const WorldRenderer &wr,
  Transform2D &transform)
{
  vec2 worldPos = wr.screen_to_world(event.x, event.y);
  vec2 direction = worldPos - transform.position;
  transform.rotation = atan2f(direction.x, direction.y) - PIHALF;
}

void create_bullet(vec2 position, float rotation, float bulletVelocity, const Sprite &bulletSpite)
{
  vec2 velocity = vec2(cos(rotation), sin(rotation));
  constexpr float bulletLifeTime = 2.f;
  float curTime = Time::time();

  ecs::create_entity<Sprite, Transform2D, vec2, float, float, ecs::Tag>(
    {"sprite", bulletSpite},
    {"transform", Transform2D(position, vec2(0.5f), -rotation)},
    {"velocity", velocity * bulletVelocity},
    {"creationTime", curTime},
    {"lifeTime", bulletLifeTime},
    {"bullet", {}}
  );
}
EVENT(require=ecs::Tag mainHero) fire_when_mouse_click(
  const MouseButtonDownEvent<MouseButton::LeftButton> &event,
  const WorldRenderer &wr,
  const SpriteFactory &sf,
  const Transform2D &transform,
  bool isWinner)
{
  if (event.action != MouseAction::Down || isWinner)
    return;
  vec2 worldPos = wr.screen_to_world(event.x, event.y);
  vec2 direction = worldPos - transform.position;
  create_bullet(transform.position, atan2f(direction.y, direction.x), 25, sf.bullet);
}

EVENT(require=ecs::Tag mainHero) circle_attack(
  const KeyDownEvent<SDLK_SPACE> &,
  const Transform2D &transform,
  const SpriteFactory &sf)
{
  for (float angle = 0; angle < PITWO; angle += PITWO / 6)
  {
    create_bullet(transform.position, -transform.rotation + angle, 8, sf.bullet);
  }
}
//stage=act+1
SYSTEM(stage=act;require= ecs::Tag bullet) destroy_old_bullets(
  ecs::EntityId eid,
  float creationTime,
  float lifeTime
)
{
  float curTime = Time::time();
  if (creationTime + lifeTime < curTime)
    ecs::destroy_entity(eid);
}


SYSTEM(stage=act; require=ecs::Tag mainHero) move_hero(
  const Transform2D &transform,
  vec2 &velocity,
  bool isWinner)
{
  if (isWinner)
    return;
  velocity = glm::rotate(
     vec2(Input::get_key(SDLK_w) - Input::get_key(SDLK_s),
          -Input::get_key(SDLK_d) + Input::get_key(SDLK_a)) * 5.f,
      -transform.rotation);
}


