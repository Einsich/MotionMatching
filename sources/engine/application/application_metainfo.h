#pragma once
#include <vector>
#include "serialization/reflection.h"
#include <ecs/ecs_std.h>
#include <string>

struct ApplicationMetaInfo
{
  REFLECT(ApplicationMetaInfo,
    (ecs::string) (firstScene),
    (bool) (startGame),
    (ecs::vector<ecs::string>) (shadersPaths),
    (ecs::vector<ecs::string>) (resourcesPaths),
    (ecs::vector<ecs::string>) (templatePaths),
    (ecs::vector<ecs::string>) (scriptFolders)
  )
  ApplicationMetaInfo(const std::string &path);
};
