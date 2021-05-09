#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <assert.h>

#include "paxos_config.h"
#include "lp_utils.h"

int set_sock_attribute(int sock, int newflag) {
   	int flags;
	flags = fcntl(sock, F_GETFL);
	if (flags == -1) {
		perror("fcntl failed reading");
        return -1;
	}
	flags |= newflag;
	if (fcntl(sock, F_SETFL, flags) == -1) {
		perror("fcntl failed reading");
        return -1;
	}
    return 0;     
}

void socket_set_reuse_port(int sock) {
    int activate = 1;
    int set_reuse_result = -1;
    set_reuse_result = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &activate, sizeof(int));
    if (set_reuse_result != 0) {
        perror("setsockopt, setting SO_REUSEADDR");
        assert(set_reuse_result == 0);
    } 

#ifdef SO_REUSEPORT //Not even defined on Linux
    set_reuse_result = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &activate, sizeof(int));
    if (set_reuse_result != 0) {
        perror("setsockopt, setting SO_REUSEPORT");
        assert(set_reuse_result == 0);
    } 
#endif
}

void socket_set_nonblocking(int sock) {
    int set_nonblock_result = -1;
    set_nonblock_result = set_sock_attribute(sock, O_NONBLOCK);
    assert(set_nonblock_result == 0);
}

void socket_set_bufsize(int sock, int size) {
    int result = 0;
    int argsize = sizeof(int);
    result += setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &size, argsize);
    result += setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &size, argsize);

    if (result != 0) {
        LOG_MSG(WARNING, ("WARNING: Failed to set socket buffer size to %d\n", size));
        LOG_MSG(WARNING, ("(Your OS is probably limiting the maximum size, use sysctl to discover and set\n"));
    }
}
