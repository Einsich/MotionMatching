#include "component_instance.h"

namespace ecs
{  
  void patch_components(vector<ComponentInstance> &components, const vector<ComponentInstance> &patch)
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
}