#include <iostream>
#include <functional>
#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    template <class F, class... Args>
    void enqueue(F&& f, Args&&... args) {
        auto task = std::make_shared<std::function<void()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }

protected:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

// MyClass类
class MyClass : public ThreadPool {
public:
    MyClass(size_t numThreads) : ThreadPool(numThreads) {}

    void classMethod(int arg) {
        std::cout << "Class method called with argument: " << arg << std::endl;
    }

    // 测试函数
    void test() {
        int arg = 42;
        enqueue([this, arg]() {
            classMethod(arg);
        });
    }
};

int main() {
    MyClass myThreadPool(4); // 创建线程池，包含4个线程

    // 调用测试函数
    myThreadPool.test();

    // 等待所有任务完成
    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}
