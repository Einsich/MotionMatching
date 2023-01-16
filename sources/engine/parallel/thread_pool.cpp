#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <string>
#include <profiler.h>

struct thread_pool {
  // the mutex, condition variable and deque form a single
  // thread-safe triggered queue of tasks:
  std::mutex m;
  std::condition_variable v;

  std::queue<std::function<void()>> tasks;

  std::vector<std::thread> workers;
  int busyWorkers = 0;
  bool terminatePool;

  void add_job(std::function<void()>&& f) {
    {
      std::unique_lock<std::mutex> l(m);
      tasks.push(std::move(f));
    }
    v.notify_one();
  }

  thread_pool():terminatePool(false)
  {
    auto num_threads = std::thread::hardware_concurrency();
    busyWorkers = num_threads;
    for (std::size_t i = 0; i < num_threads; ++i)
    {
      workers.emplace_back(std::thread(&thread_pool::infinite_loop, this, i));
    }
  }

  ~thread_pool() {
    {
      std::unique_lock<std::mutex> l(m);
      terminatePool = true;
    }
    v.notify_all();
    for (std::thread &worker : workers)
    {
      worker.join();
    }
    workers.clear();
  }
private:
  void infinite_loop(std::size_t idx) {
    std::string name = "Worker" + std::to_string(idx);
    OPTICK_THREAD(name.c_str());
    while (true){
      std::function<void()> f;
      {
        // usual thread-safe queue code:
        std::unique_lock<std::mutex> l(m);
        busyWorkers--;
        if (tasks.empty()){
          v.wait(l,[&]{return !tasks.empty() || terminatePool;});
        }
        if (terminatePool)
          return;
        f = std::move(tasks.front());
        busyWorkers++;
        tasks.pop();
      }
      f();
    }
  }
};

static thread_pool threadPool;

void add_job(std::function<void()>&& f)
{
  threadPool.add_job(std::move(f));
}

void wait_jobs()
{
  PROFILER_EVENT("thread_pool_wait")
  while (!threadPool.tasks.empty() || threadPool.busyWorkers > 0)
  {
  }
}