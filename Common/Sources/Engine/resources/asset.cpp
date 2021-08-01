#include "asset.h"

std::string get_asset_name(const std::filesystem::path &path)
{
  return path.stem().string();
}