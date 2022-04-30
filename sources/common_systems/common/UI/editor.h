#pragma once
#include <singleton.h>
#include <ui/ui.h>

struct EditorWidgets : ecs::Singleton
{
  ui::Button back;
  EditorWidgets();
};