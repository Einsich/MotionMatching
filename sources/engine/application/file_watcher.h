#pragma once
#include <vector>
#include <string>

enum class FileWatchStatus
{
  Added, Changed, Removed
};

void setup_directory_watch(const std::vector<std::string> &folders);

using FileTrackCallback = void (*)(const std::string &file, FileWatchStatus status);

void setup_track_callback(const std::vector<std::string> &extensions, FileTrackCallback callback);
void setup_track_callback(const std::string &extension, FileTrackCallback callback);

void update_watching();

