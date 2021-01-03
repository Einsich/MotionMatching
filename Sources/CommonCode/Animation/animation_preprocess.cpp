#include "animation_preprocess.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "CommonCode/math.h"
#include "CommonCode/Time/time_scope.h"

string normalName(const string& badName)
{
  return badName.substr(0, badName.find("_$AssimpFbx$_"));
}
namespace fs = filesystem;



AnimationPlayerPtr animation_preprocess(Assimp::Importer& importer, GameObjectPtr go, aiNode *root)
{
  bool fromFile = false;
  AnimationPlayerPtr animPlayer = make_shared<AnimationPlayer>();
  animPlayer->gameObject = go;
  if (fromFile)
  {
    TimeScope scope("Animation Reading from fbx file");
    string path = join_recources_path("MocapOnlineMobilityStarterPack/Animation/Root_Motion");
    
    animPlayer->animationTree = AnimationTree(root);
    animPlayer->curTransform.resize(go->get_mesh()->bonesMap.size());
    for (const auto & entry : fs::directory_iterator(path))
    {
      
      
      if (entry.is_directory())
      {
        
      } 
      else
      if (entry.is_regular_file())
      {
        const string& nextPath = entry.path();
        importer.ReadFile(nextPath, 0);
        const aiScene* scene = importer.GetScene();
        log_debug("In file %s have %d animations:", nextPath.c_str(), scene->mNumAnimations);
        for (uint animInd = 0; animInd < scene->mNumAnimations; animInd++)
        {

          animPlayer->animations.push_back(Animation());
          Animation & animationData = animPlayer->animations.back();
          const aiAnimation* animation = scene->mAnimations[animInd];
          uint duration = (uint)animation->mDuration;
          animationData.duration = duration;
          animationData.ticksPerSecond = animation->mTicksPerSecond;
          animationData.name = string(animation->mName.C_Str());
          
          map<string, AnimationChannel> &channels = animationData.channels; 
          for (uint i = 0; i < animation->mNumChannels; i++)
          {
            const aiNodeAnim * animNode = animation->mChannels[i];
            string name = normalName(string(animNode->mNodeName.C_Str()));
            auto &p = channels[name];
            if (animNode->mNumPositionKeys == duration + 1)
            {
              for (uint j = 0; j < duration; j++)
              {
                p.pos.push_back(to_vec3(animNode->mPositionKeys[j + 1].mValue));
              }
            }
            if (animNode->mNumRotationKeys == duration + 1)
            {
              for (uint j = 0; j < duration; j++)
              {
                p.rot.push_back(to_quat(animNode->mRotationKeys[j + 1].mValue));
              }
            }
          }
        }
        
      }
    }
    ofstream myfile (join_recources_path("StarterMocapLib.bin"), ios::binary);
    size_t t1 = animPlayer->serialize(myfile);
    myfile.close();
  }
  else
  {
    TimeScope scope("Animation Reading from bin file");
    ifstream l(join_recources_path("StarterMocapLib.bin"), ios::binary);
    size_t t2 = animPlayer->deserialize(l);
    l.close();
    int cadr_count = animPlayer->cadr_count();
    log_debug("Bin file use %ld KB, %ld cadres, %ld bytes on cadr", t2 / 1024, cadr_count, t2 / cadr_count);
  }
  return animPlayer;
}

