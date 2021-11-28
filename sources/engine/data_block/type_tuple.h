#pragma once
#include <vector>
#include <array>

template<typename ...Args>
struct TypeTuple
{
private:
  template<typename T, typename H, typename ... B>
  static constexpr size_t getTypeIndexInTemplateList()
  {
    if constexpr (std::is_same<T, H>::value)
      return 0;
    else
      return 1 + getTypeIndexInTemplateList<T, B...>();
  }
public:
  template<typename T>
  static constexpr size_t getIndexOfType() {  return getTypeIndexInTemplateList<T, Args...>(); }
  template<int N> using TypeOf =
  typename std::tuple_element<N, std::tuple<Args...>>::type;
  static constexpr size_t size = sizeof...(Args);
};