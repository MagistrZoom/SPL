#include "lab6.h"
#include "lib.h"
#include "bmp.h"

static void fill_bmp_header(struct bmp_header_t *obj, uint32_t width, uint32_t height){
	obj->bfType = 0x4D42;
	obj->bfileSize = 122+round_4((width)*sizeof(struct pixel_t))*(height);
	obj->bfReserved = 0;
	obj->bOffBits = 122;
	obj->biSize = 108;
	obj->biWidth = width;
	obj->biHeight = height;
	obj->biPlanes = 1;
	obj->biBitCount = 24;
	obj->biSizeImage = round_4((width)*sizeof(struct pixel_t))*(height);
	obj->biXPelsPerMeter = 3780;
	obj->biYPelsPerMeter = 3780;
	obj->biClrUsed = 0;
	obj->biCompresion = 0;
	obj->biClrImportant = 0;
}

/* read first 54 bytes of file and set up file pointer to bitmap or something
   same*/
int read_bmp_head(FILE *f_image, image_t *image){
	struct bmp_header_t header;
	size_t count;

	count = fread(&header, 1, sizeof(struct bmp_header_t), f_image);
	if(count != sizeof(struct bmp_header_t) || header.biBitCount != 24)
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
	unsigned int diff = round_4(image->width*sizeof(struct pixel_t))-image->width*sizeof(struct pixel_t);
	pixel_t *t;
	image->pixels = malloc(image->width*image->height*sizeof(struct pixel_t));
	t = image->pixels;

	for(i = 0; i < image->height; i++){
		count = fread(t, sizeof(pixel_t), image->width, f_image);
		fseek(f_image, diff, SEEK_CUR);
		if(count != image->width){
			free(image->pixels);
			return EREAD;
		}
		t += image->width;
	}
	return SUCCESS;
}

int write_bmp_head(FILE *f_image, image_t const *image){
	bmp_header_t bmp_header;
	size_t count;
	fill_bmp_header(&bmp_header, image->width, image->height);
	count = fwrite(&bmp_header, sizeof(char), sizeof(struct bmp_header_t), f_image);
	if(count != sizeof(struct bmp_header_t))
		return EWRITE;
	count = fseek(f_image, 122, SEEK_SET);
	if(count != 0)
		return EWRITE;

	return SUCCESS;
}

int write_bmp_body(FILE *f_image, image_t const *image){
	uint32_t i;
	unsigned int diff = round_4(image->width*sizeof(struct pixel_t))-image->width*sizeof(struct pixel_t);
	uint32_t trash = 0;
	pixel_t *t = image->pixels;
	for(i = 0; i < image->height; i++){
		size_t count;
		count = fwrite(t, sizeof(pixel_t), image->width, f_image);
		fwrite(&trash, 1, diff, f_image);
		if(count != image->width){
			return EWRITE;
		}
		t += count;
	}
	return SUCCESS;
}
