#pragma once 
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <iostream>
#include <string>

enum Status
{
    REGISTER_FAILED = 0,
    REGISTER_SUCCESS,
    LOGIN_FAILED,
    LOGIN_SUCCESS
};

enum RESQTYPE
{
    REGISTER_RESQ = 0,
    LOGIN_RESQ,
};
struct RegisterInfo
{
    RegisterInfo()
    {
        memset(nick_name_,'\0',sizeof(nick_name_));
        memset(school_,'\0',sizeof(school_));
        memset(passwd_,'\0',sizeof(passwd_));
    }
    char nick_name_[20];
    char school_[20];
    char passwd_[20];
};

struct ReplyInfo
{
    int resp_status;//注册成功还是登录成功
    uint32_t id_;
};

struct LogInfo
{
    LogInfo()
    {
        memset(passwd_,'\0',sizeof(passwd_));
    }
    uint32_t id_;
    char passwd_[20];
};


