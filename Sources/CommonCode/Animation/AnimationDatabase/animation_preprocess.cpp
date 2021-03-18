#include "animation_preprocess.h"
#include <filesystem>
#include <fstream>
#include <set>
#include "CommonCode/math.h"
#include "../../Serialization/serialization.h"
#include "../../Time/time_scope.h"
#include <assimp/postprocess.h>
#include "CommonCode/Application/config.h"

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
void get_tag_from_name(const string &s, set<AnimationTag> &tags)
{

  #define CHECK(SUBSTR, TAG) \
  {size_t t = s.find("_To_"), r = s.find(#SUBSTR);if (r < s.length() && (s.length() <= t || t < r)) tags.insert(AnimationTag::TAG);}

  CHECK(Crouch, Crouch)
  CHECK(CrouchWalk, Crouch)
  CHECK(Stand_Relaxed, Stay)
  CHECK(_Walk_, Stay)
  CHECK(_Run_, Stay)
  CHECK(_Jog_, Stay)
  CHECK(Jump, Jump)
  CHECK(Death, Die)
  CHECK(Conv, Speak)
}
void get_tag(const string &s, set<AnimationTag> &tags)
{
  #define CHECK_TAG(TAG) if (s == #TAG) {tags.insert(AnimationTag::TAG); return;}
  CHECK_TAG(Stay)
  CHECK_TAG(Crouch)
  CHECK_TAG(Jump)
  CHECK_TAG(Die)
  CHECK_TAG(Speak)
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
AnimationDataBasePtr animation_preprocess(Assimp::Importer& importer, aiNode *root)
{
  AnimationDataBasePtr animDatabase = make_shared<AnimationDataBase>(root);
  auto tagMap = read_tag_map(join_recources_path("AnimationTags.txt"));
  if (get_bool_config("loadDataBaseFromFBX"))
  {
    TimeScope scope("Animation Reading from fbx file");
    string path = join_recources_path("MocapOnline/Root_Motion");
    uintmax_t animation_size = 0;
    for (const auto & entry : fs::directory_iterator(path))
    {
      
      
      if (entry.is_directory())
      {
        
      } 
      else
      if (entry.is_regular_file())
      {
        animation_size += entry.file_size();
        const string& nextPath = entry.path();
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
                vecs = {vec3(0.f)};
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
                quats = {quat(1,0,0,0)};
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
  pose_matching_norma_weights(animDatabase->featureWeights);
  
  //print_tree(animDatabase->tree, 0, 0);
  return animDatabase;
}

