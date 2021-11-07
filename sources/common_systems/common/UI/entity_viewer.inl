#include <ecs.h>
#include <imgui.h>
#include <editor/template.h>
#include <manager/entity_container.h>
#include <functional>
#include "editor_window.h"

constexpr uint lockFlag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
EVENT(ecs::SystemTag::GameEditor,ecs::SystemTag::Debug) init_imgui_style(const ecs::OnSceneCreated&, EditorUI &ui)
{
  ImGuiStyle& style = ImGui::GetStyle();
  style.IndentSpacing = 5;
  //style.ItemSpacing = ImVec2(0, 0);
  //style.ItemInnerSpacing = ImVec2(0, 0);
  ui.windowFlags = lockFlag;

}
SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::GameEditor,ecs::SystemTag::Debug) entity_viewer(const EditorUI &ui)
{
  if(!ImGui::Begin("Entity viewer", nullptr, ui.windowFlags))
  {
    ImGui::End();
    return;
  }
  ImGui::PushItemWidth(170);
  const int N = 100;
  char buf[N];
  int archetypeIndex = 0;
  for (ecs::Archetype *archetype : ecs::core().entityContainer->archetypes)
  {
    if (ImGui::TreeNode(archetype->synonim.c_str()))
    {
      for (int j = 0; j < archetype->count; ++j)
      {
        ecs::EntityId eid = ecs::find_entity(archetypeIndex, j);
        snprintf(buf, N, "entity[%d]", j);
        if (ImGui::TreeNode(buf))
        {
          ImGui::SameLine();
          if (ImGui::Button("destroy"))
          {
            archetype->destroy_entity(j, true);
          }
          else
          for (const ecs::FullTypeDescription *full_descr : archetype->fullTypeDescriptions)
          {
            const ecs::TypeInfo &typeInfo = ecs::TypeInfo::types()[full_descr->typeHash];
            snprintf(buf, N, "%s %s",  full_descr->name.c_str(), typeInfo.name.c_str());
            if (ImGui::TreeNode(buf))
            {
              bool edited = typeInfo.componentEdition(archetype->components[full_descr->hash].get_component<void>(j), false);
              if (edited)
                ecs::send_event(eid, ecs::OnEntityEdited());
              ImGui::TreePop();
            }
          }
          ImGui::TreePop();
        }
      }
      ImGui::TreePop();
    }
    ++archetypeIndex;
  }
  ImGui::End();
}

string collision_error(const ecs::TemplateCollision &collision)
{
  string message = collision.collisionTrace[0]->name;
  string oldField = collision.fieldOld.get_type_string_name() + " " + collision.fieldOld.get_string_name();
  string newField = collision.fieldNew.get_type_string_name() + " " + collision.fieldNew.get_string_name();
  for (uint i = 1; i < collision.collisionTrace.size(); ++i)
  {
    message = collision.collisionTrace[i]->name + "/" + message;
  }
  
  return "new field " + newField + " collide with " + oldField + " in " + message; 
}
void imgui_error(const string &error)
{
  ImGui::TextColored(ImVec4(1.0, 0, 0, 1.0), "%s", error.c_str());
}
void imgui_error(const char *error)
{
  ImGui::TextColored(ImVec4(1.0, 0, 0, 1.0), "%s",  error);
}
void edit_template(ecs::Template &templ, bool view_only = false, int depth = 0)
{
  vector<ecs::TemplateInfo> &types = templ.types;
  vector<ecs::Template*> &subTemplates = templ.subTemplates;
  static vector<bool> editComponents;
  if (editComponents.size() < types.size())
    editComponents.resize(types.size());


  constexpr int BUFN = 255;
  char buf[BUFN];
  char bufIndex[BUFN];
  if (!view_only)
  {
    snprintf(buf, BUFN, "%s", templ.name.c_str());
    if (ImGui::InputText("##templ_name", buf, BUFN, ImGuiInputTextFlags_EnterReturnsTrue) && strcmp(templ.name.c_str(), buf))
    {
      if (!ecs::try_change_template_name(templ, buf))
        imgui_error("Enter unique template name");
    }
  }

  for (uint i = 0; i < types.size(); ++i)
  {
    ecs::TemplateInfo &type = types[i];
    const ecs::TypeInfo &typeInfo = type.get_type_info();
    ImGui::Text("%s", typeInfo.name.c_str());
    ImGui::SameLine();
    if (view_only)
      ImGui::Text("%s", type.get_name());
    else
    {
      snprintf(buf, BUFN, "%s", type.get_name());
      snprintf(bufIndex, BUFN, "##%d", i);
      static string error = "";
      if (ImGui::InputText(bufIndex, buf, BUFN, ImGuiInputTextFlags_EnterReturnsTrue))
      {
        if (!strcmp(buf, type.get_name()))
        {
          error = "";
        }
        else
        {
          ecs::TemplateInfo bufInfo = type;
          bufInfo.change_name(buf);
          ecs::TemplateCollision collision = ecs::can_change_field(&templ, bufInfo);
          if (collision.collision)
          {
            error = collision_error(collision);
          }
          else
          {
            type.change_name(buf);
            templ.edited = true;
          }
        }
      }
      if (error != "")
        imgui_error(error);
    }
    ImGui::SameLine();
    if (!editComponents[i])
    {
      snprintf(bufIndex, BUFN, "open##%d", i);
      if(ImGui::Button(bufIndex))
        editComponents[i] = true;
    } else
    {
      snprintf(bufIndex, BUFN, "close##%d", i);
      if(ImGui::Button(bufIndex))
        editComponents[i] = false;

    }
    if (!view_only)
    {
      ImGui::SameLine();
      snprintf(bufIndex, BUFN, "remove##%d", i);
      if (ImGui::Button(bufIndex))
      {
        free(types[i].data);
        types.erase(types.begin() + i);
        editComponents.erase(editComponents.begin() + i);
        --i;
        templ.edited = true;
        continue;
      }
    }
    if (editComponents[i])
    {
      templ.edited |= typeInfo.componentEdition(type.data, view_only);
    }
  }
  
  for (uint i = 0; i < subTemplates.size(); ++i)
  {
    ecs::Template *subTemplate = subTemplates[i];
    ImGui::Text("%s", subTemplate->name.c_str());
    ImGui::SameLine();
    if (!subTemplate->opened)
    {
      snprintf(bufIndex, BUFN, "open##%d %d", depth, i);
      if(ImGui::Button(bufIndex))
        subTemplate->opened = true;
    } else
    {
      snprintf(bufIndex, BUFN, "close##%d %d", depth, i);
      if(ImGui::Button(bufIndex))
        subTemplate->opened = false;

    }
    if (!view_only)
    {
      ImGui::SameLine();
      snprintf(bufIndex, BUFN, "remove##%d %d", depth, i);
      bool rem = ImGui::Button(bufIndex);
      if (rem)
      {
        subTemplates[i]->parentTemplates.erase(
            std::remove(subTemplates[i]->parentTemplates.begin(), subTemplates[i]->parentTemplates.end(), &templ),
            subTemplates[i]->parentTemplates.end());
        subTemplates.erase(subTemplates.begin() + i);
        --i;
        templ.edited = true;
        continue;
      }
    }
    if (subTemplate->opened)
    {
      edit_template(*subTemplate, true, depth + 1);
    }
  }
  if (view_only)
    return;
  static bool tryAddField = false;
  static bool tryAddSubTempl = false;

  if (tryAddField)
  {
    static std::string fieldName = "";
    snprintf(buf, BUFN, "%s", fieldName.c_str());
    bool nameChanged = ImGui::InputText("Name: ", buf, BUFN);
    if (nameChanged)
      fieldName = std::string(buf);
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
    bool typeChanged = ImGui::ListBox(cur_name, &item_current, names.data(), names.size(), names.size());
    ImGui::SameLine();
    if (0 <= item_current && item_current < (int)names.size())
    {
      static string error = "";
      if (nameChanged || typeChanged)
      {
        ecs::TemplateCollision collision = ecs::can_change_field(&templ, 
        ecs::TemplateInfo(hashes[item_current], ecs::TypeInfo::types()[hashes[item_current]], fieldName.c_str()));
        error = collision.collision ? collision_error(collision) : "";
      }
      if (error != "")
        imgui_error(error);
      else
      if (ImGui::Button("Ok"))
      {
        types.emplace_back(hashes[item_current], ecs::TypeInfo::types()[hashes[item_current]], fieldName.c_str());
        tryAddField = false;
        templ.edited = true;
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
    auto &templates = ecs::TemplateManager::instance().templates;
    static std::string searchTemplateString = "";
    snprintf(buf, BUFN, "%s", searchTemplateString.c_str());
    bool nameChanged = ImGui::InputText("SubTemplate: ", buf, BUFN);
    if (nameChanged)
      searchTemplateString = std::string(buf);

    vector<const char*> names;
    names.reserve(templates.size());
    vector<int> indexes;
    indexes.reserve(templates.size());
    for (uint i = 0; i < templates.size(); ++i)
    {
      const ecs::Template &t = *templates[i];
      if (strcmp(t.name.c_str(), templ.name.c_str()) && strstr(t.name.c_str(), searchTemplateString.c_str()))
      {
        names.push_back(t.name.c_str());
        indexes.push_back(i);
      }
    }
    
    static int item_current = -1;
    const char *cur_name = 0 <= item_current && item_current < (int)names.size()
     ? templates[indexes[item_current]]->name.c_str() : "";
    bool typeChanged = ImGui::ListBox(cur_name, &item_current, names.data(), names.size(), names.size());
    ImGui::SameLine();
    if (0 <= item_current && item_current < (int)names.size())
    {
      static string error = "";
      if (typeChanged)
      {
        ecs::TemplateCollision collision = ecs::can_add_subtemplate(&templ, templates[indexes[item_current]]);
        error = collision.collision ? collision_error(collision) : "";
      }
      if (error != "")
        imgui_error(error);
      if (ImGui::Button("Ok"))
      {
        ecs::Template *subTempl = templates[indexes[item_current]];
        templ.subTemplates.push_back(subTempl);
        subTempl->parentTemplates.push_back(&templ);
        tryAddSubTempl = false;
        templ.edited = true;
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

SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::Editor) ecs_types_viewer(const EditorUI &ui)
{
  ImGui::Begin("type viewer", nullptr, ui.windowFlags);
  ImGui::PushItemWidth(170);
  static vector<const char*> names;
  static int selectedTemplate = -1;
  static bool editTemplate = false;
  static bool addedTemplate = false;
  constexpr int BUFN = 255;
  char buf[BUFN];
  auto &templates = ecs::TemplateManager::instance().templates;
  if (editTemplate)
  {
    edit_template(*templates[selectedTemplate]);
    if (ImGui::Button("Close editing"))
      selectedTemplate = -1, editTemplate = false;
  }
  else
  {
    if (!addedTemplate)
    {
      names.resize(templates.size());
      for (uint i = 0; i < templates.size(); ++i)
        names[i] = templates[i]->name.c_str();
      bool selectedCorrectTemplate = 0 <= selectedTemplate && selectedTemplate < (int)templates.size();
      const char *cur_name = selectedCorrectTemplate ? templates[selectedTemplate]->name.c_str() : "";
      ImGui::ListBox(cur_name, &selectedTemplate, names.data(), names.size(), min(10, (int)names.size()));
      
      selectedCorrectTemplate = 0 <= selectedTemplate && selectedTemplate < (int)templates.size();
      if (selectedCorrectTemplate)
      { 
        ImGui::SameLine();
        if (ImGui::Button("Edit"))
          editTemplate = true;
        ImGui::SameLine();
        if (ImGui::Button("Delete"))
        {
          editTemplate = false;
          templates.erase(templates.begin() + selectedTemplate);
        }
      }
      if (ImGui::Button("add template"))
      {
        addedTemplate = true;
      }
    }
    else
    {
      static std::string templateName = "";
      snprintf(buf, BUFN, "%s", templateName.c_str());
      static bool goodName = false;
      if (ImGui::InputText("Template name", buf, BUFN))
      {
        goodName = !ecs::exists_template_with_name(buf);
        if (!goodName)
          imgui_error("Give the unique name for your template");
        else
          templateName = string(buf);
      }
      if (templateName == "")
      {
        goodName = false;
        imgui_error("Name your template");
      }
      if (goodName && ImGui::Button("create"))
      {
        templates.push_back(new ecs::Template(templateName.c_str()));
        addedTemplate = false;
        goodName = false;
        templateName = "";
      }
      if (ImGui::Button("Cancel"))
      {
        addedTemplate = false;
        goodName = false;
        templateName = "";
      }
      
    }
  }
    
  ImGui::End();
}


SYSTEM(ecs::SystemOrder::UI,ecs::SystemTag::Editor) entity_creater(const EditorUI &ui)
{
  if(!ImGui::Begin("Create entity", nullptr, ui.windowFlags))
  {
    ImGui::End();
    return;
  }
  ImGui::PushItemWidth(170);
  const int BUFN = 255;
  char buf[BUFN];
  
  static std::string searchString = "";
  snprintf(buf, BUFN, "%s", searchString.c_str());
  if (ImGui::InputText("<-Search", buf, BUFN))
    searchString = std::string(buf);

  auto &templates = ecs::TemplateManager::instance().templates;
  vector<const char*> names;
  vector<const ecs::Template*> templatePtrs;
  names.reserve(templates.size());
  int i = 0;
  for (const ecs::Template *t : templates)
  {
    if (strstr(t->name.c_str(), searchString.c_str()))
    {
      names.push_back(t->name.c_str());
      templatePtrs.push_back(t);
    }
    ++i;
  }
    
  static int selectedTemplate = -1;
  bool selectedCorrectTemplate = 0 <= selectedTemplate && selectedTemplate < (int)templatePtrs.size();
  if (selectedCorrectTemplate)
  {
    ImGui::SameLine();
    ImGui::NextColumn();
    if (ImGui::Button("Create"))
      ecs::create_entity(templatePtrs[selectedTemplate]);
  }
  const char *cur_name = selectedCorrectTemplate ? templatePtrs[selectedTemplate]->name.c_str() : "";
  ImGui::ListBox(cur_name, &selectedTemplate, names.data(), names.size(), min(10, (int)names.size()));
  

  ImGui::End();
}