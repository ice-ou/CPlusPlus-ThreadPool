#include "ThreadPool.h"


ThreadPool::ThreadPool(int num_threds):stop(false){
    for(int i = 0;i<num_threds;i++){
        threads.emplace_back(std::thread([this](){
             while(true){
                std::function<void()> task;
                    std::unique_lock<std::mutex> lock(task_mtx);  //初始化即锁定
                    has_job.wait(lock, [this](){
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) return;
                    task = tasks.front();
                    tasks.pop();
                    lock.unlock();
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool(){
    std::unique_lock<std::mutex> lock(task_mtx);
    stop = true;
    lock.unlock();

    std::cout<<"delete"<<std::endl;
    has_job.notify_all();  //唤醒所有当前等待在该条件变量上的线程

    for(std::thread &th:threads){
        if(th.joinable())
            th.join(); //等待线程完成
            //线程的资源已经被回收，但是std::thread对象t仍然存在
    }
}

void ThreadPool::add(std::function<void()> func){

    std::unique_lock<std::mutex> lock(task_mtx);
    if(stop)
        throw std::runtime_error("ThreadPool already stop, can't add task any more");
    tasks.emplace(func);
    lock.unlock();
    
    has_job.notify_one(); //唤醒一个正在等待该条件变量的线程
}