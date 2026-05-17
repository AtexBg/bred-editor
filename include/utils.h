#include <3ds.h>
#include <stdbool.h>

#define LINE(x) "\x1b[" #x ";1H" //specific line encoding using ANSI sequences

//bitwise operands macros (copied from stackoverflow.com lol)
#define GET_BIT(value, bit)   (((value) >> (bit)) & 1) // returns bit n°"bit" from "value" 
#define SET_BIT(value, bit)   ((value) |=  (1 << (bit))) //set bit "bit" of "value" to 1
#define CLEAR_BIT(value, bit) ((value) &= ~(1 << (bit))) //set bit "bit" of "value" to 0
#define TOGGLE_BIT(value ,bit) ((value) ^=  (1 << (bit))) //toggle bit "bit" from "value" byte

void flushFramebufferAndWaitForVBlank();
void waitForInput();
u32 getInput();
u32 getHeldKeys();
uint16_t swap16(uint16_t value);
void clearTopFramebuffer();