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


/* [ecs(stage=act)]
def test_c()
  print("c\n")

  query() <| $ [eq](counter : int)
    print("counter = {counter}")

  query() <| $ [eq](var counter : int&)
    counter = 0
    print("counter = {counter}") */

struct OnEntityCreated {}

[ecs]
def created_event(event : OnEntityCreated; counter : int)
  print("hello {counter}\n")

[ecs_cpp_event]
struct TestEvent
  inc : int


[ecs_event]
struct MyEvent
  x : int
  y : int
  z : string

[ecs]
def my_event_handler(event : MyEvent)
  print("event = {event}\n")


[ecs(required=counter`int)]
def test_event(event : TestEvent; eid : EntityId)     

  send_event([[MyEvent x=0, y=11, z="aaa"]])              
  //send_event(10)               
  query(eid) <| $[eq] (var counter : int&)
    counter += event.inc  
    //    print("counter = {counter} inc = {event.inc}\n")

struct TestRequest
  sum : int  

[ecs]
def test_request(var request : TestRequest; counter : int)
  request.sum += counter
