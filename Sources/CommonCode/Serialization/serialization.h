#pragma once
#include <ostream>
#include <istream>
#include <map>
#include <set>
#include <vector>
class ISerializable
{
protected:
  ~ISerializable() = default;

public:
  virtual size_t serialize(std::ostream& os) const = 0;
  virtual size_t deserialize(std::istream& is) = 0;
};

size_t save_object(const ISerializable &object, const std::string &path);
size_t load_object(ISerializable &object, const std::string &path);

template<typename T>
inline std::enable_if_t<!std::is_same_v<std::string, T>, size_t> write(std::ostream& os, const T& value)
{
  if (std::is_base_of_v<ISerializable, T>)
  {
    const ISerializable* p = (const ISerializable*)(&value);
  
    return p->serialize(os);
  }
  else
  {
    const auto pos = os.tellp();
    os.write(reinterpret_cast<const char*>(&value), sizeof(value));
    return static_cast<std::size_t>(os.tellp() - pos);
  }
}
template<typename T>
inline std::enable_if_t<std::is_same_v<std::string, T>, size_t> write(std::ostream& os, const T& value) 
{
  const auto pos = os.tellp();
  const auto len = static_cast<std::uint32_t>(value.size());
  os.write(reinterpret_cast<const char*>(&len), sizeof(len));
  if (len > 0)
      os.write(value.data(), len);
  return static_cast<std::size_t>(os.tellp() - pos);
}
template<typename T>
inline size_t write(std::ostream& os, const std::vector<T>& value) 
{
  const auto pos = os.tellp();
  const auto len = static_cast<std::uint32_t>(value.size());
  os.write(reinterpret_cast<const char*>(&len), sizeof(len));
  for (const T & t : value)
    write(os, t);
  return static_cast<std::size_t>(os.tellp() - pos);
}
template<typename T, typename U>
inline size_t write(std::ostream& os, const std::map<T, U>& value) 
{
  const auto pos = os.tellp();
  const auto len = static_cast<std::uint32_t>(value.size());
  os.write(reinterpret_cast<const char*>(&len), sizeof(len));
  for (const auto & t : value)
    write(os, t.first), write(os, t.second);
  return static_cast<std::size_t>(os.tellp() - pos);
}
template<typename T>
inline size_t write(std::ostream& os, const std::set<T>& value) 
{
  const auto pos = os.tellp();
  const auto len = static_cast<std::uint32_t>(value.size());
  os.write(reinterpret_cast<const char*>(&len), sizeof(len));
  for (const auto & t : value)
    write(os, t);
  return static_cast<std::size_t>(os.tellp() - pos);
}
template<typename T>
inline std::enable_if_t<!std::is_same_v<std::string, T>, size_t> read(std::istream& is, T& value)
{
   if (std::is_base_of_v<ISerializable, T>)
  {
    ISerializable* p = (ISerializable*)(&value);
    return p->deserialize(is);
  }
  else
  {
    const auto pos = is.tellg();
    is.read(reinterpret_cast<char*>(&value), sizeof(value));
    return static_cast<size_t>(is.tellg() - pos);
  }
}
template<typename T>
inline std::enable_if_t<std::is_same_v<std::string, T>, size_t> read(std::istream& is, T& value) 
{
  const auto pos = is.tellg();
  value.clear();
  int len;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));
  value.resize(len);
  
  if (len > 0)
    is.read(value.data(), len);
  return static_cast<size_t>(is.tellg() - pos);
}
template<typename T>
inline size_t read(std::istream& is, std::vector<T>& value) 
{
  const auto pos = is.tellg();
  value.clear();
  int len;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));
  value.resize(len);
  for (T & t : value)
    read(is, t);
  return static_cast<size_t>(is.tellg() - pos);
}
template<typename T, typename U>
inline size_t read(std::istream& is, std::map<T, U>& value) 
{
  const auto pos = is.tellg();
  value.clear();
  int len;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));
  for (int i = 0; i < len; i++)
  {
    T t; U u;
    read(is, t);
    read(is, u);
    value.insert({t, u});
  }
  return static_cast<size_t>(is.tellg() - pos);
}
template<typename T>
inline size_t read(std::istream& is, std::set<T>& value) 
{
  const auto pos = is.tellg();
  value.clear();
  int len;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));
  for (int i = 0; i < len; i++)
  {
    T t;
    read(is, t);
    value.insert(t);
  }
  return static_cast<size_t>(is.tellg() - pos);
}