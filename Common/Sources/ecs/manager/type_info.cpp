#include "type_info.h"
#include "3dmath.h"
#include "Engine/camera.h"
#include "Engine/transform.h"
#include "Engine/Render/skybox.h"
#include "ecs/manager/entity_id.h"
#include "ecs/base_types.h"
#include "Serialization/serialization.h"
using namespace std;
using namespace ecs;

#define MACRO(T, N) REGISTER_TYPE(T)

REGISTER_TYPE(bool)
REGISTER_TYPE(Camera)
REGISTER_TYPE(FreeCamera)
REGISTER_TYPE(ArcballCamera)
REGISTER_TYPE(Transform)
REGISTER_TYPE(SkyBox)
BASE_TYPES

