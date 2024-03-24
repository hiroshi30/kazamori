#include <stdio.h>
#include <stdbool.h>

#include <kazamori.h>
#include <print.h>

#define window_width (16 * 65)
#define window_height (9 * 65)

#define canvas_width (16 * 45)
#define canvas_height window_height

#define pixel_width (16 * 5)
#define pixel_height (9 * 5)


int main() {
    const char * save_file_name = "data.txt";
    ImageSet* image_set = ImageSet_construct(0, 3, canvas_height / pixel_height,canvas_width / pixel_width, 2);
    ImageSet_load(image_set, save_file_name);
    ImageSet_print_output(image_set);

    return 0;
}
