#include "type_info.h"
#include "3dmath.h"
#include "manager/entity_id.h" 
#include "base_types.h"
#include "../component_editor.h"
#include <type_registration.h>
using namespace std;
using namespace ecs;

#define MACRO(T, N, trivial_copy, trivial_move) ECS_REGISTER_TYPE_AND_VECTOR(T, T, trivial_copy, trivial_move);

ECS_REGISTER_TYPE(bool, bool, true, true);
BASE_TYPES

