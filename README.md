# MyHttpServer

A simple http server implementation that can host static pages

### Requirement:

- libevent 2.1.11
- openssl 1.1.1

### Feature:

- Support concurrent http connection
- Support logging

### Configure：

```
{
    "port" : 8080,
    "thread_num" : 10,
    "default_static_path" : "./common",
    "default_index_file" : "/index.html",
    "log_path" : "./log/log.txt"
}
```

### Compile：

```
make
```

### Run:

```
nohup ./main &
```