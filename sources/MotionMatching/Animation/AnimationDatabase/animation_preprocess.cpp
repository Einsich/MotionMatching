#include <filesystem>
#include <fstream>
#include <set>
#include <functional>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <application/time.h>
#include "animation_database.h"
#include <data_block/data_block.h>

string normalName(const string& badName)
{
  return badName.substr(0, badName.find("_$AssimpFbx$_"));
}
namespace fs = filesystem;

void print_tree(const AnimationTreeData &tree, int node_index, int depth)
{
  const AnimationNodeData &node = tree.nodes[node_index];
  debug_log("%s%s",string(depth, ' ').c_str(), node.name.c_str());
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

vector<string> split(const string &s, char delim) {
  vector<string> result;
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim))
    result.emplace_back(item);
  return result;
}

struct ClipProperty
{
  AnimationTags tags;
  bool loopable;
  string nextClip;
};
map<string, ClipProperty> read_tag_map(const string &path)
{
  map<string, ClipProperty> propertyMap;
  DataBlock clips(path);
  for (size_t i = 0, n = clips.blockCount(); i < n; i++)
  {
    const DataBlock *clip = clips.getBlock(i);
    if (!clip->get<int>("unused", 0))
    {
      ClipProperty prop;
      prop.tags = AnimationTags(split(clip->get<string>("tags", ""), '|'));
      prop.nextClip = clip->get<string>("next", "");
      prop.loopable = prop.nextClip == "";
      propertyMap.try_emplace(clip->name(), prop);
    }
  }
  return propertyMap;
} 

template<typename T> 
int bisearch(const vector<pair<float, T>> &a, float t)
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
void remove_reusing(vector<T> & result, uint duration, float max_time, uint n, function<pair<float,T>(int)>get_next)
{
  if (result.size() == duration)
    return;
  if (n == 1 || duration + 1 != n)
  {
    result = {get_next(0).second};
    return;
  }
  vector<pair<float, T>> bisearchList(n + 1);
  result.resize(duration);
  for (uint i = 0; i < n; ++i)
    bisearchList[i] = get_next(i);
  bisearchList.back() = make_pair(max_time, bisearchList[n-1].second);

  //debug_log("t0 = %f, tn = %f", bisearchList[0].first, bisearchList[bisearchList.size() - 1].first);
  float dt = max_time / duration;
  for (uint i = 0; i < duration; ++i)
  {
    float t = dt * i;
    int j = bisearch(bisearchList, t);
    float t0 = bisearchList[j].first, t1 = bisearchList[j + 1].first;
    result[i] = glm::mix(bisearchList[j].second, bisearchList[j+1].second, (t - t0) / (t1 - t0));
  }
}

void animation_preprocess(AnimationDataBase *animDatabase)
{
  Assimp::Importer importer;
  animDatabase->clips.clear();
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
  importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
  auto tagMap = read_tag_map(project_path("AnimationTags.blk"));

    TimeScope scope("Animation Reading from fbx file");
    string path = project_path("Root_Motion");
    uintmax_t animation_size = 0;
    
    for (const auto & entry : fs::directory_iterator(path))
    {
      if (entry.is_regular_file() && entry.path().extension() == ".fbx")
      {
        animation_size += entry.file_size();
        const string& nextPath = entry.path().string();
        importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
        importer.ReadFile(nextPath, aiProcess_GlobalScale);
        const aiScene* scene = importer.GetScene();

        for (uint animInd = 0; animInd < scene->mNumAnimations; animInd++)
        {

          const aiAnimation* animation = scene->mAnimations[animInd];
          float maxTime = animation->mDuration / animation->mTicksPerSecond;
          uint duration = (uint)(maxTime * 30.f);
           //   debug_log("dur = %f", animation->mDuration);

          map<string, vector<quat>> rotation;
          map<string, vector<vec3>> translation;
          for (uint i = 0; i < animation->mNumChannels; i++)
          {
            const aiNodeAnim * animNode = animation->mChannels[i];
            string name = normalName(string(animNode->mNodeName.C_Str()));

            function<pair<float,vec3>(int)> posf = 
              [&](int i){auto p = animNode->mPositionKeys[i]; return make_pair((float)p.mTime / animation->mTicksPerSecond, to_vec3(p.mValue));};
            function<pair<float,quat>(int)> rotf = 
              [&](int i){auto p = animNode->mRotationKeys[i]; return make_pair((float)p.mTime / animation->mTicksPerSecond, to_quat(p.mValue));};

            remove_reusing(translation[name], duration, maxTime, animNode->mNumPositionKeys, posf);
            remove_reusing(rotation[name], duration, maxTime, animNode->mNumRotationKeys, rotf);
            
          }
          string animName = string(animation->mName.C_Str());
          const ClipProperty &property = tagMap[animName];
          animDatabase->clips.emplace_back(duration, 30, animName,
              animDatabase->tree, rotation, translation, property.tags, property.loopable, property.nextClip, false);

        }
        vector<AnimationClip> &clips = animDatabase->clips;
        for (uint i = 0; i < clips.size(); ++i)
        {
          auto it = std::find_if(clips.begin(), clips.end(), [&](const AnimationClip &clip)->bool{return clips[i].nextClip == clip.name;});
          if (it != clips.end())
            clips[i].nextClipIdx = it - clips.begin();
        }
        
      }
    }

  //for (const AnimationClip &animation : animDatabase->clips)
  //  debug_log("In animation %s have %d cadres:", animation.name.c_str(), animation.duration);
  //debug_log("Animation at all: %d", animDatabase->clips.size());

   
  //print_tree(animDatabase->tree, 0, 0);
}
