#ifndef __COAP_SERVER_THREADPOOLN_
#define __COAP_SERVER_THREADPOOLN_

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <stdio.h>

#include "Log.h"

class ThreadPool {
public:
<<<<<<< HEAD
    ThreadPool(size_t numThreads);
    ~ThreadPool();
=======
    ThreadPool(size_t numThreads) : stop(false) {
        for(size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                for(;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return ;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    printf("start run task\n");
                    task();
                    printf("finish run task\n");
                }
            });
        }
    }
>>>>>>> cb1eb1c268cddcedb00aba60888b603433849bbe

    template <class F, class... Args>
    void enqueue(F&& f, Args&&... args) {
        auto task = std::make_shared<std::function<void()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

#endif