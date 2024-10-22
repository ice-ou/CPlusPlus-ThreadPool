#include <iostream>
#include <string>
#include "ThreadPool.h"

void print(int a, double b, const char* c, std::string d){
    std::cout<<a<<b<<c<<d<<std::endl;
}

void test(){
    std::cout<<"hello"<<std::endl;
}

int main(){
    ThreadPool* thpool_p = new ThreadPool(4);
    std::function<void()> func = std::bind(print, 1, 3.14, "hello", std::string("world"));
    thpool_p->add(func);
    func = test;
    thpool_p->add(func);
    delete thpool_p;
    return 0;
}