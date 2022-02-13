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
    StaticThreadPool() : m_ShouldWork(true) { }

    constexpr void start(size_t threadsCount) {
        for(size_t i = 0; i < threadsCount; ++i) {
            m_Threads.emplace_back([this](){
                workerRoutine();
            });
        }
    }

    template <typename Func, typename... Args, typename Return = std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>>
    std::future<Return> submit(const Func& task, Args&&... args) {
        auto promise = std::make_shared<std::promise<Return>>();
        auto future = promise->get_future();
        std::function<void()> function;
        if constexpr (std::is_void_v<Return>) {
            function = [fargs = std::make_tuple(std::forward<Args>(args)...), callable = task, promise]() {
                std::apply(callable, fargs);
                promise->set_value();
            };
        } else {
            function = [fargs = std::make_tuple(std::forward<Args>(args)...), callable = task, promise]() {
                promise->set_value(std::apply(callable, fargs));
            };
        }
        m_Tasks.put(std::move(function));
        return future;
    }

    void shutdown() {
        m_ShouldWork = false;
        for(auto& thread : m_Threads) {
            m_Tasks.put([](){});
            thread.join();
        }
        m_Threads.clear();
    }

private:

    void workerRoutine() {
        while(m_ShouldWork) {
            auto task = m_Tasks.take();
            task();
        }
    }

private:
    std::vector<std::thread> m_Threads;
    BlockingQueue<Task> m_Tasks;
    bool m_ShouldWork;
};
