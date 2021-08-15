#pragma once
#include "3dmath.h"
#include "mesh.h"
#include "Texture/textures.h"
#include "Engine/Resources/resources.h"

struct GlobalRenderData
{
  mat4 ViewProjection;
  alignas(16) vec3 CameraPosition;
  alignas(16) vec3 LightDirection;
};