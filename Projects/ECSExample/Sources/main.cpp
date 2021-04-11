#include <iostream>
#include "Application/application.h"
#include "Application/config.h"
#include "ecs/ecs_core.h"
#include "Systems/test_header.h"

int main(int argc, char** argv)
{
  add_configs(argc, (const char**)(argv));
  ecs::EntityId niceCock;
  ecs::initialize_ecs();
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
    ecs::ComponentInitializerList list;
    list.add<A>("v") = A("Alone");
    ecs::create_entity(list);
  }
  ecs::destroy_entity(niceCock);
  ecs::update_systems();
  ecs::send_event<MyEvent>({10});
  ecs::free_ecs();
  /*Application application(get_config("project"), 1848, 1016, true);
  application.get_scene().init(init_scene);
  application.main_loop();

  application.exit();
  */
  return 0;
}