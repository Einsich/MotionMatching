#include "resources.h"
#include "Application/application_data.h"
#include "../Render/mesh.h"

namespace fs = filesystem;

void create_all_resources_from_metadata(const fs::path &path)
{
  auto &assets = Resources::instance().assets;
  for (const auto &entry : fs::recursive_directory_iterator(path, fs::directory_options::follow_directory_symlink))
  {
    if (entry.is_regular_file())
    {
      const fs::path &path = entry.path();
      if (path.extension() == ".meta")
      {
        //debug_log("meta file %s", path.stem().string().c_str());
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
          //debug_log("creating asset %s", pathToAsset.string().c_str());
          it->second.createAsset(pathToAsset);
        }
        else
        {
          if (ext == ".scene")
          {
            add_open_scene(path);
          }
        }
      }
    }
  }

  for (auto &p : assets)
  {
    auto &res = p.second.resources;
    for (auto it = res.begin(), end = res.end(); it != end;)
    {
      (it->second) ? ++it : res.erase(it++);
    }
  }
}

void create_all_resources_from_metadata()
{
  create_all_resources_from_metadata(Application::instance().projectResourcesPath);
  
  sphere_mesh(1, true);
  sphere_mesh(5, true);
  sphere_mesh(20, true);
  cube_mesh(true);
  plane_mesh(true);
}

void save_all_resources_to_metadata()
{
  auto & assets = Resources::instance().assets;
  for (const auto &p : assets)
  {
    bool firstlog = true;
    for (const auto &asset : p.second.resources)
    {
      if (asset.second && asset.second.edited())
      {
        if (firstlog)
          debug_log("saving assets with %s extension", p.first.data());
        firstlog = false;
        debug_log("saving asset %s", asset.first.c_str());
        p.second.saveAsset(asset.second);
      }
      else
      {
        //debug_log("asset %s wasn't edit (don't save)", asset.first.c_str());
      }
    }
  }
}