#include <3ds.h>
#include <stdio.h>

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

uint16_t swap16(uint16_t value){
    return (value >> 8) | (value << 8);
}
