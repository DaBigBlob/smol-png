
#include <stdlib.h>
#include <string.h>
#ifndef BYTES
#define BYTES "0.1"

#include <stdint.h>

typedef enum _boolean {
    true = 1,
    false = 0
} boolean;

typedef struct _bytes {
    uint8_t len;
    uint8_t size;
    uint8_t* body;
    boolean lock;
} bytes;

void free_bytes(bytes* bs) {
    free(bs->body);
    free(bs);
}

bytes* new_bytes(uint8_t size) {
    bytes* b = malloc(sizeof(bytes));
    if (!b) return NULL;

    if (memset(b, 0, sizeof(bytes)) != b) {
        free(b);
        return NULL;
    }

    b->body = malloc(size);
    if (!b->body) return NULL;

    if (memset(b->body, 0, size) != b->body) {
        free(b);
        return NULL;
    }

    b->size = size;
    b->len = 0;
    b->lock = false;

    return b;
}

void add_byte_unsafe(bytes* bs, uint8_t b) {
    bs->body[bs->len] = b;
    bs->len++;
}

uint8_t add_byte(bytes* bs, uint8_t b) {
    if (bs->lock == true) return 1;
    if (bs->len+1 >= bs->size) return 2;

    add_byte_unsafe(bs, b);

    return 0;
}

void add_uint32_unsafe(bytes* bs, uint32_t b4) {
    for (uint8_t i=0;i<4;i++) {
        add_byte_unsafe(bs, (uint8_t) (b4&0b11111111));
        b4>>=8;
    }
}

uint8_t add_uint32(bytes* bs, uint32_t b4) {
    if (bs->lock == true) return 1;
    if (bs->len+1 >= bs->size) return 2;

    add_uint32_unsafe(bs, b4);

    return 0;
}

uint8_t add_bytes_from(bytes* dst, uint8_t* src, uint8_t len, boolean take_ownership) {
    if (dst->lock == true) return 1;

    if (memcpy(dst->body + dst->len, src, len) != dst->body + dst->len) return 2;

    dst->len += len;

    if (take_ownership) free(src);

    return 0;
}

uint8_t add_bytes(bytes* dst, bytes* src, uint8_t len, boolean take_ownership) {
    uint8_t ret = add_bytes_from(dst, src->body, len, false);
    if (take_ownership) free_bytes(src);
    return ret;

}

void lock_bytes(bytes* bs) {
    bs->lock = true;
}

void unlock_bytes(bytes* bs) {
    bs->lock = false;
}


#endif