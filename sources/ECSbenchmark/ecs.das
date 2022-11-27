module ecs shared

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost
require daslib/ast_block_to_loop
require daslib/decs

require ecs_impl

enum private DecsQueryType
  query
  eid_query
  find_query

[call_macro(name="query2")]
class ECSQueryMacro : AstCallMacro

  def override preVisit(prog : ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : void
    return
    let totalArgs = length(expr.arguments)
    if totalArgs!=1 && totalArgs!=2
      return
    let qt = totalArgs==2 ? DecsQueryType eid_query : DecsQueryType query
    let block_arg_index = totalArgs-1
    if !(expr.arguments[block_arg_index] is ExprMakeBlock)
      return
    var mblk = expr.arguments[block_arg_index] as ExprMakeBlock
    var qblk = mblk._block as ExprBlock
    for arg in qblk.arguments
      if arg._type != null
        if !arg._type.flags.constant
          arg._type.flags |= TypeDeclFlags ref

  def override visit ( prog:ProgramPtr; mod:Module?; var expr:smart_ptr<ExprCallMacro> ) : ExpressionPtr
    let totalArgs = length(expr.arguments)
    if totalArgs!=1 && totalArgs!=2
      macro_error(prog,expr.at,"expecting query($(block_with_arguments)) or query(eid,$(block_with_arguments))")
      return [[ExpressionPtr]]
    let qt = totalArgs==2 ? DecsQueryType eid_query : DecsQueryType query
    let block_arg_index = totalArgs-1
    return self->implement(expr, block_arg_index, qt)

  def implement ( var expr:smart_ptr<ExprCallMacro>; block_arg_index:int; qt:DecsQueryType ) : ExpressionPtr
    for arg in expr.arguments
      if arg._type.isAutoOrAlias
        macro_error(compiling_program(),expr.at,"argument types are not fully inferred")
        return [[ExpressionPtr]]
    return [[ExpressionPtr]]

[function_macro(name="hi")]
class SystemMacro : AstFunctionAnnotation

  def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool

    let argPass = find_arg("stage", args)
    if !(argPass is tString)
      errors := "need to specify stage"
      return false
    let passName = argPass as tString
    let passFuncName = "decs`pass`{passName}"
    var blk <- setup_call_list(passFuncName, func.at, false, true)
    if length(blk.list)==0
      var reg <- setup_call_list("register`decs`passes", func.at, true, true)
      var regc <- new [[ExprCall() at=func.at, name:="decs::register_decs_stage_call"]]
      regc.arguments |> emplace_new <| new [[ExprConstString() at=func.at, value:=passName]]
      regc.arguments |> emplace_new <| new [[ExprAddr() at=func.at, target:=passFuncName]]
      reg.list |> emplace(regc)
    func.flags |= FunctionFlags privateFunction
    blk.list |> emplace_new <| new [[ExprCall() at=func.at, name:="_::{func.name}"]]
    var fblk <- new [[ExprBlock() at=func.body.at]]                 // new function block
    //var cqq <- make_call(func.at, "query2")
    //var cquery = cqq as ExprCallMacro
    var qblk <- new [[ExprBlock() at=func.body.at]]                 // inside the query block
    qblk.blockFlags |= ExprBlockFlags isClosure
    qblk.returnType <- new [[TypeDecl() baseType=Type tVoid, at=func.at]]
    var req, nreq : array<string>
    for arg in args
      if arg.basicType==Type tString
        if arg.name=="REQUIRE"
          req |> push("{arg.sValue}")
        if arg.name=="REQUIRE_NOT"
          nreq |> push("{arg.sValue}")
    if !req |> empty
      var decl <- new [[AnnotationDeclaration() ]]
      var ann = find_annotation("decs_boost", "REQUIRE")
      unsafe
        decl.annotation := reinterpret<smart_ptr<Annotation>> ann
      for rq in req
        decl.arguments |> add_annotation_argument(rq, true)
      qblk.annotations |> emplace(decl)
    if !nreq |> empty
      var decl <- new [[AnnotationDeclaration() ]]
      var ann = find_annotation("decs_boost", "REQUIRE_NOT")
      unsafe
        decl.annotation := reinterpret<smart_ptr<Annotation>> ann
      for rq in nreq
        decl.arguments |> add_annotation_argument(rq, true)
      qblk.annotations |> emplace(decl)
    var fnbody = func.body as ExprBlock
    for el in fnbody.list                                           // list goes to inside query
      qblk.list |> emplace_new <| clone_expression(el)
    for ef in fnbody.finalList                                      // finally goes to new finally
      fblk.finalList |> emplace_new <| clone_expression(ef)
    for arg in func.arguments                                       // add function arguments to query arguments
      qblk.arguments |> emplace_new <| new [[Variable() at=arg.at,
        name := arg.name,
        _type <- clone_type(arg._type),
        init <- clone_expression(arg.init)
      ]]
    func.arguments |> clear
    //cquery.arguments |> emplace_new <| new [[ExprMakeBlock() _block <- qblk, at=func.at]]
    //fblk.list |> emplace(cqq)
    func.body := fblk
    return true

def type_name(_type : TypeDeclPtr)

  if _type.baseType==Type tHandle
    return "{_type.annotation.name}"
  if _type.baseType==Type tInt
    return "int"
  if _type.baseType==Type tInt2
    return "int2"
  if _type.baseType==Type tInt3
    return "int3"
  if _type.baseType==Type tInt4
    return "int4"
  elif _type.baseType==Type tFloat
    return "float"
  elif _type.baseType==Type tFloat2
    return "float2"
  elif _type.baseType==Type tFloat3
    return "float3"
  elif _type.baseType==Type tFloat4
    return "float4"
  elif _type.baseType==Type tDouble
    return "double"
  elif _type.baseType==Type tString
    return "string"
  elif _type.baseType==Type tBool
    return "bool"

  return "?"

[function_macro(name="ecs")]
class ECSSystemMacro : AstFunctionAnnotation

  def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool

    let argPass = find_arg("stage", args)

    func.flags |= FunctionFlags exports

    var req: array<tuple<string; string>>
    var not_req: array<tuple<string; string>>
    var before: array<string>
    var after: array<string>
    var tags: array<string>
    for arg in args
      if arg.basicType == Type tString
        if arg.name == "required"
          req |> push([[auto "", "{arg.sValue}"]])
        elif arg.name == "required_not"
          not_req |> push([[auto "", "{arg.sValue}"]])
        elif arg.name == "before"
          before |> push("{arg.sValue}")
        elif arg.name == "after"
          after |> push("{arg.sValue}")
        elif arg.name == "tags"
          tags |> push("{arg.sValue}")

    for arg in func.arguments
      req |> push([[auto type_name(arg._type), "{arg.name}"]])
  
    register_system("{func.name}", "{argPass}", req, not_req, before, after, tags)
  
    return true