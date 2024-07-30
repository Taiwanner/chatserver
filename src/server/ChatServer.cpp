#include "ChatServer.hpp"
#include "json.hpp"
#include "ChatService.hpp"
#include <functional>
#include <string>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

/*基于muduo库开发服务器程序
    1. 组合TcpServer对象
    2. 创建EventLoop事件循环对象的指针
    3. 明确TcpServer对象需要什么参数，输出ChatServer的构造函数
    4. 在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
    5. 设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
*/

ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg) : _server(loop, listenAddr, nameArg),
                                                _loop(loop)
{
    // 给服务器注册用户连接的创建和断开回调
    // 注册后，当有连接创建或断开的时候，会自动调用该函数
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

    // 给服务器注册用户读写事件回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    // 设置服务器端的线程数量，1个I/O线程，3个worker线程
    _server.setThreadNum(4);
}

// 开启事件循环
void ChatServer::start()
{
    _server.start();
}

// 专门处理用户的连接创建和断开
// 上报连接相关信息的回调接口
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // 断开连接
    if (! conn->connected())
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
    else
    {
    }
}

// 上报读写事件相关的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn, // 连接
                           Buffer *buffer,               // 缓冲区
                           Timestamp time)               // 接收到数据的时间信息
{
    string buf = buffer->retrieveAllAsString();
    // 数据的反序列化
    json js = json::parse(buf);
    // 达到的目的：完全解耦网络模块的代码和业务模块的代码
    // 通过js["msgid"]获取业务handler，传参conn js time
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}