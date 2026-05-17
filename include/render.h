#include <stdint.h>

#define TOP_LCD 1
#define BOTTOM_LCD 0

#define TOP_WIDTH 400  
#define BTM_WIDTH 320
#define CMN_HEIGHT 240

typedef struct image_t {
    int width;
    int height;
    uint16_t* buffer;
} image_t;

void displayImage(image_t* image, int x, int y, int screen);