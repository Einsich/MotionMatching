#include <fstream>
#include <iostream>
#include <sstream> 
#include <string>
#include <regex>
#include <filesystem>
#include <vector>
#include "Application/config.h"
#include "ecs/system_order.h"
enum class ArgType
{
  ReadWrite, // T &t, T *t
  ReadOnly, // const T &t, const T *t
  Copy// T t, const T t
};
struct ParserFunctionArgument
{
  std::string type, name;
  bool optional = false;
  ArgType argType; 
};
struct ParserSystemDescription
{
  std::string sys_file, sys_name, order;
  std::vector<ParserFunctionArgument> args;
};
#define SPACE_SYM " \n\t\r\a\f\v"
#define NAME_SYM "a-zA-Z0-9_"
#define SPACE "[" SPACE_SYM "]*"
#define NAME "[" NAME_SYM "]+"
#define ARGS "[" NAME_SYM "&*,:<>" SPACE_SYM "]*"
#define EID_ARGS "[" NAME_SYM "" SPACE_SYM "]+"
#define DEF_ARGS "[" NAME_SYM ",:" SPACE_SYM "]*"
#define ARGS_L "[(]" ARGS "[)]"
#define ARGS_R "[\\[]" ARGS "[\\]]"
#define SYSTEM "SYSTEM" SPACE "[(]" DEF_ARGS "[)]"
#define QUERY "QUERY" SPACE "[(]" DEF_ARGS "[)]"
#define EVENT "EVENT" SPACE "[(]" DEF_ARGS "[)]"

static const std::regex name_regex(NAME);
static const std::regex system_full_regex(SYSTEM SPACE NAME SPACE ARGS_L);
static const std::regex system_regex(SYSTEM);
static const std::regex query_full_regex(QUERY SPACE NAME SPACE "[(]" SPACE ARGS_R SPACE ARGS_L);
static const std::regex singl_query_full_regex(QUERY SPACE NAME SPACE "[(]" SPACE NAME SPACE "[,]" SPACE ARGS_R SPACE ARGS_L);
static const std::regex query_regex(QUERY);
static const std::regex event_full_regex(EVENT SPACE NAME SPACE ARGS_L);
static const std::regex event_regex(EVENT);
static const std::regex args_regex(ARGS_L);
static const std::regex arg_regex("[" NAME_SYM "&*:<>" SPACE_SYM "]+");

namespace fs = std::filesystem;

void log_success(const std::string &meassage)
{
  printf("[Codegen] \033[32m%s\033[39m\n", meassage.c_str());
}
void log_error(const std::string &meassage)
{
  printf("[Codegen] \033[31m%s\033[39m\n", meassage.c_str());
}
std::vector<std::string> get_matches(const std::string& str, const std::regex &reg, int max_matches = 10000000)
{
  std::vector<std::string> v;
  std::sregex_iterator curMatch(str.begin(), str.end(), reg);
  std::sregex_iterator lastMatch;

  for (int i = 0; curMatch != lastMatch && i < max_matches; i++)
  {
    v.emplace_back(curMatch->str());
    curMatch++;
  }
  return v;
}
struct MatchRange
{
  std::string::iterator begin, end;
  bool empty() const
  {
    return begin == end;
  }
  std::string str() const
  {
    return std::string(begin, end);
  }
};
MatchRange get_match(const MatchRange& str, const std::regex &reg)
{
  std::vector<std::string> v;
  std::sregex_iterator curMatch(str.begin, str.end, reg);
  std::sregex_iterator lastMatch;

  MatchRange range = str;
  if (curMatch != lastMatch)
  {
    range.begin += curMatch->position(0);
    range.end = range.begin + curMatch->length(0);
  }
  else
  {
    range.end = range.begin;
  }
  
  return range;
}
bool erase_substr(std::string& str, const std::string& to_erase)
{
  // Search for the substring in string
  size_t pos = str.find(to_erase);
  if (pos != std::string::npos)
  {
    // If found then erase it from string
    str.erase(pos, to_erase.length());
    return true;
  }
  return false;
}
ParserFunctionArgument clear_arg(std::string str)
{
  ParserFunctionArgument arg;
  
  bool optional = erase_substr(str, "*");
  bool ref = erase_substr(str, "&");
  bool const_ = erase_substr(str, "const");
  
  if (optional || ref)
  {
    arg.argType = const_ ? ArgType::Copy : ArgType::ReadWrite;
  }
  else
  {
    arg.argType = ArgType::ReadOnly;
  }
  arg.optional = optional;

  const std::regex arg_regex("[A-Za-z0-9_:<>]+");
  auto args = get_matches(str, arg_regex, 3);

  if (args.size() != 1 && args.size() != 2)
    return arg;
  arg.type = args[0];
  if (args.size() == 2)
    arg.name = args[1];
  return arg;
}
void parse_definition(const std::string &str, ParserSystemDescription &parserDescr)
{
  const std::regex order_regex("ecs::SystemOrder::[A-Za-z0-9_]+");
  auto args = get_matches(str, order_regex, 1);
  parserDescr.order = args.empty() ? "ecs::SystemOrder::NO_ORDER" : args[0] ;

}
void parse_system(std::vector<ParserSystemDescription>  &systemsDescriptions,
  const std::string &file, const std::string &file_path,
  const std::regex &full_regex, const std::regex &def_regex)
{
  
  auto systems = get_matches(file, full_regex);
  for (auto& system : systems)
  {
    auto definition_range = get_match({system.begin(), system.end()}, def_regex);
    auto name_range = get_match({definition_range.end, system.end()}, name_regex);
    auto args_range = get_match({name_range.end, system.end()}, args_regex);
    std::string definition, name, args;
    if (definition_range.empty())
    {
      log_error("System definition problem in " + system);
      return;
    }
    if (name_range.empty())
    {
      log_error("Name problem in " + system);
      return;
    }
    if (args_range.empty())
    {
      log_error("Arguments problem in " + system);
      return;
    }
    definition = definition_range.str();
    name = name_range.str();

    args_range.begin++;
    args_range.end--;
    args = args_range.str();
    ParserSystemDescription descr;
    parse_definition(definition, descr);
    descr.sys_file = file_path;
    descr.sys_name = name;
    auto matched_args = get_matches(args, arg_regex);
    for (auto& arg : matched_args)
    {
      descr.args.push_back(clear_arg(arg));
    }
    systemsDescriptions.emplace_back(descr);
  }
}
void process_inl_file(const fs::path& path)
{
  std::ifstream inFile;
  inFile.open(path); //open the input file

  std::stringstream strStream;
  strStream << inFile.rdbuf(); //read the file
  std::string str = strStream.str(); //str holds the content of the file



  std::vector<ParserSystemDescription>  systemsDescriptions;
  std::vector<ParserSystemDescription>  queriesDescriptions;
  std::vector<ParserSystemDescription>  singlqueriesDescriptions;
  std::vector<ParserSystemDescription>  eventsDescriptions;
  parse_system(systemsDescriptions, str, path.string(), system_full_regex, system_regex);
  parse_system(queriesDescriptions, str, path.string(), query_full_regex, query_regex);
  parse_system(singlqueriesDescriptions, str, path.string(), singl_query_full_regex, query_regex);
  parse_system(eventsDescriptions, str, path.string(), event_full_regex, event_regex);


  std::ofstream outFile;
  log_success(path.string() + ".cpp");
  outFile.open(path.string() + ".cpp", std::ios::out);
  outFile << "#include " << path.filename() << "\n";
  outFile << "//Code-generator production\n\n";

  
  constexpr int bufferSize = 2000;
  char buffer[bufferSize];
  for (auto& query : queriesDescriptions)
  {
    std::string query_descr = query.sys_name + "_descr";
    snprintf(buffer, bufferSize,
      "ecs::QueryDescription %s(\"%s\", {\n",
      query_descr.c_str(), query.sys_name.c_str());
    
    outFile << buffer;
    for (uint i = 0; i < query.args.size(); i++)
    {
      auto& arg  = query.args[i];
      snprintf(buffer, bufferSize,
      "  {ecs::get_type_description<%s>(\"%s\"), %s}%s\n",
      arg.type.c_str(), arg.name.c_str(), arg.optional ? "true" : "false", i + 1 == (uint)query.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize, "});\n\n");
    outFile << buffer;

    snprintf(buffer, bufferSize,
      "template<typename Callable>\n"
      "void %s(Callable lambda)\n"
      "{\n"
      "  for (ecs::QueryIterator begin = %s.begin(), end = %s.end(); begin != end; ++begin)\n"
      "  {\n"
      "    lambda(\n",
      query.sys_name.c_str(), query_descr.c_str(), query_descr.c_str());
    
    outFile << buffer;
    for (uint i = 0; i < query.args.size(); i++)
    {
      auto& arg  = query.args[i];
      snprintf(buffer, bufferSize,
      "      %sbegin.get_component<%s>(%d)%s\n",
      arg.optional ? " " : "*", arg.type.c_str(), i, i + 1 == (uint)query.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize,
        "    );\n"
        "  }\n"
        "}\n\n\n");
    outFile << buffer;
  }

  for (auto& query : singlqueriesDescriptions)
  {
    std::string query_descr = query.sys_name + "_descr";
    snprintf(buffer, bufferSize,
      "ecs::SingleQueryDescription %s(\"%s\", {\n",
      query_descr.c_str(), query.sys_name.c_str());
    
    outFile << buffer;
    for (uint i = 0; i < query.args.size(); i++)
    {
      auto& arg  = query.args[i];
      snprintf(buffer, bufferSize,
      "  {ecs::get_type_description<%s>(\"%s\"), %s}%s\n",
      arg.type.c_str(), arg.name.c_str(), arg.optional ? "true" : "false", i + 1 == (uint)query.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize, "});\n\n");
    outFile << buffer;

    snprintf(buffer, bufferSize,
      "template<typename Callable>\n"
      "void %s(const ecs::EntityId &eid, Callable lambda)\n"
      "{\n"
      "  ecs::QueryIterator begin;\n"
      "  if (ecs::get_iterator(%s, eid, begin))\n"
      "  {\n"
      "    lambda(\n",
      query.sys_name.c_str(), query_descr.c_str());
    
    outFile << buffer;
    for (uint i = 0; i < query.args.size(); i++)
    {
      auto& arg  = query.args[i];
      snprintf(buffer, bufferSize,
      "      %sbegin.get_component<%s>(%d)%s\n",
      arg.optional ? " " : "*", arg.type.c_str(), i, i + 1 == (uint)query.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize,
        "    );\n"
        "  }\n"
        "}\n\n\n");
    outFile << buffer;
  }

  
  for (auto& system : systemsDescriptions)
  {
    std::string sys_descr = system.sys_name + "_descr";
    std::string sys_func = system.sys_name + "_func";

    snprintf(buffer, bufferSize,
      "void %s();\n\n"
      "ecs::SystemDescription %s(\"%s\", {\n",
      sys_func.c_str(), sys_descr.c_str(), system.sys_name.c_str());
    
    outFile << buffer;
    for (uint i = 0; i < system.args.size(); i++)
    {
      auto& arg  = system.args[i];
      snprintf(buffer, bufferSize,
      "  {ecs::get_type_description<%s>(\"%s\"), %s}%s\n",
      arg.type.c_str(), arg.name.c_str(), arg.optional ? "true" : "false", i + 1 == (uint)system.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize, "}, %s, %s);\n\n", sys_func.c_str(), system.order.c_str());
    outFile << buffer;

    snprintf(buffer, bufferSize,
      "void %s()\n"
      "{\n"
      "  for (ecs::QueryIterator begin = %s.begin(), end = %s.end(); begin != end; ++begin)\n"
      "  {\n"
      "    %s(\n",
      sys_func.c_str(), sys_descr.c_str(), sys_descr.c_str(), system.sys_name.c_str());
    
    outFile << buffer;
    for (uint i = 0; i < system.args.size(); i++)
    {
      auto& arg  = system.args[i];
      snprintf(buffer, bufferSize,
      "      %sbegin.get_component<%s>(%d)%s\n",
      arg.optional ? " " : "*", arg.type.c_str(), i, i + 1 == (uint)system.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize,
        "    );\n"
        "  }\n"
        "}\n\n\n");
    outFile << buffer;
  }

  for (auto& event : eventsDescriptions)
  {
    std::string event_descr = event.sys_name + "_descr";
    std::string event_handler = event.sys_name + "_handler";
    std::string event_type = event.args[0].type;
    snprintf(buffer, bufferSize,
      "void %s(const %s &event);\n\n"
      "ecs::EventDescription<%s> %s(\"%s\", {\n",
      event_handler.c_str(), event_type.c_str(), event_type.c_str(), event_descr.c_str(), event.sys_name.c_str());
    
    outFile << buffer;
    for (uint i = 1; i < event.args.size(); i++)
    {
      auto& arg  = event.args[i];
      snprintf(buffer, bufferSize,
      "  {ecs::get_type_description<%s>(\"%s\"), %s}%s\n",
      arg.type.c_str(), arg.name.c_str(), arg.optional ? "true" : "false", i + 1 == (uint)event.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize, "}, %s);\n\n", event_handler.c_str());
    outFile << buffer;

    snprintf(buffer, bufferSize,
      "void %s(const %s &event)\n"
      "{\n"
      "  for (ecs::QueryIterator begin = %s.begin(), end = %s.end(); begin != end; ++begin)\n"
      "  {\n"
      "    %s(\n",
      event_handler.c_str(), event_type.c_str(), event_descr.c_str(), event_descr.c_str(), event.sys_name.c_str());
    
    outFile << buffer;

    snprintf(buffer, bufferSize,
      "      event%s\n", 1 == (uint)event.args.size() ? "" : ",");
    outFile << buffer;

    for (uint i = 1; i < event.args.size(); i++)
    {
      auto& arg  = event.args[i];
      snprintf(buffer, bufferSize,
      "      %sbegin.get_component<%s>(%d)%s\n",
      arg.optional ? " " : "*", arg.type.c_str(), i - 1, i + 1 == (uint)event.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize,
        "    );\n"
        "  }\n"
        "}\n\n\n");
    outFile << buffer;
  }
  
  for (auto& event : eventsDescriptions)
  {
    std::string event_singl_descr = event.sys_name + "_singl_descr";
    std::string event_singl_handler = event.sys_name + "_singl_handler";
    std::string event_type = event.args[0].type;
    snprintf(buffer, bufferSize,
      "void %s(const %s &event, ecs::QueryIterator &begin);\n\n"
      "ecs::SingleEventDescription<%s> %s(\"%s\", {\n",
      event_singl_handler.c_str(), event_type.c_str(), event_type.c_str(), event_singl_descr.c_str(), event.sys_name.c_str());
    
    outFile << buffer;
    for (uint i = 1; i < event.args.size(); i++)
    {
      auto& arg  = event.args[i];
      snprintf(buffer, bufferSize,
      "  {ecs::get_type_description<%s>(\"%s\"), %s}%s\n",
      arg.type.c_str(), arg.name.c_str(), arg.optional ? "true" : "false", i + 1 == (uint)event.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize, "}, %s);\n\n", event_singl_handler.c_str());
    outFile << buffer;

    bool noAgrs = (uint)event.args.size() == 1;
    snprintf(buffer, bufferSize,
      "void %s(const %s &event, ecs::QueryIterator &%s)\n"
      "{\n"
      "  %s(\n",
      event_singl_handler.c_str(), event_type.c_str(), noAgrs ? "" : "begin", event.sys_name.c_str());
    
    outFile << buffer;

    snprintf(buffer, bufferSize,
      "    event%s\n", noAgrs ? "" : ",");
    outFile << buffer;

    for (uint i = 1; i < event.args.size(); i++)
    {
      auto& arg  = event.args[i];
      snprintf(buffer, bufferSize,
      "    %sbegin.get_component<%s>(%d)%s\n",
      arg.optional ? " " : "*", arg.type.c_str(), i - 1, i + 1 == (uint)event.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize,
        "  );\n"
        "}\n\n\n");
    outFile << buffer;
  }
  
  outFile << "\n";
}
int main(int argc, char** argv)
{
  add_configs(argc, (const char**)(argv));

  const char *ecsPath = get_config("ecsPath");
  if (!ecsPath)
  {
    printf("No ecs path, add -ecsPath [path]\n");
    return 0;
  }
  std::string path(ecsPath);
  if (!fs::exists(path))
  {
    return 0;
  }
  for (auto& p : fs::recursive_directory_iterator(path))
  {
      
    if (p.is_regular_file() && p.path().has_extension())
    {
      if (p.path().extension() == ".inl")
      {
        fs::path cpp_file = fs::path(p.path().string() + ".cpp");
        fs::file_time_type last_write;
        bool exist = fs::exists(cpp_file);
        if (exist)
            last_write = fs::last_write_time(cpp_file);
        if (!exist || last_write < p.last_write_time())
            process_inl_file(p.path());
      }
    }
  }
  std::cout << "Codegen finished work\n\n";
  return 0;
}
