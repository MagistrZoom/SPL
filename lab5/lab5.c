#include "llist.h"
#include "horder.h"

#define llist_generate( T ) struct T##_llist_t {\
	T n;\
	struct T##_llist_t *next;\
};\
typedef struct T##_llist_t T##_item;\
\
T##_item const * T##_llist_node_at(T##_item const *head, size_t nth){\
	size_t size = 0;\
	T##_item const *cur = head;\
	while(cur->next!=NULL){\
		if(size == nth){\
			return cur;\
		}\
		cur = cur->next;\
		size += 1;\
	}\
	if(size == nth){\
		return cur;\
	}\
	return NULL;\
}\
\
void T##_llist_free(T##_item *head){\
	T##_item *cur = head;\
	while ((cur = head) != NULL) {\
		head = head->next;         \
		free (cur);        \
	}\
}\
\
int T##_llist_sum(T##_item const *head){\
	int s = 0;\
	T##_item const *cur = head;\
	while(cur->next!=NULL){\
		s += cur->n;\
		cur = cur->next;\
	}\
	s += cur->n;\
	return s;\
}\
\
void T##_llist_add_back(T##_item **head, T##_item *el){\
	T##_item *cur = (void *)*head;\
	if(*head == NULL){\
		*head = el;\
		return; \
	}\
	el->next=NULL;\
	while(cur->next!=NULL){\
		cur = cur->next;\
	}\
	cur->next=el;\
}\
\
void T##_llist_add_front(T##_item **head, T##_item *el){\
	el->next = *head;\
	*head = el;\
}\
\
T##_item* T##_llist_create(T n){\
	T##_item *el = malloc(sizeof(T##_item));\
	el->n = n;\
	el->next = NULL;\
	return el;\
}\
\
T T##_llist_get(T##_item const *head, size_t index, int *err){\
	T##_item const *cur = head;\
	while(index--){\
		cur = cur->next;		\
		if(cur == NULL){\
			*err = 1;\
			return 0;\
		}\
	}\
	return cur->n;\
}\


#define horder_generate( T ) void T##_foreach(T##_item * const head, void fn(T)){\
	T##_item *cur = (void *)head; \
	while(cur != NULL){ \
		fn(cur->n); \
		cur = cur->next; \
	} \
} \
T##_item * T##_map(T##_item * const head, T fn(T)){ \
	if(head != NULL){ \
		T##_item *node = T##_llist_create(fn(head->n));\
		T##_item *ret = node; \
		T##_item *cur = head->next; \
		T n; \
		while(cur != NULL){\
			n = fn(cur->n);\
			node = T##_llist_create(n);\
			T##_llist_add_back(&ret, node);\
			cur = cur->next;\
		}\
		return ret;\
	}\
	return NULL;\
}\
\
void T##_map_mut(T##_item *const head, T fn(T)){\
	T##_item *cur = (void *)head;\
	if(head != NULL){\
		while(cur != NULL){\
			cur->n = fn(cur->n);\
			cur = cur->next;\
		}\
	}\
}\
\
T T##_foldl(T acc, T fn(T, T), T##_item * const head){\
	T##_item *cur = (void *)head;\
	while(cur != NULL){\
		acc = fn(acc, cur->n);\
		cur = cur->next;\
	}\
	return acc;\
}\
\
T##_item * T##_iterate(T s, size_t n, T fn(T)){\
	T##_item *ret = T##_llist_create(s);\
	T##_item *cur = ret;\
	T##_item const *prev = ret;\
	size_t i;\
	for(i = 0l; i < n; i++){\
		cur = T##_llist_create(fn(prev->n));\
		T##_llist_add_back(&ret, cur);\
		prev = T##_llist_node_at(cur, 0);\
	}\
	return ret;\
}\


llist_generate(int)
horder_generate(int)
void print_with_spaces(int);
void print_with_cr(int);
int	 sum(int, int);
int  sqr(int);
int	 cube(int);
int  max_cmp(int, int);
int  min_cmp(int, int);
int  multiply2(int);

int main(void){
	int num, s = 0, len, acc = 0;
	int_item *head = NULL;
	int_item *sqrPtr = NULL;
	int_item *cubePtr = NULL;
	int_item *iterated = NULL;
	while(scanf("%d", &num)!=EOF){
		int_llist_add_front(&head, int_llist_create(num));
	}

	puts("Separated by space:");
	int_foreach(head, print_with_spaces);
	puts("\nSeparated by <CR>:");
	int_foreach(head, print_with_cr);

	puts("Squares:");
	sqrPtr = int_map(head, sqr);
	int_foreach(sqrPtr, print_with_spaces);
	puts("\nCubes:");
	cubePtr = int_map(head, cube);
	int_foreach(cubePtr, print_with_spaces);
	puts("");

	s = int_foldl(s, sum, head);
	acc = head->n;
	printf("Sum of this chain: %d\n", s);
	acc = int_foldl(acc, max_cmp, head);
	printf("Max element of this chain is: %d\n", acc);
	acc = head->n;
	acc = int_foldl(acc, min_cmp, head);
	printf("Min element of this chain is: %d\n", acc);

	puts("Print absolute values of chain nodes:");
	int_map_mut(head, abs);
	int_foreach(head, print_with_spaces);
	puts("");

	s = 1;
	len = 10;
	puts("Ten grades on 2:");
	iterated = int_iterate(s, len, multiply2); 
	int_foreach(iterated, print_with_cr);

	int_llist_free(iterated);
	int_llist_free(sqrPtr);
	int_llist_free(cubePtr);
	int_llist_free(head);
	return 0;
}

int multiply2(int x){
	return x*2;
}
int max_cmp(int x, int y){
	return ((long)x-(long)y)>=0l?x:y;
}

int min_cmp(int x, int y){
	return ((long)x-(long)y)<0l?x:y;
}
int sum(int x, int y){
	return x+y;
}
int sqr(int n){
	return n*n;
}

int cube(int n){
	return n*n*n;
}

void print_with_spaces(int n){
	printf("%d ", n);
}

void print_with_cr(int n){
	printf("%d\n", n);
}

