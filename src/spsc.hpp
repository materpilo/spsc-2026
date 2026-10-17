#pragma once
#include <condition_variable>
#include <cstddef>
#include <mutex>

namespace spsc {

/**
 * Single Producer - Single Consumer Queue
 *
 * @tparam T Type of the elements stored in the queue
 * @tparam Size - size of underlying storage. Queue stores up to Size-1 elements.
 *
 * Queue is implemented as FIFO circular buffer. It is optimized for single producer and single consumer threads.
 * The access is synchronized on single condition variable, therefore it is optimized for single producer and single
 * consumer threads. The maximum number of elements that can be stored in the queue is (Size - 1) because one empty slot
 * is used to signal full condition.
 */
template<class T, std::size_t Size>
class Queue
{
  static_assert(Size >= 2, "Size must be at least 2");

private:
  mutable std::mutex mutex_;
  std::condition_variable cv_;

  T storage_[Size] = {};

  /* Next to write */
  std::size_t head_ = 0;
  /* Next to read */
  std::size_t tail_ = 0;

  /**
   * Currently stored elements count
   * NOT THREAD SAFE
   */
  constexpr inline std::size_t CountInternal() const
  {
    return (head_ + Size - tail_) % Size;
  }

  /**
   * Returns true if the queue is full, false otherwise.
   * NOT THREAD SAFE
   */
  constexpr inline bool IsFull() const
  {
    return CountInternal() == Capacity();
  }

  /**
   * Returns true if the queue is empty, false otherwise.
   * NOT THREAD SAFE
   */
  constexpr inline bool IsEmpty() const
  {
    return head_ == tail_;
  }

public:
  Queue() = default;
  Queue(const Queue &) = delete;
  Queue(Queue &&) = delete;
  Queue &operator=(const Queue &) = delete;
  Queue &operator=(Queue &&) = delete;

  /**
   * Returns the maximum number of elements that can be stored in the queue.
   * Always (Size - 1)
   */
  static consteval std::size_t Capacity()
  {
    return Size - 1U;
  }

  /**
   * Pushes an element into the queue. If the queue is full, the function blocks until space is available.
   * @param el Element to be pushed into the queue.
   */
  void push(T &&el)
  {
    std::unique_lock<std::mutex> lock{ mutex_ };
    cv_.wait(lock, [this] { return !IsFull(); });
    storage_[head_] = std::move(el);
    head_ = (head_ + 1) % Size;
  }

  /**
   * Pops an element from the queue. If the queue is empty, the function blocks until an element is available.
   * @return The element popped from the queue.
   */
  T pop()
  {
    std::unique_lock<std::mutex> lock{ mutex_ };
    cv_.wait(lock, [this] { return !IsEmpty(); });
    T el{};
    std::swap(el, storage_[tail_]);
    tail_ = (tail_ + 1) % Size;
    return el;
  }

  /**
   * Returns the number of elements currently stored in the queue.
   */
  std::size_t Count() const
  {
    std::unique_lock<std::mutex> lock{ mutex_ };
    return CountInternal();
  }
};
}// namespace spsc
