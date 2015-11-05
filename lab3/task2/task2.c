#include <stdio.h>
#include <stdlib.h>


int main(void){
	int result;
	long num;
	char str[129];
	char endPtr;
	printf("Please, type number you wish to check: ");
	scanf("%s", str);
	num = strtoul(str, &endPtr, 10);
	if(str == endPtr || str[0]=='-'){
		printf("Invalid input\n");
		return 0;
	}
	result = is_prime(num);
	result?printf("yes\n"):printf("no\n");
	return 0;
}

int is_prime(unsigned long n){
	unsigned long i;
	for(i = 2; i*i < n; i++){
		if(n%i == 0){
			return 0;
		}
	}
	return 1;
}
