#include <arpa/inet.h>
#include <libgen.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "process_pool.h"

int main(int argc, char* argv[])
{
    if (argc <= 2) {
        fprintf(stderr, "Usage: %s ip_address port_number\n", basename(argv[1]));
        exit(EXIT_FAILURE);
    }

    const char* ip   = argv[1];
    int         port = atoi(argv[2]);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in listen_addr;
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &listen_addr.sin_addr);
    listen_addr.sin_port = htons(port);
    int ret              = bind(listenfd, (struct sockaddr*)&listen_addr, sizeof(listen_addr));
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    ret = listen(listenfd, 5);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    WorkerPool->create(listenfd);
    WorkerPool->run();

    close(listenfd);

    return 0;
}
