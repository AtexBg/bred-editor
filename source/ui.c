#include <stdio.h>
#include <3ds.h>
#include <stdlib.h> //atoi
#include <inttypes.h> //used for displaying uint16_t in printf
#include <string.h> //strlen

#include "utils.h"
#include "load.h"
#include "checksum.h"

#define LINE(x) "\x1b[" #x ";1H" //specific line encoding using ANSI sequences

int digitPositionsLUT[] = {1, 2, 3, 5, 6, 8, 9};
int digits[] = {0, 0, 0, 0, 0, 0, 0};
char tempTextBuffer[0xFF];

void UI_changePlayTime(save_t *save){
    consoleClear();
    int csd = 0; //CSD = CurrentlySelectedDigit

    while(1){
        u32 keys = getInput();

        if(keys & KEY_LEFT){csd--;}
        if(keys & KEY_RIGHT){csd++;}
        if(keys & KEY_UP){digits[csd]++;}
        if(keys & KEY_DOWN){digits[csd]--;}

        if(keys & KEY_B){return;} //quit func is B is pressed

        if(digits[csd] == 10){digits[csd] = 0;}
        if(digits[csd] == -1){digits[csd] = 9;}

        if(digits[3] >= 6){digits[3] = 0;} //minutes can't go higher than 59
        if(digits[5] >= 6){digits[5] = 0;} //same for seconds

        if(csd == -1){csd = 0;} //avoid OOB access
        if(csd == 7){csd = 6;}

        if(keys & KEY_A){break;} //apply

        //printf("\x1b[1;%dH\x1b[32m          V    \x1b[0m", digitPositionsLUT[csd]);
        printf("\x1b[2;1HPLAYTIME: %d%d%d:%d%d:%d%d", digits[0], digits[1], digits[2], digits[3], digits[4], digits[5], digits[6]);
        printf("\x1b[3;%dH\x1b[32m          ^    \x1b[0m", digitPositionsLUT[csd]);
        
        printf("\x1b[2;%dH\x1b[32m%d\x1b[0m", digitPositionsLUT[csd]+10, digits[csd]);
        printf(LINE(29) "--------------------------------------------------");
        printf(LINE(30) "\x1b[33mUP\x1b[0m/\x1b[33mDOWN\x1b[0m : Change Value   |  \x1b[33mB\x1b[0m : Quit  |  \x1b[33mA\x1b[0m : Apply");
        flushFramebufferAndWaitForVBlank();
    }

    //In the game, the hours counter is 8-bit, so more
    //than 255 hours would cause an interger overflow
    int enteredHours = (digits[0]*100 + digits[1]*10 + digits[2]);
    if(enteredHours > 255){
        save->playtime.hours = 255;
    }else{
        save->playtime.hours = enteredHours;
    }

    save->playtime.mins = (digits[3]*10 + digits[4]);
    save->playtime.secs = (digits[5]*10 + digits[6]);
}

void UI_showSaveInformation(save_t *save){
    consoleClear();
    //update checksum (removed because useless : savefileBuffer isn't modified yet while save struct is)
    //save->checksum = calculateChecksum(savefileBuffer);

    printf(LINE(2) "\x1b[36mPlayer Name\x1b[0m : %s", save->name.player);
    printf(LINE(3) "\x1b[36mRival Name\x1b[0m : %s", save->name.rival);
    printf(LINE(4) "\x1b[36mPlaytime\x1b[0m : %03d:%02d:%02d", save->playtime.hours, save->playtime.mins, save->playtime.secs);
    printf(LINE(5) "\x1b[36mMoney\x1b[0m : %dP$", money2int(save->money));
    printf(LINE(6) "\x1b[36mPlayer ID\x1b[0m : %05u", save->playerID);

    //note : initial checksum is the checksupm calculated when the save it loaded,
    //i can't recalculate it each time unless i flush the entire struct to the buffer
    printf(LINE(25) "\x1b[36mInitial Checksum\x1b[0m : 0x%02X", save->checksum);

    flushFramebufferAndWaitForVBlank();
    waitForInput();
}

void UI_changeMoneyAmount(save_t *save){
    consoleClear();
    static char swkbdTextBuffer[60];
    int moneyAmountToApply[3];
    static SwkbdState swkbd;

    printf(LINE(2) "Please enter new money amount :");
    flushFramebufferAndWaitForVBlank();

    //software keyboard (swkbd) stuff
    swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 1, 6);
	swkbdSetPasswordMode(&swkbd, SWKBD_PASSWORD_HIDE_DELAY);
	swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
	swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
	swkbdSetNumpadKeys(&swkbd, L'0', L'0');
    swkbdInputText(&swkbd, swkbdTextBuffer, sizeof(swkbdTextBuffer));
    printf(LINE(2) "New Money Amount is %s. Press any key to continue...", swkbdTextBuffer);
    flushFramebufferAndWaitForVBlank();
    waitForInput();

    //apply value to buffer
    int2money(atoi(swkbdTextBuffer),  moneyAmountToApply);
    for(int i=0; i<3; i++){
        save->money[i] = moneyAmountToApply[i];
    }
}

void UI_changePlayerOrRivalName(save_t *save){ 
    consoleClear();
    printf(LINE(1) "Change player of rival name?");
    int csd = 0;
    char* entries[] = {"Player", "Rival"};
    char playerOrRival[7];

    while(true){
        u32 keys = getInput();
        if(keys & KEY_DOWN){csd++;}
        if(keys & KEY_UP){csd--;}
        if(csd<0){csd=1;}
        if(csd>1){csd=0;}
        
        if(keys & KEY_A){
            strcpy(playerOrRival, entries[csd]);
            break;
        }

        for(int i=0; i < 2; i++){
            if(csd == i){
                printf("\x1b[%d;1H \x1b[32m", i+2);
            }
            printf("\x1b[%d;2H%s\x1b[0m", i+2, entries[i]);
        }
        flushFramebufferAndWaitForVBlank();

        
    }

    char enteredName[16];
    static SwkbdState swkbd;
    consoleClear();
    UI_changePlayerOrRivalName_startlabel:

    printf(LINE(1) "Enter new %s name :", playerOrRival);
    flushFramebufferAndWaitForVBlank();

    //init that software keyboard mess 
    swkbdInit(&swkbd, SWKBD_TYPE_WESTERN, 1, -1);
	swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, SWKBD_FILTER_DIGITS | SWKBD_FILTER_AT | SWKBD_FILTER_PERCENT | SWKBD_FILTER_BACKSLASH | SWKBD_FILTER_PROFANITY, 2);
	swkbdSetFeatures(&swkbd, SWKBD_MULTILINE);
    snprintf(tempTextBuffer, sizeof(tempTextBuffer), "Enter new %s name (7 char  max)", playerOrRival);
	swkbdSetHintText(&swkbd, tempTextBuffer);
	swkbdInputText(&swkbd, enteredName, sizeof(enteredName));

    if(strlen(enteredName) > 7){
        consoleClear();
        printf(LINE(2) "Name is too long, choose one with less than 7 characters.");
        printf(LINE(3) "Press any key to continue...");
        flushFramebufferAndWaitForVBlank();
        waitForInput();
        consoleClear();
        goto UI_changePlayerOrRivalName_startlabel;
    }

    printf(LINE(2) "Will apply new name %s for the %s.", enteredName, playerOrRival);
    printf(LINE(3) "Press any key to continue...");
    waitForInput();

    if(!strcmp(playerOrRival, "Player")){ //strcmp returns 0, so i invert the result 
        for(int c=0; c<strlen(enteredName); c++){ //it's funny how "c++" is basically a valid C instruction
            strncpy((char*)save->name.player, enteredName, 11);
        }

    }else{
        for(int c=0; c<strlen(enteredName); c++){
            strncpy((char*)save->name.rival, enteredName, 11);
        }

    }

    consoleClear();
}