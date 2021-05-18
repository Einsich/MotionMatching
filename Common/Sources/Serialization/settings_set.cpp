#include "settings_set.h"


template<> VarType get_var_type<float>() { return VarType::FLOAT;}
template<> VarType get_var_type<int>() { return VarType::INT;}
template<> VarType get_var_type<bool>() { return VarType::BOOL;}
template<> VarType get_var_type<string>() { return VarType::LABEL;}


