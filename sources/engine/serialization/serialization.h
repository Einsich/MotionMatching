#pragma once
#include <ostream>
#include <istream>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "application/application.h"
#include "reflection.h"
#include "iserializable.h"

template< class T >
struct is_pair : std::false_type {};

template< class T1 , class T2 >
struct is_pair< std::pair< T1 , T2 > > : std::true_type {};

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

inline size_t write(std::ostream& os, const std::string& value) 
{
  const auto pos = os.tellp();
  const auto len = static_cast<std::uint32_t>(value.size());
  os.write(reinterpret_cast<const char*>(&len), sizeof(len));
  if (len > 0)
      os.write(value.data(), len);
  return static_cast<std::size_t>(os.tellp() - pos);
}
inline size_t write(std::ostream& os, const std::filesystem::path& value) 
{
  return write(os, value.string());
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
template<typename T, typename U>
inline size_t write(std::ostream& os, const std::pair<T, U>& value) 
{
  const auto pos = os.tellp();
  write(os, value.first), write(os, value.second);
  return static_cast<std::size_t>(os.tellp() - pos);
}

template<typename T>
inline std::enable_if_t<HasReflection<T>::value, size_t> write(std::ostream& file, const T& value)
{
  uint32_t fileSize = 0;
  file.seekp((size_t)file.tellp() + sizeof(std::uint32_t));
  //auto beg = file.tellp();
  value.reflect([&](const auto &arg, const char *name){ 
    fileSize += write(file, string(name)); 
    auto p = file.tellp();
    file.seekp((size_t)file.tellp() + sizeof(std::uint32_t));

    std::uint32_t objSize = write(file, arg);
    p = file.tellp();
    file.seekp((size_t)file.tellp() - (int)objSize - sizeof(std::uint32_t));
    fileSize += write(file, objSize);
    fileSize += objSize;
    p = file.tellp();
    file.seekp((size_t)file.tellp() + objSize);
    //printf("ln %s %d\n", name, (int)(file.tellp()-beg));
  });

  file.seekp((size_t)file.tellp() - (int)fileSize - sizeof(fileSize));
  write(file, fileSize);
  file.seekp((size_t)file.tellp() + fileSize);

    //printf("ls %d\n", fileSize);
  return fileSize + sizeof(fileSize);
}
template<typename T, typename ...Args>
inline size_t write(std::ostream& os, const T &value, const Args &... args)
{
  return write(os, value) + write(os, args...);
}


template<typename T> 
inline std::enable_if_t<!std::is_base_of_v<ISerializable, T> && !HasReflection<T>::value, size_t>
 read(std::istream& is, T& value)
{
  const auto pos = is.tellg();
  is.read(reinterpret_cast<char*>(&value), sizeof(value));
  return static_cast<size_t>(is.tellg() - pos);
}
template<typename T> 
inline std::enable_if_t<std::is_base_of_v<ISerializable, T>, size_t> read(std::istream& is, T& value)
{
  ISerializable* p = (ISerializable*)(&value);
  return p->deserialize(is);
}


inline size_t read(std::istream& is, std::string& value) 
{
  const auto pos = is.tellg();

  std::uint32_t len = 0;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));

  value.resize(len);
  if (len > 0)
    is.read(value.data(), len);
  return static_cast<size_t>(is.tellg() - pos);
}
inline size_t read(std::istream& is, std::filesystem::path& value) 
{
  std::string s;
  size_t p = read(is, s);
  value = std::filesystem::path(s);
  return p;
}
template<typename T>
inline size_t read(std::istream& is, std::vector<T>& value) 
{
  const auto pos = is.tellg();
  value.clear();
  std::uint32_t len = 0;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));

  value.resize(len);
  if constexpr (std::is_base_of_v<ISerializable, T> ||
      HasReflection<T>::value ||
      std::is_same_v<std::filesystem::path, T> ||
      std::is_same_v<std::string, T> ||
      is_pair<T>::value ||
      std::is_same_v<bool, T>)
  {
    for (uint i = 0, n = value.size(); i < n; ++i)
    {
      if constexpr (std::is_same_v<bool, T>)
      {
        bool v = value[i];
        read(is, v);
        value[i] = v;
      }
      else
        read(is, value[i]);
    }
  }
  else
  {
    is.read(reinterpret_cast<char*>(value.data()), sizeof(T) * len);
  }
  
  return static_cast<size_t>(is.tellg() - pos);
}
template<typename T, typename U>
inline size_t read(std::istream& is, std::map<T, U>& value) 
{
  const auto pos = is.tellg();
  value.clear();
  std::uint32_t len = 0;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));

  for (std::uint32_t i = 0; i < len; i++)
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
  std::uint32_t len = 0;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));

  for (std::uint32_t i = 0; i < len; i++)
  {
    T t;
    read(is, t);
    value.insert(t);
  }
  return static_cast<size_t>(is.tellg() - pos);
}
template<typename T, typename U>
inline size_t read(std::istream& is, std::pair<T, U>& value) 
{
  const auto pos = is.tellg();
  read(is, value.first);
  read(is, value.second);
  return static_cast<size_t>(is.tellg() - pos);
}

template<typename T> 
inline std::enable_if_t<HasReflection<T>::value, size_t> read(std::istream& file, T& value)
{
  uint32_t curObjSize = 0;

  read(file, curObjSize);
    //printf("ls %d\n", curObjSize);

  auto beginObj = file.tellg();

  size_t fileSize = 0;
  std::uint32_t objSize = 0;
  string buf_name="";
  //auto beg = file.tellg();
  while (file.peek() != EOF && file.tellg() - beginObj < curObjSize && read(file, buf_name))
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
    //printf("ln %s %d\n", buf_name.c_str(), (int)(file.tellg()-beg));
  //std::fflush(stdout);
  }
  return fileSize;
}
template<typename T, typename ...Args>
inline size_t read(std::istream& is, T &value, Args &... args)
{
  return read(is, value) + read(is, args...);
}

void print_file_size(const std::string &path, size_t fileSize);

template <typename T>
size_t save_object_path(const T &object, const std::filesystem::path &path)
{
  std::ofstream file(path, ios::binary);
  size_t fileSize = write(file, object);
  print_file_size(path.string(), fileSize);
  file.close();
  return fileSize;
}
template <typename T>
size_t save_object_path(const T &object, const std::string &path)
{
  std::ofstream file (path, ios::binary);
  size_t fileSize = write(file, object);
  print_file_size(path, fileSize);
  file.close();
  return fileSize;
}
template <typename T>
size_t save_object(const T &object, const std::string &path)
{
  return save_object_path(object, project_path(path));
}
template <typename T>
size_t load_object_path(T &object, const std::filesystem::path &path)
{
  std::ifstream file(path, ios::binary);
  if (file.fail())
  {
    debug_error("Can not open %s, skip load object", path.c_str());
    return 0;
  }
  size_t fileSize = read(file, object);
  print_file_size(path.string(), fileSize);
  file.close();
  return fileSize;
}
template <typename T>
size_t load_object_path(T &object, const std::string &path)
{
  std::ifstream file(path, ios::binary);
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
template <typename T>
size_t load_object(T &object, const std::string &path)
{
  return load_object_path(object, project_path(path));
}