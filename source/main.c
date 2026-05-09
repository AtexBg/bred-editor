#include <3ds.h>
#include <stdio.h>
#include <stdlib.h> //malloc

#include "utils.h" //flushFramebufferAndWaitForVBlank();, waitForInput();
#include "ui.h" //UI_changePlayTime();
#include "applyModifications.h"
#include "pokemon.h"

#define APP_VERSION "b0.2.0"
#define SAVE_SIZE 0x8000

char* menuEntry[] = {"Display Save Information", "Change Playtime", "Change Player/Rival Names", "Change Money Amount", "Read Party Pokemon Stats", "Change Badges"}; //to fill with more options
int entriesAmount = sizeof(menuEntry) / sizeof(menuEntry[0]);
int currentSelectedOption = 0;
char* saveFilePath = "sdmc:/POKEMON.sav";

int main(){
    //init
    romfsInit();
	gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    // consoleInit(GFX_BTM, NULL);

    FILE *savefile = fopen(saveFilePath, "rb"); //open file
    if(!savefile){
        printf("\x1b[31mERROR: Failed to open file %s.\n", saveFilePath);
        waitForInput();
        return 69;
    }

    //load save into buffer
    unsigned char *savefileBuffer = malloc(SAVE_SIZE); //savefile will be loaded with contents of file at saveFilePath
    fread(savefileBuffer, 1, SAVE_SIZE, savefile); //copy file contents to buffer
    fclose(savefile);

    save_t save = {0};
    loadValues(savefileBuffer, &save);
	while (aptMainLoop()){
        u32 keys = getInput();

        printf(LINE(1) "BRED-EDITOR by AtexBg             VERSION: %s", APP_VERSION);
        
        if(keys & KEY_DOWN){currentSelectedOption++;}
        if(keys & KEY_UP){currentSelectedOption--;}
        
        if(currentSelectedOption == entriesAmount){currentSelectedOption=0;} //block currentSelectedOption to go further than the amount of menu entries
        if(currentSelectedOption == -1){currentSelectedOption=entriesAmount-1;}

        if(keys & KEY_A){
            switch(currentSelectedOption){
                case 0: UI_showSaveInformation(&save);; break; //todo: add more options 
                case 1: UI_changePlayTime(&save); break;
                case 2: UI_changePlayerOrRivalName(&save); break;
                case 3: UI_changeMoneyAmount(&save); break;
                case 4: loadPartyPokemonDataToStruct(savefileBuffer); break;
                case 5: UI_changeBadges(&save); break;
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

        printf(LINE(22) "                        /----------------------\\");
        printf(LINE(23) "                        |\x1b[33mUP\x1b[0m/\x1b[33mDOWN\x1b[0m : Navigate    |");
        printf(LINE(24) "                        |\x1b[33mA\x1b[0m : Select option     |");
        printf(LINE(25) "                        |\x1b[33mL\x1b[0m+\x1b[33mR\x1b[0m : Quit BRED-EDITOR|");
        printf(LINE(26) "                        |\x1b[33mSELECT\x1b[0m: \x1b[31mAPPLY CHANGES\x1b[0m |");
        printf(LINE(27) "                        \\----------------------/");
        printf(LINE(29) "--------------------------------------------------");
        printf(LINE(30) "\x1b[33mhttps://github.com/AtexBg/bred-editor @ May 2026\x1b[0m");

		if((keys & KEY_L) && (keys & KEY_R)){break;} //quit if L and R are pressed
        
        if(keys & KEY_SELECT){
            applyAndWriteSaveFile(&save, saveFilePath, savefileBuffer);
        }

        flushFramebufferAndWaitForVBlank();
	}

    //exit
    romfsExit();
	gfxExit();
	return 0;
}
