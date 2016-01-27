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

struct mem_t* allocate_blocks(struct mem_t* last, size_t request){

	char *allocated = mmap((char*)(last)+last->capacity+sizeof(struct mem_t), request, 
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if(allocated == (char*)(last)+last->capacity+sizeof(struct mem_t)){
		/* series of pages. Merge last with new pages */
		last->capacity += round_4096(request);
		return last;
	} else {
		/* page allocated somewhere else */
		last->next = (struct mem_t*)allocated;
		(last->next)->capacity = round_4096(request) - sizeof(struct mem_t);
		return (struct mem_t*)allocated;
	}
}
struct mem_t* cut_if_necessary(struct mem_t *ptr, size_t request){
	struct mem_t *free = NULL;
	if(ptr->capacity >= request + sizeof(struct mem_t)){
		/*cut into 2 blocks*/
		free = (struct mem_t*)((char*)(ptr) + request + sizeof(struct mem_t));
		free->is_free = 1;
		free->capacity = ptr->capacity - request - sizeof(struct mem_t);
		free->next = NULL;
		
		ptr->capacity = request;
		ptr->is_free = 0;
	} 
	return free;
}

void* _malloc(size_t request){
	struct mem_t *ptr = llist_look_for_space(HEAP_START, request);
	struct mem_t *new = NULL;
	struct mem_t *free = NULL;
	if(ptr){
		/*everything is cool, we can split memory and return requested piece*/
		new = (struct mem_t*)((char*)ptr + request + sizeof(struct mem_t));

		new->capacity = ptr->capacity - request - sizeof(struct mem_t);
		new->is_free = 1;

		llist_add_front(new, ptr);
		ptr->capacity = request;
		ptr->is_free = 0;

	} else {
		ptr = llist_get_a_last_element(HEAP_START);
		/*try to allocate nearest page and concat last mem_t or allocate full
		request*/
		new = allocate_blocks(ptr, request);

		free = cut_if_necessary(new, request);

		llist_add_front(free, new);
		ptr = (struct mem_t*)(new);
		
	}
	return (char*)ptr+sizeof(struct mem_t);
}

void _free(void *ptr){
	struct mem_t *mem_prev_block;
	struct mem_t *mem_block = llist_get_mem_block(HEAP_START, (struct mem_t*)((char*)ptr-sizeof(struct mem_t)));
	if(mem_block){
		mem_block->is_free = 1;
		if((mem_block->next)->is_free == 1 && 
			mem_block->next == (struct mem_t*)((char*)(mem_block)+mem_block->capacity+sizeof(struct mem_t))){
			/* need to merge following block  */
			mem_block->capacity += (mem_block->next)->capacity;
			mem_block->next = (mem_block->next)->next;
		}
		mem_prev_block = llist_get_prev_mem_block(HEAP_START, (struct mem_t*)((char*)ptr-sizeof(struct mem_t)));
		if(mem_prev_block->is_free == 1 &&
			mem_block == (struct mem_t*)((char*)(mem_prev_block)+mem_prev_block->capacity+sizeof(struct mem_t))){

			/* merge previous block */
			mem_prev_block->capacity += mem_block->capacity;
			mem_prev_block->next = mem_block->next;
		}
	} 
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
