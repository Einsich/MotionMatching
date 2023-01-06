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


[ecs]
def created_event(event : OnSceneCreated)
  print("hello world!\n")
  print("hello world!!\n")
  print("hello world!!!\n")

[ecs]
def created_event(event : OnEntityCreated; counter : int; name : string)
  print("hello counter={counter} name={name}\n")

[ecs_cpp_event]
struct TestEvent
  inc : int


[ecs_event]
struct MyEvent
  x : int
  y : int
  z : string
  w : int = 999

[ecs]
def my_event_handler(event : MyEvent)
  return
  print("event = {event}\n")

[ecs(stage=act)]
def creation_test() 
  let p = get_prefab_id("test2")

  create_entity(p)
  create_entity("test2")
  create_entity_immediate(p)
  create_entity_immediate("test2")

[ecs(required=counter`int)]
def test_event(event : TestEvent; eid : EntityId)     

  send_event([[MyEvent() x=0, y=11, z="aaa"]])               
  //send_event(10)               
  query(eid) <| $[eq] (var counter : int&)
    counter += event.inc  
    //    print("counter = {counter} inc = {event.inc}\n")

[ecs_cpp_request]
struct TestRequest
  sum : int  

[ecs]
def test_request(var request : TestRequest; counter : int)
  request.sum += counter
