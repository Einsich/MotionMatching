#pragma once
#include <iomanip>
#include <ostream>
#include <istream>
#include <map>
#include <set>
#include <type_traits>
#include <vector>
#include <fstream>
#include <sstream>
#include "iserializable.h"
#include "reflection.h"
#include <vector>
template<typename T>
struct is_simple_type
{
  static constexpr bool value = std::is_integral_v<T> || std::is_floating_point_v<T>;
};

template<typename T>
void __write_bin(std::ostream& os, const T& value)
{
  os.write(reinterpret_cast<const char*>(&t), sizeof(t));
}
template<typename T>
void __write_sym(std::ostream& os, int depth, const T& value)
{
  os << std::setfill('\t') << std::setw(depth) << value << '\n';
}
template<typename T>
inline void write(std::ostream& os, bool, int, const T&)
{
  #warning "Try to serialize not registred object"
}
template<typename T>
inline std::enable_if_t<is_simple_type<T>::value, void>
write(std::ostream& os, bool bin, int depth, const T& value)
{
  if (bin)
    __write_bin(os, value);
  else
    __write_sym(os, depth, value);
}
template<typename T>
inline std::enable_if_t<std::is_base_of_v<ISerializable, T>, void> 
write(std::ostream& os, bool bin, int depth, const T& value)
{
  static_cast<const ISerializable *>(&value)->serialize(os, bin, depth);
}

template<typename T>
inline void write(std::ostream& os, bool bin, int depth, const std::vector<T>& value) 
{
  write(os, bin, depth, static_cast<std::uint32_t>(value.size()));
  for (const T & t : value)
    write(os, t); 
}
template<typename T, typename U>
inline void write(std::ostream& os, bool bin, int depth, const std::map<T, U>& value) 
{
  write(os, bin, depth, static_cast<std::uint32_t>(value.size()));
  for (const auto & t : value)
    write(os, t.first), write(os, t.second);
}
template<typename T>
inline void write(std::ostream& os, bool bin, int depth, const std::set<T>& value) 
{
  write(os, bin, depth, static_cast<std::uint32_t>(value.size()));
  for (const auto & t : value)
    write(os, t);
}
template<typename T, typename U>
inline void write(std::ostream& os, bool bin, int depth, const std::pair<T, U>& value) 
{
  write(os, value.first), write(os, value.second);
}

template<typename T>
inline std::enable_if_t<HasReflection<T>::value, void> 
write(std::ostream& file, bool bin, int depth, const T& value)
{
  if (bin)
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
  }
    //printf("ls %d\n", fileSize);
  return fileSize + sizeof(fileSize);
}
template<typename T, typename ...Args>
inline size_t write(std::ostream& os, bool bin, int depth, const T &value, const Args &... args)
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
template<typename T>
inline size_t read(std::istream& is, std::vector<T>& value) 
{
  const auto pos = is.tellg();
  value.clear();
  std::uint32_t len = 0;
  is.read(reinterpret_cast<char*>(&len), sizeof(len));

  value.resize(len);
  for (uint i = 0, n = value.size(); i < n; ++i)
  {
    if constexpr (std::is_same_v<bool, T>)
    {
      T v = value[i];
      read(is, v);
      value[i] = v;
    }
    else
      read(is, value[i]);
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
  ofstream file (path, ios::binary);
  size_t fileSize = write(file, object);
  print_file_size(path.string(), fileSize);
  file.close();
  return fileSize;
}
template <typename T>
size_t save_object_path(const T &object, const std::string &path)
{
  ofstream file (path, ios::binary);
  size_t fileSize = write(file, object);
  print_file_size(path, fileSize);
  file.close();
  return fileSize;
}
template <typename T>
size_t save_object(const T &object, const std::string &path)
{
  return save_object_path(object, project_resources_path(path));
}
template <typename T>
size_t load_object_path(T &object, const std::filesystem::path &path)
{
  ifstream file(path, ios::binary);
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
  ifstream file(path, ios::binary);
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
  return load_object_path(object, project_resources_path(path));
}