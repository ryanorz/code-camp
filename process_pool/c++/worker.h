#ifndef worker_h
#define worker_h

#include "fdhandle.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/**
* @brief CGI server
*/
class Worker {

private:
    static const int kBufferSize = 1024;
    static int       epollfd_;
    int              connfd_;
    sockaddr_in      address_;
    char             buffer_[kBufferSize];
    int              read_idx_;

public:
    Worker()
    {
    }
    ~Worker() {}

    /**
    * @brief 初始化客户连接，清空读缓冲区
    * 
    * @param epollfd 工作进程的 epollfd
    * @param connfd 每个 连接/session 的 connfd
    * @param client_addr 连接的客户端地址
    */
    void init(int epollfd, int connfd, const sockaddr_in& client_addr)
    {
        epollfd_ = epollfd;
        connfd_  = connfd;
        address_ = client_addr;
        memset(buffer_, 0, kBufferSize);
        read_idx_ = 0;
    }

    void process()
    {
        int idx = 0;
        int ret = -1;
        while (true) {
            idx = read_idx_;
            ret = recv(connfd_, buffer_ + idx, kBufferSize - 1 - idx, 0);
            // 读数据发生错误则关闭客户连接，如果只是暂时无数据可读，则退出循环
            if (ret < 0) {
                if (errno != EAGAIN) {
                    remove_fd(epollfd_, connfd_);
                }
                break;
                // 如果对方关闭连接，则服务器也关闭
            } else if (ret == 0) {
                remove_fd(epollfd_, connfd_);
                break;
            } else {
                read_idx_ += ret;
                // 如果遇到字符　"\r\n"，　则开始处理客户请求
                for (; idx < read_idx_; ++idx) {
                    if (idx >= 1 && buffer_[idx - 1] == '\r' && buffer_[idx] == '\n') {
                        break;
                    }
                }

                // 如果没有遇到 "\r\n" 则需要读取更多客户端数据
                if (idx == read_idx_) {
                    continue;
                }

                buffer_[idx - 1] = '\0';

                char* file_name = buffer_;
                // 判断客户端要运行的 CGI 程序是否存在
                if (access(file_name, F_OK) == -1) {
                    remove_fd(epollfd_, connfd_);
                    break;
                }

                ret = fork();
                if (ret == -1) {
                    remove_fd(epollfd_, connfd_);
                    break;
                } else if (ret > 0) {
                    //　父进程只需关闭连接
                    remove_fd(epollfd_, connfd_);
                    break;
                } else {
                    //　将标准输出重定向到 connfd_, 并执行 CGI 程序
                    close(STDOUT_FILENO);
                    dup(connfd_);
                    execl(buffer_, buffer_, 0);
                    exit(0);
                }
            }
        }
    }
};

int Worker::epollfd_ = -1;

#endif
