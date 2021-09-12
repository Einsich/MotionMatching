#version 440
#define VS 1


#define USE_UV

struct Material
{
  vec3 AdditionalColor;
  #define LAMBERT
  
};


#define BONES 60






layout(location = 3)in vec4 BoneWeights  ;
layout(location = 4)in uvec4 BoneIndex  ;

void main()
{
  
}

