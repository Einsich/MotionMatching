#pragma once

namespace ecs
{
  struct Event
  {

  };
  // send after creating scene
  struct OnSceneCreated : public Event
  {

  };
  struct OnEntityCreated : public Event
  {

  }; 
  struct OnEntityEdited : public Event
  {

  };
  struct OnEntityDestroyed : public Event
  {

  };
}