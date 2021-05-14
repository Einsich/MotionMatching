#pragma once
#include "Serialization/serialization.h"

struct ManProperty : ISerializable
{
  static inline ManProperty *instance;
  float walkForwardSpeed = 1.2f;
  float walkSidewaySpeed = 1.2f;
  float walkBackwardSpeed = 1.f;
  
  float runForwardSpeed = 2.7f*1.2f;
  float runSidewaySpeed = 2.f*1.2f;
  float runBackwardSpeed = 2.1f*1.2f;

  float hipsHeightStand = 0.967f;
  float hipsHeightCrouch = 0.35f;
  float hipsHeightJump = 1.2f;

  float runSpeeds[3] = {runForwardSpeed, runSidewaySpeed, runBackwardSpeed};
  float walkSpeeds[3] = {walkForwardSpeed, walkSidewaySpeed, walkBackwardSpeed};

  float moveRate, rotationRate;
  #define PARAMS()\
  VAR(walkForwardSpeed), VAR(walkSidewaySpeed), VAR(walkBackwardSpeed),\
  VAR(runForwardSpeed), VAR(runSidewaySpeed), VAR(runBackwardSpeed),\
  VAR(hipsHeightStand), VAR(hipsHeightCrouch), VAR(hipsHeightJump),\
  VAR(moveRate), VAR(rotationRate)


  void update_array()
  {
    runSpeeds[0] = runForwardSpeed, runSpeeds[1] = runSidewaySpeed,  runSpeeds[2] = runBackwardSpeed;
    walkSpeeds[0] = walkForwardSpeed, walkSpeeds[1] = walkSidewaySpeed,  walkSpeeds[2] = walkBackwardSpeed;
  }
  virtual size_t serialize(std::ostream& os) const override
  {
    #define VAR(x) size += write(os, x)
    size_t size = 0;
    PARAMS();
    return size;
    #undef VAR
  }
  virtual size_t deserialize(std::istream& is) override
  {
    #define VAR(x) size += read(is, x)
    size_t size = 0;
    PARAMS();
    update_array();
    return size;
    #undef VAR
  }
};