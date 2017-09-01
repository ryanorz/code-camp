#ifndef fdhandle_h
#define fdhandle_h

#include <sys/epoll.h>

static int g_sig_pipe[2];

void sig_handler(int sig);

void add_sig(int sig, void (*handler)(int), bool restart = true);

int set_nonblocking(int fd);

void add_fd(int epollfd, int fd, uint32_t events = EPOLLIN | EPOLLET);

void remove_fd(int epollfd, int fd);

#endif
