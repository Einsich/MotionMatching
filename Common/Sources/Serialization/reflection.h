#pragma once
#include <type_traits>
#include "Serialization/serialization.h"

#define REFLECTION_WITH_FIELD_NAME 1
    

#define _PP_EVAL(...) __VA_ARGS__
#define _PP_EAT(...)

#define _PP_STR2(x) #x
#define _PP_STR(x) _PP_STR2(x)

#define _PP_MAP01(f, x) f(x)
#define _PP_MAP02(f, x, ...) f(x) _PP_EVAL(_PP_MAP01(f, __VA_ARGS__))
#define _PP_MAP03(f, x, ...) f(x) _PP_EVAL(_PP_MAP02(f, __VA_ARGS__))
#define _PP_MAP04(f, x, ...) f(x) _PP_EVAL(_PP_MAP03(f, __VA_ARGS__))
#define _PP_MAP05(f, x, ...) f(x) _PP_EVAL(_PP_MAP04(f, __VA_ARGS__))
#define _PP_MAP06(f, x, ...) f(x) _PP_EVAL(_PP_MAP05(f, __VA_ARGS__))
#define _PP_MAP07(f, x, ...) f(x) _PP_EVAL(_PP_MAP06(f, __VA_ARGS__))
#define _PP_MAP08(f, x, ...) f(x) _PP_EVAL(_PP_MAP07(f, __VA_ARGS__))
#define _PP_MAP09(f, x, ...) f(x) _PP_EVAL(_PP_MAP08(f, __VA_ARGS__))
#define _PP_MAP10(f, x, ...) f(x) _PP_EVAL(_PP_MAP09(f, __VA_ARGS__))
#define _PP_MAP11(f, x, ...) f(x) _PP_EVAL(_PP_MAP10(f, __VA_ARGS__))
#define _PP_MAP12(f, x, ...) f(x) _PP_EVAL(_PP_MAP11(f, __VA_ARGS__))
#define _PP_MAP13(f, x, ...) f(x) _PP_EVAL(_PP_MAP12(f, __VA_ARGS__))
#define _PP_MAP14(f, x, ...) f(x) _PP_EVAL(_PP_MAP13(f, __VA_ARGS__))
#define _PP_MAP15(f, x, ...) f(x) _PP_EVAL(_PP_MAP14(f, __VA_ARGS__))
#define _PP_MAP16(f, x, ...) f(x) _PP_EVAL(_PP_MAP15(f, __VA_ARGS__))
#define _PP_MAP17(f, x, ...) f(x) _PP_EVAL(_PP_MAP16(f, __VA_ARGS__))
#define _PP_MAP18(f, x, ...) f(x) _PP_EVAL(_PP_MAP17(f, __VA_ARGS__))
#define _PP_MAP19(f, x, ...) f(x) _PP_EVAL(_PP_MAP18(f, __VA_ARGS__))
#define _PP_MAP20(f, x, ...) f(x) _PP_EVAL(_PP_MAP19(f, __VA_ARGS__))
#define _PP_MAP21(f, x, ...) f(x) _PP_EVAL(_PP_MAP20(f, __VA_ARGS__))
#define _PP_MAP22(f, x, ...) f(x) _PP_EVAL(_PP_MAP21(f, __VA_ARGS__))
#define _PP_MAP23(f, x, ...) f(x) _PP_EVAL(_PP_MAP22(f, __VA_ARGS__))
#define _PP_MAP24(f, x, ...) f(x) _PP_EVAL(_PP_MAP23(f, __VA_ARGS__))
#define _PP_MAP25(f, x, ...) f(x) _PP_EVAL(_PP_MAP24(f, __VA_ARGS__))
#define _PP_MAP26(f, x, ...) f(x) _PP_EVAL(_PP_MAP25(f, __VA_ARGS__))
#define _PP_MAP27(f, x, ...) f(x) _PP_EVAL(_PP_MAP26(f, __VA_ARGS__))
#define _PP_MAP28(f, x, ...) f(x) _PP_EVAL(_PP_MAP27(f, __VA_ARGS__))
#define _PP_MAP29(f, x, ...) f(x) _PP_EVAL(_PP_MAP28(f, __VA_ARGS__))
#define _PP_MAP30(f, x, ...) f(x) _PP_EVAL(_PP_MAP29(f, __VA_ARGS__))
#define _PP_MAP31(f, x, ...) f(x) _PP_EVAL(_PP_MAP30(f, __VA_ARGS__))
#define _PP_MAP32(f, x, ...) f(x) _PP_EVAL(_PP_MAP31(f, __VA_ARGS__))
#define _PP_MAP33(f, x, ...) f(x) _PP_EVAL(_PP_MAP32(f, __VA_ARGS__))
#define _PP_MAP34(f, x, ...) f(x) _PP_EVAL(_PP_MAP33(f, __VA_ARGS__))
#define _PP_MAP35(f, x, ...) f(x) _PP_EVAL(_PP_MAP34(f, __VA_ARGS__))
#define _PP_MAP36(f, x, ...) f(x) _PP_EVAL(_PP_MAP35(f, __VA_ARGS__))
#define _PP_MAP37(f, x, ...) f(x) _PP_EVAL(_PP_MAP36(f, __VA_ARGS__))
#define _PP_MAP38(f, x, ...) f(x) _PP_EVAL(_PP_MAP37(f, __VA_ARGS__))
#define _PP_MAP39(f, x, ...) f(x) _PP_EVAL(_PP_MAP38(f, __VA_ARGS__))
#define _PP_MAP40(f, x, ...) f(x) _PP_EVAL(_PP_MAP39(f, __VA_ARGS__))
#define _PP_MAP41(f, x, ...) f(x) _PP_EVAL(_PP_MAP40(f, __VA_ARGS__))
#define _PP_MAP42(f, x, ...) f(x) _PP_EVAL(_PP_MAP41(f, __VA_ARGS__))
#define _PP_MAP43(f, x, ...) f(x) _PP_EVAL(_PP_MAP42(f, __VA_ARGS__))
#define _PP_MAP44(f, x, ...) f(x) _PP_EVAL(_PP_MAP43(f, __VA_ARGS__))
#define _PP_MAP45(f, x, ...) f(x) _PP_EVAL(_PP_MAP44(f, __VA_ARGS__))
#define _PP_MAP46(f, x, ...) f(x) _PP_EVAL(_PP_MAP45(f, __VA_ARGS__))
#define _PP_MAP47(f, x, ...) f(x) _PP_EVAL(_PP_MAP46(f, __VA_ARGS__))
#define _PP_MAP48(f, x, ...) f(x) _PP_EVAL(_PP_MAP47(f, __VA_ARGS__))
#define _PP_MAP49(f, x, ...) f(x) _PP_EVAL(_PP_MAP48(f, __VA_ARGS__))
#define _PP_MAP50(f, x, ...) f(x) _PP_EVAL(_PP_MAP49(f, __VA_ARGS__))
#define _PP_MAP51(f, x, ...) f(x) _PP_EVAL(_PP_MAP50(f, __VA_ARGS__))
#define _PP_MAP52(f, x, ...) f(x) _PP_EVAL(_PP_MAP51(f, __VA_ARGS__))
#define _PP_MAP53(f, x, ...) f(x) _PP_EVAL(_PP_MAP52(f, __VA_ARGS__))
#define _PP_MAP54(f, x, ...) f(x) _PP_EVAL(_PP_MAP53(f, __VA_ARGS__))
#define _PP_MAP55(f, x, ...) f(x) _PP_EVAL(_PP_MAP54(f, __VA_ARGS__))
#define _PP_MAP56(f, x, ...) f(x) _PP_EVAL(_PP_MAP55(f, __VA_ARGS__))
#define _PP_MAP57(f, x, ...) f(x) _PP_EVAL(_PP_MAP56(f, __VA_ARGS__))
#define _PP_MAP58(f, x, ...) f(x) _PP_EVAL(_PP_MAP57(f, __VA_ARGS__))
#define _PP_MAP59(f, x, ...) f(x) _PP_EVAL(_PP_MAP58(f, __VA_ARGS__))
#define _PP_MAP60(f, x, ...) f(x) _PP_EVAL(_PP_MAP59(f, __VA_ARGS__))
#define _PP_MAP61(f, x, ...) f(x) _PP_EVAL(_PP_MAP60(f, __VA_ARGS__))
#define _PP_MAP62(f, x, ...) f(x) _PP_EVAL(_PP_MAP61(f, __VA_ARGS__))
#define _PP_MAP63(f, x, ...) f(x) _PP_EVAL(_PP_MAP62(f, __VA_ARGS__))
#define _PP_MAP64(f, x, ...) f(x) _PP_EVAL(_PP_MAP63(f, __VA_ARGS__))

#define _PP_GET_NTH_ARG( \
  _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
  _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
  _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
  _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
  _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
  _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
  _61, _62, _63, _64, N, ...) N

#define _PP_MAP(f, ...) _PP_EVAL(_PP_EVAL(_PP_GET_NTH_ARG(__VA_ARGS__, \
  _PP_MAP64, _PP_MAP63, _PP_MAP62, _PP_MAP61, \
  _PP_MAP60, _PP_MAP59, _PP_MAP58, _PP_MAP57, _PP_MAP56, \
  _PP_MAP55, _PP_MAP54, _PP_MAP53, _PP_MAP52, _PP_MAP51, \
  _PP_MAP50, _PP_MAP49, _PP_MAP48, _PP_MAP47, _PP_MAP46, \
  _PP_MAP45, _PP_MAP44, _PP_MAP43, _PP_MAP42, _PP_MAP41, \
  _PP_MAP40, _PP_MAP39, _PP_MAP38, _PP_MAP37, _PP_MAP36, \
  _PP_MAP35, _PP_MAP34, _PP_MAP33, _PP_MAP32, _PP_MAP31, \
  _PP_MAP30, _PP_MAP29, _PP_MAP28, _PP_MAP27, _PP_MAP26, \
  _PP_MAP25, _PP_MAP24, _PP_MAP23, _PP_MAP22, _PP_MAP21, \
  _PP_MAP20, _PP_MAP19, _PP_MAP18, _PP_MAP17, _PP_MAP16, \
  _PP_MAP15, _PP_MAP14, _PP_MAP13, _PP_MAP12, _PP_MAP11, \
  _PP_MAP10, _PP_MAP09, _PP_MAP08, _PP_MAP07, _PP_MAP06, \
  _PP_MAP05, _PP_MAP04, _PP_MAP03, _PP_MAP02, _PP_MAP01 \
  ))(f, __VA_ARGS__))

#if REFLECTION_WITH_FIELD_NAME
#define _PP_REFLECTION_FIELD_NAME(x) _PP_STR(_PP_EVAL(x))
#else
#define _PP_REFLECTION_FIELD_NAME(x) ""
#endif

#define _PP_REFLECTION_ALL(x) _PP_EVAL x
#define _PP_REFLECTION_SECOND(x) _PP_EAT x
#define _PP_REFLECTION_FIELD(x) _PP_REFLECTION_ALL(x);
#define _PP_REFLECTION_METHOD2(x) callable(x, _PP_REFLECTION_FIELD_NAME(x));
#define _PP_REFLECTION_METHOD(x) _PP_REFLECTION_METHOD2(_PP_REFLECTION_SECOND(x))

#define _PP_REFLECTION_PROPERTIES(...) \
template<typename TCallable> \
  void reflect(TCallable&& callable) { \
    _PP_MAP(_PP_REFLECTION_METHOD, __VA_ARGS__) \
} \
template<typename TCallable> \
  void reflect(TCallable&& callable) const { \
    _PP_MAP(_PP_REFLECTION_METHOD, __VA_ARGS__) \
}
#define OBJECT_NAME_METHOD(obj) \
  static const char* class_name() { \
    return #obj; \
  }
#define REFLECT(class_name, ...) \
  OBJECT_NAME_METHOD(class_name)\
  _PP_MAP(_PP_REFLECTION_FIELD, __VA_ARGS__) \
  _PP_REFLECTION_PROPERTIES(__VA_ARGS__)


template <typename T>
class HasReflection
{
private:
    typedef char YesType[1];
    typedef char NoType[2];

    template <typename C> static YesType& test( decltype(&C::class_name) ) ;
    template <typename C> static NoType& test(...);


public:
    enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
};
