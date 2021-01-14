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

MyConf::MyConf(){
    cout<<"Construct configure"<<endl;
    ifstream jsonFile(confPath);
    if(!jsonFile){
        cout<<"Configure failed"<<endl;
        return;
    }
    jsonFile >> confJson;
    PORT = confJson.at("port");
    THREAD_NUM = confJson.at("thread_num");
    DEFAULT_STATIC_PATH = confJson.at("default_static_path");
    DEFAULT_INDEX_FILE = confJson.at("default_index_file");
    LOG_PATH = confJson.at("log_path");
}

MyConf::~MyConf(){
    cout<<"Destroy configure"<<endl;
}

shared_ptr<MyConf> MyConf::instance = nullptr;

shared_ptr<MyConf> MyConf::getInstance(){
    if(instance == nullptr){
        instance = shared_ptr<MyConf>(new MyConf());
    }
    return instance;
}
