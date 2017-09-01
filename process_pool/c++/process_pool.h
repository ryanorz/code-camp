#ifndef process_pool_h
#define process_pool_h

#include "fdhandle.h"
#include "worker.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
* @brief 描述一个子进程的类
* @param pid    目标子进程的 pid
* @param pipefd 父子进程通信管道
*/
struct Process {
    pid_t pid;
    int   pipefd[2];

    Process()
        : pid(-1)
    {
    }
};

template <typename T>
class ProcessPool {

public:
    static ProcessPool<T>* instance()
    {
        static ProcessPool<T> instance;
        return &instance;
    }

private:
    // 进程池允许的最大子进程数量
    static const int kMaxProcessNumber = 16;
    // 每个子进程最多处理的客户数量
    static const int kMaxSessionsPerProcess = 65536;
    // epoll 最多能处理的事件数
    static const int kMaxEventNumber = 10000;
    // 进程池中的进程总数
    int process_number_;
    // 子进程在池中的序号，从 0 开始
    int idx_;
    // 每个进程都有一个 epoll 内核事件表，用 m_epollfd 标识
    int epollfd_;
    // 监听 socket
    int listenfd_;
    // 子进程通过 m_stop 来决定是否停止运行
    bool stop_;
    // 保存所有子进程的描述信息
    Process* sub_process_;

private:
    ProcessPool()
        : process_number_(0)
        , idx_(-1)
        , epollfd_(-1)
        , listenfd_(-1)
        , stop_(false)
        , sub_process_(nullptr)
    {
    }
    ~ProcessPool()
    {
        delete[] sub_process_;
    }
    ProcessPool(const ProcessPool&);
    ProcessPool& operator=(const ProcessPool&);

public:
    void create(int listenfd, int process_number = 4);
    void run();

private:
    void run_parent();
    void run_child();
    void setup_sigpipe();
};

template <typename T>
void ProcessPool<T>::create(int listenfd, int process_number)
{
    if (listenfd <= 0 || process_number > kMaxEventNumber || process_number <= 0) {
        perror("create invalid_argument");
        exit(EXIT_FAILURE);
    }
    listenfd_       = listenfd;
    process_number_ = process_number;
    sub_process_    = new Process[kMaxProcessNumber];
    if (sub_process_ == nullptr) {
        perror("new");
        exit(EXIT_FAILURE);
    }

    // 创建子进程并创建父子进程通信管道
    for (size_t i = 0; i < process_number_; i++) {
        int ret = socketpair(AF_INET, SOCK_STREAM, 0, sub_process_[i].pipefd);
        if (ret == -1) {
            perror("socketpair");
            exit(EXIT_FAILURE);
        }

        sub_process_[i].pid = fork();
        if (sub_process_[i].pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (sub_process_[i].pid == 0) {
            close(sub_process_[i].pipefd[1]);
            idx_ = i;
            break;
        } else {
            close(sub_process_[i].pipefd[0]);
        }
    }
}

template <typename T>
void ProcessPool<T>::run()
{
    if (idx_ == -1) {
        run_parent();
    } else {
        run_child();
    }
}

template <typename T>
void ProcessPool<T>::setup_sigpipe()
{
    // 为每个进程创建自己使用的文件描述符
    epollfd_ = epoll_create(5);
    if (epollfd_ == -1) {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }

    if (-1 == socketpair(AF_INET, SOCK_STREAM, 0, g_sig_pipe)) {
        perror("socketpair");
        exit(EXIT_FAILURE);
    }
    set_nonblocking(g_sig_pipe[1]);
    add_fd(epollfd_, g_sig_pipe[0]);

    // 为每个进程设置信号处理
    add_sig(SIGCHLD, sig_handler);
    add_sig(SIGTERM, sig_handler);
    add_sig(SIGINT, sig_handler);
    add_sig(SIGPIPE, SIG_IGN);
}

template <typename T>
void ProcessPool<T>::run_parent()
{
    setup_sigpipe();

    add_fd(epollfd_, listenfd_);

    epoll_event events[kMaxEventNumber];

    int counter  = 0; // for RoundRobin
    int new_conn = 1;

    while (!stop_) {
        int nfds = epoll_wait(epollfd_, events, kMaxEventNumber, -1);
        if (nfds == -1 && errno != EINTR) {
            perror("epoll_wait");
            break;
        }
        for (size_t i = 0; i < nfds; i++) {
            int eventfd = events[i].data.fd;

            if (eventfd == listenfd_) { // 新连接到来
                // 选择算法，选择工作进程处理连接
                // RoundRobin 分配子进程
                int i = counter;
                do {
                    if (sub_process_[i].pid != -1) {
                        // process hit
                        break;
                    }
                    i = (i + 1) % process_number_;
                } while (i != counter);

                // 没有可用的子进程
                if (sub_process_[i].pid == -1) {
                    stop_ = true;
                    break;
                }
                send(sub_process_[i].pipefd[1], (char*)&new_conn, sizeof(new_conn), 0);
                counter = (i + 1) % process_number_;
            } else if (eventfd == g_sig_pipe[0] && (events[i].events & EPOLLIN)) { // 信号到来
                char signals[1024];
                int  ret = recv(g_sig_pipe[0], signals, sizeof(signals), 0);
                if (ret <= 0) {
                    continue;
                }
                for (int i = 0; i < ret; i++) {
                    switch (signals[i]) {
                    case SIGCHLD: {
                        pid_t pid;
                        int   stat;
                        while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
                            // 找到进程池中对应的进程, 关掉对应的管道与重置其 pid
                            for (size_t i = 0; i < process_number_; i++) {
                                if (sub_process_[i].pid == pid) {
                                    close(sub_process_[i].pipefd[1]);
                                    sub_process_[i].pid = -1;
                                    // TODO 如果子进程是异常终止崩溃，可以考虑重启子进程
                                    break;
                                }
                            }
                        }
                        // 如果子进程全部退出，则主进程停止
                        stop_ = true;
                        for (size_t i = 0; i < process_number_; i++) {
                            if (sub_process_[i].pid != -1) {
                                stop_ = false;
                                break;
                            }
                        }
                        break;
                    }
                    case SIGTERM:
                    case SIGINT: {
                        // 向每个子进程发送　TERM　信号
                        for (size_t i = 0; i < process_number_; i++) {
                            pid_t pid = sub_process_[i].pid;
                            if (pid != -1) {
                                kill(pid, SIGTERM);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                    }
                }
            }
        }
    }
    close(g_sig_pipe[0]);
    close(g_sig_pipe[1]);
    close(epollfd_);
}

template <typename T>
void ProcessPool<T>::run_child()
{
    setup_sigpipe();

    int pipefd = sub_process_[idx_].pipefd[0];
    add_fd(epollfd_, pipefd);

    epoll_event events[kMaxEventNumber];

    T* sessions = new T[kMaxSessionsPerProcess];

    while (!stop_) {
        int nfds = epoll_wait(epollfd_, events, kMaxEventNumber, -1);
        if (nfds < 0 && errno != EINTR) {
            perror("epoll_wait");
            break;
        }
        for (int i = 0; i < nfds; i++) {
            // 新连接到来
            int eventfd = events[i].data.fd;
            if ((eventfd == pipefd) && (events[i].events & EPOLLIN)) {
                int new_conn;
                int ret = recv(eventfd, (char*)&new_conn, sizeof(new_conn), 0);
                if (((ret < 0) && errno != EAGAIN) || ret == 0) {
                    continue;
                }
                struct sockaddr_in client_addr;
                socklen_t          client_addr_length;
                int                connfd = accept(listenfd_, (struct sockaddr*)&client_addr, &client_addr_length);
                if (connfd < 0) {
                    perror("accept");
                    continue;
                }
                add_fd(epollfd_, connfd);
                // 直接根据 connfd 哈希索引增加效率
                sessions[connfd].init(epollfd_, connfd, client_addr);
            } else if ((eventfd == g_sig_pipe[0]) && (events[i].events & EPOLLIN)) {
                char signals[1024];
                int  ret = recv(eventfd, signals, sizeof(signals), 0);
                if (ret <= 0) {
                    continue;
                }
                for (int i = 0; i < ret; i++) {
                    switch (signals[i]) {
                    case SIGCHLD: {
                        pid_t pid;
                        int   stat;
                        while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
                            continue;
                        }
                        break;
                    }
                    case SIGTERM:
                    case SIGINT: {
                        stop_ = true;
                        break;
                    }
                    default:
                        break;
                    }
                }
            } else if (events[i].events & EPOLLIN) {
                sessions[eventfd].process();
            }
        }
    }
    delete[] sessions;
    sessions = nullptr;
    close(pipefd);
    close(epollfd_);
    close(g_sig_pipe[0]);
    close(g_sig_pipe[1]);
}

#define WorkerPool ProcessPool<Worker>::instance()

#endif
