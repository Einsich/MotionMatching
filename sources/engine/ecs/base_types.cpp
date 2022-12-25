#include <ecs/registration.h>
#include <ecs/ecs.h>
#include <glm/glm.hpp>

using uint = unsigned int;

ECS_REGISTER_TYPE(bool, "bool", ecs::PODType);
ECS_REGISTER_TYPE_AND_VECTOR(glm::uvec2, "uvec2", ecs::PODType);
ECS_REGISTER_TYPE_AND_VECTOR(glm::uvec3, "uvec3", ecs::PODType);
ECS_REGISTER_TYPE_AND_VECTOR(glm::uvec4, "uvec4", ecs::PODType);

ECS_REGISTER_TYPE_AND_VECTOR(int, "int", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(uint, "uint", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::ivec2, "ivec2", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::ivec3, "ivec3", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::ivec4, "ivec4", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(float, "float", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::vec2, "vec2", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::vec3, "vec3", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::vec4, "vec4", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::mat2, "mat2", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::mat3, "mat3", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::mat4, "mat4", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(glm::mat3x4, "mat3x4", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(double, "double", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(ecs::string, "string", ecs::TrivialMoveConstructor)
ECS_REGISTER_TYPE_AND_VECTOR(ecs::EntityId, "EntityId", ecs::PODType)
ECS_REGISTER_TYPE_AND_VECTOR(ecs::Tag, "Tag", ecs::PODType)