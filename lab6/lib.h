#pragma once
#include "lab6.h"
typedef struct image_t image_t;

typedef int (*op_callback_t)(FILE *imagefile, image_t *image);
typedef int (*op_const_callback_t)(FILE *imagefile, image_t const *image);

typedef struct spec_ops_t{
	uint16_t type;
	op_callback_t read_spec_head;
	op_callback_t read_spec_body;

	op_const_callback_t write_spec_head;
	op_const_callback_t write_spec_body;
} spec_ops_t;

typedef struct pixel_t{
	uint8_t r, g, b;
} pixel_t;

struct image_t{
	uint32_t width;
	uint32_t height;
	spec_ops_t *ops;
	pixel_t *pixels;
};

int read_image(char const *imagepath, image_t *image);
int get_type(FILE *image, uint16_t *type);
int get_spec_ops(uint16_t type, spec_ops_t *current_ops); /*an array of callbacks*/

int rotate(image_t const *image_src, image_t *image_dest);

int write_image(char const *imagepath, image_t const *image);
uint32_t round_4(uint32_t n);
