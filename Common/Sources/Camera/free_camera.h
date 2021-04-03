#pragma once
#include "camera.h"


class FreeCamera: public Camera, public IUpdatable
{
 private:
  vec2 curRotation, wantedRotation;
  vec3 curPosition, wantedPosition;
  bool rotationEnable;
  void calculate_transform();
public: 
  FreeCamera(vec3 position, vec2 rotation);
  void mouse_move_handler(const MouseMoveEvent &e);
  void mouse_click_handler(const MouseClickEvent &e);
  virtual void update() override;

};
