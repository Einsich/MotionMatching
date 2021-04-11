#include "test_header.h"

void test_system  
( int a, float  &b)
{
	printf("[t] %d, %f\n", a, b);
	b+=1;
}

template<typename Callable>
void b_plus_f_query(Callable func);

void 
Lol_system(std::string& s, float f)
{
	printf("[l] %s, %f\n", s.c_str(), f);
	s+=")";
  b_plus_f_query([&] (float &b)
  {
    b += f;
  });
}

void math_system(A &v, B *w)
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
