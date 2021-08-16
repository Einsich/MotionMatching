#include <Engine/camera.h>
#include <ecs/ecs.h>
#include <Engine/input.h>
#include <Engine/time.h>

REGISTER_TYPE(Camera)
REGISTER_TYPE(FreeCamera)
REGISTER_TYPE(ArcballCamera)
REGISTER_TYPE(Transform)
// CameraManager

template<typename Callable>
static void find_all_created_camera(Callable);
template<typename Callable>
static void check_camera_manager(Callable);

struct CameraManager : ecs::Singleton
{
  std::vector<ecs::EntityId> sceneCameras;
};

EVENT(ecs::SystemTag::GameEditor) create_camera_manager(
  const ecs::OnSceneCreated &,
  CameraManager &manager,
  MainCamera &mainCamera)
{
  manager.sceneCameras.clear();
  mainCamera.eid = ecs::EntityId();
  QUERY(Camera camera)find_all_created_camera([&](ecs::EntityId eid, bool isMainCamera){
    manager.sceneCameras.push_back(eid);
    if (isMainCamera)
      mainCamera.eid = eid;
  });
}

template<typename Callable>
static void set_main_cam_query(const ecs::EntityId&, Callable);

static void set_main_camera_status(const ecs::EntityId &camera, bool status)
{
  QUERY()set_main_cam_query(camera, [&](bool &isMainCamera, MainCamera &mainCamera){
    isMainCamera = status;
    if (status)
      mainCamera.eid = camera;
  });
}
EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) set_main_camera(
  const OnSetMainCamera &event,
  MainCamera &mainCamera)
{
  set_main_camera_status(mainCamera.eid, false);
  set_main_camera_status(event.mainCamera, true);
}

EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) set_next_camera(
  const KeyDownEvent<SDLK_F1> &,
  CameraManager &manager,
  MainCamera &mainCamera)
{
  auto it = std::find(manager.sceneCameras.begin(), manager.sceneCameras.end(), mainCamera.eid);
  set_main_camera_status(mainCamera.eid, false);
  if (it != manager.sceneCameras.end())
  {
    int i = it - manager.sceneCameras.begin();
    i = (i + 1) % manager.sceneCameras.size();
    set_main_camera_status(manager.sceneCameras[i], true);
  }
  else
    debug_error("Can't set next camera to main Camera");
}


template<typename Callable>
static void register_cam_query(Callable);

void register_camera(ecs::EntityId camera)
{
  QUERY()register_cam_query([camera](CameraManager &manager){
    auto it = std::find(manager.sceneCameras.begin(), manager.sceneCameras.end(), camera);
    if (it == manager.sceneCameras.end())
      manager.sceneCameras.push_back(camera);
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
    float const pixToRad = DegToRad * arcballCamera.mouseSensitivity;
    arcballCamera.targetRotation += vec2(e.dx, e.dy) * pixToRad;
  }
}
EVENT() arccam_mouse_click_handler(
  const MouseClickEventAnyEvent &e,
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

EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) freecam_created(
  const ecs::OnEntityCreated &,
  ecs::EntityId eid,
  FreeCamera &freeCamera,
  Transform &transform)
{ 
  update_free_cam_from_transform(freeCamera, transform);
  freeCamera.screenSpaceMovable = freeCamera.rotationable = false;
  register_camera(eid);
}

EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) freecam_mouse_move_handler(
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
    freeCamera.wantedRotation += vec2(e.dx, e.dy) * pixToRad;
  } else
  if (freeCamera.screenSpaceMovable)
  {
    vec2 d = vec2(-e.dx, e.dy) * freeCamera.screenMoveSensitivity;
    freeCamera.wantedPosition += transform.get_right() * d.x +  transform.get_up() * d.y;
  }
}
EVENT(ecs::SystemTag::Editor,ecs::SystemTag::Game) freecam_mouse_click_handler(
  const MouseClickEventAnyEvent &e,
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

SYSTEM(ecs::SystemTag::Editor,ecs::SystemTag::Game) freecamera_update(
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
static void get_main_cam_query(const ecs::EntityId&, Callable);

SYSTEM(ecs::SystemOrder::RENDER - 5,ecs::SystemTag::GameEditor)
update_main_camera_transformations(MainCamera &mainCamera)
{
  ecs::EntityId eid = mainCamera.eid;
  bool mainCameraExists = false;
  QUERY() get_main_cam_query(eid, [&](Camera &camera, Transform &transform)
  {
    mainCamera.position = transform.get_position();
    mainCamera.transform = transform.get_transform();
    mainCamera.projection = camera.get_projection();
    mainCamera.view = inverse(mainCamera.transform);
    mainCameraExists = true;
  });
  if (!mainCameraExists)
  {
    mainCamera.position = vec3();
    mainCamera.transform = mat4(1.f);
    mainCamera.view = mat4(1.f);
    const float aspectRatio = (float)Application::get_context().get_width() / Application::get_context().get_height();
    mainCamera.projection = perspective(90 * DegToRad, aspectRatio, 0.1f, 1000.f);

  }
}



ecs::EntityId create_arcball_camera(float dist, vec2 rotation, vec3 target)
{
  Camera camera;
  camera.set_perspective(90.f, 0.01f, 5000.f);
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
  camera.set_perspective(90.f, 0.01f, 5000.f);
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
  camera.set_perspective(90.f, 0.01f, 5000.f);
  ecs::ComponentInitializerList list;
  list.add<Camera>("camera") = camera;
  list.add<FreeCamera>("freeCamera") = FreeCamera();
  list.add<Transform>("transform") = Transform(position, vec3(rotation, 0));
  list.add<bool>("isMainCamera") = false;
  return ecs::create_entity(list);
}
ecs::EntityId create_camera(vec3 position, vec2 rotation)
{
  Camera camera;
  camera.set_perspective(90.f, 0.01f, 5000.f);
  ecs::ComponentInitializerList list;
  list.add<Camera>("camera") = camera;
  list.add<Transform>("transform") = Transform(position, vec3(rotation.x, rotation.y, 0));
  list.add<bool>("isMainCamera") = false;
  return ecs::create_entity(list);
}