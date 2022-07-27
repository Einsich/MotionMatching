#shader text_shader

uniform mat3 transform;
uniform vec4 color;

#vertex_shader
struct Instance
{
  vec4 uv;
  vec4 offsets;
};

layout(std430, binding = 0) readonly buffer TextData 
{
  Instance instances[];
};

layout(location = 0)in vec2 Position;

out vec2 atlasUV;

void main() 
{
  vec2 uv = (Position + vec2(1,1)) * 0.5;
  vec4 offset = instances[gl_InstanceID].offsets;
  vec4 UVoffset = instances[gl_InstanceID].uv;
  vec2 pos = offset.xy + offset.zw * uv;

  atlasUV = UVoffset.xy + UVoffset.zw * uv;
  pos = (transform * vec3(pos, 1)).xy;
  gl_Position = vec4(pos, 0, 1);
  
}

#pixel_shader

in vec2 atlasUV;
out vec4 outColor;

uniform sampler2D atlas;

void main()
{
  outColor = color * vec4(1, 1, 1, texture(atlas, atlasUV).r);
}