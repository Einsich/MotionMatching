#include "animation_state_machine.h"


AnimationLerpedIndex AnimationStateMachine::get_current_animation() const
{
  return AnimationLerpedIndex(dataBase, 
  blendMode.anim1 ? anim_index(blendMode.anim1->clip.name) : -1,
  blendMode.anim1 ? blendMode.anim1->cadr : -1,
  blendMode.anim2 ? anim_index(blendMode.anim2->clip.name) : -1,
  blendMode.anim2 ? blendMode.anim2->cadr : -1, blendMode.t);
}

void AnimationStateMachine::update(float dt)
{
  blendMode.anim1->update(dt);
  if (blendMode.anim2)
  {
    blendMode.anim2->update(dt);
    blendMode.t += dt / blendMode.blendSeconds;
    if (blendMode.anim2->ended() || blendMode.t > 1.f)
    {
      blendMode.anim2 = nullptr;
      blendMode.t = 0.f;
    }
  }
  if (blendMode.anim1->ended())
  {
    State &state = *blendMode.curState;
    if (!state.loopable)
    {
      if (state.edges.size() == 1)
      {
        play_animation(state.edges[0].to);
      }
      else
      {
        debug_error("Can't play nex cadr in %s", blendMode.anim1->clip.name.c_str());
        blendMode.anim1->cadr = 0;
      }        
    }
    else
    {
      blendMode.anim1->cadr = 0;
    }
  }
}

void AnimationStateMachine::play_animation(int anim_index)
{
  currentAnimation = anim_index = (anim_index + animations.size()) % animations.size();
  blendMode.anim2 = blendMode.anim1 && !blendMode.anim1->ended() ? blendMode.anim1 : nullptr;
  blendMode.t = 0.f;
  blendMode.anim1 = &animations[anim_index];
  blendMode.curState = &states[anim_index];
  blendMode.anim1->cadr = 0;
  blendMode.anim1->t = 0.f;
  debug_log("play %s", animations[anim_index].clip.name.c_str());
}

int AnimationStateMachine::anim_index(const string& name) const
{
  auto it = animationMap.find(name);
  return it == animationMap.end() ? -1 : it->second;
}
void AnimationStateMachine::set_state(const string& name,  bool loopable, bool breakable )
{
  int i = anim_index(name);
  if (i < 0)
  {
    debug_error("Can't find %s animation in state list", name.c_str());
    return;
  }  
  states[i].loopable = loopable;
  states[i].breakable = breakable;
}
void AnimationStateMachine::add_edge(const string& from, const string &to, const vector<pair<string, int>> &actions)
{
  int i = anim_index(from);
  int j = anim_index(to);
  if (i < 0 || j < 0)
  {
    debug_error("Can't find %s (%d) or %s (%d) animation in state list", from.c_str(), i, to.c_str(), j);
    return;
  }
  State&state = states[i];
  state.edges.push_back(Edge(j, actions));
}
bool AnimationStateMachine::try_change_state(const string& property, int value)
{
  State &state = *blendMode.curState;
  if (state.breakable)
  {
    for (uint i = 0; i < state.edges.size(); i++)
    {
      if (state.edges[i].have_action(property, value))
      {
        if (&animations[state.edges[i].to] == blendMode.anim1)
          return true;
        
        play_animation(state.edges[i].to);
        return true;
      }
    }
  }
  return false;
}

AnimationStateMachine::AnimationStateMachine(AnimationDataBasePtr dataBase) :
dataBase(dataBase)
{
  if (!dataBase)
    return;
  for (uint i = 0; i < dataBase->clips.size(); i++)
  {
    animations.push_back(Animation(dataBase->clips[i]));
    states.push_back(State());
    animationMap[dataBase->clips[i].name] = i;
  }
  init_states();
}
void AnimationStateMachine::crouch(int crouch)
{
  try_change_state("crouch", crouch);
}
void AnimationStateMachine::jump()
{
  try_change_state("jump", 1);
}
void AnimationStateMachine::set_rotation(float rotation)
{
  int r = glm::abs(rotation) < 0.01f ? 0 : rotation < 0 ? -1 : 1;

  try_change_state("rotation", r);
}
void AnimationStateMachine::set_speed(float speed)
{
  int s = glm::abs(speed) < 0.01f ? 0 : glm::abs(speed) <= 1.5f ? 1 : 2;

  try_change_state("speed", s);
}

void AnimationStateMachine::init_states()
{
  set_state("MOB1_Crouch_Idle_V2");
  add_edge("MOB1_Crouch_Idle_V2", "MOB1_Crouch_Rlx_Turn_In_Place_L_Loop", {{"rotation", -1}});
  add_edge("MOB1_Crouch_Idle_V2", "MOB1_Crouch_Rlx_Turn_In_Place_R_Loop", {{"rotation", 1}});
  add_edge("MOB1_Crouch_Idle_V2", "MOB1_Crouch_To_Stand_Relaxed", {{"crouch", 0}});
  add_edge("MOB1_Crouch_Idle_V2", "MOB1_Crouch_To_CrouchWalk_F", {{"speed", 1}, {"speed", 2}});

  set_state("MOB1_Crouch_Rlx_Turn_In_Place_R_Loop");
  add_edge("MOB1_Crouch_Rlx_Turn_In_Place_R_Loop", "MOB1_Crouch_Idle_V2", {{"rotation", 0}});
  add_edge("MOB1_Crouch_Rlx_Turn_In_Place_R_Loop", "MOB1_Crouch_Rlx_Turn_In_Place_L_Loop", {{"rotation", -1}});

  set_state("MOB1_Crouch_Rlx_Turn_In_Place_L_Loop");
  add_edge("MOB1_Crouch_Rlx_Turn_In_Place_L_Loop", "MOB1_Crouch_Idle_V2", {{"rotation", 0}});
  add_edge("MOB1_Crouch_Rlx_Turn_In_Place_L_Loop", "MOB1_Crouch_Rlx_Turn_In_Place_R_Loop", {{"rotation", 1}});

  set_state("MOB1_Crouch_To_Stand_Relaxed", false);
  add_edge("MOB1_Crouch_To_Stand_Relaxed", "MOB1_Stand_Relaxed_Idle_v2");

  set_state("MOB1_Crouch_To_CrouchWalk_F", false);
  add_edge("MOB1_Crouch_To_CrouchWalk_F", "MOB1_CrouchWalk_F_Loop");


  set_state("MOB1_CrouchWalk_F_Loop");
  add_edge("MOB1_CrouchWalk_F_Loop", "MOB1_CrouchWalk_F_To_Crouch_RU", {{"speed", 0}});

  set_state("MOB1_CrouchWalk_F_To_Crouch_RU", false);
  add_edge("MOB1_CrouchWalk_F_To_Crouch_RU", "MOB1_Crouch_Idle_V2");


  set_state("MOB1_Stand_Relaxed_Idle_v2");
  add_edge("MOB1_Stand_Relaxed_Idle_v2", "MOB1_Stand_Rlx_Turn_In_Place_R_Loop", {{"rotation", -1}});
  add_edge("MOB1_Stand_Relaxed_Idle_v2", "MOB1_Stand_Rlx_Turn_In_Place_L_Loop", {{"rotation", 1}});
  add_edge("MOB1_Stand_Relaxed_Idle_v2", "MOB1_Stand_Relaxed_To_Crouch", {{"crouch", 1}});
  add_edge("MOB1_Stand_Relaxed_Idle_v2", "MOB1_Stand_Relaxed_To_Walk_F", {{"speed", 1}});
  add_edge("MOB1_Stand_Relaxed_Idle_v2", "MOB1_Stand_Relaxed_To_Jog_F", {{"speed", 2}});

  set_state("MOB1_Stand_Rlx_Turn_In_Place_R_Loop");
  add_edge("MOB1_Stand_Rlx_Turn_In_Place_R_Loop", "MOB1_Stand_Relaxed_Idle_v2", {{"rotation", 0}});
  add_edge("MOB1_Stand_Rlx_Turn_In_Place_R_Loop", "MOB1_Stand_Rlx_Turn_In_Place_L_Loop", {{"rotation", -1}});

  set_state("MOB1_Stand_Rlx_Turn_In_Place_L_Loop");
  add_edge("MOB1_Stand_Rlx_Turn_In_Place_L_Loop", "MOB1_Stand_Relaxed_Idle_v2", {{"rotation", 0}});
  add_edge("MOB1_Stand_Rlx_Turn_In_Place_L_Loop", "MOB1_Stand_Rlx_Turn_In_Place_R_Loop", {{"rotation", 1}});

  set_state("MOB1_Stand_Relaxed_To_Crouch", false);
  add_edge("MOB1_Stand_Relaxed_To_Crouch", "MOB1_Crouch_Idle_V2");

  set_state("MOB1_Stand_Relaxed_To_Walk_F", false);
  add_edge("MOB1_Stand_Relaxed_To_Walk_F", "MOB1_Walk_F_Loop",{{"speed", 0}});

  set_state("MOB1_Stand_Relaxed_To_Jog_F", false);
  add_edge("MOB1_Stand_Relaxed_To_Jog_F", "MOB1_Jog_F_Loop");

  set_state("MOB1_Walk_F_To_Stand_Relaxed_RU", false);
  add_edge("MOB1_Walk_F_To_Stand_Relaxed_RU", "MOB1_Stand_Relaxed_Idle_v2", {{"rotation", 1},{"rotation", -1}});

  set_state("MOB1_Walk_F_Loop");
  add_edge("MOB1_Walk_F_Loop", "MOB1_Walk_F_Jump_RU", {{"jump", 1}});
  add_edge("MOB1_Walk_F_Loop", "MOB1_Walk_F_To_Stand_Relaxed_RU", {{"speed", 0}});
  add_edge("MOB1_Walk_F_Loop", "MOB1_Jog_F_Loop", {{"speed", 2}});

  set_state("MOB1_Jog_F_Loop");
  add_edge("MOB1_Jog_F_Loop", "MOB1_Jog_F_Jump", {{"jump", 1}});
  add_edge("MOB1_Jog_F_Loop", "MOB1_Walk_F_Loop", {{"speed", 0}, {"speed", 1}});

  set_state("MOB1_Jog_F_Jump", false);
  add_edge("MOB1_Jog_F_Jump", "MOB1_Jog_F_Loop");

  set_state("MOB1_Walk_F_Jump_RU", false);
  add_edge("MOB1_Walk_F_Jump_RU", "MOB1_Walk_F_Loop");
}