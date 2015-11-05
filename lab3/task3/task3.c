#include <stdio.h>
#include <malloc.h>


int main(int argc, char ** argv){
	int sum;
	size_t size, i = 0;
	int *arr;
	scanf("%lu", &size);
	arr = malloc(size*sizeof(size_t));
	while(i < size && scanf("%d", &arr[i++])!=0);
	sum = sum_array(size, arr);
	printf("Sum is: %d\n", sum);
	free(arr);
	return 0;
}
