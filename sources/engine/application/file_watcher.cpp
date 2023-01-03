
#include <FileWatcher/FileWatcher.h>
#include <iostream>
#include "file_watcher.h"
#include <eastl/map.h>
#include <eastl/vector.h>
#include "application.h"


class FileWatcher final : public FW::FileWatchListener
{
  struct QueuedChange
  {
    std::string path;
    FileTrackCallback callback;
    FileWatchStatus status;
  };
  eastl::vector<QueuedChange> changes;
  
  void try_add_change(std::string &&path, FileTrackCallback callback, FileWatchStatus status)
  {
    for (const auto &elem : changes)
    {
      if (elem.path == path)
        return;
    }
    changes.emplace_back(QueuedChange{std::move(path), callback, status});
  }

public:
  FW::FileWatcher fileWatcher;
  eastl::map<std::string, FileTrackCallback> callbacks;
  int frame = 0;
  int updateFrequency = 20;


	void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename,
		FW::Action action) override
	{
    auto dotPos = filename.find(".");
    if (dotPos < filename.size())
    {
      const char *ext = filename.c_str() + dotPos;
      auto it = callbacks.find_as(ext, eastl::less_2<std::string, const char*>());
      if (it != callbacks.end())
      {
        FileWatchStatus status = FileWatchStatus::Added;
        if (action == FW::Action::Modified)
          status = FileWatchStatus::Changed;
        else if (action == FW::Action::Delete)
          status = FileWatchStatus::Removed;
        std::string path = dir + "/" + filename; 
        std::replace(path.begin(), path.end(), '\\', '/');
        try_add_change(std::move(path), it->second, status);
        
      }
    }
	}

  void updateGatheredChanges()
  {
    for (const auto &elem : changes)
    {
      elem.callback(elem.path, elem.status);
		    std::cout << elem.path << " status " << (int)elem.status  <<std::endl;
    }
    changes.clear();
  }

} watcher;


void setup_directory_watch(const std::vector<std::string> &folders)
{
  // add a watch to the system
  // the file watcher doesn't manage the pointer to the listener - so make sure you don't just
  // allocate a listener here and expect the file watcher to manage it - there will be a leak!
  for (const auto &str : folders)
	{

    watcher.fileWatcher.addWatch(root_path(str), &watcher, true);
  }
}

void update_watching()
{
  watcher.fileWatcher.update();
  if (watcher.frame++ == watcher.updateFrequency)
  {
    watcher.updateGatheredChanges();
    watcher.frame = 0;
  }
}


void setup_track_callback(const std::vector<std::string> &extensions, FileTrackCallback callback)
{
  for (const auto &ext : extensions)
  {
    watcher.callbacks[ext] = callback;
  }
}

void setup_track_callback(const std::string &extension, FileTrackCallback callback)
{
  watcher.callbacks[extension] = callback;
}