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
  string_hash typeHash;
  void *data;
  TemplateInfo(const string_hash type_hash, const ecs::TypeInfo &type_info):
  name(""), typeHash(type_hash), data(malloc(type_info.sizeOf))
  {
    type_info.constructor(data);
  }
};
struct Template
{
  string name;
  vector<TemplateInfo> types;
  vector<Template> subTemplates;
  Template():
  name(""), types(), subTemplates()
  {
  }
};
static vector<Template> templates(1);
bool exists_template_with_name(const char* name)
{
  for (Template & t: templates)
    if (!strcmp(t.name.c_str(), name))
      return true;
  return false;
}
bool try_change_template_name(const char* new_name)
{
  if (exists_template_with_name(new_name))
    return false;
  for (Template &t: templates)
    for (Template &subt: t.subTemplates)
      subt.name = string(new_name);
  return true;
}

void edit_template(Template &templ, bool sub_templ = false)
{
  vector<TemplateInfo> &types = templ.types;
  vector<Template> &subTemplates = templ.subTemplates;
  static vector<bool> editComponents;
  if (editComponents.size() < types.size())
    editComponents.resize(types.size());

  static vector<bool> editSubTempl;
  if (editSubTempl.size() < subTemplates.size())
    editSubTempl.resize(subTemplates.size());

  constexpr int BUFN = 255;
  char buf[BUFN];
  char bufIndex[BUFN];
  if (!sub_templ)
  {
    snprintf(buf, BUFN, "%s", templ.name.c_str());
    if (ImGui::InputText(bufIndex, buf, BUFN, ImGuiInputTextFlags_EnterReturnsTrue) && strcmp(templ.name.c_str(), buf))
    {
      if (try_change_template_name(buf))
        templ.name = std::string(buf);
      else
        ImGui::TextColored(ImVec4(1.0, 0, 0,1.0), "Enter unique template name");
    }
  }

  for (uint i = 0; i < types.size(); ++i)
  {
    TemplateInfo &type = types[i];
    const ecs::TypeInfo &typeInfo = ecs::TypeInfo::types()[type.typeHash];
    ImGui::Text("%s", typeInfo.name.c_str());
    ImGui::SameLine();
    snprintf(buf, BUFN, "%s", type.name.c_str());
    snprintf(bufIndex, BUFN, "##%d", i);
    if (ImGui::InputText(bufIndex, buf, BUFN, ImGuiInputTextFlags_EnterReturnsTrue))
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
    if (!sub_templ)
    {
      ImGui::SameLine();
      snprintf(bufIndex, BUFN, "remove##%d", i);
      if (ImGui::Button(bufIndex))
      {
        free(types[i].data);
        types.erase(types.begin() + i);
        editComponents.erase(editComponents.begin() + i);
        --i;
        continue;
      }
    }
    if (editComponents[i])
    {
      typeInfo.componentEdition(type.data);
    }

  }
  
  for (uint i = 0; i < subTemplates.size(); ++i)
  {
    Template &subTemplate = subTemplates[i];
    ImGui::Text("%s", subTemplate.name.c_str());
    ImGui::SameLine();
    if (!editSubTempl[i])
    {
      snprintf(bufIndex, BUFN, "edit##%d", i);
      if(ImGui::Button(bufIndex))
        editSubTempl[i] = true;
    } else
    {
      snprintf(bufIndex, BUFN, "close##%d", i);
      if(ImGui::Button(bufIndex))
        editSubTempl[i] = false;

    }
    if (!sub_templ)
    {
      ImGui::SameLine();
      snprintf(bufIndex, BUFN, "remove##%d", i);
      bool rem = ImGui::Button(bufIndex);
      if (rem)
      {
        subTemplates.erase(subTemplates.begin() + i);
        editSubTempl.erase(editSubTempl.begin() + i);
        --i;
        continue;
      }
    }
    if (editSubTempl[i])
    {
      edit_template(subTemplate, true);
    }
  }
  if (sub_templ)
    return;
  static bool tryAddField = false;
  static bool tryAddSubTempl = false;

  if (tryAddField)
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
    if (0 <= item_current && item_current < (int)names.size())
    {
      if (ImGui::Button("Ok"))
      {
        types.emplace_back(hashes[item_current], ecs::TypeInfo::types()[hashes[item_current]]);
        tryAddField = false;
      }
    }
    else
    {
      if (ImGui::Button("Cancel"))
      {
        tryAddField = false;
      }
    }
  }
  if (tryAddSubTempl)
  {
    static std::string searchTemplateString = "";
    snprintf(buf, BUFN, "%s", searchTemplateString.c_str());
    if (ImGui::InputText("SubTemplate: ", buf, BUFN))
      searchTemplateString = std::string(buf);

    vector<const char*> names;
    names.reserve(templates.size());
    vector<int> indexes;
    indexes.reserve(templates.size());
    for (uint i = 0; i < templates.size(); ++i)
    {
      const Template &t = templates[i];
      if (strcmp(t.name.c_str(), templ.name.c_str()) && strstr(t.name.c_str(), searchTemplateString.c_str()))
      {
        names.push_back(t.name.c_str());
        indexes.push_back(i);
      }
    }
    
    static int item_current = -1;
    const char *cur_name = 0 <= item_current && item_current < (int)names.size()
     ? templates[indexes[item_current]].name.c_str() : "";
    ImGui::ListBox(cur_name, &item_current, names.data(), names.size(), names.size());
    ImGui::SameLine();
    if (0 <= item_current && item_current < (int)names.size())
    {
      if (ImGui::Button("Ok"))
      {
        templ.subTemplates.emplace_back(templates[indexes[item_current]]);
        tryAddSubTempl = false;
      }
    }
    else
    {
      if (ImGui::Button("Cancel"))
      {
        tryAddSubTempl = false;
      }
    }
  }
  if (!tryAddField && ImGui::Button("add field"))
    tryAddField = true;
  else
  if (!tryAddSubTempl && ImGui::Button("add subtemplate"))
    tryAddSubTempl = true;

}

SYSTEM(ecs::SystemOrder::UI) ecs_types_viewer()
{
  ImGui::Begin("type viewer");
  static vector<const char*> names;
  static int selectedTemplate = -1;
  static bool editTemplate = false;
  if (editTemplate)
  {
    edit_template(templates[selectedTemplate]);
    if (ImGui::Button("Close editing"))
      selectedTemplate = -1, editTemplate = false;
  }
  else
  {
    names.resize(templates.size());
    for (uint i = 0; i < templates.size(); ++i)
      names[i] = templates[i].name.c_str();
    bool selectedSorrectTemplate = 0 <= selectedTemplate && selectedTemplate < (int)templates.size();
    const char *cur_name = selectedSorrectTemplate ? templates[selectedTemplate].name.c_str() : "";
    ImGui::ListBox(cur_name, &selectedTemplate, names.data(), names.size(), min(10, (int)names.size()));
    
    selectedSorrectTemplate = 0 <= selectedTemplate && selectedTemplate < (int)templates.size();
    if (selectedSorrectTemplate)
    { 
      ImGui::SameLine();
      if (ImGui::Button("Edit"))
        editTemplate = true;
    }
    if (exists_template_with_name(""))
    {
      ImGui::TextColored(ImVec4(1.0, 0, 0,1.0), "Give name for your templates");
    }
    else
    if (ImGui::Button("add template"))
    {
      templates.emplace_back();
    }
    
  }
    
  ImGui::End();
}