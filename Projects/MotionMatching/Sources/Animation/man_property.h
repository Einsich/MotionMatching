#pragma once
#include "Serialization/serialization.h"

struct ManProperty : ISerializable
{
  static inline ManProperty *instance;


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
  IVAR(maxLerpIndex, 2, 2, 10)

  #define FVAR(name, def, min, max) float name = def;
  #define IVAR(name, def, min, max) int name = def;

  PARAMS()

  #undef FVAR
  #undef IVAR
  float runSpeeds[3] = {runForwardSpeed, runSidewaySpeed, runBackwardSpeed};
  float walkSpeeds[3] = {walkForwardSpeed, walkSidewaySpeed, walkBackwardSpeed};

  void update_array()
  {
    runSpeeds[0] = runForwardSpeed, runSpeeds[1] = runSidewaySpeed,  runSpeeds[2] = runBackwardSpeed;
    walkSpeeds[0] = walkForwardSpeed, walkSpeeds[1] = walkSidewaySpeed,  walkSpeeds[2] = walkBackwardSpeed;
  }
  virtual size_t serialize(std::ostream& os) const override
  {
    #define FVAR(x, d, mn, mx) size += write(os, x);
    #define IVAR(x, d, mn, mx) size += write(os, x);
    size_t size = 0;
    PARAMS();
    return size;
    #undef FVAR
    #undef IVAR
  }
  virtual size_t deserialize(std::istream& is) override
  {
    #define FVAR(x, d, mn, mx) size += read(is, x);
    #define IVAR(x, d, mn, mx) size += read(is, x);
    size_t size = 0;
    PARAMS();
    update_array();
    return size;
    #undef FVAR
    #undef IVAR
  }
};