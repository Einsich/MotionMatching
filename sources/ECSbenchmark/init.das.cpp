#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
 // require strings_boost
 // require ecs_core
#include <ecs/ecs.h>
#include <das_modules/ecs_module.h>
 // require engine
#include <application/time.h>
 // require ast_boost
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require is_local
 // require safe_addr
 // require ecs

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)   // unreferenced formal parameter
#pragma warning(disable:4189)   // local variable is initialized but not referenced
#pragma warning(disable:4244)   // conversion from 'int32_t' to 'float', possible loss of data
#pragma warning(disable:4114)   // same qualifier more than once
#pragma warning(disable:4623)   // default constructor was implicitly defined as deleted
#pragma warning(disable:4946)   // reinterpret_cast used between related classes
#pragma warning(disable:4269)   // 'const' automatic data initialized with compiler generated default constructor produces unreliable results
#pragma warning(disable:4555)   // result of expression not used
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wwritable-strings"
#pragma clang diagnostic ignored "-Wunused-variable"
#if defined(__APPLE__)
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#endif
#pragma clang diagnostic ignored "-Wunsequenced"
#pragma clang diagnostic ignored "-Wunused-function"
#endif

namespace das {
namespace _anon_13735057341551878539 {

extern TypeInfo __type_info__bef6c19aaa05a9d;
extern TypeInfo __type_info__90f5007b5a6ea5c;
extern TypeInfo __type_info__90f3d07b5a6ca13;
extern TypeInfo __type_info__af63eb4c86020609;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__8988d07b5422b9e;
extern TypeInfo __type_info__90f5707b5a6f641;
extern VarInfo __var_info__2208c7192622eea2;
extern VarInfo __var_info__79f1ef3229e115c6;
extern VarInfo __var_info__ef82589224d0053c;
extern VarInfo __var_info__a437a95e4945ca7d;
extern VarInfo __var_info__981b064e320b97cf;
extern VarInfo __var_info__33667e801e8cf4d8;
extern VarInfo __var_info__3c8ac0f1d7405994;
extern FuncInfo __func_info__55fb71370c77bca7;
extern FuncInfo __func_info__87e49841c6409ea;
extern FuncInfo __func_info__80fe84e807e9d70e;

VarInfo __func_info__55fb71370c77bca7_field_0 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x2208c7192622eea2), "n", 0 };
VarInfo __func_info__55fb71370c77bca7_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x3c8ac0f1d7405994), "counter__ptr__", 0 };
VarInfo * __func_info__55fb71370c77bca7_fields[2] =  { &__func_info__55fb71370c77bca7_field_0, &__func_info__55fb71370c77bca7_field_1 };
FuncInfo __func_info__55fb71370c77bca7 = {"invoke block<(n:uint;counter__ptr__:int -const?):void> const", "", __func_info__55fb71370c77bca7_fields, 2, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x55fb71370c77bca7), 0x0 };
VarInfo __func_info__87e49841c6409ea_field_0 =  { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x2208c7192622eea2), "n", 0 };
VarInfo __func_info__87e49841c6409ea_field_1 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__90f5007b5a6ea5c, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xa437a95e4945ca7d), "m__ptr__", 0 };
VarInfo __func_info__87e49841c6409ea_field_2 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8988d07b5422b9e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x79f1ef3229e115c6), "pos__ptr__", 0 };
VarInfo __func_info__87e49841c6409ea_field_3 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8988d07b5422b9e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x981b064e320b97cf), "vel__ptr__", 0 };
VarInfo __func_info__87e49841c6409ea_field_4 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__bef6c19aaa05a9d, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0x33667e801e8cf4d8), "center__ptr__", 0 };
VarInfo * __func_info__87e49841c6409ea_fields[5] =  { &__func_info__87e49841c6409ea_field_0, &__func_info__87e49841c6409ea_field_1, &__func_info__87e49841c6409ea_field_2, &__func_info__87e49841c6409ea_field_3, &__func_info__87e49841c6409ea_field_4 };
FuncInfo __func_info__87e49841c6409ea = {"invoke block<(n:uint;m__ptr__:float const?;pos__ptr__:float3 -const?;vel__ptr__:float3 -const?;center__ptr__:float3 const?):void> const", "", __func_info__87e49841c6409ea_fields, 5, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x87e49841c6409ea), 0x0 };
VarInfo __func_info__80fe84e807e9d70e_field_0 =  { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ecs_core::ComponentInitializer"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 66, 24, UINT64_C(0xef82589224d0053c), "init", 0 };
VarInfo * __func_info__80fe84e807e9d70e_fields[1] =  { &__func_info__80fe84e807e9d70e_field_0 };
FuncInfo __func_info__80fe84e807e9d70e = {"invoke block<(init:ecs_core::ComponentInitializer#):void> const", "", __func_info__80fe84e807e9d70e_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x80fe84e807e9d70e), 0x0 };
TypeInfo __type_info__bef6c19aaa05a9d = { Type::tFloat3, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 12, UINT64_C(0xbef6c19aaa05a9d) };
TypeInfo __type_info__90f5007b5a6ea5c = { Type::tFloat, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0x90f5007b5a6ea5c) };
TypeInfo __type_info__90f3d07b5a6ca13 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0x90f3d07b5a6ca13) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__8988d07b5422b9e = { Type::tFloat3, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 12, UINT64_C(0x8988d07b5422b9e) };
TypeInfo __type_info__90f5707b5a6f641 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0x90f5707b5a6f641) };

static void resolveTypeInfoAnnotations()
{
}

TypeInfo * __tinfo_0[9] = { &__type_info__90f3d07b5a6ca13, &__type_info__90f5707b5a6f641, &__type_info__90f3d07b5a6ca13, &__type_info__90f5007b5a6ea5c, &__type_info__90f3d07b5a6ca13, &__type_info__90f3d07b5a6ca13, &__type_info__90f3d07b5a6ca13, &__type_info__bef6c19aaa05a9d, &__type_info__90f3d07b5a6ca13 };
TypeInfo * __tinfo_1[5] = { &__type_info__90f3d07b5a6ca13, &__type_info__90f5707b5a6f641, &__type_info__90f3d07b5a6ca13, &__type_info__90f3d07b5a6ca13, &__type_info__90f3d07b5a6ca13 };
namespace  { struct XXXStruct; };
namespace  { struct TestEvent; };
namespace  { struct MyEvent; };
namespace  { struct TestRequest; };
namespace ast { struct AstFunctionAnnotation; };
namespace ast { struct AstBlockAnnotation; };
namespace ast { struct AstStructureAnnotation; };
namespace ast { struct AstPassMacro; };
namespace ast { struct AstVariantMacro; };
namespace ast { struct AstForLoopMacro; };
namespace ast { struct AstCaptureMacro; };
namespace ast { struct AstSimulateMacro; };
namespace ast { struct AstReaderMacro; };
namespace ast { struct AstCommentReader; };
namespace ast { struct AstCallMacro; };
namespace ast { struct AstTypeInfoMacro; };
namespace ast { struct AstEnumerationAnnotation; };
namespace ast { struct AstVisitor; };
namespace ast_boost { struct MacroMacro; };
namespace ast_boost { struct TagFunctionAnnotation; };
namespace ast_boost { struct TagStructureAnnotation; };
namespace ast_boost { struct SetupAnyAnnotation; };
namespace ast_boost { struct SetupFunctionAnnotatoin; };
namespace ast_boost { struct SetupBlockAnnotatoin; };
namespace ast_boost { struct SetupStructureAnnotatoin; };
namespace ast_boost { struct SetupEnumerationAnnotation; };
namespace ast_boost { struct SetupContractAnnotatoin; };
namespace ast_boost { struct SetupReaderMacro; };
namespace ast_boost { struct SetupCommentReader; };
namespace ast_boost { struct SetupVariantMacro; };
namespace ast_boost { struct SetupForLoopMacro; };
namespace ast_boost { struct SetupCaptureMacro; };
namespace ast_boost { struct SetupSimulateMacro; };
namespace ast_boost { struct SetupCallMacro; };
namespace ast_boost { struct SetupTypeInfoMacro; };
namespace ast_boost { struct SetupInferMacro; };
namespace ast_boost { struct SetupDirtyInferMacro; };
namespace ast_boost { struct SetupLintMacro; };
namespace ast_boost { struct SetupGlobalLintMacro; };
namespace ast_boost { struct SetupOptimizationMacro; };
namespace ast_boost { struct TagFunctionMacro; };
namespace ast_boost { struct BetterRttiVisitor; };
namespace templates { struct DecltypeMacro; };
namespace templates { struct DecltypeNoRefMacro; };
namespace templates { struct TemplateMacro; };
namespace contracts { struct IsAnyType; };
namespace contracts { struct IsAnyArrayMacro; };
namespace contracts { struct IsDimMacro; };
namespace contracts { struct IsNotDimMacro; };
namespace contracts { struct IsAnyEnumMacro; };
namespace contracts { struct IsAnyVectorType; };
namespace contracts { struct IsAnyStructMacro; };
namespace contracts { struct IsAnyNumericMacro; };
namespace contracts { struct IsAnyWorkhorse; };
namespace contracts { struct IsAnyWorkhorseNonPtrMacro; };
namespace contracts { struct IsAnyTupleNonPtrMacro; };
namespace contracts { struct IsAnyVariantNonPtrMacro; };
namespace contracts { struct IsAnyFunctionNonPtrMacro; };
namespace templates_boost { struct Template; };
namespace templates_boost { struct TemplateVisitor; };
namespace templates_boost { struct RemoveDerefVisitor; };
namespace templates_boost { struct QRulesVisitor; };
namespace templates_boost { struct AstQCallMacro; };
namespace templates_boost { struct QMacro; };
namespace templates_boost { struct QBlockMacro; };
namespace templates_boost { struct QBlockToArrayMacro; };
namespace templates_boost { struct QBlockExprMacro; };
namespace templates_boost { struct QTypeMacro; };
namespace templates_boost { struct AstQNamedMacro; };
namespace templates_boost { struct AstQFunctionMacro; };
namespace templates_boost { struct AstQVariableMacro; };
namespace templates_boost { struct AstQNamedClassMacro; };
namespace templates_boost { struct AstQMethodMacro; };
namespace safe_addr { struct SafeAddrMacro; };
namespace safe_addr { struct SharedAddrMacro; };
namespace ecs { struct ECSComponentTypeId; };
namespace ecs { struct ECSEventTypeId; };
namespace ecs { struct ECSRequestTypeId; };
namespace ecs { struct QueryArguments; };
namespace ecs { struct ECSQueryMacro; };
namespace ecs { struct ECSSystemMacro; };
namespace ecs { struct CppEvent; };
namespace ecs { struct CppRequest; };
namespace ecs { struct DasEvent; };
namespace ecs { struct DasRequest; };
// unused enumeration CompilationError
// unused enumeration Type
// unused enumeration RefMatters
// unused enumeration ConstMatters
// unused enumeration TemporaryMatters
// unused enumeration SideEffects
// unused enumeration CaptureMode
// unused structure AstFunctionAnnotation
// unused structure AstBlockAnnotation
// unused structure AstStructureAnnotation
// unused structure AstPassMacro
// unused structure AstVariantMacro
// unused structure AstForLoopMacro
// unused structure AstCaptureMacro
// unused structure AstSimulateMacro
// unused structure AstReaderMacro
// unused structure AstCommentReader
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
// unused structure AstVisitor
// unused structure MacroMacro
// unused structure TagFunctionAnnotation
// unused structure TagStructureAnnotation
// unused structure SetupAnyAnnotation
// unused structure SetupFunctionAnnotatoin
// unused structure SetupBlockAnnotatoin
// unused structure SetupStructureAnnotatoin
// unused structure SetupEnumerationAnnotation
// unused structure SetupContractAnnotatoin
// unused structure SetupReaderMacro
// unused structure SetupCommentReader
// unused structure SetupVariantMacro
// unused structure SetupForLoopMacro
// unused structure SetupCaptureMacro
// unused structure SetupSimulateMacro
// unused structure SetupCallMacro
// unused structure SetupTypeInfoMacro
// unused structure SetupInferMacro
// unused structure SetupDirtyInferMacro
// unused structure SetupLintMacro
// unused structure SetupGlobalLintMacro
// unused structure SetupOptimizationMacro
// unused structure TagFunctionMacro
// unused structure BetterRttiVisitor
// unused structure DecltypeMacro
// unused structure DecltypeNoRefMacro
// unused structure TemplateMacro
// unused structure IsAnyType
// unused structure IsAnyArrayMacro
// unused structure IsDimMacro
// unused structure IsNotDimMacro
// unused structure IsAnyEnumMacro
// unused structure IsAnyVectorType
// unused structure IsAnyStructMacro
// unused structure IsAnyNumericMacro
// unused structure IsAnyWorkhorse
// unused structure IsAnyWorkhorseNonPtrMacro
// unused structure IsAnyTupleNonPtrMacro
// unused structure IsAnyVariantNonPtrMacro
// unused structure IsAnyFunctionNonPtrMacro
// unused structure Template
// unused structure TemplateVisitor
// unused structure RemoveDerefVisitor
// unused structure QRulesVisitor
// unused structure AstQCallMacro
// unused structure QMacro
// unused structure QBlockMacro
// unused structure QBlockToArrayMacro
// unused structure QBlockExprMacro
// unused structure QTypeMacro
// unused structure AstQNamedMacro
// unused structure AstQFunctionMacro
// unused structure AstQVariableMacro
// unused structure AstQNamedClassMacro
// unused structure AstQMethodMacro
// unused structure SafeAddrMacro
// unused structure SharedAddrMacro
// unused enumeration QueryType
// unused structure ECSComponentTypeId
// unused structure ECSEventTypeId
// unused structure ECSRequestTypeId
// unused structure QueryArguments
// unused structure ECSQueryMacro
// unused structure ECSSystemMacro
// unused structure CppEvent
// unused structure CppRequest
// unused structure DasEvent
// unused structure DasRequest
namespace  {

struct XXXStruct {
	int32_t inc;
};
static_assert(sizeof(XXXStruct)==4,"structure size mismatch with DAS");
static_assert(offsetof(XXXStruct,inc)==0,"structure field offset mismatch with DAS");
}
namespace  {

struct TestEvent {
	int32_t inc;
};
static_assert(sizeof(TestEvent)==4,"structure size mismatch with DAS");
static_assert(offsetof(TestEvent,inc)==0,"structure field offset mismatch with DAS");
}
namespace  {

struct MyEvent {
	int32_t x;
	int32_t y;
	char * z;
	int32_t w;
};
static_assert(sizeof(MyEvent)==24,"structure size mismatch with DAS");
static_assert(offsetof(MyEvent,x)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(MyEvent,y)==4,"structure field offset mismatch with DAS");
static_assert(offsetof(MyEvent,z)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(MyEvent,w)==16,"structure field offset mismatch with DAS");
}
namespace  {

struct TestRequest {
	int32_t sum;
};
static_assert(sizeof(TestRequest)==4,"structure size mismatch with DAS");
static_assert(offsetof(TestRequest,sum)==0,"structure field offset mismatch with DAS");
}

inline void _Funcecs_0x60_send_event_668f484c5594ade2 ( Context * __context__, MyEvent const  & __event_rename_at_35 );
inline void test_func_a6fab27e71e3efab ( Context * __context__, float __dt_rename_at_12, float __m_rename_at_12, float3 & __pos_rename_at_12, float3 & __vel_rename_at_12, float3 __center_rename_at_12 );
inline void das_system_test__implementation___d296e0466911cef8 ( Context * __context__, uint32_t __n_rename_at_251, float const  * __m__ptr___rename_at_251, float3 * __pos__ptr___rename_at_251, float3 * __vel__ptr___rename_at_251, float3 const  * __center__ptr___rename_at_251 );
inline void das_system_test_b809060a654d2132 ( Context * __context__, float __m_rename_at_22, float3 & __pos_rename_at_22, float3 & __vel_rename_at_22, float3 __center_rename_at_22 );
inline void das_query_test__implementation___e3bba28dd465e49 ( Context * __context__, uint32_t __n_rename_at_251 );
inline void das_query_test_c1cafffdc575571b ( Context * __context__ );
inline void created_event__implementation___d5eea9a330bd499d ( Context * __context__, uint32_t __n_rename_at_251, ::ecs::OnSceneCreated const  * __event__ptr___rename_at_251 );
inline void created_event_e699080f31c6d7d4 ( Context * __context__, ::ecs::OnSceneCreated const  & __event_rename_at_47 );
inline void created_event__implementation___96d8b7817d60378b ( Context * __context__, uint32_t __n_rename_at_251, ::ecs::OnEntityCreated const  * __event__ptr___rename_at_251, int32_t const  * __i__ptr___rename_at_251, float const  * __f__ptr___rename_at_251, ::ecs::string * __s__ptr___rename_at_251, float3 const  * __f3__ptr___rename_at_251 );
inline void created_event_49bc7bf17abc6b37 ( Context * __context__, ::ecs::OnEntityCreated const  & __event_rename_at_77, int32_t __i_rename_at_77, float __f_rename_at_77, char * const  __s_rename_at_77, float3 __f3_rename_at_77 );
inline void created_event__implementation___b948e1c63201b45e ( Context * __context__, uint32_t __n_rename_at_251, ::ecs::OnEntityCreated const  * __event__ptr___rename_at_251, int32_t const  * __counter__ptr___rename_at_251, ::ecs::string * __name__ptr___rename_at_251 );
inline void created_event_6af492e36368737f ( Context * __context__, ::ecs::OnEntityCreated const  & __event_rename_at_82, int32_t __counter_rename_at_82, char * const  __name_rename_at_82 );
inline void my_event_handler__implementation___7a7cc6957ef84965 ( Context * __context__, uint32_t __n_rename_at_251, MyEvent const  * __event__ptr___rename_at_251 );
inline void my_event_handler_6fd7d4d78f664169 ( Context * __context__, MyEvent const  & __event_rename_at_98 );
inline void creation_test__implementation___1b3063228cb52c91 ( Context * __context__, uint32_t __n_rename_at_251 );
inline void creation_test_b0e874b95a9256e6 ( Context * __context__ );
inline void test_event__implementation___434dd1282c05ca15 ( Context * __context__, uint32_t __n_rename_at_251, TestEvent const  * __event__ptr___rename_at_251, ::ecs::EntityId const  * __eid__ptr___rename_at_251 );
inline void test_event_6b4f46df34f87c64 ( Context * __context__, TestEvent const  & __event_rename_at_113, ::ecs::EntityId const  __eid_rename_at_113 );
inline void test_request__implementation___f98aee5d0ea64e90 ( Context * __context__, uint32_t __n_rename_at_251, TestRequest * __request__ptr___rename_at_251, int32_t const  * __counter__ptr___rename_at_251 );
inline void test_request_285b8f4d0bf632af ( Context * __context__, TestRequest & __request_rename_at_126, int32_t __counter_rename_at_126 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _Funcecs_0x60_send_event_668f484c5594ade2 ( Context * __context__, MyEvent const  &  __event_rename_at_35 )
{
	int32_t __typeId_rename_at_37 = ((int32_t)::ecs::event_name_to_index(((char *) "MyEvent")));
	if ( __typeId_rename_at_37 >= 0 )
	{
		::builtin_send_event(24,__typeId_rename_at_37,das_auto_cast<void * const >::cast(das_ref(__context__,__event_rename_at_35)));
	} else {
		builtin_print(((char *) "need to call default initializer in send_event([[MyEvent() ...]])\n"),__context__);
	};
}

inline void test_func_a6fab27e71e3efab ( Context * __context__, float __dt_rename_at_12, float __m_rename_at_12, float3 & __pos_rename_at_12, float3 & __vel_rename_at_12, float3 __center_rename_at_12 )
{
	float3 __dir_rename_at_13 = ((float3)(SimPolicy<float3>::Sub(__center_rename_at_12,__pos_rename_at_12,*__context__,nullptr)));
	float __distance_rename_at_14 = ((float)length3(__dir_rename_at_13));
	float3 __F_rename_at_15 = ((float3)(SimPolicy<float3>::DivVecScal((SimPolicy<float3>::MulVecScal((SimPolicy<float3>::MulVecScal(__dir_rename_at_13,cast<float>::from(__m_rename_at_12),*__context__,nullptr)),cast<float>::from(9.99999975e-05f),*__context__,nullptr)),cast<float>::from(((__distance_rename_at_14 * __distance_rename_at_14) * __distance_rename_at_14)),*__context__,nullptr)));
	float3 __a_rename_at_16 = ((float3)(SimPolicy<float3>::DivVecScal(__F_rename_at_15,cast<float>::from(__m_rename_at_12),*__context__,nullptr)));
	SimPolicy<float3>::SetAdd((char *)&(__vel_rename_at_12),(SimPolicy<float3>::MulVecScal(__a_rename_at_16,cast<float>::from(__dt_rename_at_12),*__context__,nullptr)),*__context__,nullptr);
	SimPolicy<float3>::SetAdd((char *)&(__pos_rename_at_12),(SimPolicy<float3>::MulVecScal(__vel_rename_at_12,cast<float>::from(__dt_rename_at_12),*__context__,nullptr)),*__context__,nullptr);
}

inline void das_system_test__implementation___d296e0466911cef8 ( Context * __context__, uint32_t __n_rename_at_251, float const  * __m__ptr___rename_at_251, float3 * __pos__ptr___rename_at_251, float3 * __vel__ptr___rename_at_251, float3 const  * __center__ptr___rename_at_251 )
{
	{
		bool __need_loop_253 = true;
		// i : int const
		das_iterator<range> __i_iterator(mk_range(__n_rename_at_251));
		int32_t __i_rename_at_253;
		__need_loop_253 = __i_iterator.first(__context__,(__i_rename_at_253)) && __need_loop_253;
		for ( ; __need_loop_253 ; __need_loop_253 = __i_iterator.next(__context__,(__i_rename_at_253)) )
		{
			das_system_test_b809060a654d2132(__context__,das_deref(__context__,__m__ptr___rename_at_251),das_deref(__context__,__pos__ptr___rename_at_251),das_deref(__context__,__vel__ptr___rename_at_251),das_deref(__context__,__center__ptr___rename_at_251));
			das_ptr_inc(__m__ptr___rename_at_251,4);
			das_ptr_inc(__pos__ptr___rename_at_251,12);
			das_ptr_inc(__vel__ptr___rename_at_251,12);
			das_ptr_inc(__center__ptr___rename_at_251,12);
		}
		__i_iterator.close(__context__,(__i_rename_at_253));
	};
}

inline void das_system_test_b809060a654d2132 ( Context * __context__, float __m_rename_at_22, float3 & __pos_rename_at_22, float3 & __vel_rename_at_22, float3 __center_rename_at_22 )
{
	test_func_a6fab27e71e3efab(__context__,::get_delta_time(),__m_rename_at_22,__pos_rename_at_22,__vel_rename_at_22,__center_rename_at_22);
}

inline void das_query_test__implementation___e3bba28dd465e49 ( Context * __context__, uint32_t __n_rename_at_251 )
{
	{
		bool __need_loop_253 = true;
		// i : int const
		das_iterator<range> __i_iterator(mk_range(__n_rename_at_251));
		int32_t __i_rename_at_253;
		__need_loop_253 = __i_iterator.first(__context__,(__i_rename_at_253)) && __need_loop_253;
		for ( ; __need_loop_253 ; __need_loop_253 = __i_iterator.next(__context__,(__i_rename_at_253)) )
		{
			das_query_test_c1cafffdc575571b(__context__);
		}
		__i_iterator.close(__context__,(__i_rename_at_253));
	};
}

inline void das_query_test_c1cafffdc575571b ( Context * __context__ ) { das_stack_prologue __prologue(__context__,192,"das_query_test " DAS_FILE_LINE);
{
	float __dt_rename_at_28 = ((float)::get_delta_time());
	::perform_query(das_make_block<void,uint32_t,float const  *,float3 *,float3 *,float3 const  *>(__context__,96,0,&__func_info__87e49841c6409ea,[&](uint32_t __n_rename_at_0, float const  * __m__ptr___rename_at_29, float3 * __pos__ptr___rename_at_29, float3 * __vel__ptr___rename_at_29, float3 const  * __center__ptr___rename_at_29) -> void{
		{
			bool __need_loop_183 = true;
			// i : int const
			das_iterator<range> __i_iterator(mk_range(__n_rename_at_0));
			int32_t __i_rename_at_183;
			__need_loop_183 = __i_iterator.first(__context__,(__i_rename_at_183)) && __need_loop_183;
			for ( ; __need_loop_183 ; __need_loop_183 = __i_iterator.next(__context__,(__i_rename_at_183)) )
			{
				das_invoke<void>::invoke<float,float3 &,float3 &,float3>(__context__,nullptr,[&](float const  __m_rename_at_184, float3 & __pos_rename_at_184, float3 & __vel_rename_at_184, float3 __center_rename_at_184) DAS_AOT_INLINE_LAMBDA -> void{
					test_func_a6fab27e71e3efab(__context__,__dt_rename_at_28,__m_rename_at_184,__pos_rename_at_184,__vel_rename_at_184,__center_rename_at_184);
				},das_deref(__context__,__m__ptr___rename_at_29),das_deref(__context__,__pos__ptr___rename_at_29),das_deref(__context__,__vel__ptr___rename_at_29),das_deref(__context__,__center__ptr___rename_at_29));
				das_ptr_inc(__m__ptr___rename_at_29,4);
				das_ptr_inc(__pos__ptr___rename_at_29,12);
				das_ptr_inc(__vel__ptr___rename_at_29,12);
				das_ptr_inc(__center__ptr___rename_at_29,12);
			}
			__i_iterator.close(__context__,(__i_rename_at_183));
		};
		return ;
	}),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline void created_event__implementation___d5eea9a330bd499d ( Context * __context__, uint32_t __n_rename_at_251, ::ecs::OnSceneCreated const  * __event__ptr___rename_at_251 )
{
	{
		bool __need_loop_253 = true;
		// i : int const
		das_iterator<range> __i_iterator(mk_range(__n_rename_at_251));
		int32_t __i_rename_at_253;
		__need_loop_253 = __i_iterator.first(__context__,(__i_rename_at_253)) && __need_loop_253;
		for ( ; __need_loop_253 ; __need_loop_253 = __i_iterator.next(__context__,(__i_rename_at_253)) )
		{
			created_event_e699080f31c6d7d4(__context__,das_deref(__context__,__event__ptr___rename_at_251));
			das_ptr_inc(__event__ptr___rename_at_251,1);
		}
		__i_iterator.close(__context__,(__i_rename_at_253));
	};
}

inline void created_event_e699080f31c6d7d4 ( Context * __context__, ::ecs::OnSceneCreated const  &  __event_rename_at_47 ) { das_stack_prologue __prologue(__context__,320,"created_event " DAS_FILE_LINE);
{
	builtin_print(((char *) "hello world!\n"),__context__);
	builtin_print(((char *) "hello world!!\n"),__context__);
	builtin_print(((char *) "hello world!!!\n"),__context__);
	uint32_t __p_rename_at_52 = ((uint32_t)::create_entity_prefab(((char *) "das_prefab"),das_make_block<void,ComponentInitializer &>(__context__,96,0,&__func_info__80fe84e807e9d70e,[&](ComponentInitializer & __init_rename_at_52) -> void{
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXi*/ 16861268835962280491u)),das_arg<ComponentInitializer>::pass(__init_rename_at_52),((char *) "i"),1);
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,float>(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXf*/ 16861265537427395858u)),das_arg<ComponentInitializer>::pass(__init_rename_at_52),((char *) "f"),1.f);
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXs*/ 16861279831078562601u)),das_arg<ComponentInitializer>::pass(__init_rename_at_52),((char *) "s"),((char *) "xxxx0000xxxiiii"));
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,float3>(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXf3*/ 9371518136105281299u)),das_arg<ComponentInitializer>::pass(__init_rename_at_52),((char *) "f3"),v_make_vec4f(1.f,2.f,3.f,0.f));
	}),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
	::ecs::EntityId __eid_rename_at_62 = ((::ecs::EntityId)::create_entity_with_init(__p_rename_at_52,das_make_block<void,ComponentInitializer &>(__context__,176,0,&__func_info__80fe84e807e9d70e,[&](ComponentInitializer & __init_rename_at_62) -> void{
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXs*/ 16861279831078562601u)),das_arg<ComponentInitializer>::pass(__init_rename_at_62),((char *) "s"),((char *) "lolkek"));
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXi*/ 16861268835962280491u)),das_arg<ComponentInitializer>::pass(__init_rename_at_62),((char *) "i"),55);
	}),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
	::create_entity_with_init(__p_rename_at_52,das_make_block<void,ComponentInitializer &>(__context__,240,0,&__func_info__80fe84e807e9d70e,[&](ComponentInitializer & __init_rename_at_66) -> void{
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXs*/ 16861279831078562601u)),das_arg<ComponentInitializer>::pass(__init_rename_at_66),((char *) "s"),((char *) "chebureck"));
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXi*/ 16861268835962280491u)),das_arg<ComponentInitializer>::pass(__init_rename_at_66),((char *) "i"),1111111111);
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,::ecs::EntityId const >(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXH<ecs_core::EntityId>*/ 9401185574874402403u)),das_arg<ComponentInitializer>::pass(__init_rename_at_66),((char *) "e"),__eid_rename_at_62);
	}),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
	::create_entity_immediate_with_init_n(((char *) "das_prefab"),das_make_block<void,ComponentInitializer &>(__context__,304,0,&__func_info__80fe84e807e9d70e,[&](ComponentInitializer & __init_rename_at_72) -> void{
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,char * const >(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXs*/ 16861279831078562601u)),das_arg<ComponentInitializer>::pass(__init_rename_at_72),((char *) "s"),((char *) "immediate"));
		das_invoke_function<void>::invoke<ComponentInitializer &,char * const ,int32_t>(__context__,nullptr,Func(__context__->fnByMangledName(/*ecs`set IH<ecs_core::ComponentInitializer> Cs CXi*/ 16861268835962280491u)),das_arg<ComponentInitializer>::pass(__init_rename_at_72),((char *) "i"),609);
	}),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline void created_event__implementation___96d8b7817d60378b ( Context * __context__, uint32_t __n_rename_at_251, ::ecs::OnEntityCreated const  * __event__ptr___rename_at_251, int32_t const  * __i__ptr___rename_at_251, float const  * __f__ptr___rename_at_251, ::ecs::string * __s__ptr___rename_at_251, float3 const  * __f3__ptr___rename_at_251 )
{
	{
		bool __need_loop_253 = true;
		// i : int const
		das_iterator<range> __i_iterator(mk_range(__n_rename_at_251));
		int32_t __i_rename_at_253;
		__need_loop_253 = __i_iterator.first(__context__,(__i_rename_at_253)) && __need_loop_253;
		for ( ; __need_loop_253 ; __need_loop_253 = __i_iterator.next(__context__,(__i_rename_at_253)) )
		{
			created_event_49bc7bf17abc6b37(__context__,das_deref(__context__,__event__ptr___rename_at_251),das_deref(__context__,__i__ptr___rename_at_251),das_deref(__context__,__f__ptr___rename_at_251),((char * const )(das_deref(__context__,__s__ptr___rename_at_251).c_str() /*c_str*/)),das_deref(__context__,__f3__ptr___rename_at_251));
			das_ptr_inc(__event__ptr___rename_at_251,1);
			das_ptr_inc(__i__ptr___rename_at_251,4);
			das_ptr_inc(__f__ptr___rename_at_251,4);
			das_ptr_inc(__s__ptr___rename_at_251,32);
			das_ptr_inc(__f3__ptr___rename_at_251,12);
		}
		__i_iterator.close(__context__,(__i_rename_at_253));
	};
}

inline void created_event_49bc7bf17abc6b37 ( Context * __context__, ::ecs::OnEntityCreated const  &  __event_rename_at_77, int32_t __i_rename_at_77, float __f_rename_at_77, char * const  __s_rename_at_77, float3 __f3_rename_at_77 )
{
	builtin_print(das_string_builder(__context__,SimNode_AotInterop<9>(__tinfo_0, cast<char * const >::from(((char *) "hello i=")), cast<int32_t>::from(__i_rename_at_77), cast<char * const >::from(((char *) " f=")), cast<float>::from(__f_rename_at_77), cast<char * const >::from(((char *) " s=")), cast<char * const >::from(__s_rename_at_77), cast<char * const >::from(((char *) ", f3=")), cast<float3>::from(__f3_rename_at_77), cast<char * const >::from(((char *) "\n")))),__context__);
}

inline void created_event__implementation___b948e1c63201b45e ( Context * __context__, uint32_t __n_rename_at_251, ::ecs::OnEntityCreated const  * __event__ptr___rename_at_251, int32_t const  * __counter__ptr___rename_at_251, ::ecs::string * __name__ptr___rename_at_251 )
{
	{
		bool __need_loop_253 = true;
		// i : int const
		das_iterator<range> __i_iterator(mk_range(__n_rename_at_251));
		int32_t __i_rename_at_253;
		__need_loop_253 = __i_iterator.first(__context__,(__i_rename_at_253)) && __need_loop_253;
		for ( ; __need_loop_253 ; __need_loop_253 = __i_iterator.next(__context__,(__i_rename_at_253)) )
		{
			created_event_6af492e36368737f(__context__,das_deref(__context__,__event__ptr___rename_at_251),das_deref(__context__,__counter__ptr___rename_at_251),((char * const )(das_deref(__context__,__name__ptr___rename_at_251).c_str() /*c_str*/)));
			das_ptr_inc(__event__ptr___rename_at_251,1);
			das_ptr_inc(__counter__ptr___rename_at_251,4);
			das_ptr_inc(__name__ptr___rename_at_251,32);
		}
		__i_iterator.close(__context__,(__i_rename_at_253));
	};
}

inline void created_event_6af492e36368737f ( Context * __context__, ::ecs::OnEntityCreated const  &  __event_rename_at_82, int32_t __counter_rename_at_82, char * const  __name_rename_at_82 )
{
	builtin_print(das_string_builder(__context__,SimNode_AotInterop<5>(__tinfo_1, cast<char * const >::from(((char *) "hello counter=")), cast<int32_t>::from(__counter_rename_at_82), cast<char * const >::from(((char *) " name=")), cast<char * const >::from(__name_rename_at_82), cast<char * const >::from(((char *) "\n")))),__context__);
}

inline void my_event_handler__implementation___7a7cc6957ef84965 ( Context * __context__, uint32_t __n_rename_at_251, MyEvent const  * __event__ptr___rename_at_251 )
{
	{
		bool __need_loop_253 = true;
		// i : int const
		das_iterator<range> __i_iterator(mk_range(__n_rename_at_251));
		int32_t __i_rename_at_253;
		__need_loop_253 = __i_iterator.first(__context__,(__i_rename_at_253)) && __need_loop_253;
		for ( ; __need_loop_253 ; __need_loop_253 = __i_iterator.next(__context__,(__i_rename_at_253)) )
		{
			my_event_handler_6fd7d4d78f664169(__context__,das_deref(__context__,__event__ptr___rename_at_251));
			das_ptr_inc(__event__ptr___rename_at_251,24);
		}
		__i_iterator.close(__context__,(__i_rename_at_253));
	};
}

inline void my_event_handler_6fd7d4d78f664169 ( Context * __context__, MyEvent const  &  __event_rename_at_98 )
{
}

inline void creation_test__implementation___1b3063228cb52c91 ( Context * __context__, uint32_t __n_rename_at_251 )
{
	{
		bool __need_loop_253 = true;
		// i : int const
		das_iterator<range> __i_iterator(mk_range(__n_rename_at_251));
		int32_t __i_rename_at_253;
		__need_loop_253 = __i_iterator.first(__context__,(__i_rename_at_253)) && __need_loop_253;
		for ( ; __need_loop_253 ; __need_loop_253 = __i_iterator.next(__context__,(__i_rename_at_253)) )
		{
			creation_test_b0e874b95a9256e6(__context__);
		}
		__i_iterator.close(__context__,(__i_rename_at_253));
	};
}

inline void creation_test_b0e874b95a9256e6 ( Context * __context__ )
{
	uint32_t __p_rename_at_105 = ((uint32_t)::ecs::get_prefab_id(((char *) "test2")));
	::create_entity_n(__p_rename_at_105);
	::create_entity(((char *) "test2"));
	::create_entity_immediate_n(__p_rename_at_105);
	::create_entity_immediate(((char *) "test2"));
}

inline void test_event__implementation___434dd1282c05ca15 ( Context * __context__, uint32_t __n_rename_at_251, TestEvent const  * __event__ptr___rename_at_251, ::ecs::EntityId const  * __eid__ptr___rename_at_251 )
{
	{
		bool __need_loop_253 = true;
		// i : int const
		das_iterator<range> __i_iterator(mk_range(__n_rename_at_251));
		int32_t __i_rename_at_253;
		__need_loop_253 = __i_iterator.first(__context__,(__i_rename_at_253)) && __need_loop_253;
		for ( ; __need_loop_253 ; __need_loop_253 = __i_iterator.next(__context__,(__i_rename_at_253)) )
		{
			test_event_6b4f46df34f87c64(__context__,das_deref(__context__,__event__ptr___rename_at_251),das_deref(__context__,__eid__ptr___rename_at_251));
			das_ptr_inc(__event__ptr___rename_at_251,4);
			das_ptr_inc(__eid__ptr___rename_at_251,8);
		}
		__i_iterator.close(__context__,(__i_rename_at_253));
	};
}

inline void test_event_6b4f46df34f87c64 ( Context * __context__, TestEvent const  &  __event_rename_at_113, ::ecs::EntityId const  __eid_rename_at_113 ) { das_stack_prologue __prologue(__context__,208,"test_event " DAS_FILE_LINE);
{
	MyEvent _temp_make_local_115_13_32;
	_Funcecs_0x60_send_event_668f484c5594ade2(__context__,das_arg<MyEvent>::pass((([&]() -> MyEvent& {
		das_copy((_temp_make_local_115_13_32.x),(0));
		das_copy((_temp_make_local_115_13_32.y),(11));
		das_copy((_temp_make_local_115_13_32.z),(((char *) "aaa")));
		das_copy((_temp_make_local_115_13_32.w),(999));
		return _temp_make_local_115_13_32;
	})())));
	::perform_eid_query(__eid_rename_at_113,das_make_block<void,uint32_t,int32_t *>(__context__,112,0,&__func_info__55fb71370c77bca7,[&](uint32_t __n_rename_at_0, int32_t * __counter__ptr___rename_at_117) -> void{
		{
			bool __need_loop_183 = true;
			// i : int const
			das_iterator<range> __i_iterator(mk_range(__n_rename_at_0));
			int32_t __i_rename_at_183;
			__need_loop_183 = __i_iterator.first(__context__,(__i_rename_at_183)) && __need_loop_183;
			for ( ; __need_loop_183 ; __need_loop_183 = __i_iterator.next(__context__,(__i_rename_at_183)) )
			{
				das_invoke<void>::invoke<int32_t &>(__context__,nullptr,[&](int32_t & __counter_rename_at_184) DAS_AOT_INLINE_LAMBDA -> void{
					__counter_rename_at_184 += __event_rename_at_113.inc;
				},das_deref(__context__,__counter__ptr___rename_at_117));
				das_ptr_inc(__counter__ptr___rename_at_117,4);
			}
			__i_iterator.close(__context__,(__i_rename_at_183));
		};
		return ;
	}),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}}

inline void test_request__implementation___f98aee5d0ea64e90 ( Context * __context__, uint32_t __n_rename_at_251, TestRequest * __request__ptr___rename_at_251, int32_t const  * __counter__ptr___rename_at_251 )
{
	{
		bool __need_loop_253 = true;
		// i : int const
		das_iterator<range> __i_iterator(mk_range(__n_rename_at_251));
		int32_t __i_rename_at_253;
		__need_loop_253 = __i_iterator.first(__context__,(__i_rename_at_253)) && __need_loop_253;
		for ( ; __need_loop_253 ; __need_loop_253 = __i_iterator.next(__context__,(__i_rename_at_253)) )
		{
			test_request_285b8f4d0bf632af(__context__,das_arg<TestRequest>::pass(das_deref(__context__,__request__ptr___rename_at_251)),das_deref(__context__,__counter__ptr___rename_at_251));
			das_ptr_inc(__request__ptr___rename_at_251,4);
			das_ptr_inc(__counter__ptr___rename_at_251,4);
		}
		__i_iterator.close(__context__,(__i_rename_at_253));
	};
}

inline void test_request_285b8f4d0bf632af ( Context * __context__, TestRequest &  __request_rename_at_126, int32_t __counter_rename_at_126 )
{
	__request_rename_at_126.sum += __counter_rename_at_126;
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
	// _Funcecs_0x60_send_event_668f484c5594ade2
	aotLib[0x95c57e388b6a9d01] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, MyEvent const  &  ),&_Funcecs_0x60_send_event_668f484c5594ade2>>();
	};
	// test_func_a6fab27e71e3efab
	aotLib[0xfeaa733fb59cfa2d] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, float , float , float3 & , float3 & , float3  ),&test_func_a6fab27e71e3efab>>();
	};
	// das_system_test__implementation___d296e0466911cef8
	aotLib[0x89d4e149c85ca29] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, uint32_t , float const  * , float3 * , float3 * , float3 const  *  ),&das_system_test__implementation___d296e0466911cef8>>();
	};
	// das_system_test_b809060a654d2132
	aotLib[0xf1f398c755eeeea7] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, float , float3 & , float3 & , float3  ),&das_system_test_b809060a654d2132>>();
	};
	// das_query_test__implementation___e3bba28dd465e49
	aotLib[0x67786b8a099ffed6] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, uint32_t  ),&das_query_test__implementation___e3bba28dd465e49>>();
	};
	// das_query_test_c1cafffdc575571b
	aotLib[0xa943421c2a3a950f] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__ ),&das_query_test_c1cafffdc575571b>>();
	};
	// created_event__implementation___d5eea9a330bd499d
	aotLib[0x17cb45ef59b97c49] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, uint32_t , ::ecs::OnSceneCreated const  *  ),&created_event__implementation___d5eea9a330bd499d>>();
	};
	// created_event_e699080f31c6d7d4
	aotLib[0x6edb784e1036ce45] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, ::ecs::OnSceneCreated const  &  ),&created_event_e699080f31c6d7d4>>();
	};
	// created_event__implementation___96d8b7817d60378b
	aotLib[0xa3c4c860ddb36496] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, uint32_t , ::ecs::OnEntityCreated const  * , int32_t const  * , float const  * , ::ecs::string * , float3 const  *  ),&created_event__implementation___96d8b7817d60378b>>();
	};
	// created_event_49bc7bf17abc6b37
	aotLib[0x94ee5e0324119d31] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, ::ecs::OnEntityCreated const  & , int32_t , float , char * const  , float3  ),&created_event_49bc7bf17abc6b37>>();
	};
	// created_event__implementation___b948e1c63201b45e
	aotLib[0x27a5263e2c5d3ef1] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, uint32_t , ::ecs::OnEntityCreated const  * , int32_t const  * , ::ecs::string *  ),&created_event__implementation___b948e1c63201b45e>>();
	};
	// created_event_6af492e36368737f
	aotLib[0x4eb5a89d717fe825] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, ::ecs::OnEntityCreated const  & , int32_t , char * const   ),&created_event_6af492e36368737f>>();
	};
	// my_event_handler__implementation___7a7cc6957ef84965
	aotLib[0x80a206b3767f1759] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, uint32_t , MyEvent const  *  ),&my_event_handler__implementation___7a7cc6957ef84965>>();
	};
	// my_event_handler_6fd7d4d78f664169
	aotLib[0xe7c3639b2f17fd65] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, MyEvent const  &  ),&my_event_handler_6fd7d4d78f664169>>();
	};
	// creation_test__implementation___1b3063228cb52c91
	aotLib[0x69cf1266a31288f8] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, uint32_t  ),&creation_test__implementation___1b3063228cb52c91>>();
	};
	// creation_test_b0e874b95a9256e6
	aotLib[0x39c40e7c4769cbd5] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__ ),&creation_test_b0e874b95a9256e6>>();
	};
	// test_event__implementation___434dd1282c05ca15
	aotLib[0x4cc9f31e62323761] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, uint32_t , TestEvent const  * , ::ecs::EntityId const  *  ),&test_event__implementation___434dd1282c05ca15>>();
	};
	// test_event_6b4f46df34f87c64
	aotLib[0x9d181fb5b68e4326] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TestEvent const  & , ::ecs::EntityId const   ),&test_event_6b4f46df34f87c64>>();
	};
	// test_request__implementation___f98aee5d0ea64e90
	aotLib[0x4656a44279a155e0] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, uint32_t , TestRequest * , int32_t const  *  ),&test_request__implementation___f98aee5d0ea64e90>>();
	};
	// test_request_285b8f4d0bf632af
	aotLib[0x4d632e831cc96e69] = [&](Context & ctx){
		return ctx.code->makeNode<SimNode_Aot<void (*) ( Context * __context__, TestRequest & , int32_t  ),&test_request_285b8f4d0bf632af>>();
	};
	resolveTypeInfoAnnotations();
};

AotListBase impl(registerAotFunctions);
}
}
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
