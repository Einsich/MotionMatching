#pragma once
#include "material.h"
#include "mesh.h"
#include "serialization/reflection.h"
class SkyBox
{
public:
  Mesh skyboxMesh;
  REFLECT(
    SkyBox,
    (Asset<Material>) (material)
  )
  SkyBox();
  void render() const;
};