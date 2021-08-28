#include "common.h"

#include "shader.h"
#include <filesystem>
#include <map>
#include <vector>
#include <fstream>
#include <regex>
bool compile_shader(const string &shaderName, const vector<pair<uint, const char*>> &shaders, uint &program);

namespace fs = filesystem;
#define SPACE_SYM " \n\t\r\a\f\v"
#define NAME_SYM "a-zA-Z0-9_"
#define SPACE_REGEX "[" SPACE_SYM "]+"
#define NAME_REGEX "[" NAME_SYM "]+"
#define ARGS_REGEX "[" SPACE_SYM "," NAME_SYM "]*"
static const regex shader_regex("#shader" SPACE_REGEX NAME_REGEX);
static const regex vs_regex("#vertex_shader");
static const regex ps_regex("#pixel_shader");
static const regex include_regex("#include" SPACE_REGEX NAME_REGEX);
static const regex include_with_defines_regex("#include" SPACE_REGEX NAME_REGEX SPACE_REGEX "(" ARGS_REGEX ")");//todo

enum class ShaderLexema
{
  SHADER_NAME,
  VS_SHADER,
  PS_SHADER,
  INCLUDE
};
struct MatchRange
{
  std::string::iterator begin, end;
  int beginIndex, endIndex;
  ShaderLexema type;
  string typeContent;
  bool empty() const
  {
    return begin == end;
  }
  std::string str() const
  {
    return std::string(begin, end);
  }
};

void get_matches(std::vector<MatchRange> & v, std::string& str, const std::regex &reg, ShaderLexema type)
{
  std::sregex_iterator curMatch(str.begin(), str.end(), reg);
  std::sregex_iterator lastMatch;
  string::iterator begin = str.begin();
  for (; curMatch != lastMatch; ++curMatch)
  {
    int beginInd = curMatch->position();
    int endInd = curMatch->position() + curMatch->length();
    v.emplace_back(MatchRange{begin + beginInd, begin + endInd, beginInd, endInd, type, ""});
  }
}
std::vector<MatchRange> get_matches(std::string& str, const std::regex &reg, ShaderLexema type)
{
  std::vector<MatchRange> v;
  get_matches(v, str, reg, type);
  return v;
}
struct CodeGenShader
{
  bool loaded, shaderFile;
  fs::path path;
  string content;
  vector<MatchRange> lexems;
};

void try_load_shader_code(CodeGenShader &shader)
{
  if (shader.loaded)
    return;
  ifstream shaderFile(shader.path);
  stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();  
  shaderFile.close();
  shader.content = shaderStream.str();
  shader.loaded = true;

  shader.lexems.clear();
  if (shader.shaderFile)
  {
    get_matches(shader.lexems, shader.content, shader_regex, ShaderLexema::SHADER_NAME);
    get_matches(shader.lexems, shader.content, vs_regex, ShaderLexema::VS_SHADER);
    get_matches(shader.lexems, shader.content, ps_regex, ShaderLexema::PS_SHADER);
  }
  get_matches(shader.lexems, shader.content, include_regex, ShaderLexema::INCLUDE);
  std::sort(shader.lexems.begin(), shader.lexems.end(), 
    [](const MatchRange &a, const MatchRange &b)->bool{ return a.beginIndex < b.beginIndex;});
  
  for (MatchRange &r : shader.lexems)
  {
    char buf[255];
    switch (r.type)
    {
    case ShaderLexema::SHADER_NAME: sscanf(r.begin._Ptr, "#shader %s", buf); r.typeContent.assign(buf); break;
    case ShaderLexema::INCLUDE : sscanf(r.begin._Ptr, "#include %s", buf); r.typeContent.assign(buf); break;
    default: break;
    }
  }
}
static map<string, CodeGenShader> shaders;
void add_shader_path(const fs::path &path, bool shader_file)
{
  string fileName = path.stem().string();
  if (shaders.contains(fileName))
  {
    debug_error("Already exists shader %s", fileName.c_str());
    return;
  }
  shaders.try_emplace(fileName, CodeGenShader{false, shader_file, path, "", {}});
}
CodeGenShader *get_codegen_shader(const string &name)
{
  auto it = shaders.find(name);
  if (it != shaders.end())
  {
    try_load_shader_code(it->second);
    return &it->second;
  }
  else 
    return nullptr;
}

void insert_includes(string &text, const string &source_text, const std::vector<MatchRange> &lexems, int &curLexema)
{
  if (lexems.empty())
  {
    text.insert(text.end(), source_text.begin(), source_text.end());
    return;
  }
  bool notInclude = lexems[curLexema].type != ShaderLexema::INCLUDE;
  if (notInclude)
  {
    if (curLexema + 1 < (int)lexems.size())
      text.insert(text.end(), lexems[curLexema].end, lexems[curLexema + 1].begin);
    else
      text.insert(text.end(), source_text.begin() + lexems[curLexema].endIndex, source_text.end());
    ++curLexema;
  }
  else
  {
    if (curLexema == 0)
      text.insert(text.end(), source_text.begin(), source_text.begin() + lexems[0].beginIndex);
    else
      text.insert(text.end(), lexems[curLexema - 1].end, lexems[curLexema].begin);
  }
  for (int i = curLexema; i < (int)lexems.size() && lexems[i].type == ShaderLexema::INCLUDE; ++i)
  {
    CodeGenShader *shader = get_codegen_shader(lexems[i].typeContent);
    if (shader)
    {
      int tmpLexemCounter = 0;
      debug_log("include %s file", lexems[i].typeContent.c_str());
      insert_includes(text, shader->content, shader->lexems, tmpLexemCounter);
      if (tmpLexemCounter != (int)shader->lexems.size())
        debug_error("in include file %s there is not only include read %d / %d lexems", 
        shader->path.stem().string().c_str(), tmpLexemCounter, shader->lexems.size());
    }
    else
    {
      debug_error("#include %s", lexems[i].typeContent.c_str());
    }
    if (i + 1 < (int)lexems.size())
      text.insert(text.end(), lexems[i].end, lexems[i + 1].begin);
    curLexema = i + 1;
  }
  if (curLexema == (int)lexems.size())
  {
    text.insert(text.end(), source_text.begin() + lexems[curLexema - 1].endIndex, source_text.end());
  }
}

struct ParseState
{
  string commonPart, currentShader;
  string vsPart, psPart;
  bool startShader, startPs, startVs;
  void clear()
  {
    commonPart.clear(); vsPart.clear(); psPart.clear(); currentShader.clear();
    startShader = startPs = startVs = false;

  }
};
void create_shader_from_parsed_state(const fs::path &path, ParseState &state)
{
  state.vsPart.insert(state.vsPart.begin(), state.commonPart.begin(), state.commonPart.end());
  state.vsPart.insert(0, "#version 440\n#define VS 1\n");

  state.psPart.insert(state.psPart.begin(), state.commonPart.begin(), state.commonPart.end());
  state.psPart.insert(0, "#version 440\n#define PS 1\n");
  
  vector<pair<uint, const char*>> shaderCode{
    {GL_VERTEX_SHADER, state.vsPart.c_str()},
    {GL_FRAGMENT_SHADER, state.psPart.c_str()}
  };
  uint program;
  if (compile_shader(state.currentShader, shaderCode, program))
  {
    Shader(state.currentShader, program);
    debug_log("shader %s compiled", state.currentShader.c_str());
  }
  else
  {
    {
      ofstream file(fs::path(path).concat("." + state.currentShader + ".ps"));
      file << state.psPart;
    }
    {
      ofstream file(fs::path(path).concat("." + state.currentShader + ".vs"));
      file << state.vsPart;
    }
  }
}
void process_codegen_shaders()
{
  ParseState state;
  for (auto &[shaderName, shader] : shaders)
  {
    if (shader.shaderFile)
    {
      state.clear();
      try_load_shader_code(shader);

      for (int i = 0; i < (int)shader.lexems.size();)
      {
        const MatchRange &r = shader.lexems[i];
        switch (r.type)
        {
        
        case ShaderLexema::SHADER_NAME:
          if (state.startShader)
          {
            create_shader_from_parsed_state(shader.path, state);
            state.clear();
          }
          state.startShader = true;
          state.currentShader = r.typeContent;
          
          //debug_log("shader %s file", r.typeContent.c_str());
          insert_includes(state.commonPart, shader.content, shader.lexems, i);
        break;

        case ShaderLexema::VS_SHADER:
          state.startVs = true;
          //debug_log("vs shader");
          insert_includes(state.vsPart, shader.content, shader.lexems, i);
        break;
        case ShaderLexema::PS_SHADER:
          state.startPs = true;
          //debug_log("ps shader");
          insert_includes(state.psPart, shader.content, shader.lexems, i);
        break;
        default:
          break;
        }
      }
      if (state.startShader)
        create_shader_from_parsed_state(shader.path, state);
      
    }
  }
}
/*
#shader shader_name
***
Common code
***
#vertex_shader
***
VS code
***
#pixel_shader
***
VS code
***

*/