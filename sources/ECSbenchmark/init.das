require math
require ecs
require engine
//required=(name`type)
//required_not=(name`type)
//before=(systems)
//after=(systems)
//tags=(tags)
def test_func(dt, m : float; var pos, vel : float3&; center: float3)
  let dir = center - pos
  let distance = length(dir)
  let F = dir * m * 0.0001f / (distance*distance*distance)
  let a = F / m
  vel += a * dt
  pos += vel * dt


[ecs(stage=act)]
def das_system_test(m : float; var pos, vel : float3&; center: float3)
  test_func(get_delta_time(), m, pos, vel, center)


[ecs(stage=act)]
def das_query_test()
  let dt = get_delta_time()
  query() <| $[eq] (m : float; var pos, vel : float3&; center: float3)
    test_func(dt, m, pos, vel, center)

/* [ecs(stage=act)]
def test_c()
  print("c\n")

  query() <| $ [eq](counter : int)
    print("counter = {counter}")

  query() <| $ [eq](var counter : int&)
    counter = 0
    print("counter = {counter}") */

struct XXXStruct
  inc : int

[ecs]
def created_event(event : OnSceneCreated)
  print("hello world!\n")
  print("hello world!!\n")
  print("hello world!!!\n")

  let p = create_entity_prefab("das_prefab") <| $(init)
    //let x <- [{ auto 1;2;3;4;5 }]
    init |> set("i", 1)
    init |> set("f", 1.f)
    init |> set("s", "xxxx0000xxxiiii")
    init |> set("f3", float3(1,2,3))
    //init |> set(EntityId)
    //init |> set("str", [[XXXStruct]])
    //init |> set("arr", x)

  let eid = create_entity(p) <| $(init)
    init |> set("s", "lolkek")
    init |> set("i", 55)

  create_entity(p) <| $(init)
    init |> set("s", "chebureck")
    init |> set("i", 1111111111)
    init |> set("e", eid)


  create_entity_immediate("das_prefab") <| $(init)
    init |> set("s", "immediate")
    init |> set("i", 609)

[ecs]
def created_event(event : OnEntityCreated; i : int; f : float; s : string; f3 : float3)
  print("hello i={i} f={f} s={s}, f3={f3}\n")


[ecs(required=name`ecs_string)]
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


[ecs(stage=act, required=(x`int))]
def creation_test()
  let p = get_prefab_id("test2")

  create_entity(p)
  create_entity("test2")
  create_entity_immediate(p)
  create_entity_immediate("test2")

//[ecs_type]
[persistent(mixed_heap=true)]
struct Camera
  position : float3
  name : string
  x, y : float = 2f
  a : array<string>
  /* def Camera()
    name = "mainCamera"
    a <- [{auto "loh"; "poh"}] */

var globalCounter = 0

var globalData : array<int>
var mainCamera : Camera?

def makeCamera(var memory : Camera)
  unsafe
    memory <- Camera()


[ecs(required=counter`int)]
def test_event(event : TestEvent; eid : EntityId)
  if mainCamera == null
    mainCamera = new Camera()
    print("mainCamera={mainCamera}\n")
    makeCamera(*mainCamera)
    print("mainCamera={mainCamera}\n")
    mainCamera.position = float3(1,2,3)
    mainCamera.a <- [{auto "{globalCounter}"; "{34}"}]
    print("mainCamera={mainCamera}\n")
  globalCounter++
  //globalData |> push(globalCounter)
  //print("globalCounter={globalCounter} globalData = {globalData}\n")
  let k = new Camera()
  print("mainCamera={mainCamera}\n")
  send_event([[MyEvent() x=0, y=11, z="aaa"]])
  //send_event(10)
  query(eid) <| $[eq] (var counter : int&)
    counter += event.inc
    //print("counter = {counter} inc = {event.inc}\n")

[ecs_cpp_request]
struct TestRequest
  sum : int

[ecs]
def test_request(var request : TestRequest; counter : int)
  request.sum += counter
