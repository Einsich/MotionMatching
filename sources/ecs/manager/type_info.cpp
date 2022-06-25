#include "type_info.h"
#include "3dmath.h"
#include "manager/entity_id.h"
#include <type_registration.h>
#include "string_hash.h"
#include "ecs_tag.h"
using namespace ecs;


ECS_REGISTER_TYPE(bool, bool, true, true, true, true);
ECS_REGISTER_TYPE_AND_VECTOR(uvec2, uvec2, true, true, true, true);
ECS_REGISTER_TYPE_AND_VECTOR(uvec3, uvec3, true, true, true, true);
ECS_REGISTER_TYPE_AND_VECTOR(uvec4, uvec4, true, true, true, true);

ECS_REGISTER_TYPE_AND_VECTOR(int, int, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(uint, uint, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(ivec2, ivec2, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(ivec3, ivec3, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(ivec4, ivec4, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(float, float, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(vec2, vec2, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(vec3, vec3, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(vec4, vec4, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(mat2, mat2, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(mat3, mat3, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(mat4, mat4, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(double, double, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(string, string, false, true)
ECS_REGISTER_TYPE_AND_VECTOR(EntityId, EntityId, true, true, true, true)
ECS_REGISTER_TYPE_AND_VECTOR(Tag, Tag, true, true, true, true)

namespace ecs
{
  TypeInfo::TypeInfo(TypeRTTI rtti, ecs::string &&name, UserTypeInfo userInfo):
    hashId(HashedString(name)), name(std::move(name)),
    rtti(rtti),
    userInfo(userInfo)
  {
    ECS_LOG("ECS: register %u type %s, sizeof %u, hash %u, trivial copy - %s, move - %s",
      TypeInfo::types().size(), this->name.c_str(), rtti.sizeOf, hashId,
      rtti.trivialCopy ? "true" : "false", rtti.trivialMove ? "true" : "false");
    #if ECS_USE_EASTL
    TypeInfo::types().insert({hashId, this});
    #else
    TypeInfo::types().try_emplace(hashId, this);
    #endif
  }
}