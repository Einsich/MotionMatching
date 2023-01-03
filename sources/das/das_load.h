#pragma once
#include <daScript/daScript.h>

struct DasFile
{
  das::ProgramPtr program;
  das::ContextPtr ctx;
  das::das_set<das::string> dependencies;//module names

};
using DasFilePtr = das::shared_ptr<DasFile>;

DasFilePtr load_das_script(const char *path);
void clear_das_files();
void setup_das_watcher();
