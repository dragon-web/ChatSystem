#pragma once
#include <time.h>
#include <string.h>
#include <iostream>
#include <string>



class TimeStamp
{
    public:
        //年-月-日 时:分:秒
        static void GetTimeStamp(std::string* timestamp)
        {
            /*
             * time函数 time_t time(time_t *t);
             * struct tm *localtime(const time_t *timep);
             */
            time_t sys_time;
            time(&sys_time);

			struct tm* t = localtime(&sys_time);
			//yyyy-mm-dd HH:MM:SS
    		char buf[30] = { '\0' };
			snprintf(buf, sizeof(buf) - 1, "%04d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			timestamp->assign(buf, strlen(buf));
        }
};

enum LogLevel
{
    INFO = 0,
    WARNING,
    ERROR,
    FATAL,
    DEBUG
};

const char* LevelInfo[] = 
{
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL",
    "DEBUG"
};


std::ostream& Log(LogLevel loglevel, const char* file, int line, const std::string msg)
{
    std::string timestamp;
    TimeStamp::GetTimeStamp(&timestamp);

    std::string level = LevelInfo[loglevel];

    std::cout << "[" << timestamp << " " << level << " " << file << ":" << std::to_string(line) << "]" << " " << msg;
    return std::cout;
}

#define LOG(loglevel, msg) Log(loglevel, __FILE__, __LINE__, msg)
