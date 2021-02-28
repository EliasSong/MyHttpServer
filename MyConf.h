#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include "./json.hpp"
using namespace std;
using json = nlohmann::json;

class MyConf{
public:
    static shared_ptr<MyConf> getInstance();
    int PORT = 0;
    int THREAD_NUM = 0;
    string DEFAULT_STATIC_PATH = "";
    string DEFAULT_INDEX_FILE = "";
    string LOG_PATH = "";
    ~MyConf();
private:
    string confPath = "./configure/conf.json";
    json confJson;
    MyConf();
    MyConf(const MyConf&){};
    MyConf &operator=(const MyConf&){};
    static shared_ptr<MyConf> instance;
};

