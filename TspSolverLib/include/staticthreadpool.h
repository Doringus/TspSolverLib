#pragma once

#include <thread>
#include <functional>
#include <vector>
#include <future>
#include <memory>

#include "blockingqueue.h"

using Task = std::function<void()>;

class StaticThreadPool {
public:
    StaticThreadPool() = default;

    constexpr void start(size_t threadsCount) {
        for(size_t i = 0; i < threadsCount; ++i) {
            m_Threads.emplace_back([this](){
                workerRoutine();
            });
        }
    }

    template <typename Func, typename... Args, typename Return = std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>>
    std::future<Return> submit(const Func& task, const Args&... args) {
        auto promise = std::make_shared<std::promise<Return>>();
        auto future = promise->get_future();
        if constexpr (std::is_void_v<Return>) {
            pushTask([task, args..., promise]() {
                task(args...);
                promise->set_value();
            });
        } else {
            pushTask([task, args..., promise]() {
                promise->set_value(task(args...));
            });
        }
        return future;
    }

    void shutdown() {
        for(auto& thread : m_Threads) {
            thread.join();
        }
        m_Threads.clear();
    }

private:

    template <typename  Func>
    void pushTask(const Func& task) {
        m_Tasks.put(task);
    }

    template <typename Func, typename... Args>
    void pushTask(const Func& task, Args... args) {
        pushTask([task, args...](){
            task(args...);
        });
    }

    void workerRoutine() {
        while(true) {
            auto task = m_Tasks.take();
            task();
        }
    }

private:
    std::vector<std::thread> m_Threads;
    BlockingQueue<Task> m_Tasks;
};
