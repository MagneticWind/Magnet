#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

class Timer {
public:
  Timer();

  void Start();
  void Stop();
  void Reset();
  float GetElapsedSeconds();

private:
  bool is_running_;
  int64_t ticks_per_second_;
  __int64 start_ticks_;
  __int64 stop_ticks_;
  __int64 current_ticks_;
};

#endif
