#include "ecs_core.h"
#include "ecs_event.h"
#include "ecs_event_registration.h"

ECS_EVENT_REGISTER(OnSceneCreated, ecs::OnSceneCreated)
ECS_EVENT_REGISTER(OnSceneDestroy, ecs::OnSceneDestroy)
ECS_EVENT_REGISTER(OnEntityCreated, ecs::OnEntityCreated)
ECS_EVENT_REGISTER(OnEntityDestroyed, ecs::OnEntityDestroyed)