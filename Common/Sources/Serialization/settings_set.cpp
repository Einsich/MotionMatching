#include "settings_set.h"


template<> VarType get_var_type<float>() { return VarType::FLOAT;}
template<> VarType get_var_type<int>() { return VarType::INT;}
template<> VarType get_var_type<bool>() { return VarType::BOOL;}
template<> VarType get_var_type<std::string>() { return VarType::LABEL;}
template<typename T> VarType get_var_type() { return VarType::UNKNOWN;}


