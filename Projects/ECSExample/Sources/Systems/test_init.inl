#include "ecs/ecs.h"
#include "test_header.h"


EVENT()
on_scene_create(const ecs::OnSceneCreated &)
{
  ecs::EntityId ab, niceCock;
  {
    ecs::ComponentInitializerList list;
    list.add<int>("a") = 1;
    list.add<float>("b") = 2.f;
    ecs::create_entity(list);
    ab = ecs::create_entity(list);
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
  {
    ecs::ComponentInitializerList list;
    list.add<ecs::EntityId>("cock_attach") = niceCock;
    ecs::create_entity(list);
  }
  ecs::destroy_entity(ab);
  ecs::send_event<MyEvent>({10});
}



EVENT() entity_created(const ecs::OnEntityCreated &,
                        std::string &s)
{
  s = "I was created!";
  printf("I was created\n");
}
EVENT() entity_destroyed(const ecs::OnEntityDestroyed &,
                        std::string &s)
{
  s = "I was destroyed!";
  printf("I was destroyed\n");
}
template<typename Callable>
void attsh_query_test(const ecs::EntityId&, Callable);

SYSTEM() attach_test(ecs::EntityId cock_attach)
{
  QUERY() attsh_query_test(cock_attach, [](
    const A &v,
    const B &w){
    //printf("[single query] %s %s\n", v.x.c_str(), w.x.c_str());
  });
}
