#include "animation_database.h"
#include "../settings.h"
AnimationDataBase::AnimationDataBase(aiNode *root):
tree(root), tests()
{
}
int AnimationDataBase::cadr_count() const
{
  int count = 0;
  for (const AnimationClip & anim: clips)
    count += anim.duration;
  return count;
}
size_t AnimationDataBase::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, clips);
  return size;
}
size_t AnimationDataBase::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, clips);
  return size;
}
void AnimationDataBase::save_runtime_parameters()
{
  save_object(tests, "AnimatuionTests.bin");
}
void AnimationDataBase::load_runtime_parameters()
{
  load_object(tests, "AnimatuionTests.bin");
}
