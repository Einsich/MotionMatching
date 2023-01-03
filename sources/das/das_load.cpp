#include <daScript/daScript.h>
#include "das_load.h"

void resolve_systems(const das::ContextPtr &ctx, DasFile &file);

das::TextPrinter tout;// output stream for all compiler messages (stdout. for stringstream use TextWriter)


das::das_hash_map<das::string, DasFilePtr> files;

void clear_das_files()
{
  files.clear();
}

std::string root_path(const std::string &path);
static das::smart_ptr<das::FsFileAccess> get_file_access()
{
  return das::make_smart<das::FsFileAccess>(
    root_path("sources/das/common.das_project"),
    das::make_smart<das::FsFileAccess>());
}



static bool compile_script(const das::string &file_path, das::ProgramPtr &out_program, das::ContextPtr &out_ctx)
{
  das::ModuleGroup dummyLibGroup;
  // compile program
  auto program = das::compileDaScript(file_path, get_file_access(), tout, dummyLibGroup);
  if (program->failed() ) {
    tout << "failed to compile\n";
    for ( auto & err : program->errors ) {
      tout << das::reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
    }
    return false;
  }
  auto context = das::make_shared<das::Context>(program->getContextStackSize());
  if (!program->simulate(*context, tout) ) {
    // if interpretation failed, report errors
    tout << "failed to simulate\n";
    for ( auto & err : program->errors ) {
      tout << das::reportError(err.at, err.what, err.extra, err.fixme, err.cerr );
    }
    return false;
  }
  out_program = program;
  out_ctx = context;
  return true;
}

static void gather_dependencies(DasFile &file)
{
  das::ModuleGroup dummyLibGroup;

  das::vector<das::ModuleInfo> req;
  das::vector<das::string> missing, circular, notAllowed;
  das::das_set<das::string> dependencies;
  das::getPrerequisits(file.filePath, get_file_access(), req, missing, circular, notAllowed, dependencies, dummyLibGroup, nullptr, 0, false);

  file.dependencies.clear();
  for (const auto &mod : req)
  {
    file.dependencies.emplace(mod.fileName);
    //printf("%s\n", mod.fileName.c_str());
  }
}

DasFilePtr load_das_script(const char *path)
{
  das::ModuleGroup dummyLibGroup;
  auto file = das::make_shared<DasFile>();
  file->filePath = path;

  if (compile_script(file->filePath, file->program, file->ctx))
  {
    gather_dependencies(*file);

    files.emplace(path, file);
    resolve_systems(file->ctx, *file);

  }
  return file;
}

void reload_das_script(const std::string &file_path)
{
  das::Module::ClearSharedModules();
  das::ModuleGroup dummyLibGroup;

  for (auto&[path, file] : files)
  {
    if (path == file_path || file->dependencies.find(file_path) != file->dependencies.end())
    {

      if (compile_script(file->filePath, file->program, file->ctx))
      {
        gather_dependencies(*file);

        resolve_systems(file->ctx, *file);
        printf("succesfuly recompiled %s", file_path.c_str());
      }
    }
  }

}


#include <application/file_watcher.h>

static void track_das_file(const std::string &file, FileWatchStatus status)
{
  switch (status)
  {
  case FileWatchStatus::Added : load_das_script(file.c_str()); break;
  
  case FileWatchStatus::Changed : reload_das_script(file); break;
  case FileWatchStatus::Removed : 
    break;
  }
}


void setup_das_watcher()
{

  setup_track_callback(".das", &track_das_file);
}