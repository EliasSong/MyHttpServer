#include "Connection.h"

string getCurrentTime() {
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
    return tmp;
}

Connection::Connection() {
    auto conf = MyConf::getInstance();
    basePath = conf->DEFAULT_STATIC_PATH;
    logPath = conf->LOG_PATH;
    indexFilename = conf->DEFAULT_INDEX_FILE;
}

string Connection::getMethod(struct evhttp_request *request) {
    string method = "";
    switch (evhttp_request_get_command(request)) {
        case EVHTTP_REQ_GET:
            method = "GET";
            break;
        case EVHTTP_REQ_POST:
            method = "POST";
            break;
        case EVHTTP_REQ_HEAD:
            method = "HEAD";
            break;
    }
    return method;
}

string Connection::getURL(struct evhttp_request *request) {
    return evhttp_request_get_uri(request);
}

string Connection::getRemoteHost(struct evhttp_request *request) {
    return request->remote_host;
}

void Connection::reply(struct evhttp_request *request) {
    fileMutex.lock();
    logFp = fopen(logPath.c_str(), "a+");
    if (!logFp) {
        cerr << "Log File open failed!" << endl;
        fileMutex.unlock();
        return;
    }
    string method = getMethod(request);
    if (method.size() < 1) {
        cerr << "Method parse failed" << endl;
        fileMutex.unlock();
        return;
    }
    string uri = getURL(request);
    string log = getCurrentTime() + " Client IP: " + getRemoteHost(request) +
                 " Method: " + getMethod(request) + " URL: " + uri + "\n";
    char buf[1024] = {0};
    string file_path = basePath;
    if (uri.size() < 2) {
        file_path += indexFilename;
    } else {
        file_path += uri;
    }
    evkeyvalq *outhead = evhttp_request_get_output_headers(request);
    int pos = file_path.rfind('.');
    string postFix = file_path.substr(pos + 1, file_path.size() - (pos + 1));
    string type = TypeFactory::getInstance()->getType(postFix);
    if (postFix.size() < 1 || type.size() < 1) {
        cerr << "Route parse error" << endl;
        return;
    }
    evhttp_add_header(outhead, "Content-Type", type.c_str());
    FILE *fp = fopen(file_path.c_str(), "rb");
    if (!fp) {
        evhttp_send_reply(request, HTTP_NOTFOUND, "", 0);
        return;
    }
    evbuffer *outbuf = evhttp_request_get_output_buffer(request);
    while (true) {
        int len = fread(buf, 1, sizeof(buf), fp);
        if (len <= 0) break;
        evbuffer_add(outbuf, buf, len);
    }
    fputs(log.c_str(), logFp);
    fclose(fp);
    fclose(logFp);
    logFp = nullptr;
    fileMutex.unlock();
    evhttp_send_reply(request, HTTP_OK, "", outbuf);
}
