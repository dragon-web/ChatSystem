#pragma once 
#include <iostream>
#include <string>
#include<unordered_map>
#include <unordered_set>
#include "ConnectInfo.hpp"

using namespace std;
//记录用户的信息
class UserInfo
{
    public:
    UserInfo(std::string &nickname ,std::string &school,string& passwd, uint32_t user_id) : nickname_(nickname),school_(school),passwd_(passwd),user_id_(user_id)
    {

    }
    ~UserInfo()
    {
        
    }
    private:
        std::string nickname_;
        std::string school_;
        std::string passwd_;
        uint32_t user_id_;  //用户id
};


class Usermanager   // string id      UserInfo  保存用户信息  
{
    public:
        Usermanager()
        {
            user_map_.clear();
            pthread_mutex_init(&map_lock_,NULL);
            prepare_id_ = 0;
        }
        ~Usermanager()
        {
            pthread_mutex_destroy(&map_lock_);
        }
        //处理注册的请求
        int DealRegister(string &nickname , string &school , string& passwd)  
        {
            if(nickname.size() == 0 || school.size() == 0 || passwd.size() == 0)
            {
                return -1;
            }
            //创建用户分配prepare_id
            pthread_mutex_lock(&map_lock_);
            UserInfo ui(nickname,school,passwd,prepare_id_);
            user_map_.insert(make_pair(prepare_id_,ui));
            prepare_id_++;
            pthread_mutex_destroy(&map_lock_);
        }
    private:
        unordered_map<uint32_t , UserInfo> user_map_;
        pthread_mutex_t map_lock_;
        
        uint32_t prepare_id_; //预分配用户id 

};
