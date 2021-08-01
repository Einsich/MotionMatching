#include "common.h"
#include "fbx_importer.h"
#include "resources.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void FBXMeta::create_assets()
{
  const auto &assets = Resources::instance().assets;
  auto it = assets.find("Mesh");
  if (it != assets.end())
  {
    for (const string &str : meshMetaData)
    { 
      debug_log("create mesh %s", str.c_str());
      it->second.createAsset(str);
    }
  }
}

void FBXMeta::load(const filesystem::path &path, bool reload)
{
  if (reload || loaded)
    return;
  Assimp::Importer importer;
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
  importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
  string fbxPath = path.parent_path().string() + "\\" + path.stem().string();
  importer.ReadFile(fbxPath, aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_LimitBoneWeights |
    aiPostProcessSteps::aiProcess_GenNormals | aiProcess_GlobalScale | aiProcess_FlipWindingOrder);

  const aiScene* scene = importer.GetScene();

  for (uint i = 0; i < scene->mNumMeshes; ++i)
  {
    meshMetaData.emplace_back(fbxPath + "\\" + to_string(i) + "\\" + scene->mMeshes[i]->mName.C_Str());
    debug_log("find mesh %s", meshMetaData.back().c_str());
  }
  importer.FreeScene();
  create_assets();
  loaded = true;
}
void FBXMeta::free()
{

}
bool FBXMeta::edit()
{
  return false;
}

ResourceRegister<FBXMeta> fbxImrorter({".fbx"});