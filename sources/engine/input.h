#pragma once
#include "common.h"
#include <map>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_events.h>
#include <ecs/event.h>

enum class MouseButton
{
  LeftButton,
  RightButton,
  MiddleButton
};
enum class MouseAction
{
  Down,
  Up
};

template <MouseButton buttonType>
struct MouseButtonDownEvent : public ecs::Event
{
  MouseAction action;
  int x, y;
  float time;
};

struct MouseClickEvent : public ecs::Event
{
  MouseButton buttonType;
  MouseAction action;
  int x, y;
  float time;
};

struct MouseMoveEvent : public ecs::Event
{
  int x, y;
  int dx, dy;
  float time;
};
struct MouseWheelEvent : public ecs::Event
{
  int wheel;
  float time;
};

enum class KeyAction
{
  Down,
  Press,
  Up
};

template <SDL_Keycode Key>
struct KeyDownEvent : public ecs::Event
{
  float time;
};
template <SDL_Keycode Key>
struct KeyUpEvent : public ecs::Event
{
  float time;
};
template <SDL_Keycode Key>
struct KeyPressEvent : public ecs::Event
{
  float time;
};
template <SDL_Keycode Key>
struct KeyEventAnyAction : public ecs::Event
{
  KeyAction action;
  float time;
};
template <KeyAction Action>
struct KeyEventAnyKey : public ecs::Event
{
  SDL_Keycode keycode;
  float time;
};

struct KeyEventAnyActionKey : public ecs::Event
{
  SDL_Keycode keycode;
  KeyAction action;
  float time;
};

class Input
{

  map<SDL_Keycode, pair<int, float>> keyMap;
  struct WheelData
  {
    float lastTime = 0, lastValue = 0, targetValue = 0;
  } wheelData;

  bool eventable = true;

public:
  bool mouseOverUI;
  Input(bool eventable) : eventable(eventable) {}
  Input() = default;
  static Input &input();

  void event_process(const SDL_KeyboardEvent &event, float time);
  void event_process(const SDL_MouseButtonEvent &event, float time);
  void event_process(const SDL_MouseMotionEvent &event, float time);
  void event_process(const SDL_MouseWheelEvent &event, float time);

  float get_key_impl(SDL_Keycode keycode, float reaction_time = 0.1f);
  float get_key_derivative_impl(SDL_Keycode keycode, float reaction_time = 0.1f);
  float get_wheel_impl();
  static float get_key(SDL_Keycode keycode, float reaction_time = 0.1f)
  {
    return input().get_key_impl(keycode, reaction_time);
  }
  static float get_key_derivative(SDL_Keycode keycode, float reaction_time = 0.1f)
  {
    return input().get_key_derivative_impl(keycode, reaction_time);
  }
  static float get_wheel()
  {
    return input().get_wheel_impl();
  }
};