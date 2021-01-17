#include "animation_preprocess.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "CommonCode/math.h"
#include "CommonCode/Time/time_scope.h"
#include <assimp/postprocess.h>

string normalName(const string& badName)
{
  return badName.substr(0, badName.find("_$AssimpFbx$_"));
}
namespace fs = filesystem;

vec3 extract_root_motion(vec3 & translation)
{
  vec3 rootMotion = vec3(translation.x, 0, translation.z);
  translation.x = translation.z = 0;
  return rootMotion;
}
float extract_root_motion(quat & rotation)
{
  mat4 m = toMat4(rotation);
  float x, y, z;
  glm::extractEulerAngleXYZ(m, x, y, z);
  m = glm::eulerAngleXYZ(0.f, y, z);
  rotation = quat_cast(m);
  return x;
}

AnimationPlayerPtr animation_preprocess(Assimp::Importer& importer, GameObjectPtr go, aiNode *root)
{
  bool fromFile = true;
  AnimationPlayerPtr animPlayer = make_shared<AnimationPlayer>();
  animPlayer->gameObject = go;
  if (fromFile)
  {
    TimeScope scope("Animation Reading from fbx file");
    string path = join_recources_path("MocapOnlineMobilityStarterPack/Animation/Root_Motion");
    uintmax_t animation_size = 0;
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
        animation_size += entry.file_size();
        const string& nextPath = entry.path();
        importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f);
        importer.ReadFile(nextPath, aiProcess_GlobalScale);
        const aiScene* scene = importer.GetScene();
        debug_log("In file %s have %d animations:", nextPath.c_str(), scene->mNumAnimations);
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
            bool isRoot = name == "Hips";
            if (animNode->mNumPositionKeys == duration + 1)
            {
              if (isRoot)
              {
                for (uint j = 0; j < duration; j++)
                {
                  vec3 d = to_vec3(animNode->mPositionKeys[j + 1].mValue);
                  animationData.rootMotions.push_back(extract_root_motion(d));
                  p.pos.push_back(d);
                }
              }
              else
              {
                for (uint j = 0; j < duration; j++)
                  p.pos.push_back(to_vec3(animNode->mPositionKeys[j + 1].mValue));                
              }
              
              
              
            }
            if (animNode->mNumRotationKeys == duration + 1)
            {
              if (isRoot)
              {
                for (uint j = 0; j < duration; j++)
                {
                  quat q = to_quat(animNode->mRotationKeys[j + 1].mValue);
                  animationData.rootRotations.push_back(extract_root_motion(q));
                  p.rot.push_back(q);
                }
              }
              else
              {
                for (uint j = 0; j < duration; j++)
                  p.rot.push_back(to_quat(animNode->mRotationKeys[j + 1].mValue));
              }
            }
          }
        }
        
      }
    }
    animPlayer->build_state_machine();
    ofstream myfile (join_recources_path("StarterMocapLib.bin"), ios::binary);
    size_t t1 = animPlayer->serialize(myfile);
    myfile.close();
    int cadr_count = animPlayer->cadr_count();
    debug_log("Bin file use %ld KB instead %ld KB, %ld cadres, %ld bytes on cadr", t1 / 1024, animation_size / 1024, cadr_count, t1 / cadr_count);
  }
  else
  {
    TimeScope scope("Animation Reading from bin file");
    ifstream l(join_recources_path("StarterMocapLib.bin"), ios::binary);
    size_t t2 = animPlayer->deserialize(l);
    l.close();
    int cadr_count = animPlayer->cadr_count();
    debug_log("Bin file use %ld KB, %ld cadres, %ld bytes on cadr", t2 / 1024, cadr_count, t2 / cadr_count);
  }
  return animPlayer;
}

