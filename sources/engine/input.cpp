
#include <assert.h>
#include "input.h"
#include "application/time.h"
#include "application/application.h"
#include "3dmath.h"
#include "ecs.h"

const float wheelDecayTime = 0.03f;
const float wheelHalfDecayTime = wheelDecayTime * 0.5f;
const float wheelScrollSpeed = 0.2f;
Input& Input::input()
{

  static Input *instance = new Input();
  return *instance;
}

#define KEY_CODE_SWITCH(A) \
MACRO(SDLK_0, A) MACRO(SDLK_1, A) MACRO(SDLK_2, A) MACRO(SDLK_3, A) MACRO(SDLK_4, A) \
MACRO(SDLK_5, A) MACRO(SDLK_6, A) MACRO(SDLK_7, A) MACRO(SDLK_8, A) MACRO(SDLK_9, A) \
MACRO(SDLK_a, A) MACRO(SDLK_b, A) MACRO(SDLK_c, A) MACRO(SDLK_d, A) MACRO(SDLK_e, A) \
MACRO(SDLK_f, A) MACRO(SDLK_g, A) MACRO(SDLK_h, A) MACRO(SDLK_i, A) MACRO(SDLK_j, A) \
MACRO(SDLK_k, A) MACRO(SDLK_l, A) MACRO(SDLK_m, A) MACRO(SDLK_n, A) MACRO(SDLK_o, A) \
MACRO(SDLK_p, A) MACRO(SDLK_q, A) MACRO(SDLK_r, A) MACRO(SDLK_s, A) MACRO(SDLK_t, A) \
MACRO(SDLK_u, A) MACRO(SDLK_v, A) MACRO(SDLK_w, A) MACRO(SDLK_x, A) MACRO(SDLK_y, A) \
MACRO(SDLK_z, A) \
MACRO(SDLK_F1, A) MACRO(SDLK_F2, A) MACRO(SDLK_F3, A) MACRO(SDLK_F4, A) MACRO(SDLK_F5, A) MACRO(SDLK_F6, A)\
MACRO(SDLK_F7, A) MACRO(SDLK_F8, A) MACRO(SDLK_F9, A) MACRO(SDLK_F10, A) MACRO(SDLK_F11, A) MACRO(SDLK_F12, A) \
MACRO(SDLK_BACKSPACE, A) MACRO(SDLK_TAB, A) MACRO(SDLK_SPACE, A) MACRO(SDLK_RETURN, A) MACRO(SDLK_ESCAPE, A) \
MACRO(SDLK_EXCLAIM, A) MACRO(SDLK_QUOTEDBL, A) MACRO(SDLK_HASH, A) MACRO(SDLK_PERCENT, A) MACRO(SDLK_DOLLAR, A) \
MACRO(SDLK_AMPERSAND, A) MACRO(SDLK_QUOTE, A) MACRO(SDLK_LEFTPAREN, A) MACRO(SDLK_RIGHTPAREN, A) MACRO(SDLK_ASTERISK, A) \
MACRO(SDLK_PLUS, A) MACRO(SDLK_COMMA, A) MACRO(SDLK_MINUS, A) MACRO(SDLK_PERIOD, A) MACRO(SDLK_SLASH, A) \
MACRO(SDLK_COLON, A) MACRO(SDLK_SEMICOLON, A) MACRO(SDLK_LESS, A) MACRO(SDLK_EQUALS, A) MACRO(SDLK_GREATER, A) \
MACRO(SDLK_QUESTION, A) MACRO(SDLK_AT, A) MACRO(SDLK_LEFTBRACKET, A) MACRO(SDLK_BACKSLASH, A) MACRO(SDLK_RIGHTBRACKET, A) \
MACRO(SDLK_CARET, A) MACRO(SDLK_UNDERSCORE, A) MACRO(SDLK_BACKQUOTE, A) MACRO(SDLK_CAPSLOCK, A) MACRO(SDLK_PRINTSCREEN, A) \
MACRO(SDLK_SCROLLLOCK, A) MACRO(SDLK_PAUSE, A) MACRO(SDLK_INSERT, A) MACRO(SDLK_HOME, A) MACRO(SDLK_PAGEUP, A) \
MACRO(SDLK_END, A) MACRO(SDLK_PAGEDOWN, A) MACRO(SDLK_NUMLOCKCLEAR, A) MACRO(SDLK_KP_DIVIDE, A) MACRO(SDLK_KP_MULTIPLY, A) \
MACRO(SDLK_KP_MINUS, A) MACRO(SDLK_KP_PLUS, A) MACRO(SDLK_KP_ENTER, A) MACRO(SDLK_KP_PERIOD, A) \
MACRO(SDLK_KP_1, A) MACRO(SDLK_KP_2, A) MACRO(SDLK_KP_3, A)  MACRO(SDLK_KP_4, A) MACRO(SDLK_KP_5, A) \
MACRO(SDLK_KP_6, A) MACRO(SDLK_KP_7, A) MACRO(SDLK_KP_8, A)  MACRO(SDLK_KP_9, A) MACRO(SDLK_KP_0, A) \
MACRO(SDLK_RIGHT, A) MACRO(SDLK_LEFT, A) MACRO(SDLK_DOWN, A) MACRO(SDLK_UP, A) \
MACRO(SDLK_LCTRL, A) MACRO(SDLK_LSHIFT, A) MACRO(SDLK_LALT, A) \
MACRO(SDLK_RCTRL, A) MACRO(SDLK_RSHIFT, A) MACRO(SDLK_RALT, A)


void Input::event_process(const SDL_KeyboardEvent& event, float time)
{
  SDL_Keycode key = event.keysym.sym;
  KeyAction action = event.repeat ? KeyAction::Press : event.state == SDL_PRESSED ? KeyAction::Down : KeyAction::Up;
  
  if (action == KeyAction::Down)
    keyMap[key] = {1, time};
  if (action == KeyAction::Up)
    keyMap[key] = {0, time};
  
  if (!eventable)
    return;

  ecs::send_event(KeyEventAnyActionKey{{}, key, action, time});
  #define MACRO(key, Action) case key: ecs::send_event(Key## Action ##Event<key>{{}, time}); break;

  #define KEY_ACTION_CASE(Action) case KeyAction::Action: \
  ecs::send_event(KeyEventAnyKey<KeyAction::Action>{{}, key, time}); \
    switch (key) { \
      KEY_CODE_SWITCH(Action) \
      default: debug_error("KeyAction::" #Action " unknown keyCode %d", key); break;} break; 

  switch (action)
  {
    KEY_ACTION_CASE(Press)
    KEY_ACTION_CASE(Down)
    KEY_ACTION_CASE(Up)
  
  default:debug_error("unknown KeyAction %d", (int)action);break;
  }
  #undef MACRO
  #define MACRO(Key, Action) case Key: ecs::send_event(KeyEventAnyAction<Key>{{}, action, time}); break;
  switch (key)
  {
    KEY_CODE_SWITCH(_)
    default: debug_error("keyCode %d", key); break;
  }
  #undef MACRO
}


void Input::event_process(const SDL_MouseButtonEvent& event, float time)
{
  MouseButton button = (MouseButton)-1;
  MouseAction action = (MouseAction)-1;
  int x = event.x, y = get_resolution().second - event.y;
  switch (event.button)
  {
    case SDL_BUTTON_LEFT: button = MouseButton::LeftButton; break;
    case SDL_BUTTON_RIGHT: button = MouseButton::RightButton; break;
    case SDL_BUTTON_MIDDLE: button = MouseButton::MiddleButton; break;
    default: return; break;
  }
  switch (event.type)
  {
    case SDL_MOUSEBUTTONDOWN: action = MouseAction::Down; break;
    case SDL_MOUSEBUTTONUP: action = MouseAction::Up; break;
    default: return; break;
  }
  if (!eventable)
    return;
  ecs::send_event(MouseClickEvent{{}, button, action, x, y, time});

  #define BUTTON_SWITCH(Action) \
    MACRO(LeftButton, Action) MACRO(RightButton, Action) MACRO(MiddleButton, Action)
  
  #define MACRO(Button, Action) case MouseButton::Button: ecs::send_event(MouseButtonDownEvent<MouseButton::Button>{{}, action, x, y, time}); break;
  switch (button)
  {
    BUTTON_SWITCH(_)
    default: debug_error("unknown mouse button  %d", button); break;
  }
  #undef MACRO
}
void Input::event_process(const SDL_MouseMotionEvent& event, float time)
{
  if (eventable)
    ecs::send_event(MouseMoveEvent {{}, event.x, get_resolution().second - event.y, event.xrel, -event.yrel, time});
}
void Input::event_process(const SDL_MouseWheelEvent& event, float time)
{
  int wheel = event.y;
  if (Time::time() - wheelData.lastTime > wheelDecayTime)
  {
    wheelData.lastValue = 0;
    wheelData.targetValue = glm::clamp(wheelScrollSpeed * wheel, -1.f, 1.f);
  }
  else
  {
    wheelData.lastValue = get_wheel();
    wheelData.targetValue = glm::clamp(wheelData.targetValue + wheelScrollSpeed * wheel, -1.f, 1.f);
  }
  
  wheelData.lastTime = Time::time();
  if (eventable)
    ecs::send_event(MouseWheelEvent{{}, wheel, time});
}

float Input::get_key_impl(SDL_Keycode keycode, float reaction_time)
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
float Input::get_key_derivative_impl(SDL_Keycode keycode, float reaction_time)
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
float Input::get_wheel_impl()
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
#include "ecs_event_registration.h"

ECS_EVENT_REGISTER(KeyEventAnyActionKey, KeyEventAnyActionKey)
ECS_EVENT_REGISTER(KeyEventAnyKeyPress, KeyEventAnyKey<KeyAction::Press>)
ECS_EVENT_REGISTER(KeyEventAnyKeyDown, KeyEventAnyKey<KeyAction::Down>)
ECS_EVENT_REGISTER(KeyEventAnyKeyUp, KeyEventAnyKey<KeyAction::Up>)

#define MACRO(key, Action) ECS_EVENT_REGISTER(Key## Action ##Event ##key, Key## Action ##Event<key>)

KEY_CODE_SWITCH(Press)
KEY_CODE_SWITCH(Down)
KEY_CODE_SWITCH(Up)
#undef MACRO

#define MACRO(Key, Action) ECS_EVENT_REGISTER(KeyEventAnyAction##Key, KeyEventAnyAction<Key>)
KEY_CODE_SWITCH(_)
#undef MACRO

ECS_EVENT_REGISTER(MouseButtonDownEventLeftButton, MouseButtonDownEvent<MouseButton::LeftButton>)
ECS_EVENT_REGISTER(MouseButtonDownEventRightButton, MouseButtonDownEvent<MouseButton::RightButton>)
ECS_EVENT_REGISTER(MouseButtonDownEventMiddleButton, MouseButtonDownEvent<MouseButton::MiddleButton>)
ECS_EVENT_REGISTER(MouseClickEvent, MouseClickEvent)
ECS_EVENT_REGISTER(MouseMoveEvent, MouseMoveEvent)
ECS_EVENT_REGISTER(MouseWheelEvent, MouseWheelEvent)
