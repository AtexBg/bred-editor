/* *****************
 * GameBoy emulator written in C.
 * Original code by Greg Tourville
 * gameboy-c by Deltabeard
 * (3DS port by AtexBg)
 ********************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <3ds.h>
#include <string.h> //mempcy() and strcmp

#include "gameboy_core.h"
#include "utils.h"
#include "fps.h"

// emulator data
int running = 1;
u8   frameskip = 0;
static u32 fb[LCD_HEIGHT][LCD_WIDTH]; //gameboy framebuffer
int frame = 0;
int currentFps;
u32 COLORS_Y[4] = {0xFFFFFFFF, 0x99999999, 0x44444444, 0x00000000};
u32* color_map;

uint8_t colorPalette_BW[4][3] = {
	{0x00, 0x00, 0x00},
	{0x40, 0x40, 0x40},
	{0xA0, 0xA0, 0xA0},
	{0xFF, 0xFF, 0XFF},
};

uint8_t colorPalette_RED[4][3] = {
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0xFF},
	{0x63, 0xA5, 0xFF},
	{0xFF, 0xFF, 0XFF},
};

uint8_t colorPalette_BLUE[4][3] = {
	{0x00, 0x00, 0x00},
	{0x94, 0x3A, 0x3A},
	{0xFF, 0x84, 0x84},
	{0xFF, 0xFF, 0XFF},
};

// key mappings
#define NUM_KEYS    8

// strings
char  window_caption[100];
char  window_caption_fps[100];
//char  rom_file_buf[260];
//char* rom_file = rom_file_buf;
char  save_file[260] = "sdmc:/3ds/bedit/SAVE.sav";

// pointers
u8*   rom;
u32   rom_size;
u8*   save = NULL;
u32   save_size = 0;
FILE* rom_f;
FILE* save_f;


void runEmulator(){
	consoleClear();
    consoleInit(GFX_BOTTOM, NULL);
	//starting from here, only the bottom console should be used

	int     i, x, y;
	// u32		fb[LCD_HEIGHT][LCD_WIDTH];
	char	*rom_file = "sdmc:/3ds/bedit/ROM.gb"; //hardcoded rom file, replaces original argv
	
	// Load ROM file
	rom_f = fopen(rom_file, "rb");
	if(!rom_f){
		printf(LINE(1) "File %s not found.", rom_file);
		printf(LINE(3) "Please put a Pokemon Red/Blue ROM");
		printf(LINE(4) "at \"SD:/3ds/bedit/ROM.gb\".");

		printf(LINE(6) "Press any key to quit");
		flushFramebufferAndWaitForVBlank();
		waitForInput();
		consoleClear();
		consoleInit(GFX_TOP, NULL);
		return;
	}

	fseek(rom_f, 0, SEEK_END);
	rom_size = ftell(rom_f);
	rewind(rom_f);
	rom = (u8*)malloc(rom_size);
	for (i = 0; i < rom_size; i++)
		rom[i] = 0xFF; //fill rom buffer with 0xFF bytes

	fread(rom, sizeof(u8), rom_size, rom_f); //copy file data to rom buffer
	fclose(rom_f);
	
	//fetch game name from 0x13C and check it to apply a proper color plaette
	char gameName[5];
	for(int i=0; i<5; i++){
		gameName[i] = rom[0x013C+i];
	}

	uint8_t (*palette)[3] = colorPalette_BW;
	if(!strcmp(gameName, "RED")){
		palette = colorPalette_RED;
	}
	if(!strcmp(gameName, "BLUE")){
		palette = colorPalette_BLUE;
	}

	save_size = GetSaveSize(rom);
	save = (u8*)malloc(save_size);
	save_f = fopen(save_file, "rb");
	
	if (save_f){
		fseek(save_f, 0, SEEK_SET);
		fread(save, sizeof(u8), save_size, save_f);
		fclose(save_f);
	}

	// Start the emulator
	LoadROM(rom, rom_size, save, save_size);

	color_map = COLORS_Y;
	frame=0;
	while(1){
		u32 keys = getHeldKeys();
		u8* fb8 = gfxGetFramebuffer(GFX_TOP, GFX_RIGHT, NULL, NULL);
		u16* framebuffer = (u16*)fb8; //casting fb8 to u16* cuz buffer is RGB565 when console is init'ed
		if((keys & KEY_L) && (keys & KEY_R)){break;}

		for (int k = 0; k < NUM_KEYS; k++) {
    		KeyRelease(k);
		}

		if (keys & KEY_RIGHT)  KeyPress(0);
		if (keys & KEY_LEFT)   KeyPress(1);
		if (keys & KEY_UP)     KeyPress(2);
		if (keys & KEY_DOWN)   KeyPress(3);
		if (keys & KEY_A)      KeyPress(4); 
		if (keys & KEY_B)      KeyPress(5); 
		if (keys & KEY_SELECT) KeyPress(6); 
		if (keys & KEY_START)  KeyPress(7);

		currentFps = getCurrentFPS();
		RunFrame(); 
		if(keys & KEY_L){
			RunFrame();
		}

		for (y = 0; y < LCD_HEIGHT; y++){
			for (x = 0; x < LCD_WIDTH; x++){
				fb[y][x] = color_map[gb_fb[y][x] & 3];
			}
		}
		for (int y = 0; y < LCD_HEIGHT; y++){
			for (int x = 0; x < LCD_WIDTH; x++)
			{
				uint8_t v = fb[y][x] & 3;

				uint8_t r = palette[v][0];
				uint8_t g = palette[v][1];
				uint8_t b = palette[v][2];

				framebuffer[LCD_HEIGHT-y+48 + (x+120)*240] =
					((r >> 3)) |
					((g >> 2) << 5) |
					((b >> 3) << 11);
			}
		}
		
		frame++;
		printf(LINE(1) "\x1b[33mL\x1b[0m+\x1b[33mR\x1b[0m : Shutdown Emulated GameBoy");
		printf(LINE(2) "----------------------------------------");

		printf(LINE(3) "Frame %d", frame);
		printf(LINE(4) "FPS: %d", currentFps);

		printf(LINE(29) "ROM File: %s", rom_file);
		printf(LINE(30) "Save file: %s", save_file);

		gspWaitForVBlank();
	}

	// Save game before exit
	if (save_size){
		save_f = fopen(save_file, "wb");
		if (save_f)
		{
			fseek(save_f, 0, SEEK_SET);
			fwrite(save, 1, save_size, save_f);
			fclose(save_f);
		}
	}

	//cleanup
	free(rom);
	free(save);
	consoleClear();

	//go back to top console before returning
	consoleInit(GFX_TOP, NULL);
}
