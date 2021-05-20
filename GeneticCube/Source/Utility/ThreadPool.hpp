//
// Created by Георгий Круглов on 20.05.2021.
//

#ifndef GENETICCUBE_THREADPOOL_HPP
#define GENETICCUBE_THREADPOOL_HPP

#include <thread>
#include <mutex>
#include <future>
#include <queue>
#include <functional>

namespace Utility {
    class ThreadPool {
        std::vector<std::thread> workers;
        std::queue<std::function<void()> > tasks;

        std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;

    public:
        ThreadPool(size_t);

        template<class F, class... Args>
        auto AddWork(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

        ~ThreadPool();
    };
}


#endif //GENETICCUBE_THREADPOOL_HPP
