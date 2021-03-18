#pragma once
#include "common.h"
#include "animation.h"
#include "AnimationDatabase/animation_database.h"
#include "animation_index.h"
class AnimationStateMachine
{
private:
class Edge
  {
    vector<pair<string, int>> actions;
  public:
    int to;
    Edge(int to, const vector<pair<string, int>> &actions ):
    actions(actions), to(to) {}
    bool have_action(const string& var, int value)
    {
      for (auto &p : actions)
      {
        if (p.first == var && p.second == value)
          return true;
      }
    return false;
    }
  };
  class State
  {
  public:
    bool loopable, breakable;
    vector<Edge> edges;
    State(bool loopable = true, bool breakable = true):
      loopable(loopable), breakable(breakable) { } 
  };
  struct BlendMode
  {
    Animation *anim1 = nullptr, *anim2= nullptr;
    State *curState= nullptr;
    float t;
    const float blendSeconds = 0.25f;
  } blendMode;
  map<string, int> animationMap;
  AnimationDataBasePtr dataBase;
  vector<State> states;
  vector<Animation> animations;
  int currentAnimation;

  void init_states();
  int anim_index(const string& name) const;
  void set_state(const string& name,  bool loopable = true, bool breakable = true);
  void add_edge(const string& from, const string &to, const vector<pair<string, int>> &actions = {});
  bool try_change_state(const string& property, int value);
public:
  AnimationStateMachine(AnimationDataBasePtr dataBase);

  AnimationLerpedIndex get_current_animation() const;
  void update(float dt);
  void play_animation(int anim_index);
  void crouch(int crouch);
  void jump();
  void set_rotation(float rotation);
  void set_speed(float speed);
};