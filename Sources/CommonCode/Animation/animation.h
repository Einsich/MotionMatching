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
  int cadr = 0; 
  float t = 0.f;
  vec3 rootMotion = vec3(0.f);
  float rootRotation = 0;
  vector<vec3> rootMotions;
  vector<float> rootRotations;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
  vec3 get_lerped_pos(const string &name);
  quat get_lerped_rot(const string &name);
  vec3 get_lerped_root_delta_pos();
  float get_lerped_root_delta_rot();
  void update(float dt);
  bool ended();

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
    bool loopable, breakable;
    vector<Edge> edges;
    State(bool loopable = true, bool breakable = true):
      loopable(loopable), breakable(breakable)
    { } 
  };
  struct BlendMode
  {
    Animation *anim1, *anim2;
    State *curState;
    float t;
    const float blendSeconds = 0.25f;
  } blendMode;
  float speed = 1.f;
  mat4 rootTransform;
  BoneRender boneRender;
  
  vector<State> states;
  int anim_index(const string& name);
  int state_index(const string& name);
  void add_states();
  void set_state(const string& name,  bool loopable = true, bool breakable = true);
  void add_edge(const string& from, const string &to, const vector<pair<string, int>> &actions = {});
  bool try_change_state(const string& property, int value);
  void CalculateBonesTransform(AnimationNode &node, mat4 parent, int d);
  mat4 get_lerped_pos(const string &name);
  mat4 get_lerped_rot(const string &name);
  vec3 get_lerped_root_delta_pos();
  float get_lerped_root_delta_rot();
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

  void build_state_machine();
  void crouch(int crouch);
  void jump();
  void set_rotation(float rotation);
  void set_speed(float speed);

  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
using AnimationPlayerPtr = shared_ptr<AnimationPlayer>;