require math

require daslib/ast_boost
require daslib/templates_boost
require daslib/strings_boost
require daslib/ast_block_to_loop
require daslib/decs

require ecs

[ecs(stage=act)]
def hi_func(dt, m : float; var pos, vel : float3&; center: float3)
  print("hi")


[export]
def process(dt, m : float; var pos, vel : float3&; center: float3)
{
  let dir = center - pos;
  let distance = length(dir);
  let F = dir * m * 0.0001f / (distance*distance*distance);
  let a = F / m;
  vel += a * dt;
  pos += vel * dt;
  //print("dt = {dt}, m = {m}, pos = {pos}, vel = {vel}, center = {center}\n");
}

[export]
def test
{
  print("this is nano tutorial\n");
}