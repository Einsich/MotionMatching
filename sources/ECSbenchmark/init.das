require math


require ecs
require engine
//required=(name`type)
//required_not=(name`type)
//before=(systems)
//after=(systems)
//tags=(tags)

[ecs(stage=act,required_not=(a`int))]
def hi_func(m : float; var pos, vel : float3&; center: float3)
  let dt = get_delta_time()
  let dir = center - pos
  let distance = length(dir)
  let F = dir * m * 0.0001f / (distance*distance*distance)
  let a = F / m
  vel += a * dt
  pos += vel * dt
  //print("dt = {dt}, m = {m}, pos = {pos}, vel = {vel}, center = {center}\n");


[ecs(stage=act)]
def test_c()
  print("c\n")

  query() <| $ [eq](counter : int)
    print("counter = {counter}")

  query() <| $ [eq](var counter : int&)
    counter = 0



