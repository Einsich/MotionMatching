#pragma once
#include <daScript/daScript.h>
#include <ecs/ecs.h>

MAKE_TYPE_FACTORY(Event, ecs::Event)
MAKE_TYPE_FACTORY(Request, ecs::Request)

void register_system(
    const char *stage,
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::FunctionPtr &system);

bool register_event(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_event,
    const das::FunctionPtr &event);

bool register_request(
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::Array &on_request,
    const das::FunctionPtr &request);

void register_query(
    const das::Array &require_args,
    const das::Array &require_not_args,
    das::smart_ptr<das::ExprBlock> block);
  
void perform_query(const das::Block &block, das::Context *context, das::LineInfoArg *line);
const char* get_das_type_name(const das::TypeDecl &type);