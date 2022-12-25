#include <daScript/daScript.h>
#include "das_load.h"

das::TextPrinter tout;// output stream for all compiler messages (stdout. for stringstream use TextWriter)


das::das_hash_map<das::string, DasFilePtr> files;

void clear_das_files()
{
  files.clear();
}

DasFilePtr load_das_script(const char *path)
{
  auto file = das::make_shared<DasFile>();
  static auto fsAccess = das::make_smart<das::FsFileAccess>();
  // compile program
  file->program = das::compileDaScript(path, fsAccess, tout, file->dummyLibGroup);
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
das::SimFunction *DasFile::find_function(const char *name)
{
  return ctx->findFunction(name);
}

