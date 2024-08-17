
#ifndef ZLIBSTREAM
#define ZLIBSTREAM "0.1"

#include <stdint.h>
#include "bytes.c"
#include "png.c"

uint8_t zlib_CMF = 0b00001000; //[deflate(8); log2(LZ77window)-8 = 0 => 2^8 = LZ77window]
uint8_t zlib_FLG = 0b00011101; //[FCHECK=0b11101; FDICT=0; FLEVEL=0 (fastest algorithm)]
uint8_t deflate_HDR = 0b011; //[BFINAL=1; BTYPE=01 (compressed with fixed Huffman codes)]

uint32_t* adler32() {
    return 0;
}

bytes* generate_zlibstream(pixel* px) {
    bytes* bs = new_bytes(255);
    if (!bs) return NULL;

    add_byte_unsafe(bs, zlib_CMF);
    add_byte_unsafe(bs, zlib_FLG);

    return NULL;
}

#endif