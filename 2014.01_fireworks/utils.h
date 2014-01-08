#ifndef UTILS_H
#define UTILS_H

template<typename T>
T clamp(T val) {
  using std::max;
  using std::min;
  return max(min(val, T(1.0)), T(0.0));
}

#endif
