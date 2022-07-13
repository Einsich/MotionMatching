#pragma once
#include <daScript/daScript.h>

struct DasFile
{
  das::ModuleGroup dummyLibGroup;
  das::ProgramPtr program;
  das::ContextPtr ctx;

  uint32_t get_function_count() const;
  das::SimFunction *get_function(uint32_t i);
  das::SimFunction *find_function(const char *name);
};
using DasFilePtr = das::shared_ptr<DasFile>;

DasFilePtr load_das_script(const char *path);
void clear_das_files();


template<typename T, typename ...Args>
bool verify(const DasFile &file, das::SimFunction *das_function)
{
  return das_function && das::verifyCall<T, Args...>(das_function->debugInfo, file.dummyLibGroup);
}