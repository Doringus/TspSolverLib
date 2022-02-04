#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue {
public:

    void put(T value) {
        std::lock_guard guard(m_Mutex);
        m_Tasks.push(std::move(value));
        m_NotEmpty.notify_one();
    }

    T take() {
        std::unique_lock lock(m_Mutex);
        while (m_Tasks.empty()) {
            m_NotEmpty.wait(lock);
        }
        return takeLocked();
    }

private:

    T takeLocked() {
        T front = m_Tasks.front();
        m_Tasks.pop();
        return front;
    }

private:
    std::queue<T> m_Tasks;
    std::mutex m_Mutex;
    std::condition_variable m_NotEmpty;
};
