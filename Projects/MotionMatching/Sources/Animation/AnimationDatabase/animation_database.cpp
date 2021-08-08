#include "animation_database.h"
#include <Engine/Resources/resources.h>
#include <Engine/imgui/imgui.h>
#include <ecs/component_editor.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "animation_preprocess.h"

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
void AnimationDataBase::load(const filesystem::path &, bool reload)
{
  if (!reload)
  {
    reload_tree(tree, treeSource.asset_path());
    load_runtime_parameters();
  }
  if (needForceReload)
    animation_preprocess(this);

}
void AnimationDataBase::free()
{

}
bool AnimationDataBase::edit()
{
  bool changeTree = edit_component(treeSource, "tree", false);
  if (changeTree)
    reload_tree(tree, treeSource.asset_path());
  needForceReload = ImGui::Button("force reload");

  return changeTree | needForceReload;
}; 
void AnimationDataBase::save_runtime_parameters() const
{
  save_object(tests, "AnimatuionTests.bin");
}
void AnimationDataBase::load_runtime_parameters()
{
  load_object(tests, "AnimatuionTests.bin");
}

ResourceRegister<AnimationDataBase> animDataBaseReg;