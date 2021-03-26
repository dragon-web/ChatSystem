#pragma once 
#include <iostream>
#include <string>
#include<unordered_map>
#include <unordered_set>


//记录用户的信息
class UserInfo
{
    UserInfo(std::string &nickname ,std::string &school,std::string ::passwd) : 
    private:
        std::string nickname_;
        std::string school_;
        std::string passwd_;
};
