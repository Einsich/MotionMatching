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
    (vector<string>) (resourcesPaths),
    (vector<string>) (templatePaths)
  )
  ApplicationMetaInfo(const string &path);
};
