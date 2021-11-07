#pragma once
#include <render/sprite.h>

struct StartMenuEvent{};

struct StartGameEvent{};

struct KillTargetEvent{};

constexpr int FigureCount = 6;
struct SpriteFactory : ecs::Singleton
{
  Sprite figures[FigureCount];
  Sprite arrow, bullet;
};

struct ScoreBoard : ecs::Singleton
{
  int curentLevel;
  std::vector<int> kills;
};