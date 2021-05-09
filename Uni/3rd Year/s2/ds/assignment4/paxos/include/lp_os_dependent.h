/*
This file contains definitions that change across different operating systems, 
at the moment, only Linux and Apple OS X are supported.
*/


/*
	Apple/OS X
*/
#ifdef __APPLE__
#define OS_MAX_SOCKET_RBUF_SIZE_STRING "kern.ipc.maxsockbuf"
#define OS_MAX_SOCKET_WBUF_SIZE_STRING "kern.ipc.maxsockbuf"


/*
	Linux
*/
#elif linux
#define OS_MAX_SOCKET_RBUF_SIZE_STRING "net.core.rmem_max"
#define OS_MAX_SOCKET_WBUF_SIZE_STRING "net.core.wmem_max"


/*
	Anything else...
*/
#else 
#error Operating system not recognized cannot compile!


#endif

int get_os_max_wbuf_size();
int get_os_max_rbuf_size();

