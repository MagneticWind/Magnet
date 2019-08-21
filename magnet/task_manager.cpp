#include <chrono>
#include <iostream>
#include "task_manager.h"

TaskManager* TaskManager::instance_ = nullptr;

TaskManager::TaskManager() : thread_count_(0), terminate_(false) {
}

TaskManager::~TaskManager() {
}

void TaskManager::EnqueueTask(const Task& task) {
  task_queue_.push(task);
}

bool TaskManager::DequeueTask(Task& task) {
  std::lock_guard<std::mutex> guard(queue_mutex_);
  if (!task_queue_.empty()) {
    task = task_queue_.front();
    task_queue_.pop();
    return true;
  }
  else
    return false;
}

void TaskManager::ThreadFunction() {
  while (!terminate_) {
    Task task;
    if (DequeueTask(task)) {
      task.Execute();
    }
    else {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  std::cout << "TaskManager: thread " << std::this_thread::get_id() << " terminated." << std::endl;
}

TaskManager* TaskManager::GetInstance() {
  return instance_;
}

bool TaskManager::Exist() {
  return instance_ != nullptr;
}

void TaskManager::Initialize() {
  instance_ = new TaskManager();
}

void TaskManager::Terminate() {
  instance_->EndThreads();
  delete instance_;
}

void TaskManager::BeginThreads(int thread_count) {
  thread_count_ = thread_count;
  for (int i = 0; i < thread_count_; ++i) {
    threads_.emplace_back(std::thread(&TaskManager::ThreadFunction, this));
  }
}

bool TaskManager::HasTasks() {
  std::lock_guard<std::mutex> guard(queue_mutex_);
  return !task_queue_.empty();
}

void TaskManager::EndThreads() {
  terminate_ = true;
  for (int i = 0; i < thread_count_; ++i) {
    threads_[i].join();
  }
}

