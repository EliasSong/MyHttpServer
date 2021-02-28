#include <event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include <evhttp.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>

#include "Connection.h"
#include "MyConf.h"
#include "MyThreadPool.h"
using namespace std;

void http_cb(struct evhttp_request *request, void *arg) {
    MyThreadPool::getInstance()->dispath(request);
}
int main() {
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        exit(0);
    }

    pid = setsid();
    if (pid == -1) exit(1);

    // int ret = chdir("/");
    // if (ret == -1) exit(1);

    umask(0);
    close(STDIN_FILENO);

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1) exit(1);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    auto conf = MyConf::getInstance();
    MyThreadPool::getInstance()->init(conf->THREAD_NUM);
    event_base *base = event_base_new();
    //创建http上下文
    evhttp *evh = evhttp_new(base);
    //绑定端口ip
    if (evhttp_bind_socket(evh, "0.0.0.0", conf->PORT) != 0) {
        cout << "Libevent bind failed" << endl;
        exit(1);
    }
    //设定回调函数
    evhttp_set_gencb(evh, http_cb, 0);
    event_base_dispatch(base);
    event_base_free(base);
    evhttp_free(evh);
}
