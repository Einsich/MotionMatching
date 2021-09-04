#pragma once
#include "3dmath.h"
#include "ecs/singleton.h"
#include "Engine/Render/mesh.h"
#include "Engine/Render/material.h"

struct GlobalRenderData
{
  mat4 ViewProjection;
  alignas(16) vec3 CameraPosition;
  alignas(16) vec3 LightDirection;
  alignas(16) vec3 AmbientLight;
  alignas(16) vec3 SunLight;
  alignas(16) float Time;
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