
#ifndef ZLIBSTREAM
#define ZLIBSTREAM "0.1"

#include <stdint.h>
#include "bytes.c"

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

    uint8_t ZSTR[] = {
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
            9, 251, 3, 253
    };

    add_bytes_from(bs, ZSTR, sizeof(ZSTR), false);

    return bs;
}

#endif