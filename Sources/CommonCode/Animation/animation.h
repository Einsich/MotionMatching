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
  Animation(){}
  Animation(uint duration, AnimationTree& tree, const map<string, vector<quat>>& quats, const map<string, vector<vec3>>& vecs);
  float ticksPerSecond;
  string name;
  int cadr = 0; 
  float t = 0.f;
  vector<AnimationCadr> cadres;
  vector<AnimationFeatures> features;
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
  AnimationCadr get_lerped_cadr();
  int duration() const;
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
  AnimationCadr get_lerped_cadr();

  void calculate_bones_transform();

public:
  vec3 rootDeltaTranslation= vec3(0.f);
  float rootDeltaRotation = 0 ;
  AnimationTree animationTree;
  vector<Animation> animations;
  vector<mat4> curTransform;
  GameObjectPtr gameObject;
  int currentAnimation;
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