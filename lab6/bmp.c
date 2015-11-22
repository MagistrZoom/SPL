#include "lab6.h"
#include "lib.h"
#include "bmp.h"

/* read first 54 bytes of file and set up file pointer to bitmap or something
   same*/
int read_bmp_head(FILE *f_image, image_t *image){
	struct bmp_header_t header;
	size_t count;

	count = fread(&header, 1, BMP_HEADER_SIZE, f_image);
	if(count != BMP_HEADER_SIZE || header.biBitCount != 24)
		return EWRONGHEAD; 

	count = fseek(f_image, header.bOffBits, SEEK_SET);
	if (count != 0)
		return EREAD;

	image->width = header.biWidth;
	image->height = header.biHeight;

	return SUCCESS;
}

/* at this time f_image points to bitmap. Need to read it without trash */
int read_bmp_body(FILE *f_image, image_t *image){
	size_t count;
	uint32_t i;
	unsigned int diff = round_4(image->width*3)-image->width*3;
	pixel_t *t, *buf;
	image->pixels = malloc(image->width*image->height*sizeof(pixel_t));
	t = image->pixels;
	buf = malloc(diff + image->width*3);

	for(i = 0; i < image->height; i++){
		count = fread(buf, sizeof(pixel_t), image->width, f_image);
		fseek(f_image, diff, SEEK_CUR);
		if(count != image->width){
			free(buf);
			return EREAD;
		}
		memcpy(t, buf, image->width*3);
		t += image->width;
	}
	free(buf);
	return SUCCESS;
}

int write_bmp_head(FILE *f_image, image_t *image){
	bmp_header_t bmp_header;
	size_t count;
	FILL_BMP_HEADER(bmp_header, image->width, image->height)
	count = fwrite(&bmp_header, sizeof(char), BMP_HEADER_SIZE, f_image);
	if(count != BMP_HEADER_SIZE)
		return EWRITE;
	count = fseek(f_image, 122, SEEK_SET);
	if(count != 0)
		return EWRITE;

	return SUCCESS;
}

int write_bmp_body(FILE *f_image, image_t *image){
	uint32_t i;
	unsigned int diff = round_4(image->width*3)-image->width*3;
	char trash[4] = {
		0,0,0
	};
	size_t count;
	pixel_t *t = image->pixels;
	for(i = 0; i < image->height; i++){
		count = fwrite(t, sizeof(pixel_t), image->width, f_image);
		fwrite(&trash, 1, diff, f_image);
		if(count != image->width){
			return EWRITE;
		}
		t += count;
	}
	return SUCCESS;
}
