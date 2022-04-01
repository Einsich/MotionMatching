
vec3 apply_normal_map(vec3 normal_map, vec3 N, vec3 p, vec2 uv )
{
  // get edge vectors of the pixel triangle
  vec3 dp1 = dFdx(p);
  vec3 dp2 = dFdy(p);
  vec2 duv1 = dFdx(uv);
  vec2 duv2 = dFdy(uv);

  // solve the linear system
  vec3 dp2perp = cross(N, dp2);
  vec3 dp1perp = cross(dp1, N);
  vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
  vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

  vec2 TBlen = vec2(dot(T, T), dot(B, B));
  vec2 TBlenRcp = inversesqrt(TBlen);
  // construct a scale-invariant frame 
  return normal_map.z * normalize(N) + (normal_map.x * TBlenRcp.x) * T + (normal_map.y * TBlenRcp.y) * B;
}
