module ecs shared public

require ast public
require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost
require daslib/safe_addr

require ecs_impl public

[typeinfo_macro(name="ecs_event_type_id")]
class ECSEventTypeId : AstTypeInfoMacro
  def override getAstChange (expr:smart_ptr<ExprTypeInfo>; var errors:das_string ) : ExpressionPtr
    if expr.typeexpr != null
      return new [[ExprConstInt() at=expr.at, value=get_event_idx(get_das_type_name(*expr.typeexpr))]]
    errors := "type is missing or not inferred"
    return [[ExpressionPtr]]

[typeinfo_macro(name="ecs_request_type_id")]
class ECSRequestTypeId : AstTypeInfoMacro
  def override getAstChange (expr:smart_ptr<ExprTypeInfo>; var errors:das_string ) : ExpressionPtr
    if expr.typeexpr != null
      return new [[ExprConstInt() at=expr.at, value=get_request_idx(get_das_type_name(*expr.typeexpr))]]
    errors := "type is missing or not inferred"
    return [[ExpressionPtr]]

def send_event(event : auto(TT))
  unsafe
    let typeId = typeinfo(ecs_event_type_id event)
    if typeId >= 0
      builtin_send_event(typeinfo(sizeof event), typeId, addr(event))
    else
      print("need to call default initializer in send_event([[{typeinfo(stripped_typename event)}() ...]])\n")

def send_event_immediate(event : auto(TT))
  unsafe
    let typeId = typeinfo(ecs_event_type_id event)
    if typeId >= 0
      builtin_send_event_immediate(typeinfo(sizeof event), typeId, addr(event))
    else
      print("need to call default initializer in send_event_immediate([[{typeinfo(stripped_typename event)}() ... ]])\n")

def send_event(eid : EntityId; event : auto(TT))
  unsafe
    let typeId = typeinfo(ecs_event_type_id event)
    if typeId >= 0
      builtin_send_eid_event(eid, typeinfo(sizeof event), typeId, addr(event))
    else
      print("need to call default initializer in send_event(eid, [[{typeinfo(stripped_typename event)}() ... ]])\n")

def send_event_immediate(eid : EntityId; event : auto(TT))
  unsafe
    let typeId = typeinfo(ecs_event_type_id event)
    if typeId >= 0
      builtin_send_eid_event_immediate(eid, typeinfo(sizeof event), typeId, addr(event))
    else
      print("need to call default initializer in send_event_immediate(eid, [[{typeinfo(stripped_typename event)}() ... ]])\n")

def send_request(var event : auto(TT)&)
  unsafe
    let typeId = typeinfo(ecs_request_type_id event)
    if typeId >= 0
      builtin_send_request(typeinfo(sizeof event), typeId, addr(event))
    else
      print("need to call default initializer in send_request([[{typeinfo(stripped_typename event)}() ... ]])\n")


enum QueryType
  Query
  System
  Event
  Request

def private validate_macro_args(args : AnnotationArgumentList; t : QueryType; var errors : das_string)

  for arg in args
    if arg.basicType == Type tString

      var ok = ((arg.name == "required" || arg.name == "required_not")
             || (t != QueryType Query && (arg.name == "before" || arg.name == "after" || arg.name == "tags"))
             || (t == QueryType System && arg.name == "stage")
             || (t == QueryType Event && arg.name == "on_event")
             || (t == QueryType Request && arg.name == "on_request"))
      if !ok
        errors := "unsupported name in macro argument {arg.name}"
        return false
    else
      errors := "unsupported type in macro argument {arg.name}"
      return false
  return true

def private parse_component_type(str : das_string)
  let s = string(str)
  let typeIdx = find(s, "`")
  return [[auto slice(s, 0, typeIdx), slice(s, typeIdx+1)]]

def private gather_requirements(args : AnnotationArgumentList; name : string)
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


def private gather_query_args(query_args : dasvector`smart_ptr`Variable )

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
      if arg._type.baseType != Type tString
        args.ptrPass |> emplace_new <| qmacro(*$i(argName))
        vtype.firstType := clone_type(arg._type)
      else
        args.ptrPass |> emplace_new <| qmacro((*$i(argName)).c_str)
        vtype.firstType := qmacro_type <| type<ecs_string>

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

    var blockBody : array<ExpressionPtr>
    for x in blk.list
      var cp <- x
      blockBody |> emplace(cp)

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

def private get_system_type(sys_args : dasvector`smart_ptr`Variable; args:AnnotationArgumentList)
  
  if !(find_arg("stage", args) is nothing)
    return QueryType System
  if !(find_arg("on_event", args) is nothing)
    return QueryType Event
  if !(find_arg("on_request", args) is nothing)
    return QueryType Request
  
  if length(sys_args) == 0
    return QueryType System
  let name = get_das_type_name(*sys_args[0]._type)

  if name == "Event"
    return QueryType Event

  if name == "Request"
    return QueryType Request

  let eventIdx = get_event_idx(name)
  //print("type {name} is {eventIdx}\n")
  if eventIdx >= 0
    return QueryType Event
  let requestIdx = get_request_idx(name)
  //print("type {name} is {requestIdx}\n")
  if requestIdx >= 0
    return QueryType Request

  return QueryType System

[function_macro(name="ecs")]
class ECSSystemMacro : AstFunctionAnnotation

  def override apply ( var func:FunctionPtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool

    let sysType = get_system_type(func.arguments, args)
    //print("{func.name} is {sysType}\n")
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
    let functionHash = systemLoop.getMangledNameHash
    compiling_module() |> add_function(systemLoop)

    if sysType == QueryType System
      let stage = find_arg("stage", args)
      register_system("{stage}", req, not_req, before, after, tags, func, functionHash)
  
    elif sysType == QueryType Event
      let name = get_das_type_name(*func.arguments[0]._type)
      if !(*func.arguments[0]._type).isConst
        errors := "event {systemName} should receive const {name}"
        return false

      var on_event <- gather_strings(args, "on_event")
      if name == "Event" && empty(on_event)
        errors := "event {systemName} should have not empty on_event macro arg or receive not abstract \"Event\""
        return false
      if name != "Event" && !empty(on_event)
        errors := "event {systemName} should have empty on_event macro arg when receive {name}"
        return false

      return register_event(req, not_req, before, after, tags, on_event, func, functionHash)
    
    elif sysType == QueryType Request
      let name = get_das_type_name(*func.arguments[0]._type)
      if (*func.arguments[0]._type).isConst
        errors := "request {systemName} should receive not const {name}"
        return false

      var on_request <- gather_strings(args, "on_request")
      if name == "Request" && empty(on_request)
        errors := "request {systemName} should have not empty on_request macro arg or receive not abstract \"Event\""
        return false
      if name != "Request" && !empty(on_request)
        errors := "request {systemName} should have empty on_request macro arg when receive {name}"
        return false

      return register_request(req, not_req, before, after, tags, on_request, func, functionHash)
    else
      return false
    return true



def private validate_event(var st : StructurePtr; typeId, cppSizeOf : int; var errors : das_string; typeName : string)
  if typeId < 0
    errors := "there isn't registered {typeName} {st.name} in c++ part"
    return false
  let dasSizeOf = (*st).sizeOf
  if cppSizeOf != dasSizeOf
    errors := "different sizeof of {typeName} {st.name} in das <{dasSizeOf}> and in c++ <{cppSizeOf}>"
    return false

  return true

[structure_macro(name=ecs_cpp_event)]
class CppEvent : AstStructureAnnotation
  def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
    let typeIdx = get_event_idx(string(st.name))
    return validate_event(st, typeIdx, get_event_sizeof(typeIdx), errors, "event")

[structure_macro(name=ecs_cpp_request)]
class CppRequest : AstStructureAnnotation
  def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
    let typeIdx = get_request_idx(string(st.name))
    return validate_event(st, typeIdx, get_request_sizeof(typeIdx), errors, "request")


    
[structure_macro(name=ecs_event)]
class DasEvent : AstStructureAnnotation
  def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
    register_das_event(st)
    return true
    
[structure_macro(name=ecs_request)]
class DasRequest : AstStructureAnnotation
  def override apply ( var st:StructurePtr; var group:ModuleGroup; args:AnnotationArgumentList; var errors : das_string ) : bool
    register_das_request(st)
    return true