#pragma once
#include "ecs_core.h"
#include "ecs_event.h"
#include "ecs_tag.h"
#define SYSTEM(...)static __forceinline void
#define QUERY(...)
#define EVENT(...)static __forceinline void