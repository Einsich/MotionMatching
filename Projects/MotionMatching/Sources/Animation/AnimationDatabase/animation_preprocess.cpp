#include <filesystem>
#include <fstream>
#include <set>
#include <functional>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "animation_database.h"

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
void get_tag_from_name(const string &s, set<AnimationTag> &tags)
{

  #define CHECK(SUBSTR, TAG) \
  {size_t t = s.find("_To_"), r = find_last(s, #SUBSTR);if (r < s.length() && (s.length() <= t || t < r)) tags.insert(AnimationTag::TAG);}
  #define FIND(SUBSTR, TAG) \
  {size_t r = s.find(#SUBSTR);if (r < s.length()) tags.insert(AnimationTag::TAG);}

  CHECK(Crouch, Crouch)
  CHECK(CrouchWalk, Crouch)
  CHECK(Stand_Relaxed, Stay)
  return;
  CHECK(_Walk_, Stay)
  CHECK(_Run_, Stay)
  CHECK(_Jog_, Stay)
  CHECK(Jump, Jump)
  CHECK(Conv, Speak)
}
void get_tag(const string &s, set<AnimationTag> &tags)
{
  #define CHECK_TAG(TAG) if (s == #TAG) {tags.insert(AnimationTag::TAG); return;}
  CHECK_TAG(Stay)
  CHECK_TAG(Crouch)
  CHECK_TAG(Jump)
  CHECK_TAG(Idle)
}
struct ClipProperty
{
  set<AnimationTag> tags;
  bool loopable, rotatable;
  string nextClip;
};
map<string, ClipProperty> read_tag_map(const string &path)
{
  map<string, ClipProperty> propertyMap;
  try 
  {
    ifstream tagsFile;
    tagsFile.exceptions(ifstream::badbit);
    tagsFile.open(path);
    vector<pair<string, bool>> words;
      string s;
    
    auto propertyIt = propertyMap.begin();
    while (tagsFile >> s)
    {
      if (s.substr(0, 4) == "MOB1")
      {
        auto emp = propertyMap.try_emplace(s);
        if (emp.second)
          propertyIt = emp.first;
      }
      else
      {
        if (s == "loopable")
          propertyIt->second.loopable = true;
        else
        if (s == "rotatable")
          propertyIt->second.rotatable = true;
        if (s.substr(0, 2) == "->")
          propertyIt->second.nextClip = s.substr(2);
        else
          get_tag(s, propertyIt->second.tags);
      }
    }
  }
  catch(ifstream::failure &e)
  {
    debug_error("Can't open %s", path.c_str());
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

string map_unity_name(const string &name);

void animation_preprocess(AnimationDataBase *animDatabase)
{
  Assimp::Importer importer;
  animDatabase->clips.clear();
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
  importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
  auto tagMap = read_tag_map(project_resources_path("AnimationTags.txt"));

    TimeScope scope("Animation Reading from fbx file");
    bool unityData = false;// !strcmp(get_config("AnimData"), "Unity");
    string path = project_resources_path(unityData ? "retarget_clips" : "MocapOnline/Root_Motion");
    uintmax_t animation_size = 0;
    
    if (false)
    {
      importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
      importer.ReadFile(path + "/WalkMocapSet.fbx", aiProcess_GlobalScale);
      const aiScene* scene = importer.GetScene();

      AnimationTreeData avatar(scene->mRootNode->mChildren[0]);
        //avatar.nodes[i].name = map_unity_name(avatar.nodes[i].name);
      //animDatabase->tree.apply_other_tree(avatar);
    }
    for (const auto & entry : fs::directory_iterator(path))
    {
      
      
      if (entry.is_directory())
      {
        
      } 
      else
      if (entry.is_regular_file())
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

            if (unityData)
              name = map_unity_name(name);
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
              animDatabase->tree, rotation, translation, property.tags, property.loopable, property.nextClip, property.rotatable);

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

  animDatabase->load_runtime_parameters();
   
  //print_tree(animDatabase->tree, 0, 0);
}

map<string, string> name_map
{
{ "AnimUprisingRig", "Root"},
{ "Hips", "Hips"},
{ "Spine", "Spine"},
{ "Chest", "Spine1"},
{ "UpperChest", "Spine2"},
{ "Shoulder.L", "LeftShoulder"},
{ "UpperArm.L", "LeftArm"},
{ "LowerArm.L", "LeftForeArm"},
{ "Hand.L", "LeftHand"},
{ "", "LeftHandIndex1"},
{ "", "LeftHandIndex2"},
{ "", "LeftHandIndex3"},
{ "", "LeftHandIndex4"},
{ "", "LeftHandMiddle1"},
{ "", "LeftHandMiddle2"},
{ "", "LeftHandMiddle3"},
{ "", "LeftHandMiddle4"},
{ "", "LeftHandPinky1"},
{ "", "LeftHandPinky2"},
{ "", "LeftHandPinky3"},
{ "", "LeftHandPinky4"},
{ "", "LeftHandRing1"},
{ "", "LeftHandRing2"},
{ "", "LeftHandRing3"},
{ "", "LeftHandRing4"},
{ "", "LeftHandThumb1"},
{ "", "LeftHandThumb2"},
{ "", "LeftHandThumb3"},
{ "", "LeftHandThumb4"},
{ "Shoulder.R", "RightShoulder"},
{ "UpperArm.R", "RightArm"},
{ "LowerArm.R", "RightForeArm"},
{ "Hand.R", "RightHand"},
{ "", "RightHandIndex1"},
{ "", "RightHandIndex2"},
{ "", "RightHandIndex3"},
{ "", "RightHandIndex4"},
{ "", "RightHandMiddle1"},
{ "", "RightHandMiddle2"},
{ "", "RightHandMiddle3"},
{ "", "RightHandMiddle4"},
{ "", "RightHandPinky1"},
{ "", "RightHandPinky2"},
{ "", "RightHandPinky3"},
{ "", "RightHandPinky4"},
{ "", "RightHandRing1"},
{ "", "RightHandRing2"},
{ "", "RightHandRing3"},
{ "", "RightHandRing4"},
{ "", "RightHandThumb1"},
{ "", "RightHandThumb2"},
{ "", "RightHandThumb3"},
{ "", "RightHandThumb4"},
{ "Neck", "Neck"},
{ "Head", "Head"},
{ "UpperLeg.L", "LeftUpLeg"},
{ "LowerLeg.L", "LeftLeg"},
{ "Foot.L", "LeftFoot"},
{ "Toe.L", "LeftToeBase"},
{ "UpperLeg.R", "RightUpLeg"},
{ "LowerLeg.R", "RightLeg"},
{ "Foot.R", "RightFoot"},
{ "Toe.R", "RightToeBase"},
};
string map_unity_name(const string &name)
{
  string s = name_map[name];
  return s == "" ? name : s;
}