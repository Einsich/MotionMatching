#pragma once
#include "material.h"
#include "vertex_array_object.h"
#include "serialization/reflection.h"
class SkyBox
{
public:
  VertexArrayObject skyboxVAO;
  REFLECT(
    SkyBox,
    (Asset<Material>) (material)
  )
  SkyBox();
  void render() const;
};