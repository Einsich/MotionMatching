#pragma once
#include "3dmath.h"
#include "ecs/singleton.h"
#include "Engine/Render/mesh.h"
#include "Engine/Render/material.h"

struct alignas(16) GlobalRenderData
{
  mat4 ViewProjection;
  vec3 CameraPosition;
  vec3 LightDirection;
  vec3 AmbientLight;
  vec3 SunLight;
};
struct EditorRenderSettings : ecs::Singleton
{
  bool wire_frame;
};
struct RenderStuff
{
  Asset<Material> material;
  Asset<Mesh> mesh;
};
struct RenderQueue : ecs::Singleton
{
  vector<RenderStuff> queue;
};