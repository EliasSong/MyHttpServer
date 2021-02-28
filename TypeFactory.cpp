#include "TypeFactory.h"

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