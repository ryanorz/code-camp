#include "fdhandle.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

void sig_handler(int sig)
{
    int save_errno = errno;
    int msg        = sig;
    send(g_sig_pipe[1], (char*)&msg, 1, 0);
    errno = save_errno;
}

void add_sig(int sig, void (*handler)(int), bool restart)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    if (restart) {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    if (sigaction(sig, &sa, nullptr) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

int set_nonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void add_fd(int epollfd, int fd, uint32_t events)
{
    epoll_event event;
    event.data.fd = fd;
    event.events  = events;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, &event);
    set_nonblocking(fd);
}

void remove_fd(int epollfd, int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}
