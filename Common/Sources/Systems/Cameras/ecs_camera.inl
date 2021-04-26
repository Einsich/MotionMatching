#include "Camera/camera.h"
#include "ecs/ecs.h"
#include "Event/input.h"
#include "Time/time.h"

// CameraManager


template<typename Callable>
static void set_main_cam_query(const ecs::EntityId&, Callable);

static void set_main_camera_status(const ecs::EntityId &mainCamera, bool status)
{
  QUERY()set_main_cam_query(mainCamera, [status](bool &isMainCamera){
    isMainCamera = status;
  });
}
EVENT() set_main_camera(
  const OnSetMainCamera &event,
  std::vector<ecs::EntityId> &sceneCameras,
  ecs::EntityId &mainCamera)
{
  set_main_camera_status(mainCamera, false);
  mainCamera = event.mainCamera;
  set_main_camera_status(mainCamera, true);
  if (std::find(sceneCameras.begin(), sceneCameras.end(), mainCamera) == sceneCameras.end())
  {
    sceneCameras.push_back(mainCamera);
  }
}

EVENT() set_next_camera(
  const KeyboardEvent &e,
  std::vector<ecs::EntityId> &sceneCameras,
  ecs::EntityId &mainCamera)
{
  if (e.keycode == SDLK_F1)
  {
    auto it = std::find(sceneCameras.begin(), sceneCameras.end(), mainCamera);
    if (it != sceneCameras.end())
    {
      int i = it - sceneCameras.begin();
      i = (i + 1) % sceneCameras.size();
      set_main_camera_status(mainCamera, false);
      mainCamera = sceneCameras[i];
      set_main_camera_status(mainCamera, true);
    }
    else
      debug_error("Can't set next camera to main Camera");
  }
}


template<typename Callable>
static void register_cam_query(Callable);

void register_camera(ecs::EntityId camera)
{
  QUERY()register_cam_query([camera](std::vector<ecs::EntityId> &sceneCameras){
    auto it = std::find(sceneCameras.begin(), sceneCameras.end(), camera);
    if (it == sceneCameras.end())
      sceneCameras.push_back(camera);
  });
}
//ArcballCamera

EVENT() arcball_created(
  const ecs::OnEntityCreated &,
  ecs::EntityId eid,
  ArcballCamera &arcballCamera,
  Transform &transform)
{
  arcballCamera.calculate_transform(transform);
  register_camera(eid);
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
    float const pixToRad = PI / 180.f * 0.2f;
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
  if (e.buttonType == MouseButtonType::MiddleButton)
  {
    arcballCamera.rotationEnable = e.action == MouseButtonAction::Down;
  }
}

EVENT() arccam_mouse_wheel_handler(
  const MouseWheelEvent &e,
  ArcballCamera &arcballCamera,
  bool isMainCamera)
{
  if (!isMainCamera)
    return;
  arcballCamera.targetZoom -= e.wheel * 0.03f;
  arcballCamera.targetZoom = glm::clamp(arcballCamera.targetZoom, 0.05f, 1.f);
}

template<typename Callable>
void check_arcball_target(const ecs::EntityId&, Callable);
SYSTEM() arcball_camera_update(
  ArcballCamera &arcballCamera,
  bool isMainCamera,
  Transform &transform,
  ecs::EntityId arcballCameraTarget)
{
  if (!isMainCamera)
    return;
  QUERY() check_arcball_target(arcballCameraTarget, [&](Transform &transform)
  {
    arcballCamera.target_position = transform.get_position();
  });

  arcballCamera.zoom = lerp(arcballCamera.zoom, arcballCamera.targetZoom, Time::delta_time() * 10);
  arcballCamera.rotation = lerp(arcballCamera.rotation, arcballCamera.targetRotation, Time::delta_time() * 10);
  arcballCamera.distance = arcballCamera.maxdistance * arcballCamera.zoom;
  arcballCamera.calculate_transform(transform);
}

//FreeCamera

EVENT() freecam_created(
  const ecs::OnEntityCreated &,
  ecs::EntityId eid,
  FreeCamera &freeCamera,
  Transform &transform)
{ 
  transform.set_position(freeCamera.curPosition);
  freeCamera.calculate_transform(transform);
  register_camera(eid);
}

EVENT() freecam_mouse_move_handler(
  const MouseMoveEvent &e,
  FreeCamera &freeCamera,
  bool isMainCamera)
{
  if (!isMainCamera)
    return;
  if (freeCamera.rotationEnable)
  {
    float const pixToRad = PI / 180.f * 0.2f;
    freeCamera.wantedRotation += vec2(e.dx, e.dy) * pixToRad;
  }
}
EVENT() freecam_mouse_click_handler(
  const MouseClickEvent &e,
  FreeCamera &freeCamera,
  bool isMainCamera)
{
  if (!isMainCamera)
    return;
  if (e.buttonType == MouseButtonType::MiddleButton)
  {
    freeCamera.rotationEnable = e.action == MouseButtonAction::Down;
  }
}

SYSTEM() arccam_update(
  FreeCamera &freeCamera,
  bool isMainCamera,
  Transform &transform)
{
  if (!isMainCamera)
    return;
  
  vec3 delta = -transform.get_forward() * (Input::input().get_key(SDLK_w) - Input::input().get_key(SDLK_s)) +
              transform.get_right() * (Input::input().get_key(SDLK_d) - Input::input().get_key(SDLK_a)) +
              transform.get_up() * (Input::input().get_key(SDLK_e) - Input::input().get_key(SDLK_c));
  const float minSpeed = 5.6f;
  const float maxSpeed = 20.6f;
  float speed = lerp(minSpeed, maxSpeed, Input::input().get_key(SDLK_LSHIFT));
  freeCamera.wantedPosition += delta * speed * Time::delta_time();

  float lerpFactor = Time::delta_time() * 3;
  freeCamera.curRotation = lerp(freeCamera.curRotation, freeCamera.wantedRotation, lerpFactor);
  freeCamera.curPosition = lerp(freeCamera.curPosition, freeCamera.wantedPosition, lerpFactor);
  transform.get_position() = freeCamera.curPosition;
  freeCamera.calculate_transform(transform);
}


template<typename Callable>
static void get_main_cam_query(Callable);
template<typename Callable>
static void get_main_cam_property_query(const ecs::EntityId&, Callable);

bool main_camera(mat4 &cam_transform, mat4 &cam_projection)
{
  bool result = false;
  QUERY() get_main_cam_query([&](const ecs::EntityId &mainCamera)
  {
    QUERY() get_main_cam_property_query(mainCamera, [&](Camera &camera, Transform &transform)
    {
      cam_transform = transform.get_transform();
      cam_projection = camera.get_projection();
      result = true;
    });
  });
  return result;
}


ecs::EntityId create_camera_manager()
{
  ecs::ComponentInitializerList list;
  list.add<std::vector<ecs::EntityId>>("sceneCameras") = std::vector<ecs::EntityId>();
  list.add<ecs::EntityId>("mainCamera") = ecs::EntityId();
  return ecs::create_entity(list);
}

ecs::EntityId create_arcball_camera(float dist, vec2 rotation, vec3 target)
{
  Camera camera;
  camera.set_perspective(90.f * DegToRad, 0.01f, 5000.f);
  ecs::ComponentInitializerList list;
  list.add<Camera>("camera") = camera;
  list.add<ArcballCamera>("arcballCamera") = ArcballCamera(dist, rotation, target);
  list.add<Transform>("transform") = Transform();
  list.add<bool>("isMainCamera") = false;
  list.add<ecs::EntityId>("arcballCameraTarget") = ecs::EntityId();
  ecs::EntityId cameraId = ecs::create_entity(list);
  set_main_camera_status(cameraId, true);
  return cameraId;
}
ecs::EntityId create_arcball_camera(float dist, vec2 rotation, ecs::EntityId target)
{
  Camera camera;
  camera.set_perspective(90.f * DegToRad, 0.01f, 5000.f);
  ecs::ComponentInitializerList list;
  list.add<Camera>("camera") = camera;
  list.add<ArcballCamera>("arcballCamera") = ArcballCamera(dist, rotation);
  list.add<Transform>("transform") = Transform();
  list.add<bool>("isMainCamera") = false;
  list.add<ecs::EntityId>("arcballCameraTarget") = target;
  return ecs::create_entity(list);
}

ecs::EntityId create_free_camera(vec3 position, vec2 rotation)
{
  Camera camera;
  camera.set_perspective(90.f * DegToRad, 0.01f, 5000.f);
  ecs::ComponentInitializerList list;
  list.add<Camera>("camera") = camera;
  list.add<FreeCamera>("freeCamera") = FreeCamera(position, rotation);
  list.add<Transform>("transform") = Transform();
  list.add<bool>("isMainCamera") = false;
  return ecs::create_entity(list);
}
ecs::EntityId create_camera(vec3 position, vec2 rotation)
{
  Camera camera;
  camera.set_perspective(90.f * DegToRad, 0.01f, 5000.f);
  ecs::ComponentInitializerList list;
  list.add<Camera>("camera") = camera;
  list.add<Transform>("transform") = Transform(position, vec3(rotation.x, rotation.y, 0));
  list.add<bool>("isMainCamera") = false;
  return ecs::create_entity(list);
}