#include "lab6.h"
#include "lib.h"
#include "bmp.h"

static void ts_name(char name[256]){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(name, "../../lab6_pics/%d-%d-%d %d:%d:%d.bmp", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}


int main(int argc, char **argv){
	image_t image;
	image_t rotated;
	char name[256];
	int err;
	struct spec_ops_t *ops = malloc(sizeof(spec_ops_t));

	DO_AND_CHECK(read_image(argv[1], &image))

	rotated.pixels = malloc(sizeof(pixel_t)*image.width*image.height);
	get_spec_ops(0x4D42, ops);
	rotated.ops = ops;
	
	rotate(&image, &rotated);
	
	free(image.pixels);	free(image.ops); 

	/*make a name for rotated pic*/
	ts_name(name); 
	DO_AND_CHECK(write_image(name, &rotated))

	free(rotated.pixels); free(rotated.ops); 

	return SUCCESS;
}


