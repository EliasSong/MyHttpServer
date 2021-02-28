#include "MyThread.h"

static void notifyCallback(evutil_socket_t fd, short which, void *arg) {
    MyThread *th = (MyThread *)arg;
    th->notify(fd, which);
}

void MyThread::setThreadIdx(int index) { id = index; }
int MyThread::getThreadIdx() { return id; }

void MyThread::addRequestToQueue(struct evhttp_request *request) {
    if (!request) return;
    // conn->base = this->base;
    myMutex.lock();
    tasks.push(request);
    myMutex.unlock();
}

void MyThread::start() {
    //创建管道
    int fds[2];
    if (pipe(fds)) {
        cerr << "Pipe failed" << endl;
        return;
    }
    notify_send_fd = fds[1];
    //创建libevent上下文，配置为非阻塞
    event_config *ev_conf = event_config_new();
    event_config_set_flag(ev_conf, EVENT_BASE_FLAG_NOLOCK);
    this->base = event_base_new_with_config(ev_conf);
    event_config_free(ev_conf);
    if (!base) {
        cerr << "Base create failed" << endl;
        return;
    }
    //创建回调事件
    event *ev =
        event_new(base, fds[0], EV_READ | EV_PERSIST, notifyCallback, this);
    event_add(ev, 0);
    thread th(&MyThread::main, this);
    th.detach();
}

void MyThread::main() {
    cout << "Thread: " << id << " dispath" << endl;
    event_base_dispatch(base);
    event_base_free(base);
    cout << "Thread: " << id << " dispath free" << endl;
}

void MyThread::notify(evutil_socket_t fd, short which) {
    //水平触发
    char buf[2] = {0};
    int re = read(fd, buf, 1);
    if (re <= 0) return;
    //获取任务并初始化任务
    // Connection *currentConn = nullptr;
    myMutex.lock();
    if (tasks.empty()) {
        cout << "Tasks queue is empty!!!!!" << endl;
        myMutex.unlock();
        return;
    }
    struct evhttp_request *curRequest = tasks.front();
    tasks.pop();  //先进先出
    myMutex.unlock();
    Connection *conn = new Connection();
    conn->reply(curRequest);
    delete conn;
}

void MyThread::activate() {
    int re = write(this->notify_send_fd, "c", 1);
    if (re <= 0) {
        cerr << "Activate failed" << endl;
    } else {
        cout << "Thread: " << id << " activate successfully" << endl;
    }
}