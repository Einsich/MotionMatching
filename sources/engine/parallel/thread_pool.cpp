#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

struct thread_pool {
  // the mutex, condition variable and deque form a single
  // thread-safe triggered queue of tasks:
  std::mutex m;
  std::condition_variable v;

  std::queue<std::function<void()>> tasks;

  std::vector<std::thread> workers;
  bool terminatePool;

  void start_job(std::function<void()>&& f) {
    {
      std::unique_lock<std::mutex> l(m);
      tasks.push(std::move(f));
    }
    v.notify_one();
  }

  thread_pool():terminatePool(false)
  {
    auto num_threads = std::thread::hardware_concurrency();
    for (std::size_t i = 0; i < num_threads; ++i)
    {
      workers.emplace_back(std::thread(&thread_pool::infinite_loop, this));
    }
  printf("workers %d\n", num_threads);
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
  void infinite_loop() {
    while (true){
      std::function<void()> f;
      {
        // usual thread-safe queue code:
        std::unique_lock<std::mutex> l(m);
        if (tasks.empty()){
          v.wait(l,[&]{return !tasks.empty() || terminatePool;});
        }
        if (terminatePool)
          return;
        f = std::move(tasks.front());
        tasks.pop();
      }
      f();
    }
  }
};

static thread_pool threadPool;

void start_job(std::function<void()>&& f)
{
  threadPool.start_job(std::move(f));
}