#include <camera.h>
#include <ecs/ecs.h>
#include <input.h>
#include <application/application_data.h>
#include <ecs/registration.h>

bool edit_component(Camera &camera, const char *name, bool view_only);
ECS_REGISTER_TYPE(Camera, "Camera", ecs::PODType);
ECS_REGISTER_TYPE(FreeCamera, "FreeCamera", ecs::PODType);
ECS_REGISTER_TYPE(ArcballCamera, "ArcballCamera", ecs::PODType);
// CameraManager

template<typename Callable>
static void find_all_created_camera(Callable);
template<typename Callable>
static void check_camera_manager(Callable);

struct EditorCamera : ecs::Singleton
{
  ecs::EntityId camera;
};
ECS_REGISTER_SINGLETON(MainCamera)
ECS_REGISTER_SINGLETON(EditorCamera)
EVENT(tags=game) find_main_camera_game(
  const ecs::OnSceneCreated &,
  MainCamera &mainCamera)
{
  mainCamera.eid = ecs::EntityId();
  QUERY(require=Camera camera)find_all_created_camera([&](ecs::EntityId eid, bool isMainCamera){
    if (isMainCamera)
      mainCamera.eid = eid;
  });
}
template<typename Callable>
void find_editor_camera(Callable);

EVENT(tags=editor) find_main_camera_editor(
  const ecs::OnSceneCreated &,
  EditorCamera &editorCameraManager)
{
  ecs::EntityId editorCamera;
  QUERY(require=Camera camera)find_editor_camera([&](bool isMainCamera, ecs::EntityId eid)
  {
    if (isMainCamera)
      editorCamera = eid;
  });
  if (!editorCamera)
  {
    Camera camera;
    camera.set_perspective(90.f, 0.01f, 5000.f);
    editorCamera = ecs::create_entity("free_camera", {
      {"camera", camera},
      {"isMainCamera", true}
    });
  }
  editorCameraManager.camera = editorCamera;
}

SYSTEM(stage=act; require=Camera camera) set_main_camera(
  ecs::EntityId eid,
  const bool isMainCamera,
  MainCamera &mainCamera)
{
  if (isMainCamera)
    mainCamera.eid = eid;
}

//ArcballCamera

EVENT() arcball_created(
  const ecs::OnEntityCreated &,
  ArcballCamera &arcballCamera,
  Transform &transform)
{
  arcballCamera.calculate_transform(transform);
}

EVENT() arccam_mouse_move_handler(
  const MouseMoveEvent &e,
  ArcballCamera &arcballCamera,
  bool isMainCamera)
{
  if (!isMainCamera)
    return;
  if (arcballCamera.rotationEnable)
  {
    float const pixToRad = DegToRad * arcballCamera.mouseSensitivity;
    arcballCamera.targetRotation += vec2(e.dx, e.dy) * pixToRad;
  }
}
EVENT() arccam_mouse_click_handler(
  const MouseClickEvent &e,
  ArcballCamera &arcballCamera,
  bool isMainCamera)
{
  if (!isMainCamera)
    return;
  if (e.buttonType == MouseButton::RightButton)
  {
    arcballCamera.rotationEnable = e.action == MouseAction::Down;
  }
}

EVENT() arccam_mouse_wheel_handler(
  const MouseWheelEvent &e,
  ArcballCamera &arcballCamera,
  bool isMainCamera)
{
  if (!isMainCamera)
    return;
  arcballCamera.targetZoom -= e.wheel * arcballCamera.wheelSensitivity;
  arcballCamera.targetZoom = glm::clamp(arcballCamera.targetZoom, 0.f, 1.f);
}

template<typename Callable>
void check_arcball_target(ecs::EntityId, Callable);

SYSTEM(stage=act) arcball_camera_update(
  ArcballCamera &arcballCamera,
  bool isMainCamera,
  Transform &transform,
  ecs::EntityId arcballCameraTarget)
{
  if (!isMainCamera)
    return;
  QUERY() check_arcball_target(arcballCameraTarget, [&](Transform &transform)
  {
    arcballCamera.targetPosition = transform.get_position();
  });
  float lerpFactor = arcballCamera.lerpStrength * Time::delta_time();

  arcballCamera.curZoom = lerp(arcballCamera.curZoom, arcballCamera.targetZoom, lerpFactor);
  arcballCamera.curRotation = lerp(arcballCamera.curRotation, arcballCamera.targetRotation, lerpFactor);
  arcballCamera.distance = arcballCamera.maxdistance * arcballCamera.curZoom;
  arcballCamera.calculate_transform(transform);
}

//FreeCamera

void update_free_cam_from_transform(FreeCamera &freeCamera, const Transform &transform)
{
  if (length2(freeCamera.curPosition - transform.get_position()) > 0.0001f)
  {
     freeCamera.curPosition =  freeCamera.wantedPosition = transform.get_position();
  }
  vec2 yawPitch;
  float roll;
  glm::extractEulerAngleYXZ((mat4)transform.get_rotation(), yawPitch.x, yawPitch.y, roll);
  if (length2(mod_f(yawPitch - freeCamera.curRotation, PI)) > 0.0001f)
  {
    freeCamera.curRotation = freeCamera.wantedRotation = yawPitch;
  }
}

EVENT() freecam_created(
  const ecs::OnEntityCreated &,
  FreeCamera &freeCamera,
  Transform &transform)
{ 
  update_free_cam_from_transform(freeCamera, transform);
  freeCamera.screenSpaceMovable = freeCamera.rotationable = false;
}

EVENT() freecam_mouse_move_handler(
  const MouseMoveEvent &e,
  FreeCamera &freeCamera,
  Transform &transform,
  bool isMainCamera)
{
  if (!isMainCamera)
    return;
  if (freeCamera.rotationable)
  {
    float const pixToRad = freeCamera.rotationSensitivity * DegToRad;
    float prevY = freeCamera.wantedRotation.y;
    freeCamera.wantedRotation += vec2(e.dx, e.dy) * pixToRad;
    if (abs(prevY) < PIHALF && PIHALF <= abs(freeCamera.wantedRotation.y))
      freeCamera.wantedRotation.y = prevY;
  } else
  if (freeCamera.screenSpaceMovable)
  {
    vec2 d = vec2(-e.dx, e.dy) * freeCamera.screenMoveSensitivity;
    freeCamera.wantedPosition += transform.get_right() * d.x +  transform.get_up() * d.y;
  }
}
EVENT() freecam_mouse_click_handler(
  const MouseClickEvent &e,
  FreeCamera &freeCamera,
  bool isMainCamera)
{
  if (!isMainCamera)
    return;
  switch (e.buttonType)
  {
  case MouseButton::RightButton: freeCamera.rotationable = e.action == MouseAction::Down; break;
  case MouseButton::MiddleButton: freeCamera.screenSpaceMovable = e.action == MouseAction::Down; break;
  default:
    break;
  }
}

SYSTEM(stage=act;) freecamera_update(
  FreeCamera &freeCamera,
  bool isMainCamera,
  Transform &transform)
{
  if (!isMainCamera)
    return;

  update_free_cam_from_transform(freeCamera, transform);
  vec3 delta = -transform.get_forward() * (Input::input().get_key(SDLK_w) - Input::input().get_key(SDLK_s)) +
              transform.get_right() * (Input::input().get_key(SDLK_d) - Input::input().get_key(SDLK_a)) +
              transform.get_up() * (Input::input().get_key(SDLK_e) - Input::input().get_key(SDLK_c));

  float speed = lerp(freeCamera.minSpeed, freeCamera.maxSpeed, Input::input().get_key(SDLK_LSHIFT));
  freeCamera.wantedPosition += delta * speed * Time::delta_time();

  float lerpFactor = Time::delta_time() * freeCamera.lerpStrength;
  freeCamera.curRotation = lerp(freeCamera.curRotation, freeCamera.wantedRotation, lerpFactor);
  freeCamera.curPosition = lerp(freeCamera.curPosition, freeCamera.wantedPosition, lerpFactor);
  transform.get_position() = freeCamera.curPosition;
  freeCamera.calculate_transform(transform);
}


template<typename Callable>
static void get_main_cam_query(ecs::EntityId, Callable);

void update_camera_transformation(MainCamera &mainCamera, ecs::EntityId eid)
{
  bool mainCameraExists = false;
  QUERY() get_main_cam_query(eid, [&](Camera &camera, Transform &transform)
  {
    mainCamera.position = transform.get_position();
    mainCamera.transform = transform.get_transform();
    mainCamera.projection = camera.get_projection();
    mainCamera.view = inverse(mainCamera.transform);
    mainCamera.mainFrustum = create_frustum_from_camera(camera, transform);
    mainCamera.aspectRatio = camera.aspectRatio;
    mainCameraExists = true;
  });
  if (!mainCameraExists)
  {
    mainCamera.position = vec3();
    mainCamera.transform = mat4(1.f);
    mainCamera.view = mat4(1.f);
    mainCamera.aspectRatio = Application::get_context().get_aspect_ratio();
    mainCamera.projection = perspective(90 * DegToRad, mainCamera.aspectRatio, 0.1f, 1000.f);
  }
}

SYSTEM(stage=before_render; tags=game)
update_main_camera_game_transformations(MainCamera &mainCamera)
{
  update_camera_transformation(mainCamera, mainCamera.eid);
}

SYSTEM(stage=before_render; tags=editor)
update_main_camera_editor_transformations(MainCamera &mainCamera, EditorCamera &editorCamera)
{
  update_camera_transformation(mainCamera, editorCamera.camera);
}



ecs::EntityId create_arcball_camera(float dist, vec2 rotation, vec3 target)
{
  Camera camera;
  camera.set_perspective(90.f, 0.01f, 5000.f);
  return ecs::create_entity("arcball_camera", {
    {"camera", camera},
    {"arcballCamera", ArcballCamera(dist, rotation, target)}
  });
}
ecs::EntityId create_arcball_camera(float dist, vec2 rotation, ecs::EntityId target)
{
  Camera camera;
  camera.set_perspective(90.f, 0.01f, 5000.f);
  return ecs::create_entity("arcball_camera", {
    {"camera", camera},
    {"arcballCamera", ArcballCamera(dist, rotation)},
    {"arcballCameraTarget", target}
  });
}

ecs::EntityId create_free_camera(vec3 position, vec2 rotation)
{
  Camera camera;
  camera.set_perspective(90.f, 0.01f, 5000.f);
  return ecs::create_entity("free_camera", {
    {"camera", camera},
    {"transform", Transform(position, vec3(rotation, 0))}
  });
}

ecs::EntityId create_camera(vec3 position, vec2 rotation, bool main_camera)
{
  Camera camera;
  camera.set_perspective(90.f, 0.01f, 5000.f);
  return ecs::create_entity("base_camera", {
    {"camera", camera},
    {"transform", Transform(position, vec3(rotation, 0))},
    {"isMainCamera", main_camera}
  });
}