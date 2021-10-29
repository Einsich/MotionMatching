#include "fbx_importer.h"
#include "resource_registration.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Engine/Render/mesh.h"

void FBXMeta::create_assets()
{

  for (const string &str : meshMetaData)
  { 
    Asset<Mesh> mesh(str);
    debug_log("find mesh %s | %s", mesh.asset->name.c_str(), mesh.asset->path.string().c_str());
  }
}

void FBXMeta::after_load()
{
  create_assets();
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
  filesystem::path resourceRoot = project_resources_path();
  for (uint i = 0; i < scene->mNumMeshes; ++i)
  {
    filesystem::path path_ = (fbxPath + "\\" + to_string(i) + "\\" + scene->mMeshes[i]->mName.C_Str());
    meshMetaData.emplace_back(path_.lexically_relative(resourceRoot).string());
  }
  importer.FreeScene();
  create_assets();
  loaded = true;
}
ResourceRegister<FBXMeta> fbxImrorter({".fbx"});