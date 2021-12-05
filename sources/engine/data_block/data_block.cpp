#include "data_block.h"
#include <fstream>
#include <ostream>
#include <istream>
#include <cctype>
#include "common.h"
#include "3dmath.h"

#define OUT_VEC(vec2, vec3, vec4)\
ostream& operator<<(ostream& os, const vec2& v)\
{\
  return os << v.x << ',' << v.y;\
}\
ostream& operator<<(ostream& os, const vec3& v)\
{\
  return os << v.x << ',' << v.y << ',' << v.z;\
}\
ostream& operator<<(ostream& os, const vec4& v)\
{\
  return os << v.x << ',' << v.y << ',' << v.z << ',' << v.w;\
}
OUT_VEC(vec2, vec3, vec4)
OUT_VEC(ivec2, ivec3, ivec4)
OUT_VEC(uvec2, uvec3, uvec4)

struct Stream
{
  string content;
  size_t current, line;
  Stream(std::ifstream &stream) :
  content(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()),
  current(0), line(1)
  {
    
  }
  int peek(size_t delta = 0) const
  {
    return current+delta < content.size() ? content[current+delta] : EOF;
  }
  void skip_comments()
  {
    if (peek() == '/' && peek(1) == '/')
    {
      while(peek() != EOF && peek() != '\n')
        current++;
      current++;
      line++;
    }
  }
  int get()
  {
    int c = peek();
    if (c == '\n')
      line++;
    current++;
    skip_comments();
    return c;
  }
  const char *data()
  {
    return current < content.size() ? &content[current] : nullptr;
  }
  void seek(int delta)
  {
    current += delta;
  }
};

static bool is_character(char c)
{
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static bool is_digit(char c)
{
  return '0' <= c && c <= '9';
}
static bool is_name_character(char c)
{
  return is_character(c) || is_digit(c) || c == '_';
}


static void skip_spaces(Stream &stream)
{
  for (int c; ; stream.get())
  {
    c = stream.peek();
    if (!std::isspace(c))
      break;
  }
}

static bool read_var(Stream &stream, string &var)
{
  var.clear();
  skip_spaces(stream);
  for (int ci; (ci = stream.peek()) != EOF; stream.get())
  {
    char c = ci;
    if (!is_name_character(c))
      return true;
    var += (char)c;
  }
  return true;
}

static bool read_character(Stream &stream, char character)
{
  skip_spaces(stream);
  if (stream.peek() == character)
  {
    stream.get();
    return true;
  }
  return false;
}



template<int N, typename T>
static bool parse(Stream &stream, vec<N, T, defaultp> &value)
{
  int n = 0, read = 0;
  if constexpr (is_same_v<T, float>)
  {
    if constexpr (N == 2)
      read = sscanf(stream.data(), "%f,%f%n", &value.x, &value.y, &n);
    else if constexpr (N == 3)
      read = sscanf(stream.data(), "%f,%f,%f%n", &value.x, &value.y, &value.z, &n);
    else
      read = sscanf(stream.data(), "%f,%f,%f,%f%n", &value.x, &value.y, &value.z, &value.w, &n);
  }
  else
  {
    if constexpr (N == 2)
      read = sscanf(stream.data(), "%d,%d%n", &value.x, &value.y, &n);
    else if constexpr (N == 3)
      read = sscanf(stream.data(), "%d,%d,%d%n", &value.x, &value.y, &value.z, &n);
    else
      read = sscanf(stream.data(), "%d,%d,%d,%d%n", &value.x, &value.y, &value.z, &value.w, &n);
  }
  stream.seek(n);
  return read == N;
}

static bool parse(Stream &stream, int &value)
{
  int n;
  int read = sscanf(stream.data(), "%d%n", &value, &n);
  stream.seek(n);
  return read != 0;
}
static bool parse(Stream &stream, unsigned &value)
{
  int n;
  int read = sscanf(stream.data(), "%d%n", &value, &n);
  stream.seek(n);
  return read != 0;
}

static bool parse(Stream &stream, float &value)
{
  int n;
  int read = sscanf(stream.data(), "%f%n", &value, &n);
  stream.seek(n);
  return read != 0;
}

static bool parse(Stream &stream, unsigned char &value)
{
  skip_spaces(stream);
  const char *buf = stream.data();
  int n;
  struct BoolVariant
  {
    const char* fmt;
    int size;
    bool val;
  };
  BoolVariant variants[] = {{"true%n", 4, true}, {"yes%n", 3, true}, {"false%n", 5, false}, {"no%n", 2, false}};
  for (int i = 0; i < 4; i++)
  {
    sscanf(buf, variants[i].fmt, &n);
    if (n == variants[i].size)
    {
      stream.seek(n);
      value = variants[i].val;
      return true;
    }
  }
  return false;
}

static bool parse(Stream &stream, string &value)
{
  skip_spaces(stream);
  if (stream.peek() == '"')
  {
    stream.get();
    for (int c; (c = stream.peek()) != EOF; stream.get())
    {
      if (c == '\"')
      {
        stream.get();
        return true;
      }
      if (c == '\\')//special character
      {
        stream.get();
        c = stream.peek();
        constexpr int N = 4;
        pair<char, char> specialCharacters[N] = 
            {{'"', '\"'}, {'n', '\n'}, {'\\', '\\'}, {'\'', '\''}};
        bool found = false;
        for (int i = 0; i < N; i++)
          if (c == specialCharacters[i].first)
          {
            c = specialCharacters[i].second;
            found = true;
            break;
          }
        if (!found)
        {
          debug_error("unexception special character %c in string %s", c, value.c_str());
          continue;
        }
      }
      value += (char)c;
    }
  }
  return false;
}
template<typename T>
static bool try_parse_value(Stream &stream, DataBlock &blk, const char *varName, const char *varType)
{
  T value;
  if (parse(stream, value))
  {
    blk.add<T>(varName, value);/* 
    if constexpr(std::is_same_v<T, unsigned char>)
      cout << varName << ":" << varType << "=" << (bool)value << endl;
    else
      cout << varName << ":" << varType << "=" << value << endl; */
    if (read_character(stream, ';'))
      return true;
    debug_error("expects ';' after %s:%s", varName, varType);
    return false;
  }
  else
  {
    debug_error("can't parse %s:%s=???", varName, varType);
    return false;
  }
}
//must be parrallel with DataBlock::AllowedTypes
static array<const char*, DataBlock::AllowedTypes::size> types_literals
{
  "i", "i2","i3", "i4",
  "u", "u2", "u3", "u4",
  "f", "f2", "f3", "f4",
  "t", "b"
};


GET_FUNCTIONS(parsers, try_parse_value)

static bool add_variable(Stream &stream, DataBlock &block, const string &varName, const string &varType)
{
  for (uint32_t i = 0, n = types_literals.size(); i < n; i++)
    if (strcmp(varType.c_str(), types_literals[i]) == 0)
    {
      return parsers[i](stream, block, varName.c_str(), varType.c_str());
    }
  debug_error("line %d, unknown type %s", stream.line, varType.c_str());
  return false;
}

static bool read_blk(Stream &stream, DataBlock &block);

static bool add_block(Stream &stream, DataBlock &block, const string &varName, const string &varType)
{
  DataBlock *nextBlock = block.addBlock(varName.c_str(), varType.c_str());
  /* if (varType != "")
    printf("added block %s:%s\n", varName.c_str(), varType.c_str());
  else
    printf("added block %s\n", varName.c_str()); */
  return read_blk(stream, *nextBlock);;
}

static bool read_blk(Stream &stream, DataBlock &block)
{
  string varName, varType;
  for (int c; (c = stream.peek()) != EOF; stream.get())
  {
    if (std::isspace(c))
      continue;
    if (c == '}')
    {
      stream.get();
      break;
    }
    varName.clear();
    varType.clear();
    if (read_var(stream, varName))
    {
      if (read_character(stream, ':'))
      {
        if (read_var(stream, varType))
        {
          if (read_character(stream, '='))
          {
            if (!add_variable(stream, block, varName, varType))
              return false;
          }
          else if (read_character(stream, '{'))
          {
            if (!add_block(stream, block, varName, varType))
              return false;
          }
          else
            debug_error("error in %d line, with %s:%s variable", stream.line, varName.c_str(), varType.c_str());
        }
        else
          debug_error("error in %d line, type of %s variable undefined", stream.line, varName.c_str());
      }
      else
      {
        if (read_character(stream, '{'))//start block
        {
          if (!add_block(stream, block, varName, varType))
            return false;
        }
        else
          debug_error("error in %d line, with %s block", stream.line, varName.c_str());
      }
    }
    else
    {
      debug_error("error in line %d", stream.line);
      return false;
    }
  }
  return true;
}

DataBlock::DataBlock(const char *name, const char *type):
  blockName(name?name:""), blockType(type?type:"")
{

}
DataBlock::DataBlock(const std::string &path)
{
  std::ifstream stream(path);
  Stream in_stream(stream);
  read_blk(in_stream, *this);
}
DataBlock::DataBlock(const char *path)
{  
  std::ifstream stream(path);
  Stream in_stream(stream);
  read_blk(in_stream, *this);
}
DataBlock::DataBlock(std::ifstream &stream)
{
  Stream in_stream(stream);
  read_blk(in_stream, *this);
}


const string& DataBlock::name() const
{
  return blockName;
}

const string& DataBlock::type() const
{
  return blockType;
}

size_t DataBlock::blockCount() const
{
  return blocks.size();
}
size_t DataBlock::propertiesCount() const
{
  return properties.size();
}

DataBlock *DataBlock::getBlock(const char *name)
{
  return const_cast<DataBlock *>(static_cast<const DataBlock &>(*this).getBlock(name));
}

const DataBlock *DataBlock::getBlock(const char *name) const
{
  for (const shared_ptr<DataBlock> &block : blocks)
    if (block->name() == name)
      return block.get();
  return nullptr;
}

DataBlock *DataBlock::getBlock(size_t index)
{
  return const_cast<DataBlock *>(static_cast<const DataBlock &>(*this).getBlock(index));
}

const DataBlock *DataBlock::getBlock(size_t index) const
{
  return index < blocks.size() ? blocks[index].get() : nullptr;
}

DataBlock* DataBlock::addBlock(const char *name, const char *type)
{
  return blocks.emplace_back(make_shared<DataBlock>(name, type)).get();
}

const DataBlock::Property &DataBlock::getProperty(size_t index) const
{
  return properties[index];
}