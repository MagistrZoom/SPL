#include "mem.h"
#include "llist.h"

size_t round_4096(size_t n){
	return n%4096?n+(4096-n%4096):n;
}


void* heap_init(size_t initial_size){
	void* ptr = mmap(HEAP_START, initial_size, 
			PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	struct mem_t *head = HEAP_START;
	head->next = NULL;
	head->capacity = round_4096(initial_size) - sizeof(struct mem_t); 
	head->is_free = 1;
	
	return (char*)ptr + sizeof(struct mem_t);
}

void* _malloc(size_t request){
	struct mem_t *ptr = llist_look_for_space(HEAP_START, request);
	struct mem_t *new = NULL;
	struct mem_t *last = NULL;
	struct mem_t *top = NULL;
	void *new_page;
	if(ptr){
		/*everything is cool, we can split memory and return requested piece*/
		new = (struct mem_t*)((char*)ptr + request);
		/*TODO: make a feature which will construct mem_t by address and args*/
		llist_add_front(NULL, new);
		new->capacity = ptr->capacity - request;
		new->is_free = 1;

		llist_add_front(ptr, new);
		ptr->capacity = request;
		ptr->is_free = 0;
	} else {
		/*everything is bad, need to kick the kernel */
		new_page = mmap(HEAP_START, request, 
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		last = llist_get_a_last_element(HEAP_START);
		new = (struct mem_t*)new_page;
		/* cut into 2 blocks if mmap returns bytes more than necessary */
		if(round_4096(request) - request - 2*sizeof(struct mem_t) > 0){
			top = (struct mem_t*)((char*)(new_page + request + 2*sizeof(struct mem_t)));
			top->capacity = round_4096(request) - request - 2*sizeof(struct mem_t);
			top->is_free = 1;

			new->capacity = request; 
			llist_add_front(top, new);
		} else {
			llist_add_front(new, NULL);
			new->capacity = round_4096(request) - sizeof(struct mem_t); 
		}
		new->is_free = 0;
		llist_add_front(new, last);
		ptr = new;
	}
	return ptr;
}

void free(void *ptr){
	
}

void memalloc_debug_struct_info(FILE* f,
        struct mem_t const* const address){

    size_t i;
    fprintf(f, 
            "start: %p\nsize: %lu\nis_free: %d\n",
            (void*)address, 
            address->capacity, 
            address->is_free);
        for (i = 0; i < DEBUG_FIRST_BYTES && i < address-> capacity; ++i)
            fprintf(f, "%hhX", 
                    ((char*)address)[sizeof(struct mem_t) +i ]);
    putc('\n', f);
}

void memalloc_debug_heap( FILE* f,  struct mem_t const* ptr ) {
    for( ; ptr; ptr = ptr->next) 
        memalloc_debug_struct_info(f, ptr); 
}
