#include <fstream>
#include <iostream>
#include <sstream> 
#include <string>
#include <regex>
#include <filesystem>
#include <vector>
#include "Application/config.h"

struct ParserFunctionArgument
{
  std::string type, name;
  bool optional = false;
};
struct ParserSystemDescription
{
  std::string sys_file, sys_name;
  std::vector<ParserFunctionArgument> args;
};

namespace fs = std::filesystem;
std::vector<std::string> get_matches(const std::string& str, std::regex reg, int max_matches = 10000000)
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
  arg.optional = erase_substr(str, "*");
  erase_substr(str, "&");
  erase_substr(str, "const");

  const std::regex arg_regex("[A-Za-z0-9_:]+");
  auto args = get_matches(str, arg_regex, 3);

  if (args.size() != 1 && args.size() != 2)
    return arg;
  arg.type = args[0];
  if (args.size() == 2)
    arg.name = args[1];
  return arg;
}

void process_inl_file(const fs::path& path)
{
  std::ifstream inFile;
  inFile.open(path); //open the input file

  std::stringstream strStream;
  strStream << inFile.rdbuf(); //read the file
  std::string str = strStream.str(); //str holds the content of the file


  static const std::regex sys_regex("void[ \n\t\r\a\f\v]*[a-zA-Z0-9_]+_system[ \n\t\r\a\f\v]*[(][a-zA-Z0-9_&*,: \n\t\r\a\f\v]+[)]");
  static const std::regex sys_regex1("void[ \n\t\r\a\f\v]*[a-zA-Z0-9_]+_system[ \n\t\r\a\f\v]*");
  static const std::regex sys_name_regex("[a-zA-Z0-9_]+_system");
  static const std::regex args_regex("[(][a-zA-Z0-9_&*,: \n\t\r\a\f\v]+[)]");
  static const std::regex arg_regex("[a-zA-Z0-9_&*: \n\t\r\a\f\v]+");

  std::vector<ParserSystemDescription>  systemsDescriptions;
  auto systems = get_matches(str, sys_regex);
  for (auto& system : systems)
  {
    auto args = get_matches(system, args_regex);
    auto sys_name = get_matches(system, sys_name_regex);
    if (args.size() != 1 || sys_name.size() != 1)
        return;
    args[0] = args[0].substr(1, args[0].size() - 2);

    ParserSystemDescription descr;
    descr.sys_file = path.string();
    descr.sys_name = sys_name[0];
    args = get_matches(args[0], arg_regex);
    for (auto& arg : args)
    {
      descr.args.push_back(clear_arg(arg));
    }
    systemsDescriptions.emplace_back(descr);
  }
  std::ofstream outFile;
  std::cout << "[Codegen] \033[32m" << path.string() + ".cpp"<< "\033[39m" << std::endl;
  outFile.open(path.string() + ".cpp", std::ios::out);
  outFile << "#include " << path.filename() << "\n";
  outFile << "#include \"ecs/ecs_core.h\"\n\n";
  outFile << "//Code-generator production\n\n";

  
  constexpr int bufferSize = 2000;
  char buffer[bufferSize];

  for (auto& system : systemsDescriptions)
  {
    std::string sys_descr = system.sys_name + "_descr";
    std::string sys_func = system.sys_name + "_func";

    snprintf(buffer, bufferSize,
      "void %s();\n\n"
      "ecs::SystemDescription %s({\n",
      sys_func.c_str(), sys_descr.c_str());
    
    outFile << buffer;
    for (uint i = 0; i < system.args.size(); i++)
    {
      auto& arg  = system.args[i];
      snprintf(buffer, bufferSize,
      "  {ecs::get_type_description<%s>(\"%s\"), %s}%s\n",
      arg.type.c_str(), arg.name.c_str(), arg.optional ? "true" : "false", i + 1 == (uint)system.args.size() ? "" : ",");
      outFile << buffer;
    }
    snprintf(buffer, bufferSize, "}, %s);\n\n", sys_func.c_str());
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
  
  outFile << "\n";
  std::cout << "Codegen finished work\n\n";
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
  for (auto& p : fs::recursive_directory_iterator(path))
  {
      
    if (p.is_regular_file() && p.path().has_extension())
    {
      if (p.path().extension() == ".inl")
      {
        fs::path cpp_file = fs::path(p.path().string() + ".cpp");
        fs::file_time_type last_write;
        if (fs::exists(cpp_file))
            last_write = fs::last_write_time(cpp_file);
        if (last_write < p.last_write_time())
            process_inl_file(p.path());
        }
    }
  }
  return 0;
}
