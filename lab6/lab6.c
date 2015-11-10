#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#define HEADERSIZE 54
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

char *rotate_bmp(bmp_header_t *header, char *buf, int rot);

int main(int argc, char **argv){
	FILE *bmp = NULL;
	size_t rd = 0, wr = 0;
	char *buf = NULL, *rot_buf = NULL;
	int rot = 1; /* rot 1 clockwise
			  * rot -1 anti-clockwise
			  */
	bmp_header_t *bmp_header = malloc(sizeof(bmp_header_t));

	bmp = fopen(argv[1], "r+b");

	rd = fread(bmp_header, 1, sizeof(bmp_header_t), bmp);
	if(rd != sizeof(bmp_header_t) || bmp_header->bfType != 0x4D42){
		puts("Invalid file");
		return 1;
	}
	if(bmp_header->biBitCount != (uint32_t)24){
		puts("Bitcount in this file isnt 24");
		return 2;
	}
	
	buf = malloc(bmp_header->bfileSize - bmp_header->bOffBits);
	
	if(buf == NULL){
		puts("Something went wrong");
		return 3;
	}
	fseek(bmp, bmp_header->bOffBits, SEEK_SET);	
	rd = fread(buf, 1, bmp_header->bfileSize - bmp_header->bOffBits, bmp);
	if(rd != bmp_header->bfileSize - bmp_header->bOffBits){
		puts("Invalid bitmap section");
		return 4;
	}
	
	rot_buf = malloc(bmp_header->bfileSize - bmp_header->bOffBits);

	if(argv[2] != 0){
		rot = atoi(argv[2]);
		if(rot == 1 || rot == -1){
			rot_buf = rotate_bmp(bmp_header, buf, rot);
		} else {
			return 0;
		}
	}
	fseek(bmp, 0, SEEK_SET);
	fwrite(bmp_header, 1, sizeof(bmp_header_t), bmp);
	fseek(bmp, bmp_header->bOffBits, SEEK_SET);
	wr = fwrite(buf, 1, bmp_header->bfileSize - bmp_header->bOffBits, bmp);

	/*
REMOVE
	   */
	printf("%lu", wr);

	free(bmp_header);
	free(buf);
	free(rot_buf);
	fclose(bmp);

	return 0;
}

static uint32_t round_4(uint32_t n){
	return n+(4-n%4);
}

char *rotate_bmp(bmp_header_t *header, char *original_buf, int rot){
	int i, j;
	char *buf = malloc(header->bfileSize - header->bOffBits);
	char tmp[3] = {0,0,0};
	uint32_t line = round_4(header->biWidth * 3);
	for(i = 0; i < header->biHeight/2; i++){
		for(j = 0; j < header->biWidth/2; j++){
			
			memcpy(&tmp, original_buf+i*line+j*3, 3);
			memcpy(original_buf+i*line+j*3, original_buf+i*line+(header->biWidth - j - 1)*3, 3);
			memcpy(original_buf+i*line+(header->biWidth - j - 1)*3, original_buf+(header->biHeight - i - 1)*line+j*3, 3);
			memcpy(original_buf+(header->biHeight - i - 1)*line+j*3, original_buf+(header->biHeight - i - 1)*line+(header->biWidth - j - 1)*3, 3);
			memcpy(original_buf+(header->biHeight - i - 1)*line+(header->biWidth - j - 1)*3, &tmp, 3);
			
		}
	}
			
	header->biWidth += header->biHeight;
	header->biHeight = header->biWidth - header->biHeight;
	header->biWidth -= header->biHeight;
	
	header->biXPelsPerMeter += header->biYPelsPerMeter;
	header->biYPelsPerMeter = header->biXPelsPerMeter - header->biYPelsPerMeter;
	header->biXPelsPerMeter -= header->biYPelsPerMeter;

	return buf;
}


