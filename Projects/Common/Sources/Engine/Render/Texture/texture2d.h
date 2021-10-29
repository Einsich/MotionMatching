#pragma once

#include "texture.h"
#include "Serialization/serialization.h"

class Texture2D : public Texture, public IAsset, public ISerializable
{
  bool generateMips;
  void load_from_path(const filesystem::path &path);
public:
  Texture2D();
  Texture2D(string texture_path_from_textures_folder,
  TextureColorFormat color_format = TextureColorFormat::RGB, 
  TextureFormat texture_format = TextureFormat::UnsignedByte, 
  TexturePixelFormat pixelFormat = TexturePixelFormat::Linear,
  TextureWrappFormat wrapping = TextureWrappFormat::Repeat, 
  bool generate_mips = false);
  
  virtual void load(const filesystem::path &path, bool reload) override;
  virtual bool edit() override;
  
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
