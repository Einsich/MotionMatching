#pragma once
#include "manager/type_info.h"
#include "serialization/iserializable.h"


namespace ecs
{
 
  struct TemplateInfo final: public ISerializable
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
    string_hash type_name_hash() const;
    string_hash type_hash() const;
    virtual size_t serialize(std::ostream& os) const override;
    virtual size_t deserialize(std::istream& is) override;
  };
  struct Template final: public ISerializable
  {
    string name;
    vector<TemplateInfo> types;
    vector<Template*> subTemplates;
    vector<Template*> parentTemplates;
    bool opened;
    bool processed;
    bool edited;
    Template() = default;
    Template(const char *name);
    virtual size_t serialize(std::ostream& os) const override;
    virtual size_t deserialize(std::istream& is) override;
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

  struct TemplateManager
  {
    vector<Template*> templates;
    static TemplateManager &instance() 
    {
      static TemplateManager manager;
      return manager;
    }
    static const Template *get_template_by_name(const char *name);
  };
  void load_templates();
  void save_templates();
  vector<const TemplateInfo*> linearize_field(const Template *t);
}
