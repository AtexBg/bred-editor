#include <3ds.h>
#include <string.h>

#include "render.h"
#include "gfx/font.h"

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
                memcpy(fb+y  +(CMN_HEIGHT*(j+x)), image->buffer+(image->height * j),((y + image->height > CMN_HEIGHT) ? (CMN_HEIGHT-y) : image->height) * sizeof(u16));
            }
        }
        gfxSwapBuffers(); //swap to write to the 2nd buffer
    }
}   

void renderTextWithGameBoyFont(unsigned char* text, int length, int lineX, int lineY){
    // a single GB character is 8x8 pixels, just like the libctru console font
    int x = lineX*8;
    int y = CMN_HEIGHT-(lineY*8);

    bool EOS_reached = false;

    for(int i=0; i<length; i++){
        if(!EOS_reached){
            if(text[i] >= 'A' && text[i] <= 'Z'){
                displayImage(gbfontUppercaseLetters[text[i]-'A'], x+i*8, y, TOP_LCD);
            }
            if(text[i] >= 'a' && text[i] <= 'z'){
                displayImage(gbfontLowercaseLetters[text[i]-'a'], x+i*8, y, TOP_LCD);
            }
            if(text[i] >= '0' && text[i] <= '9'){
                displayImage(gbfontNumbers[text[i]-'0'], x+i*8, y, TOP_LCD);
            }
            if(text[i] == '\0'){EOS_reached = true;}
        }
    }
}