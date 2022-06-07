#pragma once
#define BASE_TYPES \
MACRO(int, int, true, true)\
MACRO(uint, uint, true, true)\
MACRO(ivec2, glm::ivec2, true, true)\
MACRO(ivec3, glm::ivec3, true, true)\
MACRO(ivec4, glm::ivec4, true, true)\
MACRO(float, float, true, true)\
MACRO(vec2, glm::vec2, true, true)\
MACRO(vec3, glm::vec3, true, true)\
MACRO(vec4, glm::vec4, true, true)\
MACRO(mat2, glm::mat2, true, true)\
MACRO(mat3, glm::mat3, true, true)\
MACRO(mat4, glm::mat4, true, true)\
MACRO(double, double, true, true)\
MACRO(string, std::string, false, true)\
MACRO(EntityId, ecs::EntityId, true, true)\
MACRO(Tag, ecs::Tag, true, true)