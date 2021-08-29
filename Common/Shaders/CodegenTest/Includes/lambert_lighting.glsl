vec3 LightedColor(in vec3 color, in Material material, in VsOutput vsOutput, in vec3 light_dir, in vec3 camera_pos)
{
    vec3 W = normalize(camera_pos - vsOutput.WorldPosition);
    vec3 E = reflect(light_dir, vsOutput.EyespaceNormal);
    float df = max(0.0, dot(vsOutput.EyespaceNormal, -light_dir));
    float sf = max(0.0, dot(E, W));
    sf = pow(sf, material.Shininess);
    return color * (material.Ambient + df * material.Diffuse) + sf * material.Specular;
}