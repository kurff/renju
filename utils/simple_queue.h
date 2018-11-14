/**
 * Copyright (c) 2016-present, Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RENJU_UTILS_H_
#define RENJU_UTILS_H_

#include <condition_variable>  // NOLINT
#include <mutex>  // NOLINT
#include <queue>

#include "glog/logging.h"

namespace Beta {

// This is a very simple queue that Yangqing wrote when bottlefeeding the baby,
// so don't take it seriously. What it does is a minimal thread-safe queue that
// allows me to run network as a DAG.
//
// A usual work pattern looks like this: one or multiple producers push jobs
// into this queue, and one or multiple workers pops jobs from this queue. If
// nothing is in the queue but NoMoreJobs() is not called yet, the pop calls
// will wait. If NoMoreJobs() has been called, pop calls will return false,
// which serves as a message to the workers that they should exit.
template <typename T>
class SimpleQueue {
 public:
  SimpleQueue() : no_more_jobs_(false) {}

  // Pops a value and writes it to the value pointer. If there is nothing in the
  // queue, this will wait till a value is inserted to the queue. If there are
  // no more jobs to pop, the function returns false. Otherwise, it returns
  // true.
  bool Pop(T* value) {
    std::unique_lock<std::mutex> mutex_lock(mutex_);
    while (queue_.size() == 0 && !no_more_jobs_) cv_.wait(mutex_lock);
    if (queue_.size() == 0 && no_more_jobs_) return false;
    *value = queue_.front();
    queue_.pop();
    return true;
  }

  int size() {
    std::unique_lock<std::mutex> mutex_lock(mutex_);
    return queue_.size();
  }

  // Push pushes a value to the queue.
  void Push(const T& value) {
    {
      std::lock_guard<std::mutex> mutex_lock(mutex_);
      CHECK(!no_more_jobs_)<< "Cannot push to a closed queue.";
      queue_.push(value);
    }
    cv_.notify_one();
  }

  // NoMoreJobs() marks the close of this queue. It also notifies all waiting
  // Pop() calls so that they either check out remaining jobs, or return false.
  // After NoMoreJobs() is called, this queue is considered closed - no more
  // Push() functions are allowed, and once existing items are all checked out
  // by the Pop() functions, any more Pop() function will immediately return
  // false with nothing set to the value.
  void NoMoreJobs() {
    {
      std::lock_guard<std::mutex> mutex_lock(mutex_);
      no_more_jobs_ = true;
    }
    cv_.notify_all();
  }

 private:
  std::mutex mutex_;
  std::condition_variable cv_;
  std::queue<T> queue_;
  bool no_more_jobs_;
  // We do not allow copy constructors.
  SimpleQueue(const SimpleQueue& /*src*/) {}
};

}  // namespace Beta

#endif  // 
