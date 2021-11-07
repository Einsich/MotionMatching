#pragma once
#include <vector>
#include "serialization/reflection.h"
#include "common.h"


struct ApplicationMetaInfo
{
  REFLECT(ApplicationMetaInfo,
    (string) (firstScene),
    (bool) (startGame),
    (vector<string>) (shadersPaths),
    (vector<string>) (resourcesPaths)
  )
};

void load_meta_info(const string &path);
ApplicationMetaInfo &get_meta_info();