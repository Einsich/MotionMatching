#pragma once
#include "ecs_core.h"
#include "system_order.h"
#include "system_tag.h"
#include "ecs_event.h"
#define SYSTEM(...)static inline void 
#define QUERY(...)
#define EVENT(...)static inline void 