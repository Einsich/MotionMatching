#pragma once
#include "common.h"
#include <map>
#include <vector>
#include "Event/event.h"
#include "event.h"
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_events.h>
enum class MouseButtonType
{
  LeftButton,
  RightButton,
  MiddleButton,
  Count
};
enum class MouseButtonAction
{
  Down,
  Up,
  Count
};

enum class KeyAction
{
  Down,
  Press,
  Up,
  Count
};

struct MouseClickEvent
{
  MouseButtonType buttonType;
  MouseButtonAction action;
  int x, y;
  float time;
};
struct MouseMoveEvent
{
  int x, y;
  int dx, dy;
  float time;
};
struct MouseWheelEvent
{
  int wheel;
  float time;
};

struct KeyboardEvent
{
  SDL_Keycode keycode;
  KeyAction action;
  float time;
};

class Input
{
  template<int n, class InputEvent>
  class  InputEventHandler
  {
  private:
    vector<map<int, Event<InputEvent>>> events;
  public:
    InputEventHandler(): events(n, map<int, Event<InputEvent>>()) {}

    Event<InputEvent> & get_event(int i, int j)
    {
      return events[i][j];
    }  
  };

  InputEventHandler<(int)KeyAction::Count, const KeyboardEvent &> keyboardEvent;
  InputEventHandler<(int)MouseButtonType::Count, const MouseClickEvent &> mouseClickEvent;
  InputEventHandler<1, const MouseMoveEvent &> mouseMoveEvent;
  InputEventHandler<1, const MouseWheelEvent &> mouseWheelEvent;

  map<SDL_Keycode, pair<int, float>> keyMap;
  struct WheelData
  {
    float lastTime = 0, lastValue = 0, targetValue = 0;
  } wheelData; 


public:
  static Input& input();
  IEvent<const KeyboardEvent &> & keyboard_event(KeyAction action, SDL_Keycode keycode);
  IEvent<const KeyboardEvent &> & keyboard_event(KeyAction action);
  IEvent<const MouseClickEvent &> & mouse_click_event(MouseButtonType type, MouseButtonAction action);
  IEvent<const MouseClickEvent &> & mouse_click_event();
  IEvent<const MouseMoveEvent &> & mouse_move_event();
  IEvent<const MouseWheelEvent &> & mouse_wheel_event();
  void event_process(const SDL_KeyboardEvent& event, float time);
  void event_process(const SDL_MouseButtonEvent& event, float time);
  void event_process(const SDL_MouseMotionEvent& event, float time);
  void event_process(const SDL_MouseWheelEvent& event, float time);
  float get_key(SDL_Keycode keycode, float reaction_time = 0.1f);
  float get_key_derivative(SDL_Keycode keycode, float reaction_time = 0.1f);
  float get_wheel();
  
};