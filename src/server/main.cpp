#include "ChatServer.hpp"
#include "ChatService.hpp"
# include<iostream>
#include <signal.h>

using namespace std;

// 处理服务器ctrl+C结束后，重置user的状态信息
void resetHandler(int){
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, resetHandler);

    EventLoop loop; // epoll
    InetAddress addr(argv[1], atoi(argv[2]));
    ChatServer server(&loop, addr, "ChatServer-lyj");

    server.start(); // listenfd epoll_stl=>epoll（将listenfd通过epoll_stl添加到epoll上）
    loop.loop();    // epoll_wait以阻塞方式等待新用户连接，已连接用户的读写时间等

    return 0;
}