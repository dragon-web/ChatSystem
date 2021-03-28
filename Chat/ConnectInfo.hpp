#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <json/json.h>

#define TCP_PORT 17878
#define UDP_PORT 17878

#define UDP_MAX_DATA_LEN 10240

struct RegisterInfo
{
    RegisterInfo()
    {
        memset(nick_name_, '\0', sizeof(nick_name_));
        memset(school_, '\0', sizeof(school_));
        memset(passwd_, '\0', sizeof(passwd_));
    }

    char nick_name_[20];
    char school_[20];
    char passwd_[20];
};



struct LoginInfo
{
    LoginInfo()
    {
        memset(passwd_, '\0', sizeof(passwd_));
    }
    uint32_t id_;
    char passwd_[20];
};



struct RelpyInfo
{
    int resp_status_;
    uint32_t id_;
};

//枚举状态
enum Status 
{
    REGISTER_FAILED = 0,
    REGISTRE_SUCCESS = 1,
    LOGIN_FAILED = 2,
    LOGIN_SUCCESS = 3,
    ONLINE = 4
};


enum ResqType
{
    REGISTER_RESQ = 0,
    LOGIN_RESQ
};


class UdpMsg
{
    public:
        UdpMsg()
        {

        }

        ~UdpMsg()
        {

        }
        void serialize(std::string* msg)
        {
            Json::Value json_msg;

            json_msg["nick_name"] = nick_name_;
            json_msg["school"] = school_;
            json_msg["user_id"] = user_id_;
            json_msg["msg"] = msg_;

            Json::FastWriter writer;
            *msg = writer.write(json_msg);
        }

        void deserialize(std::string msg)
        {
            Json::Reader reader;
            Json::Value val;
            reader.parse(msg, val);

            nick_name_ = val["nick_name"].asString();
            school_ = val["school"].asString();
            user_id_ = val["user_id"].asUInt();
            msg_ = val["msg"].asString();
        }
    public:
        std::string nick_name_;
        std::string school_;
        uint32_t user_id_;
        std::string msg_;
};
