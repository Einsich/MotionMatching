#include <ecs/ecs.h>
#include "test_header.h"

SYSTEM(ecs::SystemOrder::LATE_RENDER) test_system ( int a, float  &b)
{
	b = 1;
	//printf("[test_system] %d, %f\n\n", a, b);
}

template<typename Callable>
void b_plus_f_query(Callable func);

SYSTEM() system_with_query(float f, std::string &s)
{
  QUERY() b_plus_f_query([&] (float &b)
  {
    b += f;
		//printf("[system_with_query] query works, s = %s, b = %f\n\n", s.c_str(), b);
  });
}
SYSTEM(ecs::SystemOrder::EARLY_RENDER) nullable_system(A &v, B *w)
{
	//printf("[nullable_system] %s, %s\n\n", v.x.c_str(), w ? w->x.c_str() : "(null)");
}

struct TestEvent
{
	std::string message;
};
SYSTEM(ecs::SystemOrder::LATE_RENDER) event_sender(float  &b)
{
	TestEvent e; e.message = "b var is " + std::to_string(b);
	//printf("[event_sender] send %s\n\n", e.message.c_str());
	ecs::send_event(e);
}
EVENT()test_handler(const TestEvent &e, std::string &s)
{
	//printf("[test_handler] received %s in %s\n\n", e.message.c_str(), s.c_str());
}
SYSTEM() eid_test(const ecs::EntityId &eid)
{
	//printf("[eid_test] archtype %u, index %u\n\n", eid.archetype_index(), eid.array_index());
}