#include "type_info.h"
#include "3dmath.h"
#include "manager/entity_id.h" 
#include "base_types.h"
#include "../component_editor.h"
using namespace std;
using namespace ecs;

#define MACRO(T, N) ECS_DECLARE_TYPE_EXT(T)

ECS_DECLARE_TYPE_EXT(bool)
BASE_TYPES

