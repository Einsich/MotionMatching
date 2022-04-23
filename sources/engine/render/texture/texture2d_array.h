#pragma once

#include "texture.h"
#include "serialization/serialization.h"

class Texture2DArray : public Texture, public IAsset, public ISerializable
{
  bool generateMips;
public:
  Texture2DArray();

  virtual void load(const filesystem::path &path, bool reload, AssetStatus &status) override;
  virtual bool edit() override;
  
  virtual size_t serialize(std::ostream& os) const override;
  virtual size_t deserialize(std::istream& is) override;
};
