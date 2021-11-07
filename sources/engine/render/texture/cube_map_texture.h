#pragma once

#include "texture.h"
#include "resources/asset.h"
class CubeMap : public Texture, public IAsset, public ISerializable
{
public:

  CubeMap();
  CubeMap(string cubemap_path_from_textures_folder,
  TextureColorFormat color_format = TextureColorFormat::RGB, 
  TextureFormat texture_format = TextureFormat::UnsignedByte, 
  TexturePixelFormat pixelFormat = TexturePixelFormat::Linear);
  
  virtual void load(const filesystem::path &path, bool reload) override;
  virtual bool edit() override;

  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};