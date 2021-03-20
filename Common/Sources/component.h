#pragma once
#include <memory>
class GameObject;
class Camera;
class DirectionLight;
class Component
{
private:
  GameObject *gameObject = nullptr;
public:
  virtual ~Component(){  }
  friend class GameObject;
  GameObject *game_object() const
  {
    return gameObject;
  }
};

class IUpdatable
{
public:
    virtual void update() = 0;
};
class IRenderable
{
public:
    virtual void render(const Camera& mainCam, const DirectionLight& light, bool wire_frame = false) = 0;
};
class IUIRenderable
{
public:
    virtual void ui_render() = 0;
};
class ISaveable
{
public:
    virtual void save() = 0;
};
using ComponentPtr = std::shared_ptr<Component>;

template<typename T>
ComponentPtr make_component(T* t)
{
  return std::shared_ptr<Component>((Component*)t);
}
