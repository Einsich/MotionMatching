#include <fstream>
#include <iostream>
#include <sstream> 
#include <string>
#include <regex>
#include <filesystem>
#include <vector>
#include <stdarg.h>

typedef unsigned int uint;
enum class ArgType
{
  ReadWrite, // T &t, T *t
  ReadOnly, // const T &t, const T *t
  Copy// T t, const T t
};
struct ParserFunctionArgument
{
  std::string type, name;
  bool reference = false;
  bool optional = false;
  ArgType argType; 
};
struct ParserSystemDescription
{
  std::string sys_file, sys_name;
  std::string stage;
  std::string tags;
  std::vector<ParserFunctionArgument> args, req_args, req_not_args;
  std::vector<std::string> before, after;
  std::string isJob;
};
#define SPACE_SYM " \n\t\r\a\f\v"
#define NAME_SYM "a-zA-Z0-9_"
#define SPACE "[" SPACE_SYM "]*"
#define NAME "[" NAME_SYM "]+"
#define ARGS "[" NAME_SYM "&*,:<>\\+\\-" SPACE_SYM "]*"

#define SYSTEM_LEXEMA NAME_SYM "&*,:<>\\]\\[" SPACE_SYM
#define SYSTEM_ANNOTATION "[" SYSTEM_LEXEMA "=;]*"
#define LEXEMA_ANNOTATION "[" SYSTEM_LEXEMA "=]+"
#define NEW_SYSTEM_ARGS "[(][" SYSTEM_LEXEMA "=;]*[)]"
#define VAR_NAME "[a-zA-Z]+[a-zA-Z0-9_]*"
#define VAR_TYPE "[a-zA-Z]+[a-zA-Z0-9_:]*"
#define TYPE_NAME VAR_TYPE SPACE VAR_NAME
#define TYPE_REGEX1 "(" VAR_TYPE SPACE "<" SPACE VAR_TYPE "(" SPACE "," SPACE VAR_TYPE ")*" SPACE ">|" VAR_TYPE ")"
#define TYPE_REGEX2 "(" VAR_TYPE SPACE "<" SPACE TYPE_REGEX1 "(" SPACE "," SPACE TYPE_REGEX1 ")*" SPACE ">|" VAR_TYPE ")" "|" VAR_NAME
#define SYSTEM_ARG  "((" VAR_TYPE SPACE "<" SPACE TYPE_REGEX1 "(" SPACE "," SPACE TYPE_REGEX1 ")*" SPACE ">|" VAR_TYPE ")" SPACE VAR_NAME ")|" VAR_NAME

#define ARGS_L "[(]" ARGS "[)]"
#define ARGS_R "[\\[]" ARGS "[\\]]"
#define SYSTEM "SYSTEM" SPACE "[(]" SYSTEM_ANNOTATION "[)]"
#define QUERY "QUERY" SPACE "[(]" SYSTEM_ANNOTATION "[)]"
#define EVENT "EVENT" SPACE "[(]" SYSTEM_ANNOTATION "[)]"

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

static const std::regex new_system_args_regex(NEW_SYSTEM_ARGS);
static const std::regex new_system_annotation_regex(LEXEMA_ANNOTATION);
static const std::regex new_system_arg_regex(SYSTEM_ARG);
static const std::regex type_regex(TYPE_REGEX2);

enum CallableType
{
  SYSTEM_TYPE,
  EVENT_TYPE,
  QUERY_TYPE
};
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
  arg.reference = ref;

  auto args = get_matches(str, type_regex, 3);

  if (args.size() != 1 && args.size() != 2)
    return arg;
  arg.type = args[0];
  if (args.size() == 2)
    arg.name = args[1];
  return arg;
}
void parse_definition(std::string &str, ParserSystemDescription &parserDescr, CallableType type)
{
  auto args_range = get_match({str.begin(), str.end()}, new_system_args_regex);
  
  //printf("%s\n", str.c_str());
  if (!args_range.empty())
  {
    auto args = get_matches(args_range.str(), new_system_annotation_regex);
    
    for (auto &arg : args)
    {
      auto args0 = get_matches(arg, new_system_arg_regex);

/*       printf("%*c %s\n", 2, ' ', arg.c_str());
      for (auto &arg : args0)
        printf("%*c %s\n", 4, ' ', arg.c_str()); */

      if (args0.empty())
        log_error("bad lexema \"" + arg + "\" in ");
      else if (args0.size() == 1)
        log_error("need at least one value for " + args0[0]);
      else
      {
        const std::string &key = args0[0];
        if (key == "tags")
        {
          for (uint i = 1; i < args0.size(); i++)
            parserDescr.tags += std::string(parserDescr.tags.empty() ? "" : ", ") + '\"' + args0[i] + '\"';;
        } else if (key == "stage")
        {
          parserDescr.stage = '\"' + args0[1] + '\"';
          if (args0.size() > 2)
            log_error("there are more than 1 stage definition");
        } else if (key == "before")
        {
          for (uint i = 1; i < args0.size(); i++)
            parserDescr.before.emplace_back(std::move(args0[i]));
        } else if (key == "after")
        {
          for (uint i = 1; i < args0.size(); i++)
            parserDescr.after.emplace_back(std::move(args0[i]));
        } else if (key == "require")
        {
          for (uint i = 1; i < args0.size(); i++)
            parserDescr.req_args.emplace_back(clear_arg(args0[i]));
        } else if (key == "require_not")
        {
          for (uint i = 1; i < args0.size(); i++)
            parserDescr.req_not_args.emplace_back(clear_arg(args0[i]));
        } else if (key == "job")
        {
          if (args0.size() > 1)
            parserDescr.isJob = std::move(args0[1]);
        }else
        {
          log_error("bad lexema " + arg);
        }
      }
    }
  }
  if (type != CallableType::SYSTEM_TYPE)
  {
    if (!parserDescr.stage.empty())
      log_error("there is stage " + parserDescr.stage + " in " + parserDescr.sys_name);

  }
  else
  {
    if (parserDescr.stage.empty())
    {
      parserDescr.stage = "\"\"";//warning system without stage
      log_error(parserDescr.sys_name + " without stage");
    }
  }
  if (parserDescr.isJob.empty())
    parserDescr.isJob = "false";
}

void parse_system(std::vector<ParserSystemDescription>  &systemsDescriptions,
  const std::string &file, const std::string &file_path,
  const std::regex &full_regex, const std::regex &def_regex, CallableType type)
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
    descr.sys_file = file_path;
    descr.sys_name = name;
    parse_definition(definition, descr, type);
    auto matched_args = get_matches(args, arg_regex);
    for (auto& arg : matched_args)
    {
      descr.args.push_back(clear_arg(arg));
    }
    systemsDescriptions.emplace_back(descr);
  }
}

constexpr int bufferSize = 2000;
static char buffer[bufferSize];

static void fill_arguments(std::ofstream &outFile, const ParserSystemDescription &system, bool event = false)
{
  const auto &args = system.args;
  const auto &req_args = system.req_args;
  const auto &req_not_args = system.req_not_args;
  for (uint i = event ? 1 : 0; i < args.size(); i++)
  {
    auto& arg  = args[i];
    snprintf(buffer, bufferSize,
    "  {ecs::get_type_description<%s>(\"%s\"), %s}%s\n",
    arg.type.c_str(), arg.name.c_str(), arg.optional ? "true" : "false", i + 1 == (uint)args.size()  && req_args.empty() ? "" : ",");
    outFile << buffer;
  }
  for (uint i = 0; i < req_args.size(); i++)
  {
    auto& arg  = req_args[i];
    snprintf(buffer, bufferSize,
    "  {ecs::get_type_description<%s>(\"%s\"), %s}%s\n",
    arg.type.c_str(), arg.name.c_str(), arg.optional ? "true" : "false", i + 1 == (uint)req_args.size()? "" : ",");
    outFile << buffer;
  }
  snprintf(buffer, bufferSize,
  "}, {\n");
  outFile << buffer;
  for (uint i = 0; i < req_not_args.size(); i++)
  {
    auto& arg  = req_not_args[i];
    snprintf(buffer, bufferSize,
    "  {ecs::get_type_description<%s>(\"%s\"), %s}%s\n",
    arg.type.c_str(), arg.name.c_str(), arg.optional ? "true" : "false", i + 1 == (uint)req_not_args.size()? "" : ",");
    outFile << buffer;
  }
}


void fill_array(std::ofstream &outFile, const std::vector<std::string> &args)
{
  outFile << "{";
  for (uint i = 0; i < args.size(); i++)
  {
    auto& arg  = args[i];
    snprintf(buffer, bufferSize,
    "\"%s\"%s",arg.c_str(), i + 1 == (uint)args.size() ? "" : ", ");
    outFile << buffer;
  }
  outFile << "}";
}

void pass_arguments(std::ofstream &outFile, const std::vector<ParserFunctionArgument> &args, bool event = false)
{
  int i0 = event ? 1 : 0;
  for (uint i = i0; i < args.size(); i++)
    {
      auto& arg  = args[i];
      snprintf(buffer, bufferSize,
      "      %sbegin.get_component<%s, %d>()%s\n",
      arg.optional ? " " : "*", arg.type.c_str(), i - i0, i + 1 == (uint)args.size() ? "" : ",");
      outFile << buffer;
    }
}

void template_arguments(std::ofstream &outFile, const std::vector<ParserFunctionArgument> &args, bool event = false)
{
  int i0 = event ? 1 : 0;
  for (uint i = i0; i < args.size(); i++)
    {
      auto& arg  = args[i];
      snprintf(buffer, bufferSize, "%s%s%s",
        arg.type.c_str(), arg.optional ? "*" : arg.reference ? "&" : "", i + 1 == (uint)args.size() ? "" : ", ");
      outFile << buffer;
    }
}
    
void write(std::ofstream &outFile, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, bufferSize, fmt, args);
  va_end(args);
  outFile << buffer;
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
  std::string pathStr = path.string();
  parse_system(systemsDescriptions, str, pathStr, system_full_regex, system_regex, CallableType::SYSTEM_TYPE);
  parse_system(queriesDescriptions, str, pathStr, query_full_regex, query_regex, CallableType::QUERY_TYPE);
  parse_system(singlqueriesDescriptions, str, pathStr, singl_query_full_regex, query_regex, CallableType::QUERY_TYPE);
  parse_system(eventsDescriptions, str, pathStr, event_full_regex, event_regex, CallableType::EVENT_TYPE);

  std::ofstream outFile;
  log_success(pathStr + ".cpp");
  outFile.open(pathStr + ".cpp", std::ios::out);
  outFile << "#include " << path.filename() << "\n";
  outFile << "#include <ecs_perform.h>\n";
  outFile << "//Code-generator production\n\n";

  for (auto& query : queriesDescriptions)
  {
    std::string query_descr = query.sys_name + "_descr";
    write(outFile,
    "ecs::QueryDescription %s(\"%s\", {\n",
    query_descr.c_str(), query.sys_name.c_str());
    
    fill_arguments(outFile, query);
    write(outFile,
    "});\n\n");

    write(outFile,
    "template<typename Callable>\n"
    "void %s(Callable lambda)\n"
    "{\n"
    "  ecs::perform_query<", query.sys_name.c_str());
    template_arguments(outFile, query.args);

    write(outFile,
    ">\n"
    "  (%s, lambda);\n"
    "}\n\n\n",  query_descr.c_str());
  }

  for (auto& query : singlqueriesDescriptions)
  {
    std::string query_descr = query.sys_name + "_descr";
    write(outFile,
    "ecs::QueryDescription %s(\"%s\", {\n",
    query_descr.c_str(), query.sys_name.c_str());

    fill_arguments(outFile, query);

    write(outFile,
    "});\n\n");

    write(outFile,
    "template<typename Callable>\n"
    "void %s(ecs::EntityId eid, Callable lambda)\n"
    "{\n"
    "  ecs::perform_query<", query.sys_name.c_str());

    template_arguments(outFile, query.args);

    write(outFile,
    ">\n"
    "  (%s, eid, lambda);\n"
    "}\n\n\n",  query_descr.c_str());
  }

  
  for (auto& system : systemsDescriptions)
  {
    std::string sys_descr = system.sys_name + "_descr";
    std::string sys_func = system.sys_name + "_func";

    write(outFile,
    "void %s();\n\n"
    "ecs::SystemDescription %s(\"%s\", {\n",
    sys_func.c_str(), sys_descr.c_str(), system.sys_name.c_str());
    
    fill_arguments(outFile, system);
    write(outFile, "},\n");
    fill_array(outFile, system.before);
    write(outFile, ",\n");
    fill_array(outFile, system.after);
    write(outFile,
    ",\n%s, %s, {%s}, %s);\n\n", sys_func.c_str(), system.stage.c_str(), system.tags.c_str(), system.isJob.c_str());

    bool isJob = system.isJob == "true";
    write(outFile,
    "void %s()\n"
    "{\n"
    "  ecs::%s(%s, %s);\n"
    "}\n\n",
    sys_func.c_str(), isJob ? "perform_job_system" : "perform_system",
    sys_descr.c_str(), system.sys_name.c_str());
  }

  for (auto& event : eventsDescriptions)
  {
    std::string event_descr = event.sys_name + "_descr";
    std::string event_handler = event.sys_name + "_handler";
    std::string event_singl_handler = event.sys_name + "_singl_handler";
    std::string event_type = event.args[0].type;
    write(outFile,
    "void %s(const ecs::Event &event);\n"
    "void %s(const ecs::Event &event, ecs::EntityId eid);\n\n"
    "ecs::EventDescription %s(\n"
    "  ecs::get_mutable_event_handlers<%s>(), \"%s\", {\n",
    event_handler.c_str(),
    event_singl_handler.c_str(),
    event_descr.c_str(),
    event_type.c_str(),
    event.sys_name.c_str());

    fill_arguments(outFile, event, true);
    write(outFile, "},\n");
    fill_array(outFile, event.before);
    write(outFile, ",\n");
    fill_array(outFile, event.after);
    write(outFile, 
    ",\n%s, %s, {%s});\n\n", event_handler.c_str(), event_singl_handler.c_str(), event.tags.c_str());

    write(outFile,
    "void %s(const ecs::Event &event)\n"
    "{\n"
    "  ecs::perform_event((const %s&)event, %s, %s);\n"
    "}\n",
    event_handler.c_str(), event_type.c_str(), event_descr.c_str(), event.sys_name.c_str());

    write(outFile,
    "void %s(const ecs::Event &event, ecs::EntityId eid)\n"
    "{\n"
    "  ecs::perform_event((const %s&)event, %s, eid, %s);\n"
    "}\n\n",
    event_singl_handler.c_str(), event_type.c_str(), event_descr.c_str(), event.sys_name.c_str());
  }
  
  outFile << "\n";
  outFile.close();
}

void process_folder(const std::string &path)
{
  if (!fs::exists(path))
  {
    printf("Didn't exist path %s\n", path.c_str());
    return;
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

}
int main(int argc, char** argv)
{
  for (int i = 1; i < argc; i++)
    process_folder(argv[i]);
  printf("Codegen finished work\n\n");
}
