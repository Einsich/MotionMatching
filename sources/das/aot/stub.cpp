#include "../das_modules/ecs_module.h"

void register_system(
    const char *stage,
    const das::Array &require_args,
    const das::Array &require_not_args,
    const das::Array &before,
    const das::Array &after,
    const das::Array &tags,
    const das::FunctionPtr &system)
{
  printf("register_system %s\n", system->name.c_str());
}

void register_query(
    const das::Array &require_args,
    const das::Array &require_not_args,
    das::smart_ptr<das::ExprBlock> block)
{
  printf("register_query %s:%d\n", block->at.fileInfo->name.c_str(), block->at.line);
}

void perform_query(const das::Block &block, das::Context *context, das::LineInfoArg *line)
{
  assert(0);
}