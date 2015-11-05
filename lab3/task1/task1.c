#include <stdio.h>

const int array[4]={8,-2,11,5};
int sum_array(int, const int[]); /* prototype */

int main(int argc, char * argv){
	/* 1 task */
	size_t size = sizeof(array[1])/sizeof(array[0]);
	int sum = sum_array(size, array);
	printf("%d\n", sum);
	
	return 0;
}

