#include "timer.h"

Timer::Timer() : is_running_(false), start_ticks_(0), stop_ticks_(0), current_ticks_(0) {
}

void Timer::Start() {
  is_running_ = true;
}

void Timer::Stop() {
  is_running_ = false;
}

void Timer::Reset() {
  is_running_ = false;
  ticks_per_second_ = 0;
  start_ticks_ = 0;
  stop_ticks_ = 0;
  current_ticks_ = 0;
}

float Timer::GetElapsedSeconds() {
  __int64 delta_int = current_ticks_ - start_ticks_;
  double delta = static_cast<double>(delta_int) / static_cast<double>(ticks_per_second_);
  return static_cast<float>(delta);
}