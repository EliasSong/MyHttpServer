#include "MyConf.h"

MyConf::MyConf(){
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
