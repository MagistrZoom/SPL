#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

#pragma pack(push, 2)
typedef struct {
       uint16_t bfType;                /*magic header should be BM*/
       uint32_t bfileSize;             /*size of bmp in bytes*/
       uint32_t bfReserved;            /*reserved*/
       uint32_t bOffBits;              /*bytes before raster*/
       uint32_t biSize;                /*bitmap size*/
       uint32_t biWidth;               /**/
       uint32_t biHeight;              /**/
       uint16_t biPlanes;              /*planes should be 1*/
       uint16_t biBitCount;            /*1,4,8,24; must be 24*/
       uint32_t biCompresiion;         /*compression type*/
       uint32_t biSizeImage;           /*0 or compressed size*/
       uint32_t biXPelsPerMeter;       /*x resolution p per meter*/
       uint32_t biYPelsPerMeter;       /*y --/--/--/--/*/
       uint32_t biClrUsed;             /*how many colors used*/
       uint32_t biClrImportant;        /*important colors*/
} bmp_header_t;
#pragma pack(pop)

char *rotate_bmp(bmp_header_t*, char*, int, uint32_t);
bmp_header_t *modify_headers(bmp_header_t*, uint32_t);
static uint32_t round_4(uint32_t);
static void swap(uint32_t*, uint32_t*);
