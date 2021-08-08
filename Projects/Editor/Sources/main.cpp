#include <iomanip>
#include <ostream>
#include <istream>
#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <type_traits>
#include <vector>
#include <fstream>
#include <sstream>
int main()
{
  std::stringstream file("lol 11", std::ios::in);
  int x;
  std::string s;
  bool b;
  b = !(file >> s).fail();
  std::cout << s << " " << b << std::endl;
  b = !(file >> x).fail();
  std::cout << x << " " << b << std::endl;
  b = !(file >> x).fail();
  std::cout << x << " " << b << std::endl;
  return 0;
}