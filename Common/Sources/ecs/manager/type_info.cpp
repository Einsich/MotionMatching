#include "type_info.h"
#include "3dmath.h"
#include "ecs/manager/entity_id.h" 
#include "ecs/base_types.h"
#include "../component_editor.h"
using namespace std;
using namespace ecs;

#define MACRO(T, N) REGISTER_TYPE(T)

REGISTER_TYPE(bool)
BASE_TYPES

