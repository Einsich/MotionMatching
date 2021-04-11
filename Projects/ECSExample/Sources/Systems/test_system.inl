#include "ecs/ecs.h"
#include "test_header.h"

SYSTEM(ecs::SystemOrder::INPUT_DEPEND)
test_system  
( int a, float  &b)
{
	printf("[t] %d, %f\n", a, b);
	b+=1;
}

template<typename Callable>
void b_plus_f_query(Callable func);

SYSTEM()
Lols(std::string& s, float f)
{
	printf("[l] %s, %f\n", s.c_str(), f);
	s+=")";
  QUERY()
	b_plus_f_query([&] (float &b)
  {
    b += f;
		printf("query works");
  });
}

SYSTEM(ecs::SystemOrder::SPECIAL)
math_system(A &v, B *w)
{
	v.x += ".";
	if (w)
	{
		w->x += "_";
		printf("[m] %s, %s\n", v.x.c_str(), w->x.c_str());
	}
	else
	{
		printf("[m] %s, (null)\n", v.x.c_str());
	}
	
}

EVENT()
event_test(const MyEvent &e, const std::string& s)
{
	printf("Event %d to %s", e.x, s.c_str());
}
