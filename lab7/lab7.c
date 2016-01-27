#include "mem.h"

int main(void){
	heap_init(1);
	void *ptr1 = _malloc(1024);
	void *ptr2 = _malloc(1024);
	void *ptr3 = _malloc(1024);
	void *ptr4 = _malloc(1024);
	void *ptr5 = _malloc(1024);
	void *ptr6 = _malloc(1024);
	void *ptr7 = _malloc(1024);
	void *ptr8 = _malloc(1024);
	void *ptr9 = _malloc(1024);
	memalloc_debug_heap(stdout, (void*)((char*)ptr5-20));
	_free(ptr5);
	puts("\n\nptr5 is free\n");
	memalloc_debug_heap(stdout, (void*)((char*)ptr5-20));
	_free(ptr6);
	puts("\n\nptr6 is free\n");
	memalloc_debug_heap(stdout, (void*)((char*)ptr5-20));
	return 0;
}
