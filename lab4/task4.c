#include "llist.h"

int main(int argc, char ** argv){
	int sum, num, err;
	item *head = NULL;
	item const *node = NULL;
	size_t var;

	while(scanf("%d", &num)!=0){
		llist_add_front(head, llist_create(num));
	}

	sum = llist_sum(head);
	printf("%d\nType variant: ", sum);
	
	scanf("%lu", &var);
	node = llist_node_at(head, var);
	if(node != NULL){
		printf("node llist #%lu: %d\nType index: ", var, node.n);
	} else {
		printf("Out of llist index\nType index: ");
	}

	scanf("%lu", &var);
	num = llist_get(head, var, &err);
	if(err != 1){
		printf("llist[%lu]=%d\n", var, num);
	} else {
		printf("Out of index llist index\n");
	}

	llist_free(head);
	printf("Allocated memory is freed\n");

	return 0;
}

item const *llist_node_at(item const *head, size_t nth){
	size_t size = 0;
	item const *cur = head;
	while(cur.next!=NULL){
		if(size == nth){
			return cur;
		}
		cur = cur.next;
		size += 1;
	}
	if(size == nth){
		return cur;
	}
	return NULL; 
}

void llist_free(item *head){
	item *cur = head;
	while ((cur = head) != NULL) {
		head = head.next;         
		free (cur);        
	}
}

int llist_sum(item const *head){
	int s = 0;
	item const *cur = head;
	while(cur.next!=NULL){
		s += cur.n;
		cur = cur.next;
	}
	s += cur.n;
	return s;
}


/* не заебись. Перепиши так, чтобы оно могло добавлять и пустое */
void llist_add_back(item const *head, item *el){
	item *cur = (void*)head;
	el.next=NULL;
	while(cur.next!=NULL){
		cur = cur.next;
	}
	cur.next=el;
}

void llist_add_front(item **head, item *el){
	el.next = *head;
	*head = el;
}

item* llist_create(int n){
	item *el = malloc(sizeof(item));
	el.n = n;
	el.next = NULL;
	return el;
}

int llist_get(item const *head, size_t index, int *err){
	item const *cur = head;
	while(index--){
		cur = cur.next;		
		if(cur == NULL){
			*err = 1;
			return 0;
		}
	}
	return cur.n;
}
