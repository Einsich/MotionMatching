
#include "tmp_allocator.h"
#include <eastl/vector.h>
#include <application/time.h>

eastl::vector<eastl::pair<int, int>> allocCount = 
{
  {10, 4},
  {20, 8},
  {20, 16},
  {100, 32},
  {100, 64},
  {100, 128},
  {100, 256},
  {50, 512},
  {50, 1024},
  {10, 2048}
};


template<typename Container, bool with_allocation>
void test(int max_count, int allocationPerFrame)
{
  for (int i = 0; i < allocationPerFrame; i++)
  {
    int c = rand() % max_count;
    for (auto[freq, size] : allocCount)
    {
      if (c < freq)
      {
        Container container;
        if constexpr(with_allocation)
        {
          container.reserve(size);
        }
        break;
      }
      else
      {
        c -= freq;
      }
    }
  }
}


template<typename Container, bool tmp_allocator>
float test(int frameCount, int allocationPerFrame, int cnt)
{
  srand(frameCount * allocationPerFrame);
  TimeScope f("");
  for (int i = 0; i < frameCount; i++)
  {
    test<Container, true>(cnt, allocationPerFrame);
  }
  float dtAlloc = f.stop(true);
  
  TimeScope g("");
  for (int i = 0; i < frameCount; i++)
  {
    test<Container, false>(cnt, allocationPerFrame);
    if constexpr (tmp_allocator)
    {
      clear_tmp_allocation();
    }
  }
  float dtNoAlloc = g.stop(true);
  return dtAlloc - dtNoAlloc;

}
void test()
{
  int cnt = 0;
  for (auto[freq, size] : allocCount)
    cnt += freq;
  struct Test
  {
    int frameCount;
    int allocationPerFrame;
    float tmpAllocTime;
    float baseAllocTime;
  };
  const int N = 8;
  const int M = 8;
  Test tests[N*M];
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++)
    {
      Test &t = tests[i*M+j];
      t.frameCount = (1 + i * i) * 30;
      t.allocationPerFrame = (1 + j * j) * 20;
      t.baseAllocTime = test<eastl::vector<int>, false>(t.frameCount, t.allocationPerFrame, cnt);
      t.tmpAllocTime = test<eastl::vector<int, tmp_allocator>, true>(t.frameCount, t.allocationPerFrame, cnt);
    }
  
  printf("alloc per frame ");
  for (int j = 0; j < M; j++) 
    printf("%d; ", tests[j].allocationPerFrame);
  printf("\n");

  for (int i = 0; i < N; i++)
  {
    printf("frames %d ", tests[i*M].frameCount);
    for (int j = 0; j < M; j++)
    {
      const Test &t = tests[i*M+j];
      printf("%f %f %f;",
        t.tmpAllocTime, t.baseAllocTime, t.baseAllocTime / t.tmpAllocTime);
    }
    printf("\n");
  }  
}