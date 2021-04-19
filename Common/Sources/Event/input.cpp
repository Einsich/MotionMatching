
#include <assert.h>
#include "input.h"
#include "Time/time.h"
#include "math.h"


const float wheelDecayTime = 0.03f;
const float wheelHalfDecayTime = wheelDecayTime * 0.5f;
const float wheelScrollSpeed = 0.2f;
Input& Input::input()
{

  static Input *instance = new Input();
  return *instance;
}

IEvent<const KeyboardEvent &> & Input::keyboard_event(KeyAction action, SDL_Keycode keycode)
{
  return keyboardEvent.get_event((int)action, (int)keycode);
}
IEvent<const KeyboardEvent &> & Input::keyboard_event(KeyAction action)
{
  return keyboardEvent.get_event((int)action, 0);
}
IEvent<const MouseClickEvent &> & Input::mouse_click_event(MouseButtonType type, MouseButtonAction action)
{
  return mouseClickEvent.get_event((int)type, (int)action);
}
IEvent<const MouseClickEvent &> & Input::mouse_click_event()
{
  return mouseClickEvent.get_event(0, -1);
}
IEvent<const MouseMoveEvent &> & Input::mouse_move_event()
{
  return mouseMoveEvent.get_event(0, 0);
}
IEvent<const MouseWheelEvent &> & Input::mouse_wheel_event()
{
  return mouseWheelEvent.get_event(0, 0);
}
void Input::event_process(const SDL_KeyboardEvent& event, float time)
{
  KeyboardEvent e;
  e.keycode = event.keysym.sym;
  e.time = time;
  if (event.repeat)
    e.action = KeyAction::Press;
  else
  if (event.state == SDL_PRESSED)
  {
    e.action = KeyAction::Down;
    keyMap[e.keycode] = {1, time};
  }
  else
  {
    e.action = KeyAction::Up;
    keyMap[e.keycode] = {0, time};

  }  
  keyboardEvent.get_event((int)e.action, 0)(e);
  keyboardEvent.get_event((int)e.action, (int)e.keycode)(e);
}
void Input::event_process(const SDL_MouseButtonEvent& event, float time)
{
  MouseClickEvent e;
  e.time = time;
  
  switch (event.button)
  {
    case SDL_BUTTON_LEFT: e.buttonType = MouseButtonType::LeftButton; break;
    case SDL_BUTTON_RIGHT: e.buttonType = MouseButtonType::RightButton; break;
    case SDL_BUTTON_MIDDLE: e.buttonType = MouseButtonType::MiddleButton; break;
    default: break;
  }
  switch (event.type)
  {
    case SDL_MOUSEBUTTONDOWN: e.action = MouseButtonAction::Down; break;
    case SDL_MOUSEBUTTONUP: e.action = MouseButtonAction::Up; break;
    default: break;
  }
  mouseClickEvent.get_event(0, -1)(e);
  mouseClickEvent.get_event((int)e.buttonType, (int)e.action)(e);
  
}
void Input::event_process(const SDL_MouseMotionEvent& event, float time)
{
  MouseMoveEvent e
  {
    e.x = event.x, e.y = event.y,
    e.dx = event.xrel, e.dy = event.yrel,
    e.time = time
  };
  mouseMoveEvent.get_event(0, 0)(e);
}
void Input::event_process(const SDL_MouseWheelEvent& event, float time)
{
  MouseWheelEvent e;
  e.wheel = event.y;
  e.time = time;
  if (Time::time() - wheelData.lastTime > wheelDecayTime)
  {
    wheelData.lastValue = 0;
    wheelData.targetValue = glm::clamp(wheelScrollSpeed * e.wheel, -1.f, 1.f);
  }
  else
  {
    wheelData.lastValue = get_wheel();
    wheelData.targetValue = glm::clamp(wheelData.targetValue + wheelScrollSpeed * e.wheel, -1.f, 1.f);
  }
  
  wheelData.lastTime = Time::time();
  mouseWheelEvent.get_event(0, 0)(e);
}

float Input::get_key(SDL_Keycode keycode, float reaction_time)
{
  auto p = keyMap[keycode];
  if (p.first == 0)
  {
    float t = 1.f - (Time::time() - p.second) / reaction_time;
    return t < 0 ? 0 : t;
  }
  else
  {
    float t = (Time::time() - p.second) / reaction_time;
    return t < 1 ? t : 1;
  }
}
float Input::get_key_derivative(SDL_Keycode keycode, float reaction_time)
{
  auto p = keyMap[keycode];
  if (p.first == 0)
  {
    float t = 1.f - (Time::time() - p.second) / reaction_time;
    return t < 0 ? 0 : -1;
  }
  else
  {
    float t = (Time::time() - p.second) / reaction_time;
    return t < 1 ? 1 : 0;
  }
}
float Input::get_wheel()
{
  float dt = Time::time() - wheelData.lastTime;
  if (dt > wheelDecayTime)
    return 0;
  float wheel = 0;
  if (dt < wheelHalfDecayTime)
  {
    float t = dt / wheelHalfDecayTime;
    wheel = lerp(wheelData.lastValue, wheelData.targetValue, t);
  }
  else
  {
    float t = (dt - wheelHalfDecayTime) / wheelHalfDecayTime;
    wheel = lerp(wheelData.targetValue, 0.f, t);
  }
  return wheel;
}
