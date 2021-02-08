#include "animation_preprocess.h"
#include <filesystem>
#include "CommonCode/math.h"
#include "../../Serialization/serialization.h"
#include "../../Time/time_scope.h"
#include <assimp/postprocess.h>

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

AnimationDataBasePtr animation_preprocess(Assimp::Importer& importer, aiNode *root)
{
  bool fromFile = true;
  AnimationDataBasePtr animDatabase = make_shared<AnimationDataBase>(root);
  if (fromFile)
  {
    TimeScope scope("Animation Reading from fbx file");
    string path = join_recources_path("MocapOnlineMobilityStarterPack/Animation/Root_Motion");
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
              for (uint j = 0; j < duration; j++)
                vecs.push_back(to_vec3(animNode->mPositionKeys[j + 1].mValue));
            }
            if (animNode->mNumRotationKeys == duration + 1)
            {
              for (uint j = 0; j < duration; j++)
                quats.push_back(to_quat(animNode->mRotationKeys[j + 1].mValue));
            }
          }

          animDatabase->clips.push_back(AnimationClip(duration, animation->mTicksPerSecond, string(animation->mName.C_Str()),
              animDatabase->tree, rotation, translation));
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
  for (const AnimationClip &animation : animDatabase->clips)
    debug_log("In animation %s have %d cadres:", animation.name.c_str(), animation.duration);
  debug_log("Animation at all: %d", animDatabase->clips.size());
  //print_tree(animDatabase->tree, 0, 0);
  return animDatabase;
}

