int sum_array(size_t size, const int array[]){
	int s = 0;
	size_t i;
	for(i = 0; i < size; i++){
		s += array[i];
	}
	return s;
}

