#include <filesystem>
#include <fstream>
#include <set>
#include <functional>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <application/time.h>
#include "animation_database.h"
#include <data_block/data_block.h>

std::string normalName(const std::string& badName)
{
  return badName.substr(0, badName.find("_$AssimpFbx$_"));
}
namespace fs = filesystem;

void print_tree(const AnimationTreeData &tree, int node_index, int depth)
{
  const AnimationNodeData &node = tree.nodes[node_index];
  debug_log("%s%s", std::string(depth, ' ').c_str(), node.name.c_str());
  for (int child : node.childs)
    print_tree(tree, child, depth + 1);
}
int find_last(const std::string &s, const char *substr)
{
  size_t off = 0;
  size_t pos = s.find(substr, off);
  while(pos < s.size())
  {
    off = pos;
    pos = s.find(substr, off+1);
  } 
  return off == 0 ? pos : off;
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim))
    result.emplace_back(item);
  return result;
}

struct ClipProperty
{
  AnimationTags tags;
  bool loopable;
  std::string nextClip;
};

std::vector<std::pair<std::string, ClipProperty>> read_clips(const DataBlock &clips)
{
  std::vector<std::pair<std::string, ClipProperty>> propertyMap;
  for (size_t i = 0, n = clips.blockCount(); i < n; i++)
  {
    const DataBlock *clip = clips.getBlock(i);
    if (!clip->get<int>("unused", 0))
    {
      ClipProperty prop;
      prop.tags = AnimationTags(split(clip->get<std::string>("tags", ""), '|'));
      prop.nextClip = clip->get<std::string>("next", "");
      prop.loopable = prop.nextClip == "";
      propertyMap.emplace_back(clip->name(), prop);
    }
  }
  return propertyMap;
} 

template<typename T> 
int bisearch(const std::vector<std::pair<float, T>> &a, float t)
{
  int l = 0, r = a.size() - 1, m = (l + r) / 2;
  while (r - l > 1)
  {
    float sampleT = a[m].first;
    if (sampleT <= t)
      l = m;
    else
      r = m;
    m = (l + r) / 2;
  }
  return l;
}
template<typename T> 
void remove_reusing(std::vector<T> & result, uint duration, float max_time, uint n, std::function<std::pair<float,T>(int)>get_next)
{
  if (result.size() == duration)
    return;
  if (n == 1)
  {
    result = {get_next(0).second};
    return;
  }
  if (n == duration)
  {
    result.resize(duration);
    for (uint i = 0; i < n; ++i)
      result[i] = get_next(i).second;
    return;
  }
  std::vector<std::pair<float, T>> bisearchList(n);
  result.resize(duration);
  for (uint i = 0; i < n; ++i)
    bisearchList[i] = get_next(i);

  float dt = max_time / duration;
  for (uint i = 0; i < duration; ++i)
  {
    float t = dt * i;
    int j = bisearch(bisearchList, t);
    float t0 = bisearchList[j].first, t1 = bisearchList[j + 1].first;
    result[i] = glm::mix(bisearchList[j].second, bisearchList[j+1].second, (t - t0) / (t1 - t0));
  }
}

void animation_preprocess(AnimationDataBase &animDatabase)
{
  Assimp::Importer importer;
  animDatabase.clips.clear();
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
  importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);

  TimeScope scope("Animation Reading from fbx file");
  std::string path = root_path("resources/Animations/Root_Motion");
    
  DataBlock clips(project_path("AnimationTags.blk"));
  animDatabase.tagsNames = split(clips.get<std::string>("tags", ""), '|');
  register_tags(animDatabase.tagsNames);

    for (const auto & entry : read_clips(clips))
    {
      const std::string& nextPath = path + "/" + entry.first + ".fbx";

      if (fs::exists(nextPath))
      {
        importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
        importer.ReadFile(nextPath, aiProcess_GlobalScale);
        const aiScene* scene = importer.GetScene();

        for (uint animInd = 0; animInd < scene->mNumAnimations; animInd++)
        {

          const aiAnimation* animation = scene->mAnimations[animInd];
          std::string animName = std::string(animation->mName.C_Str());
          const ClipProperty &property = entry.second;
          float maxTime = animation->mDuration / animation->mTicksPerSecond;
          uint duration = animation->mDuration+1;
          uint ticksPerSecond = animation->mTicksPerSecond;

          std::map<std::string, std::vector<quat>> rotation;
          std::map<std::string, std::vector<vec3>> translation;
          for (uint i = 0; i < animation->mNumChannels; i++)
          {
            const aiNodeAnim * animNode = animation->mChannels[i];
            std::string name = normalName(std::string(animNode->mNodeName.C_Str()));

            std::function<std::pair<float,vec3>(int)> posf = 
              [&](int i){auto p = animNode->mPositionKeys[i]; return std::make_pair((float)p.mTime / animation->mTicksPerSecond, to_vec3(p.mValue));};
            std::function<std::pair<float,quat>(int)> rotf = 
              [&](int i){auto p = animNode->mRotationKeys[i]; return std::make_pair((float)p.mTime / animation->mTicksPerSecond, to_quat(p.mValue));};

            remove_reusing(translation[name], duration, maxTime, animNode->mNumPositionKeys, posf);
            remove_reusing(rotation[name], duration, maxTime, animNode->mNumRotationKeys, rotf);
            
          }
          animDatabase.clips.emplace_back(duration, ticksPerSecond, animName,
              animDatabase.tree, rotation, translation, property.tags, property.loopable, property.nextClip, false);

        }
        std::vector<AnimationClip> &clips = animDatabase.clips;
        for (uint i = 0; i < clips.size(); ++i)
        {
          auto it = std::find_if(clips.begin(), clips.end(), [&](const AnimationClip &clip)->bool{return clips[i].nextClip == clip.name;});
          if (it != clips.end())
            clips[i].nextClipIdx = it - clips.begin();
        }
        
      }
    }


}
