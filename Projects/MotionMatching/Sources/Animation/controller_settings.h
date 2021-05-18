#pragma once
#include "Serialization/settings_set.h"
struct ControllerSettings : SettingsSet
{
  static inline ControllerSettings *instance;
  #define FVAR DECL_FVAR
  #define IVAR DECL_IVAR
  #define BVAR DECL_BVAR
  #define LABEL(s)

  #define PARAMS()

  PARAMS()
  #undef FVAR
  #undef IVAR
  #undef BVAR
  #undef LABEL


  ControllerSettings()
  {
    #define FVAR INIT_FVAR
    #define IVAR INIT_IVAR
    #define BVAR INIT_BVAR
    #define LABEL INIT_LABEL
    PARAMS() 
    #undef PARAMS
    #undef FVAR
    #undef IVAR
    #undef BVAR
    #undef LABEL
  }


};