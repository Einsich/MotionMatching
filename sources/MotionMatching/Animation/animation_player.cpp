#include "animation_player.h"
#include "animation_ik.h"
#include <component_editor.h>
#include <resources/resources.h>

AnimationPlayer::AnimationPlayer(AnimationDataBasePtr dataBase, AnimationPlayerType playerType):
playerType(playerType), speed(1.f),
index(dataBase, 0, 0), 
motionMatching(dataBase, index, MotionMatchingSolverType::BruteForce),
tree(&dataBase->tree), 
currentCadr(index.get_lerped_cadr()),
dataBase(dataBase)
{
}


void AnimationPlayer::set_data_to_IK(const mat4 &t, int i, vec3 foot, vec3 toe, vec3 norm, const char *foot_name, const char *toe_name)
{

  if (onGround & (1u << i))
  {
    if(!ikFoot[i].onGround)
    {  
      ikFoot[i].onGround = true;
      ikFoot[i].footPosition = t * vec4(foot, 1);
      ikFoot[i].toePosition = t * vec4(toe, 1);
      ikFoot[i].footNode = tree.get_child(foot_name);
      ikFoot[i].toeNode = tree.get_child(toe_name);
      ikFoot[i].normal = norm;
    }
  }
  else
  {
    ikFoot[i].onGround = false;
  }
}


void AnimationPlayer::animation_selector(const KeyEventAnyActionKey &event)
{
  if (playerType !=  AnimationPlayerType::AnimationPlayer)
    return;
  int anim = index.current_index().get_clip_index();
  if (event.keycode == SDLK_RIGHT)
    anim++;
  if (event.keycode == SDLK_LEFT)
    anim--;
  int n = index.get_data_base()->clips.size();
  anim = (n + anim) % n;
  index.current_index().set_index(anim, 0);
  index.current_index().set_index(anim, 1);
}

MotionMatching *AnimationPlayer::get_motion_matching()
{
  return playerType ==  AnimationPlayerType::MotionMatching ? &motionMatching : nullptr;
}
const MotionMatching *AnimationPlayer::get_motion_matching() const
{
  return playerType ==  AnimationPlayerType::MotionMatching ? &motionMatching : nullptr;
}
AnimationLerpedIndex AnimationPlayer::get_index() const
{
  return index;
}


template<>
bool edit_component(AnimationPlayer &component, const char *, bool view_only)
{
  bool edited = false;
  AnimationIndex index = component.index ? component.index.current_index() : AnimationIndex();
  int clipFrame[2]{index.get_clip_index(), index.get_cadr_index()};

  {
    bool playerType = !(bool)component.playerType;
    edited |= ImGui::Checkbox("motion matching", &playerType);
    component.playerType = (AnimationPlayerType)!playerType;
  }
  edited |= ImGui::InputFloat("speed", &component.speed);
  if (index)
  {
    bool indexChange = ImGui::InputInt2("clip index", clipFrame);
    edited |= indexChange;
    if (indexChange)
    {
      clipFrame[0] = glm::clamp(clipFrame[0], 0, (int)index.get_data_base()->clips.size() - 1);
      clipFrame[1] = glm::clamp(clipFrame[1], 0, (int)index.get_data_base()->clips[clipFrame[0]].duration - 1);
    
      component.index = AnimationLerpedIndex(component.dataBase, clipFrame[0], clipFrame[1]);
      component.currentCadr = AnimationCadr(component.index.get_lerped_cadr());
      component.motionMatching.index = component.index;
      component.tree.set_cadr(component.currentCadr);
      component.tree.calculate_bone_transforms();
    }
  }
  bool datasetChange = edit_component(component.dataBase, "dataset", view_only);
  edited |= datasetChange;
  
  if (datasetChange && component.dataBase)
  {    
    component.index = index ? AnimationLerpedIndex(component.dataBase, clipFrame[0], clipFrame[1]) 
       : AnimationLerpedIndex(component.dataBase, 0, 0);
    component.currentCadr = AnimationCadr(component.index.get_lerped_cadr());
    component.tree = AnimationTree(&component.dataBase->tree);
    component.tree.set_cadr(component.currentCadr);
    component.tree.calculate_bone_transforms();
    component.motionMatching = MotionMatching(component.dataBase, component.index, MotionMatchingSolverType::BruteForce);
  }
  return edited;
}

size_t AnimationPlayer::serialize(std::ostream& os) const
{
  int clip = 0, frame = 0;
  if (index)
    clip = index.current_index().get_clip_index(), frame = index.current_index().get_cadr_index();
  return write(os, dataBase, playerType, speed, clip, frame);
}
size_t AnimationPlayer::deserialize(std::istream& is)
{
  int clip = 0, frame = 0;
  size_t size = read(is, dataBase, playerType, speed, clip, frame);
  if (dataBase)
  {
    index = AnimationLerpedIndex(dataBase, clip, frame);
    if (index)
      currentCadr = AnimationCadr(index.get_lerped_cadr());
    tree = AnimationTree(&dataBase->tree);
    motionMatching = MotionMatching(dataBase, index, MotionMatchingSolverType::BruteForce);
  }
  return size;
}