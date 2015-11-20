#include "lab6.h"

static uint32_t round_4(uint32_t n){
	return n%4!=0?n+(4-n%4):n;
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
	
	return SUCCESS;
}
int write_bmp_head(FILE *f_image, image_t *image){
	return SUCCESS;
}
int write_bmp_body(FILE *f_image, image_t *image){
	return SUCCESS;
}

int get_spec_ops(uint16_t type, spec_ops_t *ops){
	int i;
	struct spec_ops_t bmp_ops; 

	spec_ops_t ops_list[1];

	bmp_ops.type = 0x4D42; 
	bmp_ops.read_spec_head = read_bmp_head; 
	bmp_ops.read_spec_body = read_bmp_body; 
	bmp_ops.write_spec_head = write_bmp_head; 
	bmp_ops.write_spec_body = write_bmp_body;
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

	err = get_type(f_image, &type);
	if(err != SUCCESS) return err;
	/* now we need to get list of operations for this filetype*/
	err = get_spec_ops(type, ops);			
	if(err != SUCCESS) return err;
	image->ops = ops;
	
	/* and now we can read file */
	err = (image->ops)->read_spec_head(f_image, image);			
	if(err != SUCCESS) return err;
	
	err = (image->ops)->read_spec_body(f_image, image);			
	if(err != SUCCESS) return err;

	return SUCCESS;
}


int main(int argc, char **argv){
	image_t image;
	int err = read_image("gingerkitten.bmp", &image);
	return err;
}

/*char *rotate_bmp(bmp_header_t *header, char *original_buf, int rot, uint32_t size){
	uint32_t i, j;
	char *buf = malloc(2 * size);
	uint32_t line = round_4(header->biWidth * 3);
	uint32_t rot_line = round_4(header->biHeight * 3);
	
	if(rot == 1){
		for(i = 0; i < header->biHeight; i++){
			for(j = 0; j < header->biWidth; j++){
				memcpy(buf+(header->biWidth - j - 1)*rot_line+(i)*3, original_buf+i*line+j*3, 3);
			}
		}
	} else 
  
	if(rot == -1){
		for(i = 0; i < header->biHeight; i++){
			for(j = 0; j < header->biWidth; j++){
				memcpy(buf+j*rot_line+(header->biHeight - i - 1)*3, original_buf+i*line+j*3, 3);
			}
		}
	}
	return buf;
}
*/
