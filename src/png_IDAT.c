
#ifndef PNG_IDAT
#define PNG_IDAT "0.1"

#include <stdint.h>
#include "bytes.c"

uint8_t IDAT_type_sig[] = {73, 68, 65, 84};

typedef struct _png_IDAT_struct {
    uint32_t len;
    bytes* zlibstream;
    uint32_t crc;
} png_IDAT_struct;

typedef bytes png_IDAT_data;

png_IDAT_data* serialize_png_IDAT(png_IDAT_struct* istr) {
    bytes* pid = new_bytes(255);
    if (!pid) return NULL;

    if (add_uint32(pid, istr->len) != 0) {
        free_bytes(pid);
        return NULL;
    }

    if(add_bytes_from(pid, IDAT_type_sig, sizeof(IDAT_type_sig), false) != 0) {
        free_bytes(pid);
        return NULL;
    }

    if(add_bytes(pid, istr->zlibstream, istr->zlibstream->len, true) != 0) {
        free_bytes(pid);
        return NULL;
    }

    if (add_uint32(pid, istr->crc) != 0) {
        free_bytes(pid);
        return NULL;
    }

    return pid;
}

uint8_t populate_crc(png_IDAT_struct* istr) {
    //TODO
    return 0;
}

#endif