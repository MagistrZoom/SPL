#include "mem.h"
struct mem_t* llist_look_for_space(
		struct mem_t *ptr, size_t capacity);
void llist_add_front(struct mem_t *head, struct mem_t *el);
struct mem_t* llist_get_a_last_element(struct mem_t *ptr);
struct mem_t* llist_get_mem_block(struct mem_t *ptr, struct mem_t *target);
struct mem_t* llist_get_prev_mem_block(struct mem_t *ptr, struct mem_t *target);
