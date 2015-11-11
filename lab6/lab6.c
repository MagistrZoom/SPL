#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

#pragma pack(push, 2)
typedef struct {
	uint16_t bfType;			/*magic header should be BM*/
	uint32_t bfileSize;			/*size of bmp in bytes*/
	uint32_t bfReserved;		/*reserved*/
	uint32_t bOffBits;			/*bytes before raster*/
	uint32_t biSize;			/*bitmap size*/
	uint32_t biWidth;			/**/
	uint32_t biHeight;			/**/
	uint16_t biPlanes;			/*planes should be 1*/
	uint16_t biBitCount;		/*1,4,8,24; must be 24*/
	uint32_t biCompresiion;		/*compression type*/
	uint32_t biSizeImage;		/*0 or compressed size*/
	uint32_t biXPelsPerMeter;	/*x resolution p per meter*/
	uint32_t biYPelsPerMeter;	/*y --/--/--/--/*/
	uint32_t biClrUsed;			/*how many colors used*/
	uint32_t biClrImportant;	/*important colors*/
} bmp_header_t;
#pragma pack(pop)

char *rotate_bmp(bmp_header_t *header, char *buf, int rot, uint32_t *size);
bmp_header_t *modify_headers(bmp_header_t *original_header, uint32_t size);

int main(int argc, char **argv){
	FILE *bmp = NULL;
	size_t rd = 0;
	char *buf = NULL, *rot_buf = NULL;
	uint32_t size = 0;
	int rot = 1; /*rot 1 clockwise
			  * rot -1 anti-clockwise
			  */
	time_t t;
	struct tm tm;

	char *generated_name = malloc(256*sizeof(char));
	bmp_header_t *bmp_header = malloc(sizeof(bmp_header_t));
	bmp_header_t *header = malloc(sizeof(bmp_header_t));

	t = time(NULL);
	tm = *localtime(&t);
	sprintf(generated_name, "%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	bmp = fopen(argv[1], "rb");

	rd = fread(bmp_header, 1, sizeof(bmp_header_t), bmp);
	if(rd != sizeof(bmp_header_t) || bmp_header->bfType != 0x4D42){
		puts("Invalid file");
		return 1;
	}
	if(bmp_header->biBitCount != (uint32_t)24){
		puts("Bitcount in this file isnt 24");
		return 2;
	}
	
	size = bmp_header->biSizeImage;

	buf = malloc(size);
	
	if(buf == NULL){
		puts("Something went wrong");
		return 3;
	}

	fseek(bmp, bmp_header->bOffBits, SEEK_SET);	
	rd = fread(buf, 1, size, bmp);
	
	fclose(bmp);
	if(rd != size){
		puts("Invalid bitmap section");
		return 4;
	}
	
	rot_buf = malloc(size);
	
	if(argv[2] != 0){
		rot = atoi(argv[2]);
		if(rot == 1 || rot == -1){
			rot_buf = rotate_bmp(bmp_header, buf, rot, &size);
			header = modify_headers(bmp_header, size);
		} else {
			return 0;
		}
	}

	bmp = fopen(generated_name, "wb");

	fseek(bmp, 0, SEEK_SET);
	fwrite(header, 1, sizeof(bmp_header_t), bmp);
	fseek(bmp, header->bOffBits, SEEK_SET);
	fwrite(rot_buf, 1, size, bmp);

	fclose(bmp);

	free(bmp_header);
	free(header);
	free(buf);
	free(rot_buf);

	return 0;
}

static uint32_t round_4(uint32_t n){
	return n%4!=0?n+(4-n%4):n;
}

bmp_header_t *modify_headers(bmp_header_t *original_header, uint32_t size){	 
	bmp_header_t *header = malloc(sizeof(bmp_header_t));
	memcpy(header, original_header, sizeof(bmp_header_t));

	header->biWidth += original_header->biHeight;
	header->biHeight = header->biWidth - original_header->biHeight;
	header->biWidth -= header->biHeight;
	
	header->biSizeImage = size;
	header->bfileSize = size + original_header->bOffBits;

	header->biXPelsPerMeter += original_header->biYPelsPerMeter;
	header->biYPelsPerMeter = header->biXPelsPerMeter - original_header->biYPelsPerMeter;
	header->biXPelsPerMeter -= header->biYPelsPerMeter;
	
	return header;
}
char *rotate_bmp(bmp_header_t *header, char *original_buf, int rot, uint32_t *size){
	uint32_t i, j;
	char *buf = malloc(*size);
	uint32_t line = round_4(header->biWidth * 3);
	uint32_t rot_line = round_4(header->biHeight * 3);

	*size = 0;
	/* clockwise */
	if(rot == 1){
		for(i = 0; i < header->biHeight; i++){
			for(j = 0; j < header->biWidth; j++){
				memcpy(buf+(header->biWidth-j-1)*rot_line+(i)*3, original_buf+i*line+j*3, 3);
				*size += 3;
			}
			*size += line - j*3;
		}
	} else 
	/* anti-clockwise */	
	if(rot == -1){
		for(i = 0; i < header->biHeight; i++){
			for(j = 0; j < header->biWidth; j++){
				memcpy(buf+j*rot_line+(header->biHeight - i - 1)*3, original_buf+i*line+j*3, 3);
				*size += 3;
			}
			*size += line - j*3;
		}
	}

	return buf;
}


