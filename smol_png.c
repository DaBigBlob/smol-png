#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int deflate_RGBA(uint8_t* bytes, uint8_t R, uint8_t G, uint8_t B, uint8_t A) {
    
    return 0;
}

int main() {
    FILE* f = fopen("smol.png", "w");
    if (!f) return 1;

    uint8_t bytes[] = {
        137, 80, 78, 71, 13, 10, 26, 10, //file sig

        0, 0, 0, 13,        //chunk len
        73, 72, 68, 82,     //chunk type //IHDR [header, info]
        //chunk data
        0, 0, 0, 1,     //width
        0, 0, 0, 1,     //height
        8,              //bit depth
        6,              //color type [Each pixel is an R,G,B triple, followed by an alpha sample.]
        0,              //png compress method [zlib(deflate)]
        0,              //filter method
        0,              //interlace method [none]
        31, 21, 196, 137,   //crc

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

        0, 0, 0, 0,         //chunk len
        73, 69, 78, 68,     //chunk type //IEND [end of file]
        174, 66, 96, 130    //crc
    };


    size_t sizeof_bytes = sizeof(bytes);

    if (fwrite(bytes, 1, sizeof_bytes, f) != sizeof_bytes) return 2;

    return 0;
}