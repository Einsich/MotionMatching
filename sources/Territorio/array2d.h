#pragma once
#include <vector>

template<typename T>
class array2d
{
private:
  uint32_t h, w, n;
  std::vector<T> content;
public:
  array2d() = default;
  array2d(uint32_t h, uint32_t w):
  h(h), w(w), n(h*w), content(n)
  {

  }

  uint32_t heigth() const
  {
    return h;
  }

  uint32_t width() const
  {
    return w;
  }
  //[heigth, width];
  std::pair<uint32_t, uint32_t> dimension() const
  {
    return {h, w};
  }
  bool test(int32_t i, int32_t j) const
  {
    return 0 <= i && i < (int32_t)h && 0 <= j && j < (int32_t)w;
  }
  const T* data() const
  {
    return content.data();
  }

  T* data()
  {
    return content.data();
  }

  struct _proxy_index
  {
    T * data;
    uint32_t w;
    _proxy_index(T *data, uint32_t w):
      data(data), w(w) {}
    T& operator[](uint32_t j)
    {
      assert(j < w);
      return data[j];
    }  
  };
  struct _proxy_index_const
  {
    const T * data;
    uint32_t w;
    _proxy_index_const(const T *data, uint32_t w):
      data(data), w(w) {}
    const T& operator[](uint32_t j) const
    {
      assert(j < w);
      return data[j];
    }  
  };

  _proxy_index operator[](uint32_t i)
  {
    assert(i < h);
    return _proxy_index(content.data() + i * w, w);
  }  
  _proxy_index_const operator[](uint32_t i) const
  {
    assert(i < h);
    return _proxy_index_const(content.data() + i * w, w);
  }
};