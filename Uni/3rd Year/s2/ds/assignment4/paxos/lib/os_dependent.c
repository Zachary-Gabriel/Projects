#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdlib.h>
#include <stdio.h>

/*
	Apple/OS X
*/
#ifdef __APPLE__
int get_os_max_rbuf_size() {
	int value;
	size_t value_size = sizeof(int);
	int result = sysctlbyname("kern.ipc.maxsockbuf", &value, &value_size, NULL, 0);
	if(result != 0) {
		perror("Error reading kern.ipc.maxsockbuf");
		return -1;
	}
	return value;
}

int get_os_max_wbuf_size() {
	return get_os_max_rbuf_size();
}

/*
	Linux
*/
#elif linux
static int read_proc_variable(const char * path) {
	int value;
	char linebuf[100];
	FILE* fp;
    fp = fopen(path,"r");
	if (fp == NULL) {
		perror("fopen");
		return -1;
	}
		
	if(fgets(linebuf, 100, fp) == NULL) {		
		perror("fgets");
		fclose(fp);
		return -1;
	}
	
	value = atoi(linebuf);
	if (value <= 0) {
		fclose(fp);
		return -1;
	}
	
	fclose(fp);
	return value;
}

int get_os_max_wbuf_size() {
	return read_proc_variable("/proc/sys/net/core/wmem_max");

}
int get_os_max_rbuf_size() {
	return read_proc_variable("/proc/sys/net/core/wmem_max");
}

/*
	Anything else...
*/
#else 
#error Operating system not recognized cannot compile!


#endif
