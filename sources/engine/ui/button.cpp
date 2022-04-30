#include "button.h"
#include <resources/resources.h>
#include <imgui.h>

namespace ui
{
  Button::Button(const char *name, vec2 size,
      const vec4 &active_color,
      const vec4 &hover_color,
      const vec4 &default_color):
  size(size), uv0(0, 1), uv1(1, 0),
  active_color(active_color), hover_color(hover_color), default_color(default_color),
  disabled(false)
  {
    image = get_resource<Texture2D>(name);
    image.load();
  }
  bool Button::get() const
  {
    return ImGui::ImageButton(
      (void*)(intptr_t)image->get_texture_object(), size, uv0, uv1, hover_color, active_color, default_color, disabled);
  }
}
