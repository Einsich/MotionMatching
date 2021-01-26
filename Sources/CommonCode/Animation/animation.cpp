#include "animation.h"
#include "CommonCode/Time/time.h"
#include "CommonCode/Light/direction_light.h"
vec3 get_vec3(const map<string, vector<vec3>>& vecs, const string& name, uint i)
{
  auto it = vecs.find(name);
  return (it == vecs.cend() || i < 0 || it->second.size() <= i) ? vec3(0.f) : it->second[i];
}
quat get_quat(const map<string, vector<quat>>& vecs, const string& name, uint i)
{
  auto it = vecs.find(name);
  return (it == vecs.cend() || i < 0 || it->second.size() <= i) ? quat() : it->second[i];
}
Animation::Animation(uint duration, const AnimationTree& tree, const map<string, vector<quat>>& quats, const map<string, vector<vec3>>& vecs)
{
  
  cadres.resize(duration);
  for (uint i = 0; i < duration; i++)
  {
    AnimationCadr& cadr = cadres[i];
    cadr.nodeRotation.resize(tree.nodes.size());
    for (uint j = 0; j < tree.nodes.size(); j++)
    {
      const AnimationNode& node = tree.nodes[j];
      quat rotation = get_quat(quats, node.name, i);
      if (node.name == "Hips")
      {
        vec3 translation = get_vec3(vecs, node.name, i);
        cadr.rootTranslationDelta = vec3(translation.x, 0, translation.z);
        cadr.nodeTranslation = vec3(0, translation.y, 0);
      
        mat4 m = toMat4(rotation);
        float x, y, z;
        glm::extractEulerAngleXYZ(m, x, y, z);
        m = glm::eulerAngleXYZ(0.f, y, z);
        cadr.rootRotationDelta = x;
        rotation = quat_cast(m);
      }
      cadr.nodeRotation[j] = rotation;
    }    
  }

  debug_log("%f, %f", cadres[0].rootTranslationDelta.x, cadres[0].rootTranslationDelta.z);
  debug_log("%f, %f", cadres[duration -1].rootTranslationDelta.x, cadres[duration-1].rootTranslationDelta.z);
  for (int i = duration - 1; i >=0; i--)
  {
    AnimationCadr& cadr1 = cadres[i];
    AnimationCadr& cadr2 = cadres[i - 1];
    cadr1.rootRotationDelta -= cadr2.rootRotationDelta;
    cadr1.rootTranslationDelta -= cadr2.rootTranslationDelta;
  }
  cadres[0].rootRotationDelta = 0;
  cadres[0].rootTranslationDelta = vec3(0.f);

}
int Animation::duration() const
{
  return cadres.size();
}

AnimationCadr Animation::get_lerped_cadr()
{
  return cadr < cadres.size() + 1 ? lerped_cadr(cadres[cadr], cadres[cadr + 1], t) : cadres[cadr];
}
AnimationCadr AnimationPlayer::get_lerped_cadr()
{
  assert(blendMode.anim1 != nullptr);
    
  if (!blendMode.anim2)
    return blendMode.anim1->get_lerped_cadr();
  return lerped_cadr(blendMode.anim1->get_lerped_cadr(), blendMode.anim2->get_lerped_cadr(), 1.f - blendMode.t);
}
void AnimationPlayer::calculate_bones_transform()
{
  AnimationCadr cadr = get_lerped_cadr();
  float dt = blendMode.anim1->ticksPerSecond;
  rootDeltaTranslation = cadr.rootTranslationDelta * dt;
  rootDeltaRotation = cadr.rootRotationDelta * dt;
  for (uint i = 0; i < animationTree.nodes.size(); i++)
  {
    AnimationNode& node = animationTree.nodes[i];
    node.animationTransform = node.transform;
    mat4& nodeTransform = node.animationTransform;
    mat4 rotation = glm::toMat4(cadr.nodeRotation[i]);
    mat4 translation = (node.name == "Hips") ? glm::translate(mat4(1.f), cadr.nodeTranslation) * mat4(mat3(nodeTransform)) : nodeTransform;
    nodeTransform = translation * rotation;
    if (node.parent >= 0)
    {
      nodeTransform = animationTree.nodes[node.parent].animationTransform * nodeTransform;
    }
    
  }
  
  for (uint i = 0; i < animationTree.nodes.size(); i++)
  {
    AnimationNode& node = animationTree.nodes[i];
    auto it2 = gameObject->get_mesh()->bonesMap.find(node.name);
    if (it2 != gameObject->get_mesh()->bonesMap.end())
    {
      curTransform[it2->second] = node.animationTransform * node.meshToBone;
    }
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
  return cadr >= duration() - 1;
}
void AnimationPlayer::PlayNextCadr()
{
  
  calculate_bones_transform();
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
    count += anim.duration();
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
  size += write(os, ticksPerSecond);
  size += write(os, name);
  size += write(os, cadres);
  return size;
}
size_t Animation::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, ticksPerSecond);
  size += read(is, name);
  size += read(is, cadres);
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
  for (uint i = 0; i < animations.size(); i++)
  {
    if (animations[i].name == name)
      return i;
  }
  return -1;
}
void AnimationPlayer::add_states()
{
  for (uint i = 0; i < animations.size(); i++)
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

