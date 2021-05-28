#pragma once
#include <ostream>
#include <istream>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include "Engine/time.h"
#include "Application/application.h"
#include "reflection.h"
class ISerializable
{
public:
  virtual size_t serialize(std::ostream& os) const = 0;
  virtual size_t deserialize(std::istream& is) = 0;
};


template<typename T>
inline std::enable_if_t<!std::is_base_of_v<ISerializable, T> && !HasReflection<T>::value, size_t>
 write(std::ostream& os, const T& value)
{
  const auto pos = os.tellp();
  os.write(reinterpret_cast<const char*>(&value), sizeof(value));
  return static_cast<std::size_t>(os.tellp() - pos);
}
template<typename T>
inline std::enable_if_t<std::is_base_of_v<ISerializable, T>, size_t> write(std::ostream& os, const T& value)
{
  const ISerializable *serializable = (const ISerializable*)(&value);
  return serializable->serialize(os);
}
template<typename T>
inline std::enable_if_t<HasReflection<T>::value, size_t> write(std::ostream& file, const T& value)
{
  size_t fileSize = 0;
  value.reflect([&](const auto &arg, const char *name){ 
    fileSize += write(file, string(name)); 
    file.seekp((size_t)file.tellp() + sizeof(size_t));
    size_t objSize = write(file, arg);
    file.seekp((size_t)file.tellp() - objSize - sizeof(size_t));
    fileSize += write(file, objSize);
    fileSize += objSize;
    file.seekp((size_t)file.tellp() + objSize);
  });
  return fileSize;
}

inline size_t write(std::ostream& os, const std::string& value) 
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
inline std::enable_if_t<!std::is_base_of_v<ISerializable, T> && !HasReflection<T>::value, size_t>
 read(std::istream& is, T& value)
{
  const auto pos = is.tellg();
  is.read(reinterpret_cast<char*>(&value), sizeof(value));
  if (is.fail())
    return 0;
  return static_cast<size_t>(is.tellg() - pos);
}
template<typename T> 
inline std::enable_if_t<std::is_base_of_v<ISerializable, T>, size_t> read(std::istream& is, T& value)
{
  ISerializable* p = (ISerializable*)(&value);
  return p->deserialize(is);
}
template<typename T> 
inline std::enable_if_t<HasReflection<T>::value, size_t> read(std::istream& file, T& value)
{
  size_t fileSize = 0;
  size_t objSize = 0;
  string buf_name;
  while (read(file, buf_name))
  {
    bool readed = false;
    read(file, objSize);
    value.reflect([&](auto &arg, const char *name)
    { 
      if (name == buf_name && !readed)
      {
        fileSize += read(file, arg); 
        readed = true;
      }
    });
    if (!readed)
      file.seekg((size_t)file.tellg() + objSize);
  }
  return fileSize;
}

inline size_t read(std::istream& is, std::string& value) 
{
  const auto pos = is.tellg();
  //value.clear();
  int len;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));
  if (is.fail())
    return 0;
  value.resize(len);
  
  if (len > 0)
    is.read(value.data(), len);
  if (is.fail())
    return 0;
  return static_cast<size_t>(is.tellg() - pos);
}
template<typename T>
inline size_t read(std::istream& is, std::vector<T>& value) 
{
  const auto pos = is.tellg();
  value.clear();
  int len;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));
  if (is.fail())
    return 0;
  value.resize(len);
  for (T & t : value)
  {
    read(is, t);
    if (is.fail())
      return 0;
  }
  return static_cast<size_t>(is.tellg() - pos);
}
template<typename T, typename U>
inline size_t read(std::istream& is, std::map<T, U>& value) 
{
  const auto pos = is.tellg();
  value.clear();
  int len;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));
  if (is.fail())
    return 0;
  for (int i = 0; i < len; i++)
  {
    T t; U u;
    read(is, t);
    if (is.fail())
      return 0;
    read(is, u);
    if (is.fail())
      return 0;
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
  if (is.fail())
    return 0;
  for (int i = 0; i < len; i++)
  {
    T t;
    read(is, t);
    if (is.fail())
      return 0;
    value.insert(t);
  }
  return static_cast<size_t>(is.tellg() - pos);
}

void print_file_size(const std::string &path, size_t fileSize);

template <typename T>
size_t save_object(const T &object, const std::string &path)
{
  ofstream file (project_resources_path(path), ios::binary);
  size_t fileSize = write(file, object);
  print_file_size(path, fileSize);
  file.close();
  return fileSize;
}

template <typename T>
size_t load_object(T &object, const std::string &path)
{
  ifstream file(project_resources_path(path), ios::binary);
  if (file.fail())
  {
    debug_error("Can not open %s, skip load object", path.c_str());
    return 0;
  }
  size_t fileSize = read(file, object);
  print_file_size(path, fileSize);
  file.close();
  return fileSize;
}