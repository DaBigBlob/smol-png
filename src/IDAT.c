
#include <stdio.h>
#ifndef IDAT
#define IDAT "0.1"

#include <stdint.h>
#include "bytes.c"

uint8_t IDAT_type_sig[] = {73, 68, 65, 84};

uint32_t generate_crc(bytes* pid, uint8_t crc_load_begin, uint8_t crc_load_end) {
    // x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1
    //TODO
    uint8_t CRC[] = {12, 187, 91, 231};
    uint32_t crc = 0;
    for (uint8_t i=0; i<4; i++) crc += (CRC[i] << (i*8));
    return crc;
}

bytes* generate_IDAT(bytes* zlstrm) {
    bytes* pid = new_bytes(255);
    if (!pid) goto ret;

    if (add_uint32_be(pid, (uint32_t) zlstrm->len) != 0) goto bad;

    uint8_t crc_load_begin = pid->len;

    if(add_bytes_from(pid, IDAT_type_sig, sizeof(IDAT_type_sig), false) != 0) goto bad;

    if(add_bytes(pid, zlstrm, zlstrm->len, true) != 0) goto bad;

    if (add_uint32_le(pid, generate_crc(pid, crc_load_begin, pid->len)) != 0) goto bad;

    return pid;

    bad:
        free_bytes(pid);
    ret:
        return NULL;
}

#endif