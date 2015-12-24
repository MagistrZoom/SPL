#include "llist.h"

void llist_add_front(struct mem_t *head, struct mem_t *el){
	el->next = head;
	head = el;
}

/* return address mem_t struct of block */
struct mem_t* llist_look_for_space(
	struct mem_t *ptr, size_t capacity){
	for( ; ptr; ptr = ptr->next){
		if(ptr->is_free == 1 && ptr->capacity >= capacity + sizeof(struct mem_t))
			return ptr;
	}
	return NULL;
}

/* return address of last mem_t struct in llist */
struct mem_t* llist_get_a_last_element(struct mem_t* ptr){
	for( ; ptr->next; ptr = ptr->next);
	return ptr;
}
