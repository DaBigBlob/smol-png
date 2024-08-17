
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

bytes* new_png() {
    bytes* pd = new_bytes(255);
    if (!pd) return NULL;

    if (add_bytes_from(pd, SIG_IHDR, sizeof(SIG_IHDR), false) != 0) return NULL;;

    return pd;
}

uint8_t complete_png_data(bytes* pd) {
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

uint8_t add_pixel(bytes* pd, pixel* px) {
    uint8_t DUMMY[] = {
        0, 0, 0, 11,        //chunk len
        73, 68, 65, 84,     //chunk type //IDAT [actual image data]
            //chunk data begin (zlib)
            0b00001000, //CMF [deflate(8); log2(LZ77window)-8 = 0 => 2^8 = LZ77window]
            0b00011101, //FLG [FCHECK=0b11101; FDICT=0; FLEVEL=0 (fastest algorithm)]
                //compressed data (deflate)
                0b01100011, //[BFINAL=1; BTYPE=01 (compressed with fixed Huffman codes); extra=00110]
                248, 15, 4, 0,
                /** bitstream = 00110 00011111 11110000 00100000 00000000
                    bitstream_coded = 00110000 111111111 0000001 0 0000000 00000

                    00110000 //0 literal
                    111111111 //255 literal
                    0000001 //length = 3
                    0 //dist = 1
                    0000000 //256 end of block
                    00000 //padding

                    deflated = 0 255 255 255 255
                */
                //compressed data
            9, 251, 3, 253, //adler32 of uncompressed data
            //chunk data end
        12, 187, 91, 231,   //crc of chunk type and chunk data
    };
    uint8_t ret = add_bytes_from(pd, DUMMY, sizeof(DUMMY), false);
    if (ret != 0) return ret;

    return 0;
} 

#endif