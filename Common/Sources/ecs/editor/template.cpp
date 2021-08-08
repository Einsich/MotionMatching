#include "template.h"
#include "../manager/type_description.h"
namespace ecs
{
  
  TemplateInfo::TemplateInfo():name(""), typeHash(0), nameHash(0), data(nullptr){}
  TemplateInfo::TemplateInfo(const string_hash type_hash, const ecs::TypeInfo &type_info, const char *name):
  name(name), typeHash(type_hash), nameHash(HashedString(name)), data(malloc(type_info.sizeOf))
  {
    type_info.constructor(data);
  }
  const char *TemplateInfo::get_name() const
  {
    return name.c_str();
  }
  const string &TemplateInfo::get_string_name() const
  {
    return name;
  }
  void TemplateInfo::change_name(const char *new_name)
  {
    nameHash = HashedString(new_name);
    name = string(new_name);
  }
  bool TemplateInfo::operator==(const TemplateInfo &other) const
  {
    return typeHash == other.typeHash && nameHash == other.nameHash;
  }
  const ecs::TypeInfo &TemplateInfo::get_type_info() const
  {
    return ecs::TypeInfo::types()[typeHash];
  }
  const char *TemplateInfo::get_type_name() const
  {
    return ecs::TypeInfo::types()[typeHash].name.c_str();
  }
  const string &TemplateInfo::get_type_string_name() const
  {
    return ecs::TypeInfo::types()[typeHash].name;
  }
  string_hash TemplateInfo::type_name_hash() const
  {
    return TypeDescription::hash(nameHash, typeHash);
  }
  string_hash TemplateInfo::type_hash() const
  {
    return typeHash;
  }

  Template::Template(const char *name):
  name(name){}


  void clear_processed_field()
  {
    for (Template *t: TemplateManager::instance().templates)
      t->processed = false;
  }

  bool find_collision(Template *t, const vector<const TemplateInfo*> &v, TemplateCollision &collision, bool only_down)
  {
    t->processed = true;
    
    for (const TemplateInfo &type : t->types)
      for (const TemplateInfo *field : v)
        if (type.get_string_name() == field->get_string_name())
        {
          collision.collision = true;
          collision.fieldNew = *field;
          collision.fieldOld = type;
          collision.collisionTrace.push_back(t);
          return true;
        }
    for (Template *subtempl : t->subTemplates)
    {
      if (!subtempl->processed && find_collision(subtempl, v, collision, true))
      {
        collision.collisionTrace.push_back(t);
        return true;
      }
    }
    if (!only_down)
    {
      for (Template *subtempl : t->parentTemplates)
      {
        if (!subtempl->processed && find_collision(subtempl, v, collision, false))
        {
          return true;
        }
      }
    }

    return false;
  }
  TemplateCollision find_collision(Template *t, const vector<const TemplateInfo*> &v)
  {
    TemplateCollision collision;
    collision.collision = false;
    clear_processed_field();
    find_collision(t, v, collision, false);
    return collision;
  }
  void linearize_field(const Template *t, vector<const TemplateInfo*> &v)
  {
    for (uint i = 0; i < t->types.size(); ++i)
      v.emplace_back(&(t->types[i]));
    for (const Template *subtempl : t->subTemplates)
      linearize_field(subtempl, v);
  }
  vector<const TemplateInfo*> linearize_field(const Template *t)
  {
    vector<const TemplateInfo*> v;
    linearize_field(t, v);
    return v;
  }
  TemplateCollision can_change_field(Template *t, const TemplateInfo &info)
  {
    vector<const TemplateInfo*> v = {&info};
    return find_collision(t, v);
  }
  TemplateCollision can_add_subtemplate(Template *t, Template *subtempl)
  {
    vector<const TemplateInfo*> v = linearize_field(subtempl);
    TemplateCollision collision = find_collision(t, v);
    return collision;
  }
  
  bool exists_template_with_name(const char* name)
  {
    for (const Template *t: TemplateManager::instance().templates)
      if (!strcmp(t->name.c_str(), name))
        return true;
    return false;
  }
  bool try_change_template_name(Template &t, const char* new_name)
  {
    if (exists_template_with_name(new_name))
      return false;
    t.name = string(new_name);
    t.edited = true;
    return true;
  }
}