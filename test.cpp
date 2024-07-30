#include <iostream>
#include <mysql/mysql.h>
#include <string>
using namespace std;

// 数据库配置信息
static string server = "192.168.232.128";
static string user = "root";
static string password = "456852";
static string dbname = "chat";
int main(){
    auto _conn = mysql_init(nullptr);
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(),
                                  password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    cout << p << endl;
}