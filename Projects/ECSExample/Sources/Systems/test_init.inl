#include "ecs/ecs.h"
#include "test_header.h"


EVENT()
on_scene_create(const ecs::OnSceneCreated &)
{
  ecs::EntityId niceCock;
  {
    ecs::ComponentInitializerList list;
    list.add<int>("a") = 1;
    list.add<float>("b") = 2.f;
    ecs::create_entity(list);
  }
  {
    ecs::ComponentInitializerList list;
    list.add<std::string>("s") = std::string("Yeee");
    list.add<float>("f") = 3.14f;
    ecs::create_entity(list);
  }
  {
    ecs::ComponentInitializerList list;
    list.add<A>("v") = A("Nice");
    list.add<B>("w") = B("Cock");
    niceCock = ecs::create_entity(list);
  }
  {
    for (int i = 0; i < 5; i++)
    {
      ecs::ComponentInitializerList list;
      list.add<A>("v") = A(std::string("\\0_0/", i +1));
      ecs::create_entity(list);
    }
  }
  ecs::destroy_entity(niceCock);
  ecs::send_event<MyEvent>({10});
}