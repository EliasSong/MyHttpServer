#pragma once
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/keyvalq_struct.h>
#include <event2/util.h>
#include <evhttp.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include "MyConf.h"
#include "TypeFactory.h"
using namespace std;

string getCurrentTime();

class Connection {
   public:
    void reply(struct evhttp_request *request);
    ~Connection() { cout << "Connection destory" << endl; }
    Connection();
    string getMethod(struct evhttp_request *request);
    string getURL(struct evhttp_request *request);
    string getRemoteHost(struct evhttp_request *request);

   private:
    mutex fileMutex;
    FILE *logFp = nullptr;
    string basePath = "";
    string logPath = "";
    string indexFilename = "";
};
