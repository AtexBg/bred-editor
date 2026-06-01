#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "addreses.h"
#include "text.h"
#include "load.h"
#include "checksum.h"

//reminder : "save_t save;" is the buffer we modify to edit the same,
//and "unsigned char saveFile;" is the data from the .sav file we open
//and then don't touch again.

void loadValues(unsigned char *saveFile, save_t *save) {

    //load player name
    for(int i=0; i < 11; i++){ //names are stored as 11 allocated bytes in the save, although the game's interface only allows for 7 chars
        if(saveFile[ADDR_PLAYER_NAME + i]==0x50){
            save->name.player[i] = '\0';
            break;
        }
        save->name.player[i] = gbchar2char(saveFile[ADDR_PLAYER_NAME + i]);
    }
    save->name.player[10] = '\0'; //add '\0' at the end to printf can use it

    //load rival name
    for(int i=0; i < 11; i++){
        if(saveFile[ADDR_RIVAL_NAME + i]==0x50){
            save->name.rival[i] = '\0';
            break;
        }
        save->name.rival[i] = gbchar2char(saveFile[ADDR_RIVAL_NAME + i]);
    }
    save->name.rival[10] = '\0';

    for(int i=0; i < 3; i++){
        save->money[i] = saveFile[ADDR_MONEY+i];
    }
    
    save->badges = saveFile[ADDR_BADGES]; //badges is a single byte, every bit is a separate badge
    save->playerID = (saveFile[ADDR_PLAYER_ID]*0xFF) + saveFile[ADDR_PLAYER_ID+1];
    
    save->playtime.hours = saveFile[ADDR_PLAYTIME_HOURS];
    save->playtime.mins = saveFile[ADDR_PLAYTIME_MINS];
    save->playtime.secs = saveFile[ADDR_PLAYTIME_SECS];

    save->pokemonCount = saveFile[PARTY_POKE_COUNT_ADDRESS];

    save->bagItemsCount = saveFile[BAG_ITEMS_COUNT];

    save->checksum = calculateChecksum(saveFile);
    save->hasUnsavedChanges = false;
}

int money2int(uint8_t money[3]) {
    //To convert money array to int because money is stored as BCD
    //inside the save file (ex: 15000P$ = 0x015000 in memory)
    return ((money[0] >> 4)*10 + (money[0] & 0xF))*10000
         + ((money[1] >> 4)*10 + (money[1] & 0xF))*100
         + ((money[2] >> 4)*10 + (money[2] & 0xF));
}

//10 -> 1
//123456 -> 123401
void int2money(int integer, int *money) {
    for (int i=2; i>=0; i--) {
        int lowNibble = integer % 10; integer /= 10;
        int highNibble = integer % 10; integer /= 10;

        money[i] = (highNibble << 4) | lowNibble;
    }

    // //temp test
    // printf(LINE(5) "DEBUG: %02X %02X %02X\n", money[0], money[1], money[2]);
    // flushFramebufferAndWaitForVBlank();
    // waitForInput();
}

void restoreBackedUpSaveFile(){
    FILE *backup_sav = fopen("sdmc:/3ds/bedit/BACKUP.sav", "rb"); //open file
    if(!backup_sav){
        printf(LINE(1) "\x1b[31mNo Backup to restore...");
        printf(LINE(2) "Nothing was done.");
        waitForInput();
        consoleClear();
        return;
    }

    //load save into buffer
    unsigned char *savefileBuffer = malloc(0x8000); //savefilebuffer will be loaded with contents of file at saveFilePath
    fread(savefileBuffer, 1, 0x8000, backup_sav); //copy file contents to buffer
    fclose(backup_sav);

    FILE *save_sav = fopen("sdmc:/3ds/bedit/SAVE.sav", "wb"); //open file to write to

    fwrite(savefileBuffer, 1, 0x8000, save_sav);
    fclose(save_sav);
}