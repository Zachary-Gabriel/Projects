#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef void*(*malloc_type)(size_t size);

int main(int argc, char const *argv[])
{
	void * handle = dlopen ("./libmalloc.so", RTLD_LAZY);
	malloc_type malloc_handle = (malloc_type) dlsym (handle, "malloc");
	// malloc_type calloc_handle = (malloc_type) dlsym (handle, "calloc");
	// malloc_type realloc_handle = (malloc_type) dlsym (handle, "realloc");
	// malloc_type free_handle = (malloc_type) dlsym (handle, "free");
	// free_handle (1);
	// realloc_handle (1);
	// calloc_handle (1);
	malloc_handle (1);
	dlclose (handle);
	return 0;
}
