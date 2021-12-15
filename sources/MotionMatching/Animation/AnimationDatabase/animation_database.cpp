#include "animation_database.h"
#include <resources/resource_registration.h>
#include <imgui.h>
#include <component_editor.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


int AnimationDataBase::cadr_count() const
{
  int count = 0;
  for (const AnimationClip & anim: clips)
    count += anim.duration;
  return count;
}
void reload_tree(AnimationTreeData &tree, const filesystem::path &path)
{
  string fbxPath = path.parent_path().string() + "/" + path.stem().string();
  Assimp::Importer importer;
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
  importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
  
  importer.ReadFile(fbxPath, aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_LimitBoneWeights |
    aiPostProcessSteps::aiProcess_GenNormals | aiProcess_GlobalScale | aiProcess_FlipWindingOrder);

  const aiScene* scene = importer.GetScene();
  if (scene && scene->mRootNode && scene->mRootNode->mChildren[0])
    tree = AnimationTreeData(scene->mRootNode->mChildren[0]);
}

void animation_preprocess(AnimationDataBase &data_base);

void AnimationDataBase::load(const filesystem::path &, bool reload)
{
  if (!reload)
  {
    reload_tree(tree, treeSource.asset_path());
  }
  if (needForceReload)
    animation_preprocess(*this);

}
bool AnimationDataBase::edit()
{
  bool changeTree = edit_component(treeSource, "tree", false);
  if (changeTree)
    reload_tree(tree, treeSource.asset_path());
  needForceReload = ImGui::Button("force reload");

  return changeTree | needForceReload;
}; 


ResourceRegister<AnimationDataBase> animDataBaseReg;