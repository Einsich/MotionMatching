#include "asset.h"

string get_asset_name(const filesystem::path &path)
{
  return path.stem().string();
}