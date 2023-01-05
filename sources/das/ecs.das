module ecs shared

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost

require ecs_impl public

/* enum private DecsQueryType
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
 */

enum QueryType
  Query
  System
  Event
  Request

def validate_macro_args(args : AnnotationArgumentList; t : QueryType; var errors : das_string)
  for arg in args
    if arg.basicType == Type tString

      var ok = ((arg.name == "required" || arg.name == "required_not")
             || (t != QueryType Query && (arg.name == "before" || arg.name == "after" || arg.name == "tags"))
             || (t == QueryType System && arg.name == "stage"))
      if !ok
        errors := "unsupported name in macro argument {arg.name}"
        return false
    else
      errors := "unsupported type in macro argument {arg.name}"
      return false
  return true

def parse_component_type(str : das_string)
  let s = string(str)
  let typeIdx = find(s, "`")
  return [[auto slice(s, 0, typeIdx), slice(s, typeIdx+1)]]

def gather_requirements(args : AnnotationArgumentList; name : string)
  var req: array<tuple<string; string>>
  for arg in args
    if arg.basicType == Type tString
      if arg.name == name
        req |> push(parse_component_type(arg.sValue))
  return <- req

def gather_strings(args : AnnotationArgumentList; name : string)
  var names: array<string>
  for arg in args
    if arg.basicType == Type tString
      if arg.name == name
        names |> push("{arg.sValue}")
  return <- names

struct QueryArguments
  vars : array<VariablePtr>
  prtInc : array<ExpressionPtr>
  ptrPass : array<ExpressionPtr>


def gather_query_args(query_args : dasvector`smart_ptr`Variable )

  var args : QueryArguments


  args.vars |> emplace_new <| new [[Variable() name := "n", _type<-qmacro_type(type<uint>)]]

  for arg in query_args 

    var vtype <- clone_type(arg._type)
    vtype.baseType = Type tPointer
    vtype.flags = bitfield(0)
    let argName = "{arg.name}`ptr"
    args.prtInc |> emplace_new <| qmacro($i(argName)++)
    if arg._type.isPointer
      args.ptrPass |> emplace_new <| qmacro($i(argName))
      vtype.firstType := clone_type(arg._type.firstType)
    else
      args.ptrPass |> emplace_new <| qmacro(*$i(argName))
      vtype.firstType := clone_type(arg._type)
    vtype.firstType.flags &= ~TypeDeclFlags ref
    
    
    args.vars |> emplace_new <| new [[Variable() at=arg.at,
        name := argName,
        _type <- vtype
      ]]
  return <- args

[block_macro(name="eq")]
class ECSQueryMacro : AstBlockAnnotation

  def override apply ( var blk:smart_ptr<ExprBlock>; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
    
    if !validate_macro_args(args, QueryType Query, errors)
      return false

    let req <- gather_requirements(args, "required")
    let not_req <- gather_requirements(args, "required_not")

    var vars <- gather_query_args(blk.arguments)

    register_query(req, not_req, blk)
    var queryBody <- blk.list[0]
    var blockBody <- [{ auto queryBody}]

    var queryBlock <- qmacro_block <| 
      unsafe
        for i in range(n)
          invoke( $($a(blk.arguments)) {$b(blockBody);}, $a(vars.ptrPass))
          $b(vars.prtInc)
      return
    
    blk.list |> clear()
    blk.arguments |> clear()
    for v in vars.vars
      blk.arguments |> emplace(v)
    blk.list |> emplace(queryBlock) 
    
    //print(describe(blk))

    return true

def get_system_type(sys_args : dasvector`smart_ptr`Variable; args:AnnotationArgumentList)
  
  if !(find_arg("stage", args) is nothing)
    return QueryType System
  if !(find_arg("on_event", args) is nothing)
    return QueryType Event
  if !(find_arg("on_request", args) is nothing)
    return QueryType Request
  
  if length(sys_args) == 0
    return QueryType System
  let name = get_das_type_name(*sys_args[0]._type)
  let eventIdx = get_event_idx(name)
  print("type {name} is {eventIdx}\n")
  if eventIdx >= 0
    return QueryType Event
  let requestIdx = get_request_idx(name)
  print("type {name} is {requestIdx}\n")
  if requestIdx >= 0
    return QueryType Request

  return QueryType System

[function_macro(name="ecs")]
class ECSSystemMacro : AstFunctionAnnotation

  def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool

    let sysType = get_system_type(func.arguments, args)
    print("{func.name} is {sysType}\n")
    if !validate_macro_args(args, sysType, errors)
      return false


    let req <- gather_requirements(args, "required")
    let not_req <- gather_requirements(args, "required_not")
    var before <- gather_strings(args, "before")
    var after <- gather_strings(args, "after")
    var tags <- gather_strings(args, "tags")

    var vars <- gather_query_args(func.arguments)

    if sysType != QueryType System
      vars.prtInc |> erase(0)

    let systemName = string(func.name)
    var systemLoop <- qmacro_function("{systemName}`implementation") <| $($a(vars.vars))
      unsafe
        for i in range(n)
          $c(systemName)($a(vars.ptrPass))
          $b(vars.prtInc)
      return
    
    //print(describe(systemLoop))
    
    systemLoop.flags |= FunctionFlags exports
    compiling_module() |> add_function(systemLoop)

    if sysType == QueryType System
      let stage = find_arg("stage", args)
      register_system("{stage}", req, not_req, before, after, tags, func)
  
    elif sysType == QueryType Event
      register_event(req, not_req, before, after, tags, func)

    else
      return false
    return true