#include "animation_preprocess.h"
#include <filesystem>
#include "CommonCode/math.h"
#include "../Serialization/serialization.h"
#include "../Time/time_scope.h"
#include <assimp/postprocess.h>

string normalName(const string& badName)
{
  return badName.substr(0, badName.find("_$AssimpFbx$_"));
}
namespace fs = filesystem;

void print_tree(const AnimationTree &tree, int node_index, int depth)
{
  const AnimationNode &node = tree.nodes[node_index];
  debug_log("%s%s",string(depth, ' ').c_str(), node.name.c_str());
  for (int child : node.childs)
    print_tree(tree, child, depth + 1);
}

AnimationPlayerPtr animation_preprocess(Assimp::Importer& importer, GameObjectPtr go, aiNode *root)
{
  bool fromFile = false;
  AnimationPlayerPtr animPlayer = make_shared<AnimationPlayer>();
  animPlayer->gameObject = go;
  if (fromFile)
  {
    TimeScope scope("Animation Reading from fbx file");
    string path = join_recources_path("MocapOnlineMobilityStarterPack/Animation/Root_Motion");
    uintmax_t animation_size = 0;
    AnimationTree& tree = animPlayer->animationTree = AnimationTree(root);
    animPlayer->curTransform.resize(go->get_mesh()->bonesMap.size());
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

          animPlayer->animations.push_back(Animation(duration, tree, rotation, translation));
          Animation & animationData = animPlayer->animations.back();
          animationData.ticksPerSecond = animation->mTicksPerSecond;
          animationData.name = string(animation->mName.C_Str());
        }
        
      }
    }
    animPlayer->build_state_machine();
    size_t t1 = save_object(*animPlayer, "StarterMocapLib.bin");
    int cadr_count = animPlayer->cadr_count();
    debug_log("Bin file use %ld KB instead %ld KB, %ld cadres, %ld bytes on cadr", t1 / 1024, animation_size / 1024, cadr_count, t1 / cadr_count);
  }
  else
  {
    
    size_t t2 = load_object(*animPlayer, "StarterMocapLib.bin");
    int cadr_count = animPlayer->cadr_count();
    debug_log("Bin file use %ld KB, %ld cadres, %ld bytes on cadr", t2 / 1024, cadr_count, t2 / cadr_count);
  }
  for (Animation &animation : animPlayer->animations)
    debug_log("In animation %s have %d cadres:", animation.name.c_str(), animation.duration());
  debug_log("Animation at all: %d", animPlayer->animations.size());
  //print_tree(animPlayer->animationTree, 0, 0);
  return animPlayer;
}

