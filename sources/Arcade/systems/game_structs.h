#pragma once
#include <render/sprite.h>
#include <ecs/event.h>
#include <ecs/singleton.h>

struct LoadSceneEvent : public ecs::Event{};

struct StartGameEvent : public ecs::Event{};

struct KillTargetEvent : public ecs::Event{};

constexpr int FigureCount = 6;
struct SpriteFactory : ecs::Singleton
{
  Sprite figures[FigureCount];
  Sprite arrow, bullet;
};

struct ScoreBoard : ecs::Singleton
{
  int curentLevel = 0;
  std::vector<int> kills;
};