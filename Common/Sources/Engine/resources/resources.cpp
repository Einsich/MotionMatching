#include "resources.h"
#include "Application/application.h"

namespace fs = filesystem;

void create_all_resources_from_metadata(const string &path)
{
  const auto &assets = Resources::instance().assets;
  for (const auto &entry : fs::recursive_directory_iterator(path))
  {
    if (entry.is_regular_file())
    {
      const fs::path &path = entry.path();
      if (path.extension() == ".meta")
      {
        debug_log("meta file %s", path.stem().string().c_str());
        string strPath = path.string();
        fs::path resPath = strPath.substr(0, strPath.size() - 5);
        if (!fs::exists(resPath))
        {
          fs::remove(path);
          debug_log("meta file %s deleted", path.stem().string().c_str());
          continue;
        }
      }
      else
      {
        string ext = path.extension().string();
        const string_view &extView = Resources::instance().extToAssetName[ext];
        auto it = assets.find(extView);
        if (it != assets.end())
        {
          const fs::path &pathToAsset = it->second.metaDataAsset ? path.string() + ".meta" : path;
          debug_log("creating asset %s", pathToAsset.string().c_str());
          it->second.createAsset(pathToAsset);
        }
      }
    }

  }
}

void create_all_resources_from_metadata()
{

  create_all_resources_from_metadata(Application::instance().commonResourcesPath);
  create_all_resources_from_metadata(Application::instance().projectResourcesPath);
}

void save_all_resources_to_metadata()
{
  auto & assets = Resources::instance().assets;
  for (const auto &p : assets)
  {
    debug_log("saving assets with %s extension", p.first.data());
    for (const auto &asset : p.second.resources)
    {
      debug_log("saving asset %s", asset.first.c_str());
      p.second.saveAsset(asset.second);
    }
  }
}