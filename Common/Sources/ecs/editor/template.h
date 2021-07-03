#pragma once
#include "ecs/manager/type_info.h"

//add new template

namespace ecs
{
 
  struct TemplateInfo
  {
  private:
    string name;
    string_hash typeHash, nameHash;
  public:
    void *data;
    TemplateInfo();
    TemplateInfo(const string_hash type_hash, const ecs::TypeInfo &type_info, const char *name);
    const char *get_name() const;
    const string &get_string_name() const;
    void change_name(const char *new_name);
    bool operator==(const TemplateInfo &other) const;
    const ecs::TypeInfo &get_type_info() const;
    const char *get_type_name() const;
    const string &get_type_string_name() const;
  };
  struct Template
  {
    string name;
    vector<TemplateInfo> types;
    vector<Template*> subTemplates;
    vector<Template*> parentTemplates;
    bool opened;
    bool processed;
    Template() = default;
    Template(const char *name);
    static vector<Template*> &templates()
    {
      static vector<Template*> templates;
      return templates;
    }
  };

  struct TemplateCollision
  {
    bool collision;
    TemplateInfo fieldNew;
    TemplateInfo fieldOld;//field->get_type_name() field->get_name()
    vector<const Template*> collisionTrace;//t->name()
    TemplateCollision() = default;
  };
  bool try_change_template_name(Template &t, const char* new_name);
  TemplateCollision can_change_field(Template *t, const TemplateInfo &info);
  TemplateCollision can_add_subtemplate(Template *t, Template *subtempl);
  bool exists_template_with_name(const char* name);
}
