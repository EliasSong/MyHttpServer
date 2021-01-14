#include <iostream>
#include <fstream>
#include <memory>
#include "./json.hpp"
using namespace std;
using json = nlohmann::json;

class MyConf{
public:
    static shared_ptr<MyConf> getInstance();


private:
    string confPath = "./Configure/conf.json"
    MyConf(){};
    MyConf(const MyConf&){};
    MyConf &operator=(const MyConf&){};
    static shared_ptr<MyConf> instance;
};
