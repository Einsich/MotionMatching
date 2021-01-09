#include "animation.h"
#include "CommonCode/Time/time.h"
#include "CommonCode/Light/direction_light.h"
void AnimationPlayer::CalculateBonesTransform(AnimationNode &node, mat4 parent, int d)
{
  mat4 nodeTransform = node.transform;
  auto it = animations[curAnim].channels.find(node.name);
  if (it != animations[curAnim].channels.end())
  {
    AnimationChannel& channel = it->second;
    
    if (d == 1)
    {
      if (rootMotion)
      {
        nodeTransform = channel.get_lerped_translation(curCadr, curT) * mat4(mat3(nodeTransform)) * channel.get_lerped_rotation(curCadr, curT);
      }
      else
      {
        vec3 translation;
        float rotation;
        nodeTransform = channel.get_lerped_locked_translation(curCadr, curT, translation) * mat4(mat3(nodeTransform)) *
           channel.get_lerped_locked_rotation(curCadr, curT, rotation);
        if (curCadr != 0)
        {
          rootDeltaTranslation = translation - rootTranslation;
          rootDeltaRotation = rotation - rootRotation;
        }
        else
        {
          rootDeltaTranslation = vec3(0.f);
          rootDeltaRotation = 0;
        }
        rootTranslation = translation;
        rootRotation = rotation;
      }
      
    }   
    else
    {
      nodeTransform = nodeTransform * channel.get_lerped_rotation(curCadr, curT);
    }             
  }
  node.animationTransform = nodeTransform;
  nodeTransform = parent * nodeTransform;

  auto it2 = gameObject->get_mesh()->bonesMap.find(node.name);
  if (it2 != gameObject->get_mesh()->bonesMap.end())
  {
    curTransform[it2->second] = nodeTransform * node.meshToBone;
  }
  for (uint i = 0; i < node.childs.size(); i++)
  {
    CalculateBonesTransform(animationTree.nodes[node.childs[i]], nodeTransform, d + 1);
  }
}
void AnimationPlayer::PlayNextCadr()
{
  
  CalculateBonesTransform(animationTree.nodes[0], mat4(1.f), 0);
  
  curT += speed * Time::delta_time() * animations[curAnim].ticksPerSecond;
  int skippedCardes = (int)curT;
  curT -= skippedCardes;
  curCadr += skippedCardes;
  if (curCadr >= states[curAnim].duration - 1)
  {
    curCadr = 0;
    curT = 0;
    State &state = states[curAnim];
    if (!state.loopable)
    {
      if (state.edges.size() == 1)
      {
        play_animation(state.edges[0].to);
      }
      else
      {
        debug_error("Can't play nex cadr in %s", state.name.c_str());
      }        
    }
  }
}
int AnimationPlayer::cadr_count()
{
  int count = 0;
  for (Animation & anim: animations)
    count += anim.duration;
  return count;
}
void AnimationPlayer::render(const Camera& mainCam, const DirectionLight& light)
{
  gameObject->get_shader().use();
  gameObject->get_shader().set_mat4x4("Bones", curTransform, false);
  gameObject->render(mainCam, light);
  //boneRender.render(gameObject->get_transform().get_transform(), animationTree, mainCam, light);
}

void AnimationPlayer::play_animation(int anim_index)
{
  curAnim = (anim_index + animations.size()) % animations.size();
  curCadr = 0;
  debug_log("play %s", animations[curAnim].name.c_str());
}
void AnimationPlayer::animation_selector(const KeyboardEvent &event)
{
  return;
  if (event.keycode == SDLK_RIGHT)
    play_animation(curAnim + 1);
  if (event.keycode == SDLK_LEFT)
    play_animation(curAnim - 1);
  
}
void AnimationPlayer::set_root_motion(bool root_motion)
{
  rootMotion = root_motion;
}

size_t Animation::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, duration);
  size += write(os, ticksPerSecond);
  size += write(os, name);
  size += write(os, channels);
  return size;
}
size_t Animation::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, duration);
  size += read(is, ticksPerSecond);
  size += read(is, name);
  size += read(is, channels);
  return size;
}
size_t AnimationPlayer::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, animationTree);
  size += write(os, animations);
  size += write(os, curTransform.size());
  return size;
}
size_t AnimationPlayer::deserialize(std::istream& is)
{
  size_t size = 0;
  size_t nodeCount;
  size += read(is, animationTree);
  size += read(is, animations);
  size += read(is, nodeCount);
  curTransform.resize(nodeCount);
  build_state_machine();
  return size;
}


int AnimationPlayer::anim_index(const string& name)
{
  for (int i = 0; i < animations.size(); i++)
  {
    if (animations[i].name == name)
      return i;
  }
  return -1;
}
int AnimationPlayer::state_index(const string& name)
{
  for (int i = 0; i < states.size(); i++)
  {
    if (states[i].name == name)
      return i;
  }
  return -1;
}
void AnimationPlayer::add_states()
{
  for (int i = 0; i < animations.size(); i++)
  {
    states.push_back(State(i, animations));
  }
}
void AnimationPlayer::set_state(const string& name,  bool loopable, bool breakable )
{
  int i = state_index(name);
  if (i < 0)
  {
    debug_error("Can't find %s animation in state list", name.c_str());
    return;
  }  
  states[i].loopable = loopable;
  states[i].breakable = breakable;
}
void AnimationPlayer::add_edge(const string& from, const string &to, const vector<pair<string, int>> &actions)
{
  int i = state_index(from);
  int j = state_index(to);
  if (i < 0 || j < 0)
  {
    debug_error("Can't find %s (%d) or %s (%d) animation in state list", from.c_str(), i, to.c_str(), j);
    return;
  }
  State&state = states[i];
  state.edges.push_back(Edge(j, actions));
}
bool AnimationPlayer::try_change_state(const string& property, int value)
{
  State &state = states[curAnim];
  if (state.breakable)
  {
    for (int i = 0; i < state.edges.size(); i++)
    {
      if (state.edges[i].have_action(property, value))
      {
        if ( state.edges[i].to == curAnim)
          return true;
        
        state_values[property] = value;
        play_animation(state.edges[i].to);
        return true;
      }
    }
  }
  return false;
}

void AnimationPlayer::build_state_machine()
{
  add_states();

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

  set_state("MOB1_Crouch_To_Stand_Relaxed", false, false);
  add_edge("MOB1_Crouch_To_Stand_Relaxed", "MOB1_Stand_Relaxed_Idle_v2");

  set_state("MOB1_Crouch_To_CrouchWalk_F", false, false);
  add_edge("MOB1_Crouch_To_CrouchWalk_F", "MOB1_CrouchWalk_F_Loop");


  set_state("MOB1_CrouchWalk_F_Loop");
  add_edge("MOB1_CrouchWalk_F_Loop", "MOB1_CrouchWalk_F_To_Crouch_RU", {{"speed", 0}});

  set_state("MOB1_CrouchWalk_F_To_Crouch_RU", false, false);
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

  set_state("MOB1_Stand_Relaxed_To_Crouch", false, false);
  add_edge("MOB1_Stand_Relaxed_To_Crouch", "MOB1_Crouch_Idle_V2");

  set_state("MOB1_Stand_Relaxed_To_Walk_F", false, false);
  add_edge("MOB1_Stand_Relaxed_To_Walk_F", "MOB1_Walk_F_Loop");

  set_state("MOB1_Stand_Relaxed_To_Jog_F", false, false);
  add_edge("MOB1_Stand_Relaxed_To_Jog_F", "MOB1_Jog_F_Loop");

  set_state("MOB1_Walk_F_To_Stand_Relaxed_RU", false, false);
  add_edge("MOB1_Walk_F_To_Stand_Relaxed_RU", "MOB1_Stand_Relaxed_Idle_v2");

  set_state("MOB1_Walk_F_Loop");
  add_edge("MOB1_Walk_F_Loop", "MOB1_Walk_F_Jump_RU", {{"jump", 1}});
  add_edge("MOB1_Walk_F_Loop", "MOB1_Walk_F_To_Stand_Relaxed_RU", {{"speed", 0}});
  add_edge("MOB1_Walk_F_Loop", "MOB1_Jog_F_Loop", {{"speed", 2}});

  set_state("MOB1_Jog_F_Loop");
  add_edge("MOB1_Jog_F_Loop", "MOB1_Jog_F_Jump", {{"jump", 1}});
  add_edge("MOB1_Jog_F_Loop", "MOB1_Walk_F_Loop", {{"speed", 0}, {"speed", 1}});

  set_state("MOB1_Jog_F_Jump", false, false);
  add_edge("MOB1_Jog_F_Jump", "MOB1_Jog_F_Loop");

  set_state("MOB1_Walk_F_Jump_RU", false, false);
  add_edge("MOB1_Walk_F_Jump_RU", "MOB1_Walk_F_Loop");
}
void AnimationPlayer::crouch(int crouch)
{
  try_change_state("crouch", crouch);
}
void AnimationPlayer::jump()
{
  try_change_state("jump", 1);
  state_values["jump"] = 0;
}
void AnimationPlayer::set_rotation(float rotation)
{
  int r = glm::abs(rotation) < 0.01f ? 0 : rotation < 0 ? -1 : 1;

  try_change_state("rotation", r);
}
void AnimationPlayer::set_speed(float speed)
{
  int s = glm::abs(speed) < 0.01f ? 0 : glm::abs(speed) <= 1.5f ? 1 : 2;

  try_change_state("speed", s);
}

