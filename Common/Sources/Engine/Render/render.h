#pragma once
#include "3dmath.h"
#include "ecs/singleton.h"

struct GlobalRenderData
{
  mat4 ViewProjection;
  alignas(16) vec3 CameraPosition;
  alignas(16) vec3 LightDirection;
};
struct EditorRenderSettings : ecs::Singleton
{
  bool wire_frame;
};