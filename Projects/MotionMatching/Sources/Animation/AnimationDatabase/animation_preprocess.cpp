#include "animation_preprocess.h"
#include <filesystem>
#include <fstream>
#include <set>
#include "3dmath.h"
#include "Serialization/serialization.h"
#include "Engine/time.h"
#include <assimp/postprocess.h>
#include "Application/application.h"
#include "config.h"

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
  FIND(Loop, Loopable)
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
  CHECK_TAG(Loopable)
}
map<string, set<AnimationTag>> read_tag_map(const string &path)
{
  map<string, set<AnimationTag>> tagMap;
  try 
  {
    ifstream tagsFile;
    tagsFile.exceptions(ifstream::badbit);
    tagsFile.open(path);
    vector<pair<string, bool>> words;
    while (!tagsFile.eof())
    {
      string s;
      tagsFile >> s;
      words.push_back({s, s.substr(0, 4) == "MOB1"});
    }
    for (uint i = 0; i < words.size();)
    {
      if (words[i].second)
      {
        set<AnimationTag> &tags = tagMap[words[i].first];
        get_tag_from_name(words[i].first, tags);
        i++;
        for (uint j = i; j < words.size() && !words[j].second; j++, i++)
        {
          get_tag(words[j].first, tags);
        }
      }
    }
  }
  catch(ifstream::failure &e)
  {
    debug_error("Can't open %s", path.c_str());
  }
  return tagMap;
} 


string map_unity_name(const string &name);
AnimationDataBasePtr animation_preprocess(Assimp::Importer& importer, aiNode *model)
{
  AnimationDataBasePtr animDatabase = make_shared<AnimationDataBase>(model);
  auto tagMap = read_tag_map(project_resources_path("AnimationTags.txt"));
  if (get_bool_config("loadDataBaseFromFBX"))
  {
    TimeScope scope("Animation Reading from fbx file");
    bool unityData = !strcmp(get_config("AnimData"), "Unity");
    string path = project_resources_path(unityData ? "UnityMocap/Root_Motion" : "MocapOnline/Root_Motion");
    uintmax_t animation_size = 0;
    
    if (unityData)
    {
      importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
      importer.ReadFile(path + "/WalkingMocapSet.fbx", aiProcess_GlobalScale);
      const aiScene* scene = importer.GetScene();

      AnimationTreeData avatar(scene->mRootNode->mChildren[0]);
      for (uint i = 0 ; i < avatar.nodes.size(); i++)
        avatar.nodes[i].name = map_unity_name(avatar.nodes[i].name);
      animDatabase->tree.apply_other_tree(avatar);
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
          uint duration = (uint)animation->mDuration;

          map<string, vector<quat>> rotation;
          map<string, vector<vec3>> translation;
          for (uint i = 0; i < animation->mNumChannels; i++)
          {
            const aiNodeAnim * animNode = animation->mChannels[i];
            string name = normalName(string(animNode->mNodeName.C_Str()));

            if (unityData)
              name = map_unity_name(name);
            auto &quats = rotation[name];
            auto &vecs = translation[name];
            if (animNode->mNumPositionKeys == duration + 1)
            {
              vecs.resize(duration);
              for (uint j = 0; j < duration; j++)
                vecs[j] = to_vec3(animNode->mPositionKeys[j + 1].mValue);
            }
            else
            {
              if (vecs.size() != duration)
              {
                if (vecs.size() == 0)
                vecs = {animNode->mNumPositionKeys == 1 ? to_vec3(animNode->mPositionKeys[0].mValue):vec3(0.f)};
              }
            }
            
            if (animNode->mNumRotationKeys == duration + 1)
            {
              quats.resize(duration);
              for (uint j = 0; j < duration; j++)
                quats[j] = to_quat(animNode->mRotationKeys[j + 1].mValue);
            }
            else
            {
              if (quats.size() != duration)
              {
                if (quats.size() == 0)
                quats = {animNode->mNumRotationKeys == 1 ? to_quat(animNode->mRotationKeys[0].mValue):quat(1,0,0,0)};
              }
            }
          }
          string animName = string(animation->mName.C_Str());
          animDatabase->clips.emplace_back(duration, animation->mTicksPerSecond, animName,
              animDatabase->tree, rotation, translation, tagMap[animName]);

        }
        
      }
    }
    size_t t1 = save_object(*animDatabase, "StarterMocapLib.bin");
    int cadr_count = animDatabase->cadr_count();
    debug_log("Bin file use %ld KB instead %ld KB, %ld cadres, %ld bytes on cadr", t1 / 1024, animation_size / 1024, cadr_count, t1 / cadr_count);
  }
  else
  {
    
    size_t t2 = load_object(*animDatabase, "StarterMocapLib.bin");
    int cadr_count = animDatabase->cadr_count();
    debug_log("Bin file use %ld KB, %ld cadres, %ld bytes on cadr", t2 / 1024, cadr_count, t2 / cadr_count);
  }
  //for (const AnimationClip &animation : animDatabase->clips)
  //  debug_log("In animation %s have %d cadres:", animation.name.c_str(), animation.duration);
  //debug_log("Animation at all: %d", animDatabase->clips.size());

  animDatabase->load_runtime_parameters();
  
  //print_tree(animDatabase->tree, 0, 0);
  return animDatabase;
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