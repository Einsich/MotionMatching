vec3 LightedColor(
    in vec3 color,
    in Material material,
    in vec3 world_position,
    in vec3 world_normal,
    in vec3 light_dir,
    in vec3 camera_pos)
{
    vec3 W = normalize(camera_pos - world_position);
    vec3 E = reflect(light_dir, world_normal);
    float df = max(0.0, dot(world_normal, -light_dir));
    float sf = max(0.0, dot(E, W));
    sf = pow(sf, material.Shininess);
    return color * (AmbientLight + df * SunLight) + vec3(1,1,1) * sf * material.Metallness;
}