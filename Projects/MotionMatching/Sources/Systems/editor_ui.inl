#include "ecs/ecs.h"
#include "Engine/imgui/imgui.h"
#include <functional>
SYSTEM(ecs::SystemOrder::UI) entity_viewer()
{
  if(!ImGui::Begin("Entity viewer"))
  {
    ImGui::End();
    return;
  }
  int i = 0;
  const int N = 100;
  char buf[N];
  for (ecs::Archetype *archetype : ecs::core().archetypes)
  {
    snprintf(buf, N, "archetype[%d]", ++i);
    if (ImGui::TreeNode(buf))
    {
      for (int j = 0; j < archetype->count; ++j)
      {
        snprintf(buf, N, "entity[%d]", j);
        if (ImGui::TreeNode(buf))
        {
          for (const ecs::FullTypeDescription *full_descr : archetype->fullTypeDescriptions)
          {
            const ecs::TypeInfo &typeInfo = ecs::TypeInfo::types()[full_descr->typeHash];
            snprintf(buf, N, "%s %s",  full_descr->name.c_str(), typeInfo.name.c_str());
            if (ImGui::TreeNode(buf))
            {
              typeInfo.componentEdition(archetype->components[full_descr->hash].get_component<void>(j));
              ImGui::TreePop();
            }
          }
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
  }
  ImGui::End();
}
struct TemplateInfo
{
  string name;
  const ecs::TypeInfo *typeInfo;
  void *data;
  TemplateInfo(const ecs::TypeInfo *typeInfo):
  name(""), typeInfo(typeInfo), data(malloc(typeInfo->sizeOf))
  {
    typeInfo->constructor(data);
  }
};

SYSTEM(ecs::SystemOrder::UI) ecs_types_viewer()
{
  ImGui::Begin("type viewer");
  static vector<TemplateInfo> types;

  static vector<bool> editComponents;
  if (editComponents.size() < types.size())
    editComponents.resize(types.size());
  constexpr int BUFN = 255;
  char buf[BUFN];
  char bufIndex[BUFN];
  for (uint i = 0; i < types.size(); ++i)
  {
    TemplateInfo &type = types[i];
    ImGui::Text("%s", type.typeInfo->name.c_str());
    ImGui::SameLine();
    snprintf(buf, BUFN, "%s", type.name.c_str());
    snprintf(bufIndex, BUFN, "##%d", i);
    if (ImGui::InputText(bufIndex, buf, BUFN))
      type.name = std::string(buf);
    ImGui::SameLine();
    if (!editComponents[i])
    {
      snprintf(bufIndex, BUFN, "edit##%d", i);
      if(ImGui::Button(bufIndex))
        editComponents[i] = true;
    } else
    {
      snprintf(bufIndex, BUFN, "close##%d", i);
      if(ImGui::Button(bufIndex))
        editComponents[i] = false;

    }
    
    ImGui::SameLine();
    snprintf(bufIndex, BUFN, "remove##%d", i);
    bool rem = ImGui::Button(bufIndex);
    if (editComponents[i])
    {
      type.typeInfo->componentEdition(type.data);
    }
    if (rem)
    {
      free(types[i].data);
      types.erase(types.begin() + i);
      editComponents.erase(editComponents.begin() + i);
      --i;
    }

  }
  static bool tryadd = false;
  if (!tryadd && ImGui::Button("add"))
    tryadd = true;
  if (tryadd)
  {
    static std::string searchString = "";
    snprintf(buf, BUFN, "%s", searchString.c_str());
    if (ImGui::InputText("Search: ", buf, BUFN))
      searchString = std::string(buf);

    int typeCount = ecs::TypeInfo::types().size();
    vector<const char*> names;
    names.reserve(typeCount);
    vector<string_hash> hashes;
    hashes.reserve(typeCount);
    for (const auto &p : ecs::TypeInfo::types())
    {
      if (strstr(p.second.name.c_str(), searchString.c_str()))
      {
        names.push_back(p.second.name.c_str());
        hashes.push_back(p.first);
      }
    }
    
    static int item_current = -1;
    const char *cur_name = 0 <= item_current && item_current < (int)names.size()
     ? ecs::TypeInfo::types()[hashes[item_current]].name.c_str() : "";
    ImGui::ListBox(cur_name, &item_current, names.data(), names.size(), names.size());
    ImGui::SameLine();
    if (0 <= item_current && item_current < (int)names.size() && ImGui::Button("Ok"))
    {
      types.emplace_back(&ecs::TypeInfo::types()[hashes[item_current]]);
      tryadd = false;
    }
  }
  ImGui::End();
}