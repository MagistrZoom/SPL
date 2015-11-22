#include "lab6.h"
#include "lib.h"
#include "bmp.h"
pixel_t *get_pixel(pixel_t *pixels, uint32_t line, uint32_t x, uint32_t y){
	return pixels+y*line+x;
}

uint32_t round_4(uint32_t n){
	return n%4?n+(4-n%4):n;
}
/* Read 2 first bytes of file
 * @RETURN error code
 * EREAD - file < FORMAT_SIZE
 * SUCCESS - everything is OK
 */
int get_type(FILE *f_image, uint16_t *type){
	uint16_t format;
	int count = fread(&format, sizeof(char), FORMAT_SIZE, f_image);
	fseek(f_image, 0, SEEK_SET);
	if(count != FORMAT_SIZE)
		return EREAD;

	*type = format;
	return SUCCESS;
}


int get_spec_ops(uint16_t type, spec_ops_t *ops){
	int i;

	spec_ops_t ops_list[1];
	OPERATION_LIST(0x4D42, bmp)
	ops_list[0] = bmp_ops;

	for(i = 0; i < 1; i++){
		if(ops_list[i].type == type){
			*ops = ops_list[i];	
			return SUCCESS;
		}
	}

	return ENOIMAGE; 
}

int read_image(const char *imagepath, image_t *image){
	/* first of all we need to recognize which type of this file*/
	int err;
	uint16_t type;
	/* dont forget free dat memory */
	struct spec_ops_t *ops = malloc(sizeof(spec_ops_t));  

	FILE *f_image = fopen(imagepath, "rb");
	if(f_image == NULL)
		return EOPENFILE;

	DO_AND_CHECK(get_type(f_image, &type))

	/* now we need to get list of operations for this filetype*/
	DO_AND_CHECK(get_spec_ops(type, ops))
	image->ops = ops;
	
	/* and now we can read file */
	DO_AND_CHECK((image->ops)->read_spec_head(f_image, image))			
	DO_AND_CHECK((image->ops)->read_spec_body(f_image, image))
	fclose(f_image);
	return SUCCESS;
}

int write_image(char *imagepath, image_t *image){
	int err;
	FILE *f_image = fopen(imagepath, "wb");
	if(f_image == NULL)
		return EWRITE;

	DO_AND_CHECK((image->ops)->write_spec_head(f_image, image))
	DO_AND_CHECK((image->ops)->write_spec_body(f_image, image))

	fclose(f_image);
	return SUCCESS;
}

int rotate(image_t *old, image_t *new){
	uint32_t i, j;
	new->width = old->height;
	new->height = old->width;

	for(i = 0; i < old->height; i++){
		for(j = 0; j < old->width; j++){
			*(get_pixel(new->pixels, new->width, i, new->height - j - 1)) = *(get_pixel(old->pixels, old->width, j, i));	
		}
	}
	return 0;
}


