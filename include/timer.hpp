#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

namespace timer {

class Timer final {

public:
  using microsecs = std::chrono::microseconds;
  using millisecs = std::chrono::milliseconds;
  using clock_t = std::chrono::high_resolution_clock;
  //
private:
  std::chrono::time_point<clock_t> m_beg;

public:
  Timer() : m_beg(clock_t::now()) {}

  void reset_time() { m_beg = clock_t::now(); }

  template <typename time_type> double elapsed() const {
    return std::chrono::duration_cast<time_type>(clock_t::now() - m_beg)
        .count();
  }
};
} // namespace timer

#endif // TIMER_HPP