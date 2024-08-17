
#include <stdint.h>
#ifndef PNG
#define PNG "0.1"

#include <stdlib.h>
#include <string.h>
#include "bytes.c"

uint8_t SIG_IHDR[] = {
    //SIG
    137, 80, 78, 71, 13, 10, 26, 10,

    //IHDR
    0, 0, 0, 13, //len
    73, 72, 68, 82, //type
    0, 0, 0, 1, //width
    0, 0, 0, 1, //height
    8,              //bit depth
    6,              //color type [RGBA/px]
    0,              //[zlib(deflate)]
    0,              //[no filter]
    0,              //[no interlace]
    31, 21, 196, 137, //crc
};

uint8_t IEND[] = {
    //IEND
    0, 0, 0, 0, //len
    73, 69, 78, 68, //type
    174, 66, 96, 130 //crc
};

typedef bytes png_data;

png_data* new_png() {
    png_data* pd = new_bytes(255);
    if (!pd) return NULL;

    if (add_bytes_from(pd, SIG_IHDR, sizeof(SIG_IHDR), false) != 0) return NULL;;

    return pd;
}

uint8_t complete_png_data(png_data* pd) {
    uint8_t ret = add_bytes_from(pd, IEND, sizeof(IEND), false);
    if (ret != 0) return ret;

    lock_bytes(pd);

    return 0;
}

typedef struct _pixel {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
} pixel;

pixel* new_pixel(uint8_t R, uint8_t G, uint8_t B, uint8_t A) {
    pixel* px = malloc(sizeof(pixel));
    if (!px) return NULL;

    px->R = R;
    px->G = G;
    px->B = B;
    px->A = A;

    return px;
}

uint8_t add_pixel(png_data* pd, pixel* px) {

    return 0;
} 

#endif