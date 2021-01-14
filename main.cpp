#include <iostream>
#include "MyThreadPool.h"
#include<evhttp.h>
#include<event2/keyvalq_struct.h>
#include<event2/buffer.h>
#include "Connection.h"
#include "MyConf.h"
using namespace std;


void http_cb(struct evhttp_request *request, void *arg) {
	MyThreadPool::getInstance() -> dispath(request);
}
int main(){
	auto conf = MyConf::getInstance();
    MyThreadPool::getInstance()->init(conf->THREAD_NUM);
    event_base *base = event_base_new();
	//创建http上下文
	evhttp *evh = evhttp_new(base);
	//绑定端口ip
	if (evhttp_bind_socket(evh,"0.0.0.0",conf->PORT) != 0){
		cout<<"Libevent bind failed"<<endl;
	}
	//设定回调函数
	evhttp_set_gencb(evh,http_cb,0);
	event_base_dispatch(base);
	event_base_free(base);
	evhttp_free(evh);
    return 0;
}
