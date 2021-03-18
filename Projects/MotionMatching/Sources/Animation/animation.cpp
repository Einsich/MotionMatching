#include "animation.h"
void Animation::update(float dt)
{
  t += clip.ticksPerSecond * dt;
  int skippedCardes = (int)t;
  t -= skippedCardes;
  cadr += skippedCardes;
}
bool Animation::ended()
{
  return cadr >= (int)clip.duration - 1;
}
float Animation::ticksPerSecond() const
{
  return clip.ticksPerSecond;
}
AnimationCadr Animation::get_lerped_cadr() const
{
  return clip.get_frame(cadr);
}



