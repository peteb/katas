#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <streambuf>
#include <vector>

template<typename T>
T clamp(T val) {
  using std::max;
  using std::min;
  return max(min(val, T(1.0)), T(0.0));
}

std::vector<char> read_file(const char *filename) {
  std::vector<char> buffer;
  
  std::ifstream stream(filename);
  stream.seekg(0, std::ios::end);
  buffer.reserve(stream.tellg());
  stream.seekg(0, std::ios::beg);  
  buffer.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
  
  return buffer;
}

#endif
