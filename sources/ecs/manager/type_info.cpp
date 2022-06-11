#include "type_info.h"
#include "3dmath.h"
#include "manager/entity_id.h" 
#include "base_types.h"
#include "../component_editor.h"
#include <type_registration.h>
#include "common.h"
#include "string_hash.h"
#include "ecs_tag.h"
using namespace std;
using namespace ecs;

#define MACRO(T, N, trivial_copy, trivial_move) ECS_REGISTER_TYPE_AND_VECTOR(T, T, trivial_copy, trivial_move);

ECS_REGISTER_TYPE(bool, bool, true, true);
ECS_REGISTER_TYPE_AND_VECTOR(uvec2, uvec2, true, true);
ECS_REGISTER_TYPE_AND_VECTOR(uvec3, uvec3, true, true);
ECS_REGISTER_TYPE_AND_VECTOR(uvec4, uvec4, true, true);
BASE_TYPES
namespace ecs
{
  TypeInfo::TypeInfo(TypeRTTI rtti, std::string &&name, bool trivialCopy, bool trivialRelocatable, UserTypeInfo userInfo):
    hashId(HashedString(name)), name(std::move(name)),
    trivialCopy(trivialCopy), trivialRelocatable(trivialRelocatable),
    rtti(rtti),
    userInfo(userInfo)
  {
    debug_log("ECS: register %u type %s, sizeof %u, hash %u, trivial copy - %s, move - %s",
      TypeInfo::types().size(), this->name.c_str(), rtti.sizeOf, hashId,
      trivialCopy ? "true" : "false", trivialRelocatable ? "true" : "false");

    TypeInfo::types().try_emplace(hashId, this);
  }
}