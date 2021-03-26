#pragma once 
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "ConnectInfo.hpp"
#include "tools.hpp"
#include "UserManager.hpp"

#define TCP_PORT 12345

enum error_no
{
    CREATE_SOCK_FAILED = 0,
    BIND_FAILED,
    LISTEN_FAILED,
    ACCEPT_FAILED
};

class TcpConnect{
    public:
        TcpConnect()
        {
            new_sock_ = -1;
            server_ = nullptr;
        }
        ~TcpConnect()
        {}
        void SetSockfd(int fd)
        {
            new_sock_ = fd;
        }
        void SetServer(void* server)
        {
            server_ = server;
        }
        int GetSockFd()
        {
            return new_sock_;
        }

        void* GetServer()
        {
            return server_;
        }
    private:
        int new_sock_;
        void* server_; //保存ChatServer这个类的this指针,确保在tcp的线程入口函数中可以获取到用户管理模块的实例化指针
};
class ChatServer{
    public:
    ChatServer()
    {
        //登录注册
        tcp_sock_ = -1; //初始化-1
        tcp_port = TCP_PORT;
        this->user_manager_ = nullptr;
    }
    ~ChatServer()
    {
        
    }
    int InitSvr(uint16_t tcp_port = TCP_PORT) //创建tcp_socket
    {                
        tcp_sock_ = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(tcp_sock_ < 0)
        {
            return CREATE_SOCK_FAILED; 
        }
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(tcp_port);
        addr.sin_addr.s_addr = inet_addr("0.0.0.0");
        int ret = bind(tcp_sock_,(struct sockaddr*)&addr,sizeof(addr));
        if(ret < 0)
        {
            return BIND_FAILED;
        }
       ret =  listen(tcp_sock_,5);
       if(ret < 0)
       {
           return LISTEN_FAILED; 
       }
       string str = "listen port is";
       Log(INFO,__FILE__,__LINE__, str) << " " << tcp_port << std::endl;
      user_manager_ = new Usermanager();
      if(user_manager_ == nullptr)
      {
          return -1;
      }
       
       return 0;
    }
    //创建tcp udp线程 有客户端来就需要进行创建 
    int Start()
    {
        struct sockaddr_in peer_addr;//对端地址信息
        socklen_t peer_addrlen = sizeof(peer_addr);
        while(1)
        {
        int new_sock = accept(tcp_sock_,(struct sockaddr*)&peer_addr,&peer_addrlen); 
        if(new_sock < 0)
        {
            continue;
        }
        //正常接收到了创建线程
        pthread_t tid;
       TcpConnect *tc = new TcpConnect();
       tc->SetSockfd(new_sock);
       tc->SetServer((void*)this);
        int ret = pthread_create(&tid,NULL,LoginRegisterStart,NULL);
        if(ret < 0)
        {
            close(new_sock);
            delete tc;
            continue;
        }
        }
     }
    private:
    static void* LoginRegisterStart(void* arg) // 处理登录注册请求
    {
        
        pthread_detach(pthread_self());
        TcpConnect* tc = (TcpConnect*)arg; 
        ChatServer* cs = (ChatServer*)tc->GetServer();
        char ques_type = -1;
        recv(tc->GetSockFd(),&ques_type,1,0);
        ssize_t recv_size = recv(tc->GetSockFd(),&ques_type,1,0);
    
        if(recv_size < 0) 
        {
            close(tc->GetSockFd());
            return NULL;
        }
        else if(recv_size == 0) //对端关闭连接
        {
            close(tc->GetSockFd()); 
            return NULL;
        }
        else{
            switch(ques_type)
            {
                case REGISTER_RESQ:
                    {
                        //处理注册请求
                        cs->DealRegister(tc->GetSockFd(),cs);
                        break;
                    }
                case LOGIN_RESQ:
                    {
                        break;
                    }
            }
        }
        return nullptr;
    }
    //成功失败都要给客户端返回一个响应
    int DealRegister(int new_sock,ChatServer* cs)
    {
        struct RegisterInfo s;
        ssize_t recv_size = recv(new_sock,&s,sizeof(s),0);
        if(recv_size < 0)
        {
            return -1;
        }
        else if(recv_size == 0)
        {
            close(new_sock);
            return -2;
        }
        //正常接收到 
        string temp = s.nick_name_;
        string temp1 = s.passwd_;
        string temp2 = s.school_;
        cs->user_manager_->DealRegister(temp,temp1,temp2);
    }
    int DealLogin()
    {
        

    }


    private:
        int tcp_sock_;
        int udp_sock_;
        uint16_t tcp_port; 
        Usermanager* user_manager_;
};
