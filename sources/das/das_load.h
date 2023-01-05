#pragma once
#include <daScript/daScript.h>
#include <ecs/query_description.h>

struct DasFile
{
  das::string filePath;
  das::ProgramPtr program;
  das::ContextPtr ctx;
  das::das_set<das::string> dependencies;//module names

  das::vector<ecs::QueryHandle> resolvedQueries;
  das::vector<ecs::SystemHandle> resolvedSystems;
  das::vector<ecs::EventHandle> resolvedEvents;
  das::vector<ecs::RequestHandle> resolvedRequests;

  void clearResolvedSystems()
  {
    resolvedQueries.clear();
    resolvedSystems.clear();
    resolvedEvents.clear();
    resolvedRequests.clear();
  }

};
using DasFilePtr = das::shared_ptr<DasFile>;

DasFilePtr load_das_script(const char *path);
void clear_das_files();
void setup_das_watcher();
