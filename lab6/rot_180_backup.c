
char *rotate_bmp(bmp_header_t *header, char *original_buf, int rot){
	int i, j = 0;
	char *buf = malloc(header->bfileSize - header->bOffBits);
	uint32_t line = round_4(header->biWidth * 3);
	/*printf("%u", (header->bfileSize - header->bOffBits) / round_4(header->biWidth * 3 ));*/
	for(i = 0; i < header->biHeight; i++){
		for(j = 0; j < line; j += 3){
			memcpy(buf+(header->biHeight-i)*line+j, original_buf+i*line+j, 3);
		}
	}
	/* можно использовать матрицу поворота, поместив фигуру в центр. То есть внести поправку на половину высоты\длины
	   */
			/*
	header->biWidth += header->biHeight;
	header->biHeight = header->biWidth - header->biHeight;
	header->biWidth -= header->biHeight;*/
	return buf;
}
