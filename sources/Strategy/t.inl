#include <ecs.h>


SYSTEM( tags = debug ; stage = render ;
before = x ; require = int h , float t , vector < int > v ;
require_not = ecs::Tag lol)
t(int x, int &y)
{
  debug_log("%d %d", x, y);
  y = 1;
}
SYSTEM(scene=game,menu;
stage=render;after=t;
require = int h, vector < pair < int, int  > >  v;require_not=ecs::Tag lol)
g(int x, int y)
{
  debug_log("%d %d", x, y);
}