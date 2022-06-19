#include "component_instance.h"

namespace ecs
{  
  template<typename T>
  void patch_components_impl(ecs::vector<ComponentInstance> &components, T patch)
  {
    for (size_t i = 0, n = patch.size(); i < n; ++i)
    {
      bool patched = false;
      for (size_t j = 0, m = components.size(); j < m; ++j)
      {
        if (patch[i].typeNameHash == components[j].typeNameHash)
        {
          components[j] = patch[i];
          patched = true;
          break;
        }
      }
      if (!patched)
        components.emplace_back(patch[i]);
    }
  }
  void patch_components(ecs::vector<ComponentInstance> &components, const ecs::vector<ComponentInstance> &patch)
  {
    patch_components_impl(components, patch);
  }
  void patch_components(ecs::vector<ComponentInstance> &components, ecs::vector<ComponentInstance> &&patch)
  {
    patch_components_impl(components, std::move(patch));
  }
}