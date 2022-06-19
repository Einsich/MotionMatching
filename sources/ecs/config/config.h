#pragma once


#define ECS_NO_LOG 0
#define ECS_FEW_LOG 1
#define ECS_VERBOSE 2

#define ECS_LOG_LEVEL ECS_VERBOSE

void debug_log(const char *format, ...);
#define ECS_LOG debug_log

#ifndef ECS_LOG
#define ECS_LOG(FORMAT, ...) printf(FORMAT, __VA_ARGS__)
#endif

#define ECS_USE_EASTL 1

#if ECS_USE_STD

  #include <vector>
  #include <string>
  #include <map>

namespace ecs
{
  template<typename T>
  using vector = std::vector<T>;
  using string = std::string;
  template<typename U, typename V>
  using map = std::map<U, V>;
}
#elif ECS_USE_EASTL

  #include <eastl/vector.h>
  #include <eastl/string.h>
  #include <eastl/vector_map.h>

namespace ecs
{
  template<typename T>
  using vector = eastl::vector<T>;
  using string = eastl::string;
  template<typename U, typename V>
  using map = eastl::vector_map<U, V>;
}
#else
  static_assert(0, "ECS_USE_STD or ECS_USE_EASTL should be set to 1");
#endif

namespace ecs
{
  using uint = unsigned int;
}

