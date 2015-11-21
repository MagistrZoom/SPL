#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

#define FORMAT_SIZE 2
#define BMP_HEADER_SIZE 54
#define OPERATION_LIST(CODE, NAME) \
	struct spec_ops_t NAME##_ops; \
	NAME##_ops.type = CODE; \
	NAME##_ops.read_spec_head = read_##NAME##_head; \
	NAME##_ops.read_spec_body = read_##NAME##_body; \
	NAME##_ops.write_spec_head = write_##NAME##_head;\
	NAME##_ops.write_spec_body = write_##NAME##_body;

#define DO_AND_CHECK(COMMAND) \
	err = COMMAND;\
	if(err != SUCCESS) return err;

#define FILL_BMP_HEADER(OBJ, WIDTH, HEIGHT)\
	OBJ.bfType = 0x4D42;\
	OBJ.bfileSize = 122+round_4(WIDTH*3)*HEIGHT;\
	OBJ.biSize = 108;\
	OBJ.biWidth = WIDTH;\
	OBJ.biHeight = HEIGHT;\
	OBJ.biPlanes = 1;\
	OBJ.biBitCount = 24;\
	OBJ.biSizeImage = OBJ.bfileSize - 122;\
	OBJ.biXPelsPerMeter = 3780;\
	OBJ.biYPelsPerMeter = 3780;\

typedef enum img_errors{
	SUCCESS,
	EOPENFILE,
	EREAD,
	EWRITE,
	ENOIMAGE,
	EWRONGHEAD
} img_errors;


/* TODO: MOVE AWAY */
#pragma pack(push, 2)
typedef struct bmp_header_t{
       uint16_t bfType;                /*magic header should be BM*/
       uint32_t bfileSize;             /*size of bmp in bytes*/
       uint32_t bfReserved;            /*reserved*/
       uint32_t bOffBits;              /*bytes before raster*/
       uint32_t biSize;                /*bitmap size*/
       uint32_t biWidth;               /**/
       uint32_t biHeight;              /**/
       uint16_t biPlanes;              /*planes should be 1*/
       uint16_t biBitCount;            /*1,4,8,24; must be 24*/
       uint32_t biCompresion;         /*compression type*/
       uint32_t biSizeImage;           /*0 or compressed size*/
       uint32_t biXPelsPerMeter;       /*x resolution p per meter*/
       uint32_t biYPelsPerMeter;       /*y --/--/--/--/*/
       uint32_t biClrUsed;             /*how many colors used*/
       uint32_t biClrImportant;        /*important colors*/
} bmp_header_t;
#pragma pack(pop)

typedef struct image_t image_t;

typedef int (*op_callback_t)(FILE *imagefile, image_t *image);

typedef struct spec_ops_t{
	uint16_t type;
	op_callback_t read_spec_head;
	op_callback_t read_spec_body;

	op_callback_t write_spec_head;
	op_callback_t write_spec_body;
} spec_ops_t;

typedef struct pixel_t{
	uint8_t b;
	uint8_t g;
	uint8_t r;
} pixel_t;

struct image_t{
	uint32_t width;
	uint32_t height;
	spec_ops_t *ops;
	pixel_t *pixels;
};

int read_image(const char *imagepath, image_t *image);
int get_type(FILE *image, uint16_t *type);
int get_spec_ops(uint16_t type, spec_ops_t *current_ops); /* an array of callbacks */

int rotate(image_t *image_src, image_t *image_dest);

int write_image(char *imagepath, image_t *image);
