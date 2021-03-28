#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <unordered_map>

#include "ConnectInfo.hpp"



class UserInfo
{
    public:
        UserInfo(const std::string& nick_name, const std::string& school, const std::string& passwd, uint32_t user_id)
        {
            nick_name_ = nick_name;
            school_ = school;
            passwd_ = passwd;
            user_id_ = user_id;
            user_status_ = REGISTER_FAILED;

            //新增udp地址信息的初始化
            memset((void*)&addr_, '\0', sizeof(struct sockaddr_in));
            addr_len_ = 0;
        }

        ~UserInfo()
        {

        }

        std::string& GetPasswd()
        {
            return passwd_;
        }

        void SetUserStatus(int status)
        {
            user_status_ = status;
        }

        int GetUserStatus()
        {
            return user_status_;
        }

        void SetaddrInfo(struct sockaddr_in addr)
        {
            memcpy(&addr_, &addr, sizeof(addr));
        }

        void SetaddrLenInfo(socklen_t addr_len)
        {
            addr_len_ = addr_len;
        }

        struct sockaddr_in& GetAddrInfo()
        {
            return addr_;
        }

        socklen_t GetAddrLen()
        {
            return addr_len_;
        }
    private:
        std::string nick_name_;
        std::string school_;
        std::string passwd_;

        //用户id
        uint32_t user_id_;

        int user_status_;

        //新增udp地址信息
        struct sockaddr_in addr_;
        socklen_t addr_len_;
};


class UserManager
{
    public:
        UserManager()
        {
            user_map_.clear();
            pthread_mutex_init(&map_lock_, NULL);
            prepare_id_ = 0;

            Online_user_.clear();
        }

        ~UserManager()
        {
            pthread_mutex_destroy(&map_lock_);
        }
        int DealRegister(const std::string& nick_name, const std::string& school, const std::string& passwd, uint32_t* user_id)
        {
            //1.判断字段是否为空
            if(nick_name.size() == 0 || school.size() == 0 || passwd.size() == 0)
            {
                return -1;
            }
            //2.创建单个用户 UserInfo这个类的对象
            pthread_mutex_lock(&map_lock_);
            //3.分配用户id
            UserInfo ui(nick_name, school, passwd, prepare_id_);
            *user_id = prepare_id_;
            //更改当前用户的状态
            ui.SetUserStatus(REGISTRE_SUCCESS);
            
            //4.将用户的数据插入到map当中
            user_map_.insert(std::make_pair(prepare_id_, ui));
            //5.更新预分配的用户id
            prepare_id_++;
            pthread_mutex_unlock(&map_lock_);
            return 0;
        }

        int DealLogin(uint32_t id, const std::string& passwd)
        {
            if(passwd.size() == 0)
            {
                return -1;
            }

            std::unordered_map<uint32_t, UserInfo>::iterator iter;

            pthread_mutex_lock(&map_lock_);
            iter = user_map_.find(id);
            if(iter == user_map_.end())
            {
                pthread_mutex_unlock(&map_lock_);
                return -2;
            }
            std::string reg_passwd = iter->second.GetPasswd();
            if(reg_passwd != passwd)
            {
                iter->second.SetUserStatus(LOGIN_FAILED);
                pthread_mutex_unlock(&map_lock_);
                return -3;
            }
            iter->second.SetUserStatus(LOGIN_SUCCESS);
            pthread_mutex_unlock(&map_lock_);
            return 0;
        }

     
        int IsLogin(uint32_t user_id, struct sockaddr_in addr, socklen_t addr_len)
        {
            std::unordered_map<uint32_t, UserInfo>::iterator iter;
            pthread_mutex_lock(&map_lock_);
            iter = user_map_.find(user_id);
            if(iter == user_map_.end())
            {
                pthread_mutex_unlock(&map_lock_);
                return -1;
            }
            if(iter->second.GetUserStatus() <= LOGIN_FAILED)
            {
                pthread_mutex_unlock(&map_lock_);
                return -1;
            }
            else if(iter->second.GetUserStatus() == LOGIN_SUCCESS)
            {
                std::cout << "test islogin" << std::endl;
                iter->second.SetUserStatus(ONLINE);
                iter->second.SetaddrInfo(addr);
                iter->second.SetaddrLenInfo(addr_len);
                Online_user_.push_back(iter->second);
            }
            pthread_mutex_unlock(&map_lock_);
            return 0;
        }

        void GetOnlineUser(std::vector<UserInfo>* vec)
        {
            *vec = Online_user_;
        }
    private:
        std::unordered_map<uint32_t, UserInfo> user_map_;
        pthread_mutex_t map_lock_;

        uint32_t prepare_id_;

        std::vector<UserInfo> Online_user_;
};
