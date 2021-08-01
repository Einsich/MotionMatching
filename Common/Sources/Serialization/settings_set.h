#pragma once

#include "serialization.h"

enum class VarType
{
  FLOAT,
  INT,
  BOOL,
  LABEL,
  UNKNOWN
};
  #define VAR_MAPS() VAR_MAP(float); VAR_MAP(int); VAR_MAP(bool);
  

template<typename T> VarType get_var_type();


struct VarBase
{
  VarType varType;
  const std::string name;
  VarBase(VarType varType, const std::string &name):varType(varType), name(name)
  {}
};
template<typename T>
struct Var : public VarBase
{
  T &value;
  const T min_val, max_val;
  Var(const std::string &name, T& value, T min_val, T max_val):
  VarBase(get_var_type<T>(), name),
  value(value), min_val(min_val), max_val(max_val)
  {
  }
  void set_var(T new_value)
  {
    value = new_value < min_val ? min_val : max_val < new_value ? max_val : new_value;
  }
};
struct Label : public VarBase
{
  Label(const std::string &name):
  VarBase(VarType::LABEL, name)
  {}
};

class SettingsSet : public ISerializable
{
public:
  std::vector<VarBase*> vars;



  virtual size_t serialize(std::ostream& os) const override
  {
    #define VAR_MAP(type) std::map<string, type> type##Vars;
    VAR_MAPS();
    #undef VAR_MAP
    for (VarBase *varBase : vars)
    {
      switch (varBase->varType)
      {
        #define CASE_READ(TYPE, type) case VarType::TYPE: {Var<type>*var##type = (Var<type>*)varBase; type##Vars[varBase->name] = var##type->value; break;}
        CASE_READ(FLOAT, float)
        CASE_READ(INT, int)
        CASE_READ(BOOL, bool)
        default: break;
      }
    }
    #define VAR_MAP(type) size += write(os, type##Vars);
    size_t size = 0;
    VAR_MAPS();
    return size;
    #undef VAR_MAP 
  }
  virtual size_t deserialize(std::istream& is) override
  {
    size_t size = 0;
    #define VAR_MAP(type) std::map<string, type> type##Vars;
    VAR_MAPS();
    #undef VAR_MAP
    #define VAR_MAP(type) size += read(is, type##Vars);
    VAR_MAPS();
    #undef VAR_MAP 
    #define VAR_MAP(type) for(const auto &p : type##Vars) {auto it = std::find_if(vars.begin(), vars.end(), [&](const VarBase *varBase){return varBase->name == p.first;}); if (it != vars.end()) ((Var<type>*)(*it))->set_var(p.second);}
    VAR_MAPS();
    #undef VAR_MAP
    return size;
  }
};
#define DECL_FVAR(name, def, min, max) static inline float name = def;
#define DECL_IVAR(name, def, min, max) static inline int   name = def;
#define DECL_BVAR(name, def)           static inline bool  name = def;
          

#define INIT_FVAR(name, def, min, max) vars.push_back((VarBase*)(new Var<float>(#name, name, min, max)));
#define INIT_IVAR(name, def, min, max) vars.push_back((VarBase*)(new Var<int>  (#name, name, min, max)));
#define INIT_BVAR(name, def)           vars.push_back((VarBase*)(new Var<bool> (#name, name, false, true)));
#define INIT_LABEL(name)          vars.push_back((VarBase*)(new Label (name)));
