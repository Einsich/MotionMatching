
#shader water_shader

#define USE_UV
struct Material
{
  #include material
  float waterLevel;
  float waterDepthScale;
  float waterTile;
  float waterTimeScale;
  float foamScale;
  float foamTimeScale;
  float foamDepth;
};
#include vs_output
#include instancing
#include common_data

#vertex_shader

#include vs_input
void main()
{
  vec3 vertex_position = Position;
  #include common_vs
}

#pixel_shader

#include ps_input
out vec4 FragColor;

uniform sampler2D heightMap;
uniform sampler2D waterNoise;
uniform sampler2D waterColor;
uniform sampler2D waterFoam;
uniform samplerCube skyReflection;

vec4 flowMapUV(vec2 scaled_uv, float scaled_time)
{
	vec2 time1 = scaled_time * vec2(0.3f, 0.7f);
	vec2 time2 = -scaled_time * 0.75f * vec2(0.8f, 0.2f);
	vec2 uv1 = scaled_uv;
	vec2 uv2 = scaled_uv * 1.3;
  return vec4(uv1 + time1, uv2 + time2) * 3.0;
}

vec3 CalcWaterNormal(vec2 scaled_uv, float scaled_time)
{
  vec4 uv = flowMapUV(scaled_uv, scaled_time);

	vec3 noiseNormal1 = texture(waterNoise, uv.xy).rgb - 0.5f;
	vec3 noiseNormal2 = texture(waterNoise, uv.zw).rgb - 0.5f;
	vec3 normalNoise = noiseNormal1 + noiseNormal2 + vec3(0.0f, 0.0f, 1.5f);
	return normalize(normalNoise).xzy;
}


float saturate(float x)
{
  return clamp(x, 0, 1);
}
vec3 saturate(vec3 x)
{
  return clamp(x, 0, 1);
}
vec3 ComposeSpecular( vec3 vColor, float vSpecular ) 
{
	return saturate(vColor + vSpecular);// * STANDARD_HDR_RANGE + ( 1.0f - STANDARD_HDR_RANGE ) * vSpecular;
}
void main()
{
  vec2 waterUV = vsOutput.UV * material_inst.waterTile;
  float waterTime = Time.x*material_inst.waterTimeScale;

  vec2 foamUV = vsOutput.UV * material_inst.foamScale;
  float foamTime = Time.x*material_inst.foamTimeScale;

  float height = texture(heightMap, vsOutput.UV).y;
  float alpha = (material_inst.waterLevel - height) * material_inst.waterDepthScale;
  vec3 texColor =  texture(waterColor, vsOutput.UV).rgb;

  if (alpha < material_inst.foamDepth)
  {
    vec4 uv = flowMapUV(foamUV, foamTime);

    vec4 foam1 = texture(waterFoam, uv.xy);
    vec4 foam2 = texture(waterFoam, uv.zw);
    vec4 foam = mix(foam1, foam2, foam2.a);
    float t = alpha / material_inst.foamDepth;
    t *= t;
    texColor = mix(texColor, foam.rgb, foam.a * (1 - t));
  }
  else
    alpha = 1;

  vec3 normal = CalcWaterNormal(waterUV, waterTime);

  vec3 vEyeDir = normalize(vsOutput.WorldPosition - CameraPosition);
  vec3 vLightDir = LightDirection;
	vec3 normalReflection = normal;
	vec3 reflection = reflect(vEyeDir, normalReflection);
	vec3 reflectiveColor = texture(skyReflection, reflection).rgb;

  vec3 refractiveColor = texColor;
  float vIceFade = 0;
	float fresnelBias = 0.25f;
	float fresnel = saturate(dot(-vEyeDir, normal)) * 0.5f;
	fresnel = saturate(fresnelBias + (1.0f - fresnelBias) * pow(1.0f - fresnel, 10.0f) );

	fresnel *= (1.0f-vIceFade); //No fresnel when we have snow
	
	vec3 H = normalize(-vLightDir + -vEyeDir);

	float vSpecWidth = 500;
	
	float vSpecMultiplier = 1.0f;
	float specular = saturate(pow(saturate(dot(H, normal)), vSpecWidth) * vSpecMultiplier);
	
	refractiveColor = mix( refractiveColor, texColor, 0.3f+(0.7f*vIceFade) );
	vec3 outColor = refractiveColor * ( 1.0f - fresnel ) + reflectiveColor * fresnel;
  outColor = ComposeSpecular(outColor, specular);

  FragColor = vec4(outColor, alpha );
}