#include <daScript/daScript.h>
#include "das_load.h"

void resolve_systems(const das::ContextPtr &ctx);

das::TextPrinter tout;// output stream for all compiler messages (stdout. for stringstream use TextWriter)


das::das_hash_map<das::string, DasFilePtr> files;

void clear_das_files()
{
  files.clear();
}

std::string root_path(const std::string &path);

DasFilePtr load_das_script(const char *path)
{
  static auto fsAccess = das::make_smart<das::FsFileAccess>(root_path("sources/das/common.das_project"), das::make_smart<das::FsFileAccess>());
  das::ModuleGroup dummyLibGroup;
  auto file = das::make_shared<DasFile>();

  // compile program
  file->program = das::compileDaScript(path, fsAccess, tout, dummyLibGroup);
  if (file->program->failed() ) {
    tout << "failed to compile\n";
    for ( auto & err : file->program->errors ) {
      tout << das::reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
    }
    return nullptr;
  }
  file->ctx = das::make_shared<das::Context>(file->program->getContextStackSize());
  if ( !file->program->simulate(*file->ctx, tout) ) {
    // if interpretation failed, report errors
    tout << "failed to simulate\n";
    for ( auto & err : file->program->errors ) {
      tout << das::reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
    }
    return nullptr;
  }
  files.emplace(path, file);
  resolve_systems(file->ctx);
  return file;
}

uint32_t DasFile::get_function_count() const
{
  return ctx->getTotalFunctions();
}

das::SimFunction *DasFile::get_function(uint32_t i)
{
  return ctx->getFunction(i);
}
das::SimFunction *DasFile::find_function(const char *name) const 
{
  return ctx->findFunction(name);
}

