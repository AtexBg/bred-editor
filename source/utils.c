#include <3ds.h>
#include <stdio.h>
#include <string.h>

void flushFramebufferAndWaitForVBlank(){
    gfxFlushBuffers();
	gfxSwapBuffers();

	gspWaitForVBlank();
}

void waitForInput(){
    while(true){ 
        hidScanInput();
        u32 keys = hidKeysDown();

        flushFramebufferAndWaitForVBlank();

        if(keys){ //no bitmask operation = any key can be detected
            //consoleClear();
            return;
        }
    }
}
    
u32 getInput(){
    hidScanInput();
	return hidKeysDown();
}

u32 getHeldKeys(){
    hidScanInput();
	return hidKeysHeld();
}
uint16_t swap16(uint16_t value){
    return (value >> 8) | (value << 8);
}

void clearTopFramebuffer(){
    for(int i=0; i<2; i++){
        u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
        memset(fb, 0, (240*400*2));
    }
}