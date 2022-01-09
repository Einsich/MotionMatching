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

  if (needForceReload || !reload)
  {
    acceleration_structs();    
  }
}
bool AnimationDataBase::edit()
{
  bool changeTree = edit_component(treeSource, "tree", false);
  if (changeTree)
    reload_tree(tree, treeSource.asset_path());
  needForceReload = ImGui::Button("force reload");

  return changeTree | needForceReload;
}; 

void AnimationDataBase::acceleration_structs()
{
  vpTrees.clear();
  coverTrees.clear();
  if (ecs::get_singleton<SettingsContainer>().motionMatchingSettings.empty())
    return;
  map<Tag, size_t> tagMap;
  vector<AnimationTags> treeTags;
  for (const AnimationClip &clip : clips)
  {
    auto it = tagMap.find(clip.tags.tags);
    if (it == tagMap.end())
    {
      tagMap.try_emplace(clip.tags.tags, tagMap.size());
      treeTags.emplace_back(clip.tags);
    }
  }
  vector<vector<VPTree::Node>> nodes(tagMap.size());
  vector<vector<CoverTree::Node>> nodes2(tagMap.size());

  for (uint i = 0; i < clips.size(); i++)
  {
    size_t j = tagMap[clips[i].tags.tags];
    for (uint k = 0; k < clips[i].features.size(); k++)
    {
      nodes[j].emplace_back(VPTree::Node{&clips[i].features[k], i, k, 0.f, 0.f});
      nodes2[j].emplace_back(CoverTree::Node{{}, &clips[i].features[k], i, k, 0.f, 0.f});
    }
  }
  vpTrees.reserve(nodes.size());
  auto f = [&](const FrameFeature &a, const FrameFeature &b)
  {
    return get_score(a, b, ecs::get_singleton<SettingsContainer>().motionMatchingSettings[0].second).full_score;
  };
  for (uint i = 0; i < nodes.size(); i++)
  {
    vpTrees.emplace_back(treeTags[i], std::move(nodes[i]), f);
    coverTrees.emplace_back( treeTags[i], std::move(nodes2[i]), f);
  }
}

ResourceRegister<AnimationDataBase> animDataBaseReg;