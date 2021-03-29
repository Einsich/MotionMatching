#include "animation_trajectory.h"


TrajectoryPoint::TrajectoryPoint():
point(0), rotation(0), timeDelay(0){}
size_t TrajectoryPoint::serialize(std::ostream& os) const 
{
  size_t size = 0;
  size += write(os, point);
  size += write(os, rotation);
  size += write(os, timeDelay);
  return size;
}
size_t TrajectoryPoint::deserialize(std::istream& is) 
{
  size_t size = 0;
  size += read(is, point);
  size += read(is, rotation);
  size += read(is, timeDelay);
  return size;
}


size_t AnimationTrajectory::serialize(std::ostream& os) const
{
  size_t size = 0;
  size += write(os, trajectory);
  return size;
}
size_t AnimationTrajectory::deserialize(std::istream& is)
{
  size_t size = 0;
  size += read(is, trajectory);
  return size;
}