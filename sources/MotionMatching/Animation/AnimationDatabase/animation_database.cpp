#include "animation_database.h"
#include <resources/resource_registration.h>
#include <imgui.h>
#include <ecs/component_editor.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <application/time.h>

int AnimationDataBase::cadr_count() const
{
  int count = 0;
  for (const AnimationClip & anim: clips)
    count += anim.duration;
  return count;
}
void reload_tree(AnimationTreeData &tree, const filesystem::path &path)
{
  std::string fbxPath = path.parent_path().string() + "/" + path.stem().string();
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

void AnimationDataBase::load(const filesystem::path &, bool reload, AssetStatus &status)
{
  if (!reload)
  {
    reload_tree(tree, treeSource.asset_path());
  }
  if (needForceReload)
    animation_preprocess(*this);
  register_tags(tagsNames);

  if (matchingScore.empty() || needForceReload)
  {
    matchingScore.resize(clips.size());
    for (uint i = 0; i < matchingScore.size(); i++)
      matchingScore[i].resize(clips[i].duration, 0);
  }

  status = AssetStatus::Loaded;
}
bool AnimationDataBase::edit()
{
  bool changeTree = edit_component(treeSource, "tree", false);
  if (changeTree)
    reload_tree(tree, treeSource.asset_path());
  needForceReload = ImGui::Button("force reload");

  for (auto &clip : clips)
  {
    ImGui::Text("%s tags = %s, duration = %d",
    clip.name.c_str(), clip.tags.to_string().c_str(), clip.duration);
  }

  return changeTree | needForceReload;
}; 

void AnimationDataBase::acceleration_structs(bool check_existance)
{
  if (check_existance)
  {
    if (!vpTrees.empty() && !coverTrees.empty() && !kdTrees.empty())
      return;
  }
  
  vpTrees.clear();
  coverTrees.clear();
  kdTrees.clear();
  if (ecs::get_singleton<SettingsContainer>()->motionMatchingSettings.empty())
    return;
  TimeScope scope("Creating acceleration structs");
  std::map<Tag, size_t> tagMap;
  std::vector<AnimationTags> treeTags;
  for (const AnimationClip &clip : clips)
  {
    auto it = tagMap.find(clip.tags.tags);
    if (it == tagMap.end())
    {
      tagMap.try_emplace(clip.tags.tags, tagMap.size());
      treeTags.emplace_back(clip.tags);
    }
  }
  std::vector<std::vector<VPTree::Node>> nodes(tagMap.size());
  std::vector<std::vector<CoverTree::Node>> nodes2(tagMap.size());
  std::vector<std::vector<KdTree::Node>> nodes3(tagMap.size());

  for (uint i = 0; i < clips.size(); i++)
  {
    size_t j = tagMap[clips[i].tags.tags];
    for (uint k = 0; k < clips[i].features.size(); k++)
    {
      nodes[j].emplace_back(VPTree::Node{&clips[i].features[k], i, k, 0.f, 0.f});
      nodes2[j].emplace_back(CoverTree::Node{{}, &clips[i].features[k], i, k, 0.f, 0.f});
      nodes3[j].emplace_back(KdTree::Node{&clips[i].features[k], i, k});
    }
  }
  vpTrees.reserve(nodes.size());
  const auto &settings = ecs::get_singleton<SettingsContainer>()->motionMatchingSettings[0].second;
  auto f = [&](const FrameFeature &a, const FrameFeature &b)
  {
    return get_score(a, b, settings).full_score;
  };
  for (uint i = 0; i < nodes.size(); i++)
  {
    vpTrees.emplace_back(treeTags[i], std::move(nodes[i]), f);
    coverTrees.emplace_back( treeTags[i], std::move(nodes2[i]), f);
    kdTrees.emplace_back(settings, treeTags[i], std::move(nodes3[i]), f);
  }
}

ResourceRegister<AnimationDataBase> animDataBaseReg;