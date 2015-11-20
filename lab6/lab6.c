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
	char format[FORMAT_SIZE];
	int count = fread(&format, char, FORMAT_SIZE, f_image);
	if(count != FORMAT_SIZE)
		return EREAD;


	return SUCCESS;
}

int read_image(const char *imagepath, image_t *image){
	/* first of all we need to recognize which type of this file*/
	int err;
	uint16_t type;

	FILE *f_image = fopen(imagepath, "rb");
	if(f_image == NULL)
		return EOPENFILE;

	err = get_type(f_image, &type);

	return SUCCESS;
}

int main(int argc, char **argv){
	return 0;
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
