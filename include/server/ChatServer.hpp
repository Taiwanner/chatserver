#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
// #include <iostream>
// #include <functional>

// using namespace std;
using namespace muduo;
using namespace muduo::net;
// using namespace placeholders;

/*基于muduo库开发服务器程序
    1. 组合TcpServer对象
    2. 创建EventLoop事件循环对象的指针
    3. 明确TcpServer对象需要什么参数，输出ChatServer的构造函数
    4. 在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
    5. 设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
*/
// 聊天服务器主类
class ChatServer
{
public:
    // 初始化聊天服务器对象
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg);

    // 开启事件循环
    void start();

private:
    // 专门处理用户的连接创建和断开
    // 上报连接相关信息的回调函数
    void onConnection(const TcpConnectionPtr &conn);

    // 专门处理用户的读写时间
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time);              // 接收到数据的时间信息
    TcpServer _server;                           // 组合的muduo库，实现服务器功能的类对象
    EventLoop *_loop;                            // 指向事件循环对象的指针
};

#endif