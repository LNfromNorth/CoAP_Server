#include <iostream>
#include <functional>
#include <memory>

#include "Threadpooln.h"

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