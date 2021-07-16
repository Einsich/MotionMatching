#pragma once

#include "texture.h"
#include "Serialization/serialization.h"

class Texture2D : public Texture, public std::enable_shared_from_this<Texture2D>, public IAsset, public ISerializable
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
  virtual void free() override;
  virtual bool edit() override;
  
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
TexturePtr make_texture2d();
TexturePtr make_texture2d(const string& texture_path_from_textures_folder,
  TextureColorFormat color_format, 
  TextureFormat texture_format, 
  TexturePixelFormat pixelFormat = TexturePixelFormat::Linear,
  TextureWrappFormat wrapping = TextureWrappFormat::Repeat, 
  bool generate_mips = false);
  TexturePtr make_texture2d(const string& texture_path_from_textures_folder,
  TexturePixelFormat pixelFormat = TexturePixelFormat::Linear,
  TextureWrappFormat wrapping = TextureWrappFormat::Repeat, 
  bool generate_mips = false,
  TextureColorFormat color_format = TextureColorFormat::RGB, 
  TextureFormat texture_format = TextureFormat::UnsignedByte);