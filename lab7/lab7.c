#include "mem.h"

int main(void){
	heap_init(1);
	void *ptr = _malloc(4050);
	void *ptr3 = _malloc(100);
	void *ptr4 = _malloc(100);
	void *ptr5 = _malloc(100);
	void *ptr6 = _malloc(3999);
	memalloc_debug_heap(stdout, ptr);
	return 0;
}
