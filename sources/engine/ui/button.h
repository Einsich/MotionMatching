#pragma once
#include <render/texture/texture2d.h>

namespace ui
{
  struct Button
  {
    Asset<Texture2D> image;
    vec2 size, uv0, uv1;
    vec4 active_color, hover_color, default_color;
    bool disabled;
    Button(const char *name, vec2 size,
      const vec4 &active_color = vec4(0.5f),
      const vec4 &hover_color = vec4(0.75f),
      const vec4 &default_color = vec4(1.f));
    bool get() const;
  };
};