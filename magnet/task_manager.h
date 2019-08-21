#include <thread>
#include <string>
#include <mutex>
#include <queue>
#include <functional>

struct Task {
  Task() {}
  std::function<void()> func;
  void Execute() {
    func();
  }
};

class TaskManager {
private:
  TaskManager();
  ~TaskManager();
  TaskManager(const TaskManager&) = delete;
  TaskManager& operator=(const TaskManager&) = delete;

public:
  static TaskManager* GetInstance();
  static bool Exist();
  static void Initialize();
  static void Terminate();

  void EnqueueTask(const Task& task);
  bool DequeueTask(Task& task);

  void BeginThreads(int thread_count);

  bool HasTasks();  

private:
  void EndThreads();
  void ThreadFunction();

private:
  static TaskManager* instance_;

  int thread_count_;
  bool terminate_;
  std::queue<Task> task_queue_;
  std::mutex queue_mutex_;
  std::vector<std::thread> threads_;

};