#pragma once
#include "Serialization/serialization.h"

struct Settings : ISerializable
{
  static inline Settings *instance;


  #define PARAMS()\
  FVAR(walkForwardSpeed, 1.2f, 0.f, 20.f)\
  FVAR(walkSidewaySpeed, 1.2f, 0.f, 20.f)\
  FVAR(walkBackwardSpeed, 1.0f, 0.f, 20.f)\
  FVAR(runForwardSpeed, 2.7f*1.2f, 0.f, 20.f)\
  FVAR(runSidewaySpeed, 2.f*1.2f, 0.f, 20.f)\
  FVAR(runBackwardSpeed, 2.1f*1.2f, 0.f, 20.f)\
  FVAR(hipsHeightStand, 0.967f, 0.f, 20.f)\
  FVAR(hipsHeightCrouch, 0.35f, 0.f, 20.f)\
  FVAR(hipsHeightJump, 1.2f, 0.f, 20.f)\
  FVAR(moveRate, 9.0f, 0.f, 20.f)\
  FVAR(rotationRate, 9.0f, 0.f, 20.f)\
  FVAR(lerpTime, 0.2f, 0.f, 1.f)\
  IVAR(maxLerpIndex, 2, 2, 10)\
  BVAR(debugNodes, false)\
  BVAR(debugTrajectory, false)\
  BVAR(debugBones, false)\
  BVAR(MatchingStatistic, false)

  #define FVAR(name, def, min, max) static inline float name = def;
  #define IVAR(name, def, min, max) static inline int   name = def;
  #define BVAR(name, def)           static inline bool  name = def;

  PARAMS()

  #undef FVAR 
  #undef IVAR 
  #undef BVAR 
  static inline float runSpeeds[3] = {runForwardSpeed, runSidewaySpeed, runBackwardSpeed};
  static inline float walkSpeeds[3] = {walkForwardSpeed, walkSidewaySpeed, walkBackwardSpeed};

  static void update_array()
  {
    runSpeeds[0] = runForwardSpeed, runSpeeds[1] = runSidewaySpeed,  runSpeeds[2] = runBackwardSpeed;
    walkSpeeds[0] = walkForwardSpeed, walkSpeeds[1] = walkSidewaySpeed,  walkSpeeds[2] = walkBackwardSpeed;
  }
  virtual size_t serialize(std::ostream& os) const override
  {
    #define FVAR(x, d, mn, mx) size += write(os, x);
    #define IVAR(x, d, mn, mx) size += write(os, x);
    #define BVAR(x, d)         size += write(os, x);
    size_t size = 0;
    PARAMS();
    return size;
    #undef FVAR
    #undef IVAR
    #undef BVAR 
  }
  virtual size_t deserialize(std::istream& is) override
  {
    #define FVAR(x, d, mn, mx) size += read(is, x);
    #define IVAR(x, d, mn, mx) size += read(is, x);
    #define BVAR(x, d)         size += read(is, x);
    size_t size = 0;
    PARAMS();
    update_array();
    return size;
    #undef FVAR
    #undef IVAR
    #undef BVAR 
  }
};