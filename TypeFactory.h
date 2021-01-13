#pragma once
#include <memory>
#include <iostream>
#include<unordered_map>
#include<string>
using namespace std;

class TypeFactory
{
public:
    static shared_ptr<TypeFactory> getInstance();
    static shared_ptr<TypeFactory> instance;
    string getType(string s);
private:
    TypeFactory(){
        cout<<"Constructing TypeFactory...."<<endl;
        rec["jpg"] = "image/jpg";
        rec["gif"] = "image/gif";
        rec["png"] = "image/png";
        rec["zip"] = "application/zip";
        rec["html"] = "text/html";
        rec["css"] = "text/css";
        rec["js"] = "text/css";
        rec["ico"] = "text/ico";
        cout<<"Construct TypeFactory successfully."<<endl;
    };
    TypeFactory(const TypeFactory&){};
    TypeFactory &operator=(const TypeFactory&){};
    unordered_map<string,string> rec;
};

shared_ptr<TypeFactory> TypeFactory::instance = nullptr;

shared_ptr<TypeFactory> TypeFactory::getInstance(){
    if(instance == nullptr){
        instance = shared_ptr<TypeFactory>(new TypeFactory());
    }
    return instance;
}

string TypeFactory::getType(string s){
    return rec[s];
}