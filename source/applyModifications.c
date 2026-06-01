#include <stdio.h>
#include <unistd.h> //open/close
#include <stdlib.h>
#include <3ds.h>
#include <string.h> //strlen

#include "utils.h"
#include "load.h"
#include "addreses.h"
#include "checksum.h"
#include "text.h"
#include "pokemon.h"
#include "items.h"

void applyAndWriteSaveFile(save_t *save, char* saveFilePath, unsigned char *savefileBuffer, pokemon_t *party, item_t *bag, int isLoadingGbEmulator)<%
    consoleClear();
    printf(LINE(1) "Opening file \x1b[33m%s\x1b[0m", saveFilePath);
    flushFramebufferAndWaitForVBlank();
    FILE *savefile = fopen(saveFilePath, "wb"); //open file
    
    printf(LINE(2) "Writing data to buffer...");
    flushFramebufferAndWaitForVBlank();

    for(int i=0; i<3; i++)<%
        savefileBuffer[ADDR_MONEY + i] = save->money[i];
    %>
    
    savefileBuffer[ADDR_PLAYTIME_HOURS] = save->playtime.hours;
    savefileBuffer[ADDR_PLAYTIME_MINS] = save->playtime.mins;
    savefileBuffer[ADDR_PLAYTIME_SECS] = save->playtime.secs;

    //casting strlen arguments to const char* to avoid "differ in signedness" warnings
    for(int c=0; c<strlen((const char*)save->name.player); c++)<% //funny how "c++" is valid C syntax
        savefileBuffer[ADDR_PLAYER_NAME + c] = char2gbchar(save->name.player[c]);
    %>
    for(int c=0; c<strlen((const char*)save->name.rival); c++)<%
        savefileBuffer[ADDR_RIVAL_NAME + c] = char2gbchar(save->name.rival[c]);
    %>

    //Adding string-terminator on names (0x50, equivalent of '\0' on pkmn)
    savefileBuffer[ADDR_PLAYER_NAME + strlen((const char*)save->name.player)] = 0x50;
    savefileBuffer[ADDR_RIVAL_NAME + strlen((const char*)save->name.rival)] = 0x50;

    savefileBuffer[ADDR_BADGES] = save->badges;

    //temporarely removed because it corrupted the save for some reason 
    // //apply modified pokemons
    // for(int i=0; i<savefileBuffer[PARTY_POKE_COUNT_ADDRESS]; i++)<%
    //     memcpy(&savefileBuffer[BASE_PARTY_POKE_ADDRESS+POKE_STRUCT_SIZE*i], &party[i], POKE_STRUCT_SIZE);
    // %>

    //apply modified bag items
    for(int i=0; i<save->bagItemsCount; i++){
        memcpy(&savefileBuffer[BASE_BAG_ADDRESS+0x02*i], &bag[i], 0x02); //here i spent 30min trying to find why the items won't save, just to realize that BASE_BAG_ADDRESS was empty, so the items were written to 0x0000 lol
    }


    //TODO: add more data here
    
    printf(LINE(3) "Fixing save file checksum...");
    flushFramebufferAndWaitForVBlank();
    uint8_t checksum = calculateChecksum(savefileBuffer);
    printf(LINE(4) "New checksum is \x1b[36m0x%02X\x1b[0m", checksum);
    savefileBuffer[ADDR_CHECKSUM] = checksum;
    save->checksum = checksum; //update it for further use

    printf(LINE(5) "Flushing buffer into file \x1b[33m%s\x1b[0m...", saveFilePath);
    flushFramebufferAndWaitForVBlank();
    fwrite(savefileBuffer, 1, 0x8000, savefile);
    fclose(savefile);

    save->hasUnsavedChanges = false;
    if(!isLoadingGbEmulator)<%
        printf(LINE(6) "Done! Press any key to continue...");
        flushFramebufferAndWaitForVBlank();
        waitForInput();
    %>
    consoleClear();
%>