#pragma once
#include <functional>  //函数指针
#include <vector>
#include <queue>
#include <thread>  //C++11中引入的线程
#include <mutex>  //互斥锁
#include <condition_variable>  //条件变量
#include <iostream>

class ThreadPool{
private:
    std::vector<std::thread> threads;  //线程池容器
    std::queue<std::function<void()>> tasks;  //工作队列
    //std::function<void()> 是 std::function 的一个特化版本，
    //它表示一个没有参数且不返回任何值的可调用对象
    std::mutex task_mtx;
    std::condition_variable has_job;
    bool stop;
public:
    ThreadPool(int num_threds =10);
    ~ThreadPool();

    void add(std::function<void()>);

};