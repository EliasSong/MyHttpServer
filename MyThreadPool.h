#include <iostream>
#include "MyThread.h"
#include <memory>
#include <vector>
#include "Connection.h"
using namespace std;

class MyThreadPool
{
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

shared_ptr<MyThreadPool> MyThreadPool::instance = nullptr;

shared_ptr<MyThreadPool> MyThreadPool::getInstance()
{
    if (instance == nullptr)
    {
        instance = shared_ptr<MyThreadPool>(new MyThreadPool());
    }
    return instance;
}

void MyThreadPool::init(int num)
{
    if (num < 1)
    {
        cerr << "The threadNum is invaild!" << endl;
        return;
    }
    else if (threadNum > 0)
    {
        cerr << "The ThreadPool had initialized!" << endl;
        return;
    }
    threadNum = num;
    currentThreadIdx = -1;
    for (int i = 0; i < threadNum; ++i)
    {
        MyThread *temp = new MyThread();
        temp->setThreadIdx(i + 1);
        threads.push_back(temp);
        cout << "Creating the thread id: " << temp->getThreadIdx() << " ..." << endl;
        temp->start();
        cout << "Create the thread id: " << temp->getThreadIdx() << " successfully." << endl;
        this_thread::sleep_for(10ms);
    }
}

void MyThreadPool::dispath(struct evhttp_request *request)
{
    if (!request)
        return;
    int tid = (currentThreadIdx + 1) % threadNum;
    currentThreadIdx = tid;
    threads[tid]->addRequestToQueue(request);
    threads[tid]->activate();
}

MyThreadPool::~MyThreadPool()
{
    cout << "Destroy ThreadPool!!" << endl;
}
MyThreadPool::MyThreadPool()
{
    cout << "Construct ThreadPool!!" << endl;
}