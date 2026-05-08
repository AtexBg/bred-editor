#include <3ds.h>

#define LINE(x) "\x1b[" #x ";1H" //specific line encoding using ANSI sequences

void flushFramebufferAndWaitForVBlank();
void waitForInput();
u32 getInput();