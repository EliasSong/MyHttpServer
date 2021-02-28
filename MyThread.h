#pragma once
#include <event2/event.h>
#include <event2/util.h>
#include <unistd.h>

#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "Connection.h"
using namespace std;

class MyThread {
   public:
    void setThreadIdx(int index);
    int getThreadIdx();
    void addRequestToQueue(struct evhttp_request *request);
    void start();
    void activate();
    void main();
    void notify(evutil_socket_t fd, short which);

   private:
    int id;
    queue<struct evhttp_request *> tasks;
    mutex myMutex;
    int notify_send_fd = 0;
    struct event_base *base = 0;  //线程中event_base上下文
};
