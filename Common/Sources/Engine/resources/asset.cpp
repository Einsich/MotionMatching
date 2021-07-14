#include "asset.h"
#include "Engine/Render/Texture/texture2d.h"

class Test final : IAsset
{
public:
  Test() = default;

  virtual void load(const std::string &path) override
  {

  }
  virtual void free() override
  {

  }
  virtual bool edit() override
  {
    
  }
};
void f()
{
  std::string path = "path";

  Asset<Test> tex(path);
}
