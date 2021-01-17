#include "animation.h"
#include "CommonCode/Time/time.h"
#include "CommonCode/Light/direction_light.h"
vec3 Animation::get_lerped_root_delta_pos()
{
  vec3 p = glm::mix(rootMotions[cadr], rootMotions[cadr + 1], t);
  vec3 d = p - rootMotion;
  rootMotion = p;
  return cadr == 0 ? vec3(0.f) : d;
}
float Animation::get_lerped_root_delta_rot()
{
  float r = glm::mix(rootRotations[cadr], rootRotations[cadr + 1], t);
  float d = r - rootRotation;
  rootRotation = r;
  return  cadr == 0 ? 0.f : d;
}
vec3 Animation::get_lerped_pos(const string &name)
{
  auto it = channels.find(name);
  if (it == channels.end() || it->second.pos.size() == 0)
    return vec3(0.f);
  return glm::mix(it->second.pos[cadr], it->second.pos[cadr + 1], t);
}
quat Animation::get_lerped_rot(const string &name)
{
  auto it = channels.find(name);
  if (it == channels.end() || it->second.rot.size() == 0)
    return quat();
  return glm::mix(it->second.rot[cadr], it->second.rot[cadr + 1], t);
}
vec3 AnimationPlayer::get_lerped_root_delta_pos()
{
  if (!blendMode.anim1)
    return vec3(0.f);
  if (!blendMode.anim2)
    return blendMode.anim1->get_lerped_root_delta_pos();
  return glm::mix(blendMode.anim1->get_lerped_root_delta_pos(), blendMode.anim2->get_lerped_root_delta_pos(), 1.f - blendMode.t);
}
float AnimationPlayer::get_lerped_root_delta_rot()
{
  if (!blendMode.anim1)
    return 0.f;
  if (!blendMode.anim2)
    return blendMode.anim1->get_lerped_root_delta_rot();
  return glm::mix(blendMode.anim1->get_lerped_root_delta_rot(), blendMode.anim2->get_lerped_root_delta_rot(), 1.f - blendMode.t);
}
mat4 AnimationPlayer::get_lerped_pos(const string &name)
{
  if (!blendMode.anim1)
    return mat4(1.f);
  if (!blendMode.anim2)
    return translate(mat4(1.f), blendMode.anim1->get_lerped_pos(name));
  
  return translate(mat4(1.f), glm::mix(blendMode.anim1->get_lerped_pos(name), blendMode.anim2->get_lerped_pos(name), 1.f - blendMode.t));
}
mat4 AnimationPlayer::get_lerped_rot(const string &name)
{
  if (!blendMode.anim1)
    return mat4(1.f);
  if (!blendMode.anim2)
    return toMat4(blendMode.anim1->get_lerped_rot(name));
  
  return toMat4(glm::mix(blendMode.anim1->get_lerped_rot(name), blendMode.anim2->get_lerped_rot(name), 1.f - blendMode.t));
}
void AnimationPlayer::CalculateBonesTransform(AnimationNode &node, mat4 parent, int d)
{
  mat4 nodeTransform = node.transform;
  auto it = blendMode.anim1->channels.find(node.name);
  if (it != blendMode.anim1->channels.end())
  {
    AnimationChannel& channel = it->second;
    
    if (d == 1)
    {
      nodeTransform = get_lerped_pos(node.name) * mat4(mat3(nodeTransform)) * get_lerped_rot(node.name);

      rootDeltaTranslation = get_lerped_root_delta_pos();
      rootDeltaRotation = get_lerped_root_delta_rot();
    }   
    else
    {
      nodeTransform = nodeTransform * get_lerped_rot(node.name);
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

void Animation::update(float dt)
{
  t += ticksPerSecond * dt;
  int skippedCardes = (int)t;
  t -= skippedCardes;
  cadr += skippedCardes;
}
bool Animation::ended()
{
  return cadr >= duration - 1;
}
void AnimationPlayer::PlayNextCadr()
{
  
  CalculateBonesTransform(animationTree.nodes[0], mat4(1.f), 0);
  float dt = speed * Time::delta_time();
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
        debug_error("Can't play nex cadr in %s", blendMode.anim1->name.c_str());
        blendMode.anim1->cadr = 0;
      }        
    }
    else
    {
      blendMode.anim1->cadr = 0;
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
  anim_index = (anim_index + animations.size()) % animations.size();
  blendMode.anim2 = blendMode.anim1 && !blendMode.anim1->ended() ? blendMode.anim1 : nullptr;
  blendMode.t = 0.f;
  blendMode.anim1 = &animations[anim_index];
  blendMode.curState = &states[anim_index];
  blendMode.anim1->cadr = 0;
  blendMode.anim1->t = 0.f;
  debug_log("play %s", animations[anim_index].name.c_str());
}
void AnimationPlayer::animation_selector(const KeyboardEvent &event)
{
  return;
  static int anim = 0;
  if (event.keycode == SDLK_RIGHT)
    anim++;
  if (event.keycode == SDLK_LEFT)
    anim--;
  play_animation(anim);  
}

size_t Animation::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, duration);
  size += write(os, ticksPerSecond);
  size += write(os, name);
  size += write(os, channels);
  size += write(os, rootMotion);
  size += write(os, rootRotation);
  return size;
}
size_t Animation::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, duration);
  size += read(is, ticksPerSecond);
  size += read(is, name);
  size += read(is, channels);
  size += read(is, rootMotion);
  size += read(is, rootRotation);
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
void AnimationPlayer::add_states()
{
  for (int i = 0; i < animations.size(); i++)
  {
    states.push_back(State());
  }
}
void AnimationPlayer::set_state(const string& name,  bool loopable, bool breakable )
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
void AnimationPlayer::add_edge(const string& from, const string &to, const vector<pair<string, int>> &actions)
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
bool AnimationPlayer::try_change_state(const string& property, int value)
{
  State &state = *blendMode.curState;
  if (state.breakable)
  {
    for (int i = 0; i < state.edges.size(); i++)
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
void AnimationPlayer::crouch(int crouch)
{
  try_change_state("crouch", crouch);
}
void AnimationPlayer::jump()
{
  try_change_state("jump", 1);
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

