//
// Created by Георгий Круглов on 20.05.2021.
//

#ifndef GENETICCUBE_THREADPOOL_HPP
#define GENETICCUBE_THREADPOOL_HPP

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace Utility {
    class ThreadPool {
        std::vector<std::thread> threads;
        std::queue<std::function<void()> > tasks;

        std::mutex queueMutex;
        std::condition_variable cv;
        bool stop;


    public:
        ThreadPool(size_t);

        template<class F, class... Args>
        auto Enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

        void Invalidate() { stop = true; }

        ~ThreadPool();
    };


    inline ThreadPool::ThreadPool(size_t size)
            : stop(false) {
        for (size_t i = 0; i < size; ++i)
            threads.emplace_back(
                    [this] {
                        for (;;) {
                            std::function<void()> task;

                            {
                                std::unique_lock<std::mutex> lock(this->queueMutex);
                                this->cv.wait(lock,
                                              [this] { return this->stop || !this->tasks.empty(); });
                                if (this->stop && this->tasks.empty())
                                    return;
                                task = std::move(this->tasks.front());
                                this->tasks.pop();
                            }

                            task();
                        }
                    }
            );
    }

    template<class F, class... Args>
    auto ThreadPool::Enqueue(F &&f, Args &&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()> >(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (stop)
                throw std::runtime_error("Enqueue on stopped ThreadPool");

            tasks.emplace([task]() { (*task)(); });
        }
        cv.notify_one();
        return res;
    }

    inline ThreadPool::~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        cv.notify_all();
        for (std::thread &worker: threads)
            worker.join();
    }
}


#endif //GENETICCUBE_THREADPOOL_HPP
