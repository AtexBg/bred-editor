#include <3ds.h>
#include <string.h>

#include "render.h"

void displayImage(image_t* image, int x, int y, int screen){

    uint16_t* fb = NULL;
    //copy image data in both framebuffers
    for(int i=0; i<2; i++){
        if(screen == TOP_LCD){
            fb = (uint16_t*)gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
        }else if(screen == BOTTOM_LCD){
            //fb is casted to uint16_t* because it's RGB565 (aka 16 bits per pixel)
            fb = (uint16_t*)gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
        }

        for(int j=0; j<image->width; j++){
            if(j+x < (screen==TOP_LCD ? TOP_WIDTH : BTM_WIDTH)){
                //memcpy(fb+y*3+(CMN_HEIGHT*(j+x)*3), image->buffer+(image->height*j*3), (y+image->height > CMN_HEIGHT) ? (CMN_HEIGHT-y)*3 : image->height*3);
                memcpy(fb+y  +(CMN_HEIGHT*(j+x)),   image->buffer+(image->height * j),((y + image->height > CMN_HEIGHT)? (CMN_HEIGHT-y): image->height) * sizeof(u16));
            }
        }
        gfxSwapBuffers(); //swap to write to the 2nd buffer
    }
}   