#pragma once

#include "texture.h"
#include "serialization/serialization.h"

class Texture2D : public Texture, public IAsset, public ISerializable
{
  bool generateMips;
  void load_from_path(const std::filesystem::path &path);
  void create_from_pointer(const unsigned char*, int, int, size_t);
  unsigned char *stbiData;
public:
  Texture2D();
  Texture2D(std::string texture_path_from_textures_folder,
  TextureColorFormat color_format = TextureColorFormat::RGB, 
  TextureFormat texture_format = TextureFormat::UnsignedByte, 
  TexturePixelFormat pixelFormat = TexturePixelFormat::Linear,
  TextureWrappFormat wrapping = TextureWrappFormat::Repeat, 
  bool generate_mips = false);
  Texture2D(int w, int h,
  TextureColorFormat color_format = TextureColorFormat::RGB, 
  TextureFormat texture_format = TextureFormat::UnsignedByte, 
  TexturePixelFormat pixelFormat = TexturePixelFormat::Linear,
  TextureWrappFormat wrapping = TextureWrappFormat::Repeat, 
  bool generate_mips = false);

  void update_sub_region(int level, int x_offset, int y_offset, int width, int height, const void *data);
  
  virtual void load(const std::filesystem::path &path, bool reload, AssetStatus &status) override;
  virtual bool edit() override;
  
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
