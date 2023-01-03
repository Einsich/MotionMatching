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
  das::string filePath = path;
  static auto fsAccess = das::make_smart<das::FsFileAccess>(root_path("sources/das/common.das_project"), das::make_smart<das::FsFileAccess>());
  das::ModuleGroup dummyLibGroup;
  auto file = das::make_shared<DasFile>();

  // compile program
  file->program = das::compileDaScript(filePath, fsAccess, tout, dummyLibGroup);
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
  das::vector<das::ModuleInfo> req;
  das::vector<das::string> missing, circular, notAllowed;
  das::das_set<das::string> dependencies;
  das::getPrerequisits(filePath, fsAccess, req, missing, circular, notAllowed, dependencies, dummyLibGroup, nullptr, 0, false);

  for (const auto &mod : req)
  {
    file->dependencies.emplace(mod.fileName);
  }

  files.emplace(path, file);
  resolve_systems(file->ctx);
  return file;
}


#include <application/file_watcher.h>

static void track_das_file(const std::string &file, FileWatchStatus status)
{
  switch (status)
  {
  case FileWatchStatus::Added : 
    break;
  
  case FileWatchStatus::Changed : 
    break;
  case FileWatchStatus::Removed : 
    break;
  }
}


void setup_das_watcher()
{
  setup_track_callback(".das", &track_das_file);
}