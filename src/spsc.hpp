#pragma once
#include <cstddef>

namespace spsc {

template<class T, std::size_t Size>
class Queue
{
public:
  void push(const T &)
  {}
  const T pop()
  {
    return T{};
  }
};
}// namespace spsc
