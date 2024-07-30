#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include"user.hpp"
#include<vector>

using namespace std;

class FriendModel{
public:
    // 添加好友信息
    void insert(int userid, int friendid);

    // 返回用户好友列表
    vector<User> quiry(int userid);
};

#endif