#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
using namespace std;
using namespace muduo;
using namespace muduo::net;

#include "redis.hpp"
#include "groupmodel.hpp"
#include "friendmodel.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "json.hpp"
using json = nlohmann::json;

// 表示处理消息的事件方法回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp time)>;

// 聊天服务器业务类
class ChatService
{
public:
    // 获取单例对象的接口函数
    static ChatService *instance();
    // 处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 添加好友业务 msgid, id, friendid
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 创建群组业务 msgid, groupname, groupdesc
    void creatGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务 msgid, id, groupid
    void joinGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务 msgid, id, groupid
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    // 服务器异常，业务重置方法 
    void reset();
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    // 从reids消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int userid, string msg);

private:
    ChatService();

    // 存储消息id和其对应的业务处理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;

    // 存储在线用户的在线连接
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    // 设置互斥锁，保证_userConnMap的线程安全
    mutex _connmutex;

    // 数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    // redis操作对象
    Redis _redis;
};

#endif