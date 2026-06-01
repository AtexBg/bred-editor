//why are you looking in there?
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h>

#include "utils.h" //flushFramebufferAndWaitForVBlank();, waitForInput();
#include "ui.h" //UI_changePlayTime();
#include "applyModifications.h"
#include "pokemon.h"
#include "render.h"
#include "gameboy_emu.h"
#include "items.h"

#define APP_VERSION "b0.4.0"
#define SAVE_SIZE 0x8000

char* menuEntry[] = {"Display Save Information", "Change Playtime", "Change Player/Rival Names", "Change Money Amount", "Read Party Pokemon Stats", "Change Badges", "Bag Items Editor", "Launch Save on Emulator"}; //to fill with more options
int entriesAmount = sizeof(menuEntry) / sizeof(menuEntry[0]);
int currentSelectedOption = 0;
char* saveFilePath = "sdmc:/3ds/bedit/SAVE.sav";
char* backupsavepath = "sdmc:/3ds/bedit/BACKUP.sav";

int main(){
    //init
    romfsInit();
	gfxInitDefault();

    //set fb format to RGB565 so the display funcs will work properly
    gfxSetScreenFormat(GFX_TOP, GSP_RGB565_OES);
    gfxSetScreenFormat(GFX_BOTTOM, GSP_RGB565_OES);
    
    consoleInit(GFX_TOP, NULL);

    FILE *savefile = fopen(saveFilePath, "rb"); //open file
    if(!savefile){
        printf(LINE(1) "\x1b[31mFailed to open file %s.\n", saveFilePath);
        printf(LINE(2) "\x1b[0mIt probably just doesn't exist...\n");
        waitForInput();
        return 42;
    }

    //load save into buffer
    unsigned char *savefileBuffer = malloc(SAVE_SIZE); //savefile will be loaded with contents of file at saveFilePath
    fread(savefileBuffer, 1, SAVE_SIZE, savefile); //copy file contents to buffer
    fclose(savefile);

    //backup the sav file
    FILE *save_sav = fopen(backupsavepath, "wb"); //open file to write to
    fwrite(savefileBuffer, 1, 0x8000, save_sav);
    fclose(save_sav);

    save_t save = {0};
    loadValues(savefileBuffer, &save);

    pokemon_t party[save.pokemonCount];
    item_t bag[save.bagItemsCount];

	while (aptMainLoop()){
        u32 keys = getInput();

        printf(LINE(1) "BRED-EDITOR by AtexBg             VERSION: %s", APP_VERSION);
        
        if(keys & KEY_DOWN){currentSelectedOption++;}
        if(keys & KEY_UP){currentSelectedOption--;}
        
        if(currentSelectedOption == entriesAmount){currentSelectedOption=0;} //block currentSelectedOption to go further than the amount of menu entries
        if(currentSelectedOption == -1){currentSelectedOption=entriesAmount-1;}

        if(keys & KEY_A){
            switch(currentSelectedOption){
                case 0: UI_showSaveInformation(&save); break; //todo: add more options 
                case 1: UI_changePlayTime(&save); break;
                case 2: UI_changePlayerOrRivalName(&save); break;
                case 3: UI_changeMoneyAmount(&save); break;
                case 4: loadPartyPokemonDataToStruct(savefileBuffer, &save, party); break;
                case 5: UI_changeBadges(&save); break;
                case 6: editBagItems(savefileBuffer, bag, &save); break;
                case 7: applyAndWriteSaveFile(&save, saveFilePath, savefileBuffer, party, bag, true); runEmulator(); break;
            }
            consoleClear();
        }

        //display options
        for(int i=0; i < entriesAmount; i++){
            if(currentSelectedOption == i){
                printf("\x1b[%d;1H \x1b[32m", i+4);
            }
            printf("\x1b[%d;2H%s\x1b[0m", i+4, menuEntry[i]);
        }

        //some ANSI mess, i like that
        printf(LINE(22) "                        /----------------------\\");
        printf(LINE(23) "                        |\x1b[33mUP\x1b[0m/\x1b[33mDOWN\x1b[0m : Navigate    |");
        printf(LINE(24) "                        |\x1b[33mA\x1b[0m : Select option     |");
        printf(LINE(25) "                        |\x1b[33mL\x1b[0m+\x1b[33mR\x1b[0m : Quit BRED-EDITOR|");
        printf(LINE(26) "                        |\x1b[33mSELECT\x1b[0m: \x1b[31mAPPLY CHANGES\x1b[0m |");
        printf(LINE(27) "                        \\----------------------/");
        if(save.hasUnsavedChanges){printf(LINE(28) "\x1b[31mUnsaved Changes Detected, press SELECT before quit\x1b[0m");}
        printf(LINE(29) "--------------------------------------------------");
        printf(LINE(30) "\x1b[33mhttps://github.com/AtexBg/bred-editor @ May 2026\x1b[0m");

		if((keys & KEY_L) && (keys & KEY_R)){break;} //quit if L and R are pressed
        
        if((keys & KEY_X) && (keys & KEY_Y)){
            restoreBackedUpSaveFile();
            consoleClear();
            printf(LINE(1) "Succesfully restored backup, program will quit.");
            printf(LINE(2) "Press any key to continue...");
            waitForInput();
            break;
        }

        if(keys & KEY_SELECT){
            applyAndWriteSaveFile(&save, saveFilePath, savefileBuffer, party, bag, false);
        }

        flushFramebufferAndWaitForVBlank();
	}

    //exit
    romfsExit();
	gfxExit();
	return 0;
}
