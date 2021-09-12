#pragma once
#include "Serialization/reflection.h"
#include "common.h"

struct ApplicationMetaInfo
{
  REFLECT(ApplicationMetaInfo,
    (string) (firstScene),
    (bool) (startGame)
  )
};

void load_meta_info(const string &path);
void save_meta_info(const string &path);
ApplicationMetaInfo &get_meta_info();