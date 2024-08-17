
#include "bytes.c"
#include "png.c"
#include <stdint.h>

int main() {
    bytes* pd = new_png();
    if (!pd) return 1;

    print_bytes(pd);

    pixel* px = new_pixel(255, 255, 0, 255);
    if (!px) return 2;

    if (add_pixel(pd, px) != 0) return 3;

    print_bytes(pd);

    if (complete_png_data(pd) != 0) return 4;

    print_bytes(pd);

    if (bytes_to_file(pd, "smol.png") != 0) return 5;

    return 0;
}