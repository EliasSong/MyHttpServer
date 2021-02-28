#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "Connection.h"
#include "MyThread.h"
using namespace std;

class MyThreadPool {
   public:
    static shared_ptr<MyThreadPool> getInstance();
    void init(int num);
    void dispath(struct evhttp_request *request);
    ~MyThreadPool();

   private:
    MyThreadPool();
    MyThreadPool(const MyThread &){};
    MyThreadPool &operator=(const MyThread &){};
    static shared_ptr<MyThreadPool> instance;
    int currentThreadIdx = -1;
    int threadNum = 0;
    vector<MyThread *> threads;
};
