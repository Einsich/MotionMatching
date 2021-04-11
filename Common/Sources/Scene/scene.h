#pragma once
#include "common.h"
#include "GameObject/game_object.h"
#include "Light/direction_light.h"
#include "iscene.h"
#include <vector>
class Scene : IScene
{
private:
  vector<GameObjectPtr> gameObjects;
  DirectionLight sun;
public:
  void init(void (vector<GameObjectPtr>&, DirectionLight&));
 
  void start_scene() override;
  void update_logic() override;
  void update_render() override;
  void update_ui() override;
  void process_events() override;
  void destroy_scene() override;
};