#include <stdio.h>
#include <3ds.h>
#include <string.h>

#include "utils.h"
#include "items.h"
#include "load.h"

const char* itemNamesList[] = {"Corrupted", "Master Ball", "Ultra Ball", "Great Ball", "Poke Ball", "Town Map", "Bicycle", "Corrupted", "Safari Ball", "Pokedex", "Moon Stone", "Antidote", "Burn Heal", "Ice Heal", "Awakening", "Parlyz Heal", "Full Restore", "Max Potion", "Hyper Potion", "Super Potion", "Potion", "BoulderBadge", "CascadeBadge", "ThunderBadge", "RainbowBadge", "SoulBadge", "MarshBadge", "VolcanoBadge", "EarthBadge", "Escape Rope", "Repel", "Old Amber", "Fire Stone", "Thunderstone", "Water Stone", "HP Up", "Protein", "Iron", "Carbos", "Calcium", "Rare Candy", "Dome Fossil", "Helix Fossil", "Secret Key", "Corrupted", "Bike Voucher", "X Accuracy", "Leaf Stone", "Card Key", "Nugget", "PP Up*", "Poke Doll", "Full Heal", "Revive", "Max Revive", "Guard Spec.", "Super Repel", "Max Repel", "Dire Hit", "Coin", "Fresh Water", "Soda Pop", "Lemonade", "S.S. Ticket", "Gold Teeth", "X Attack", "X Defend", "X Speed", "X Special", "Coin Case", "Oak's Parcel", "Itemfinder", "Silph Scope", "Poke Flute", "Lift Key", "Exp. All", "Old Rod", "Good Rod", "Super Rod", "PP Up", "Ether", "Max Ether", "Elixer", "Max Elixer","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted","Corrupted", "Corrupted","Corrupted","Corrupted","Corrupted", "HM01", "HM02", "HM03", "HM04", "HM05", "TM01", "TM02", "TM03", "TM04", "TM05", "TM06", "TM07", "TM08", "TM09", "TM10", "TM11", "TM12", "TM13", "TM14", "TM15", "TM16", "TM17", "TM18", "TM19", "TM20", "TM21", "TM22", "TM23", "TM24", "TM25", "TM26", "TM27", "TM28", "TM29", "TM30", "TM31", "TM32", "TM33", "TM34", "TM35", "TM36", "TM37", "TM38", "TM39", "TM40", "TM41", "TM42", "TM43", "TM44", "TM45", "TM46", "TM47", "TM48", "TM49", "TM50", "TM51", "TM52", "TM53", "TM54", "TM55"};

void editBagItems(unsigned char* savebuffer, item_t *bag, save_t *save){
    consoleClear();

    /*accroding to "https://bulbapedia.bulbagarden.net/wiki/Save_data_structure_(Generation_I)#bank1_main_bag", 
    the bags items are stored as 2 bytes, one for type and one for quantity, the amount of items in the bag
    is stored at 0x25C9 and the items structs starts at the next byte (0x25CA)*/
    int csi=0; //csi=currentlySelectedItem
    int tempValue=0;
    bool inEditingMode = false;
    int itemCount = save->bagItemsCount;
    static bool bagDataDownloaded=false;
    
    if(!bagDataDownloaded){
        for(int i=0; i<itemCount; i++){
            //copy data from savefile memory buffer directly into struct (struct is already properly padded/structured) 
            memcpy(&bag[i], savebuffer + 0x25CA+(2*i), sizeof(item_t));
            bagDataDownloaded=1;
        }
    }

    while(true){
        u32 keys = getInput();
        if(csi<0){csi=0;}
        if(csi>itemCount-1){csi=itemCount-1;}

        printf("\x1b[1;1H/--------------------\\\x1b[%d;1H\\--------------------/\x1b[2;1H", itemCount+2); //that mess display borders
        for(int i=0; i<itemCount; i++){
            if(i==csi){
                printf("|\x1b[32m >%-13s x%02d \x1b[0m|\n", itemNamesList[bag[i].itemID],  (bag[i].amount==0) ? 1 : bag[i].amount);
            }else{
                printf("|  %-13s x%02d |\x1b[0m\n", itemNamesList[bag[i].itemID], (bag[i].amount==0) ? 1 : bag[i].amount);
            }
        }

        printf(LINE(29) "--------------------------------------------------");
        if(inEditingMode){
            printf(LINE(30) " \x1b[33mLEFT\x1b[0m/\x1b[33mRIGHT\x1b[0m: -10/+10 | \x1b[33mUP\x1b[0m/\x1b[33mDOWN\x1b[0m: +1/-1 | \x1b[33mA\x1b[0m: Confirm");
            tempValue = bag[csi].amount;
            while(true){
                u32 keys = getInput();

                if(keys & KEY_UP){tempValue++;}
                if(keys & KEY_DOWN){tempValue--;}
                if(keys & KEY_LEFT){tempValue-=10;}
                if(keys & KEY_RIGHT){tempValue+=10;}

                if(tempValue<0){tempValue=0;}
                if(tempValue>99){tempValue=99;}

                printf("\x1b[33m\x1b[%d;19H%02d\x1b[0m", csi+2, tempValue); //display value being modified
                if(keys & KEY_A){
                    inEditingMode=false;
                    save->hasUnsavedChanges = true;
                    bag[csi].amount = tempValue;
                    consoleClear();
                    break;
                }
                flushFramebufferAndWaitForVBlank();
            }
        }else{
            if(keys & KEY_DOWN){csi++;}
            if(keys & KEY_UP){csi--;}
            if(keys & KEY_A){inEditingMode=true;}

            if(keys & KEY_B){ //quitting function
                save->bagItemsCount = itemCount;
                consoleClear();
                return;
            }
            printf(LINE(30) " \x1b[33mA\x1b[0m : Change Quantity  |  \x1b[33mB\x1b[0m : Quit \x1b[2;1H");
        }
        flushFramebufferAndWaitForVBlank();
    }
}