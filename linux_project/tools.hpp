#pragma once 
#include <iostream>
#include <string>
#include <time.h>
#include <string.h>
#include "tools.hpp"

using namespace std;
enum LogLevel
{
    INFO = 0,
    WARNNING,
    ERROR,
    FATAL,
    DEBUG,
};

const char* levelInfo[] = 
{
    "INFO",
    "WARNNING",
    "ERROR",
    "FATAL",
    "DEBUG"
};

//#define LOG(Loglevel,msg) log(loglevel, __FILE__ ,__LINE__,msg)


// Loglevel 日志等级
// file 源码文件名称           年-月-日-时-分-秒
// msg: 需要记录的具体信息     [时间戳 等级 源文件名称 行号 具体信息] 

class TimeStamp
{
    public:
        static void GetTimeStamp(string *timestamp)
        {
            time_t sys_time;
            time(&sys_time);

            struct tm* t = localtime(&sys_time);
            if(t == nullptr)
            {
                return;
            }
            char buf[30] = {'\0'}; 
            snprintf(buf,sizeof(buf) - 1,"%04d-%02d-%02d %02d:%02d:%02d",t->tm_year + 1900, t->tm_mon , t->tm_mday , t->tm_hour, t->tm_min , t->tm_sec);
            timestamp->assign(buf,strlen(buf));  
        }
};

std::ostream& Log(LogLevel Loglevel,string file,int line,string& msg)
{
    string timestamp;
    TimeStamp::GetTimeStamp(&timestamp);
    std::string level = levelInfo[Loglevel];
    cout << "[" << timestamp << " " << level  << " " << __FILE__ << ":" << __LINE__ << "]" << " " << msg;
    return std::cout;
} 
