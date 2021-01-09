#pragma once
#include "CommonCode/common.h"
#include "CommonCode/math.h"
#include "animation_channel.h"
#include "animation_tree.h"
#include "animation_state_machine.h"
#include "bone_render.h"
#include "CommonCode/GameObject/game_object.h"
#include "CommonCode/Serialization/serialization.h"
#include "CommonCode/Event/input.h"
#include <map>


class Animation: public ISerializable
{
private:

public:
  uint duration;
  float ticksPerSecond;
  string name;
  map<string, AnimationChannel> channels;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;

};

class AnimationPlayer: public ISerializable
{
private:
class Edge
  {
    vector<pair<string, int>> actions;
  public:
    int to;
    Edge(int to, const vector<pair<string, int>> &actions ):
    actions(actions), to(to)
    {}
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
    string name;
    int index;
    uint duration;
    bool loopable, breakable;
    vector<Edge> edges;
    State(int anim_index, const vector<Animation>& animations, bool loopable = true, bool breakable = true):
      name(animations[anim_index].name), index(anim_index), duration(animations[anim_index].duration),
      loopable(loopable), breakable(breakable)
    {
      
    } 
  };
  uint curCadr = 0, curAnim = 0;
  float speed = 1.f, curT = 0;
  mat4 rootTransform;
  BoneRender boneRender;
  bool rootMotion = false;
  vec3 rootTranslation = vec3(0.f);
  float rootRotation = 0;
  vector<State> states;
  map<string, int> state_values;
  int anim_index(const string& name);
  int state_index(const string& name);
  void add_states();
  void set_state(const string& name,  bool loopable = true, bool breakable = true);
  void add_edge(const string& from, const string &to, const vector<pair<string, int>> &actions = {});
  bool try_change_state(const string& property, int value);
  void CalculateBonesTransform(AnimationNode &node, mat4 parent, int d);
public:
  vec3 rootDeltaTranslation= vec3(0.f);
  float rootDeltaRotation = 0 ;
  AnimationTree animationTree;
  vector<Animation> animations;
  vector<mat4> curTransform;
  GameObjectPtr gameObject;
  void PlayNextCadr();
  void render(const Camera& mainCam, const DirectionLight& light);
  int cadr_count();
  void play_animation(int anim_index);
  void animation_selector(const KeyboardEvent &event);
  void set_root_motion(bool root_motion);

  void build_state_machine();
  void crouch(int crouch);
  void jump();
  void set_rotation(float rotation);
  void set_speed(float speed);

  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
using AnimationPlayerPtr = shared_ptr<AnimationPlayer>;