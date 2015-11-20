#include "lab.h"

int main(int argc, char **argv){
	FILE *bmp = NULL;
	size_t rd = 0;
	char *buf = NULL, *rot_buf = NULL;
	uint32_t size = 0;
	int rot; /*rot 1 clockwise
			  * rot -1 anti-clockwise
			  */
	time_t t;
	struct tm tm;

	char *generated_name = malloc(256*sizeof(char));
	bmp_header_t *bmp_header = malloc(sizeof(bmp_header_t));
	bmp_header_t *header = NULL;

	if(argc != 3){
		puts("Invalid arguments");
		return 1;
	}
	bmp = fopen(argv[1], "rb");
	if(bmp == NULL){
		puts("Invalid file");
		return 2;
	}
	rd = fread(bmp_header, 1, sizeof(bmp_header_t), bmp);
	if(rd != sizeof(bmp_header_t) || bmp_header->bfType != 0x4D42){
		puts("Invalid file");
		return 2;
	}

	size = bmp_header->biSizeImage;

	buf = malloc(size);

	if(buf == NULL){
		puts("Something went wrong");
		return 3;
	}

	if(bmp_header->biBitCount != 24){
		puts("Bitcount in this file isnt 24");
		return 4;
	}

	fseek(bmp, bmp_header->bOffBits, SEEK_SET);	
	rd = fread(buf, 1, size, bmp);
	fclose(bmp);

	if(rd != size){
		puts("Invalid bitmap section");
		return 5;
	}
	
	rot = atoi(argv[2]);
	if(rot == 1 || rot == -1){
		rot_buf = rotate_bmp(bmp_header, buf, rot, size);

		size = calculate_size(bmp_header);

		header = modify_headers(bmp_header, size);

		t = time(NULL);
		tm = *localtime(&t);
		sprintf(generated_name, "pics/%d* %d-%d-%d %d:%d:%d.bmp", (rot==1)?90:-90,tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
		bmp = fopen(generated_name, "wb");
	
		fwrite(header, 1, sizeof(bmp_header_t), bmp);
		fseek(bmp, header->bOffBits, SEEK_SET);
		fwrite(rot_buf, 1, size, bmp);
		fclose(bmp);
	} else {
		puts("Invalid direction");
		return 5;
	}

	free(bmp_header);
	free(header);
	free(rot_buf);
	free(buf);
	free(generated_name);
	return 0;
}

static uint32_t calculate_size(bmp_header_t *bmp_header){	
	uint32_t line = round_4(bmp_header->biWidth * 3);
 	uint32_t rot_line = round_4(bmp_header->biHeight * 3);
	uint32_t size;

	if(bmp_header->biWidth > bmp_header->biHeight){
		size = (line-(bmp_header->biWidth-1)*3)*bmp_header->biHeight + 3*(bmp_header->biWidth)*(bmp_header->biHeight);
	} else {
		size = (rot_line-(bmp_header->biHeight-1)*3)*bmp_header->biWidth + 3*(bmp_header->biWidth)*(bmp_header->biHeight);
	}
	return size;
}

static uint32_t round_4(uint32_t n){
	return n%4!=0?n+(4-n%4):n;
}

static void swap(uint32_t *a, uint32_t *b){
	uint32_t tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

bmp_header_t *modify_headers(bmp_header_t *original_header, uint32_t size){	 
	bmp_header_t *header = malloc(sizeof(bmp_header_t));
	memcpy(header, original_header, sizeof(bmp_header_t));
	
	swap(&header->biWidth, &header->biHeight);
	swap(&header->biXPelsPerMeter, &header->biYPelsPerMeter);

	header->biSizeImage = size;
	header->bfileSize = size + original_header->bOffBits;

	return header;
}

char *rotate_bmp(bmp_header_t *header, char *original_buf, int rot, uint32_t size){
	uint32_t i, j;
	char *buf = malloc(2 * size);
	uint32_t line = round_4(header->biWidth * 3);
	uint32_t rot_line = round_4(header->biHeight * 3);
	/* clockwise */
	if(rot == 1){
		for(i = 0; i < header->biHeight; i++){
			for(j = 0; j < header->biWidth; j++){
				memcpy(buf+(header->biWidth - j - 1)*rot_line+(i)*3, original_buf+i*line+j*3, 3);
			}
		}
	} else 
	/* anti-clockwise */	
	if(rot == -1){
		for(i = 0; i < header->biHeight; i++){
			for(j = 0; j < header->biWidth; j++){
				memcpy(buf+j*rot_line+(header->biHeight - i - 1)*3, original_buf+i*line+j*3, 3);
			}
		}
	}
	return buf;
}
