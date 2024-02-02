#include <stdio.h>
#include "Threadpool.h"

int main() {
    int initThreadCount = 1;
    int maxThreadCount = 4;
    int idleSec = 6;
    ThreadPool pool(initThreadCount, maxThreadCount, idleSec);

    pool.AddTask([](void* arg){
        printf("this is task1.\n");
    }, nullptr);

    pool.AddTask([](void* arg){
        printf("this is task2.\n");
    }, nullptr);

    pool.AddTask([](void* arg){
        printf("this is task3.\n");
    }, nullptr);

    return 0;
}