#include <stdio.h>
#include <string.h> //memcpy
#include <3ds.h>

#include "utils.h"
#include "pokemon.h"
#include "load.h"

#define BASE_PARTY_POKE_ADDRESS 0x2F34
#define PARTY_POKE_COUNT_ADDRESS 0x2F2C
#define POKE_STRUCT_SIZE 0x2C

//Thanks to `names.asm` from the Pokémon Red decompilation for the names and ID lists
int   normalPokedexIDs[] = {0, 112, 115, 32, 35, 21, 100, 34, 80, 2, 103, 108, 102, 88, 94, 29, 31, 104, 111, 131, 59, 151, 130, 90, 72, 92, 123, 120, 9, 127, 114, 0, 0, 58, 95, 22, 16, 79, 64, 75, 113, 67, 122, 106, 107, 24, 47, 54, 96, 76, 0, 126, 0, 125, 82, 109, 0, 56, 86, 50, 128, 0, 0, 0, 83, 48, 149, 0, 0, 0, 84, 60, 124, 146, 144, 145, 132, 52, 98, 0, 0, 0, 37, 38, 25, 26, 0, 0, 147, 148, 140, 141, 116, 117, 0, 0, 27, 28, 138, 139, 39, 40, 133, 136, 135, 134, 66, 41, 23, 46, 61, 62, 13, 14, 15, 0, 85, 57, 51, 49, 87, 0, 0, 10, 11, 12, 68, 0, 55, 97, 42, 150, 143, 129, 0, 0, 89, 0, 99, 91, 0, 101, 36, 110, 53, 105, 0, 93, 63, 65, 17, 18, 121, 1, 3, 73, 0, 118, 119, 0, 0, 0, 0, 77, 78, 19, 20, 33, 30, 74, 137, 142, 0, 81, 0, 0, 4, 7, 5, 8, 6, 0, 0, 0, 0, 43, 44, 45, 69, 70, 71};
char* pokemonNamesList[] = {"MISSINGNO.", "RHYDON", "KANGASKHAN", "NIDORAN♂", "CLEFAIRY", "SPEAROW", "VOLTORB", "NIDOKING", "SLOWBRO", "IVYSAUR", "EXEGGUTOR", "LICKITUNG", "EXEGGCUTE", "GRIMER", "GENGAR", "NIDORAN♀", "NIDOQUEEN", "CUBONE", "RHYHORN", "LAPRAS", "ARCANINE", "MEW", "GYARADOS", "SHELLDER", "TENTACOOL", "GASTLY", "SCYTHER", "STARYU", "BLASTOISE", "PINSIR", "TANGELA", "MISSINGNO.", "MISSINGNO.", "GROWLITHE", "ONIX", "FEAROW", "PIDGEY", "SLOWPOKE", "KADABRA", "GRAVELER", "CHANSEY", "MACHOKE", "MR.MIME", "HITMONLEE", "HITMONCHAN", "ARBOK", "PARASECT", "PSYDUCK", "DROWZEE", "GOLEM", "MISSINGNO.", "MAGMAR", "MISSINGNO.", "ELECTABUZZ", "MAGNETON", "KOFFING", "MISSINGNO.", "MANKEY", "SEEL", "DIGLETT", "TAUROS", "MISSINGNO.", "MISSINGNO.", "MISSINGNO.", "FARFETCH'D", "VENONAT", "DRAGONITE", "MISSINGNO.", "MISSINGNO.", "MISSINGNO.", "DODUO", "POLIWAG", "JYNX", "MOLTRES", "ARTICUNO", "ZAPDOS", "DITTO", "MEOWTH", "KRABBY", "MISSINGNO.", "MISSINGNO.", "MISSINGNO.", "VULPIX", "NINETALES", "PIKACHU", "RAICHU", "MISSINGNO.", "MISSINGNO.", "DRATINI", "DRAGONAIR", "KABUTO", "KABUTOPS", "HORSEA", "SEADRA", "MISSINGNO.", "MISSINGNO.", "SANDSHREW", "SANDSLASH", "OMANYTE", "OMASTAR", "JIGGLYPUFF", "WIGGLYTUFF", "EEVEE", "FLAREON", "JOLTEON", "VAPOREON", "MACHOP", "ZUBAT", "EKANS", "PARAS", "POLIWHIRL", "POLIWRATH", "WEEDLE", "KAKUNA", "BEEDRILL", "MISSINGNO.", "DODRIO", "PRIMEAPE", "DUGTRIO", "VENOMOTH", "DEWGONG", "MISSINGNO.", "MISSINGNO.", "CATERPIE", "METAPOD", "BUTTERFREE", "MACHAMP", "MISSINGNO.", "GOLDUCK", "HYPNO", "GOLBAT", "MEWTWO", "SNORLAX", "MAGIKARP", "MISSINGNO.", "MISSINGNO.", "MUK", "MISSINGNO.", "KINGLER", "CLOYSTER", "MISSINGNO.", "ELECTRODE", "CLEFABLE", "WEEZING", "PERSIAN", "MAROWAK", "MISSINGNO.", "HAUNTER", "ABRA", "ALAKAZAM", "PIDGEOTTO", "PIDGEOT", "STARMIE", "BULBASAUR", "VENUSAUR", "TENTACRUEL", "MISSINGNO.", "GOLDEEN", "SEAKING", "MISSINGNO.", "MISSINGNO.", "MISSINGNO.", "MISSINGNO.", "PONYTA", "RAPIDASH", "RATTATA", "RATICATE", "NIDORINO", "NIDORINA", "GEODUDE", "PORYGON", "AERODACTYL", "MISSINGNO.", "MAGNEMITE", "MISSINGNO.", "MISSINGNO.", "CHARMANDER", "SQUIRTLE", "CHARMELEON", "WARTORTLE", "CHARIZARD", "MISSINGNO.", "MISSINGNO.", "MISSINGNO.", "MISSINGNO.", "ODDISH", "GLOOM", "VILEPLUME", "BELLSPROUT", "WEEPINBELL", "VICTREEBEL"};
int pokemonNamesValues[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE};

/*
pokemon team structure : 
1 byte = poke party count (1 to 6)
1 byte each = party poke IDs (x6)
1 byte = padding (always 0xFF)
0x2C bytes each = pokemon struct (x6)
0x0B bytes each = OT name (x6)
0x0B bytes each = Pokemon name (x6)
*/

const char* getStatusConditionName(uint8_t status){
    if(status == 0x00) return "OK";

    if(status & 0x07) return "\x1b[32mSLP\x1b[0m";
    if(status & 0x08) return "\x1b[35mPSN\x1b[0m";
    if(status & 0x10) return "\x1b[31mBRN\x1b[0m";
    if(status & 0x20) return "\x1b[36mFRZ\x1b[0m";
    if(status & 0x40) return "\x1b[33mPAR\x1b[0m";

    return "???";
}

uint8_t removeUpperBitsOfPPValue(uint8_t value){
    value = value << 2;
    return value >> 2;
}

const char* getTypeName(uint8_t typeValue){
    if(typeValue == 0x00){return "Normal";}
    if(typeValue == 0x01){return "Fighting";}
    if(typeValue == 0x02){return "Flying";}
    if(typeValue == 0x03){return "Poison";}
    if(typeValue == 0x04){return "Ground";}
    if(typeValue == 0x05){return "Rock";}
    if(typeValue == 0x06){return "Bird (unused)";}
    if(typeValue == 0x07){return "Bug";}
    if(typeValue == 0x08){return "Ghost";}
    if(typeValue == 0x14){return "Fire";}
    if(typeValue == 0x15){return "Water";}
    if(typeValue == 0x16){return "Grass";}
    if(typeValue == 0x17){return "Electric";}
    if(typeValue == 0x18){return "Psychic";}
    if(typeValue == 0x19){return "Ice";}
    if(typeValue == 0x1A){return "Dragon";}
    return "Unknown";
}

void loadPartyPokemonDataToStruct(unsigned char* savebuffer, save_t *save){
    consoleClear();
    int pCount = savebuffer[PARTY_POKE_COUNT_ADDRESS];
    if(pCount==0){ //at least ive implemented a failsafe
        printf(LINE(1) "No Pokemons detected into the savefile.\nMaybe it's a new save?");
        printf(LINE(4) "Press any key to continue...");
        flushFramebufferAndWaitForVBlank();
        waitForInput();
        return;
    }
    pokemon_t party[pCount];

    for(int p=0; p<pCount; p++){
        //copy data from savefile memory buffer directly into struct (struct is already properly padded/structured) 
        memcpy(&party[p], savebuffer + BASE_PARTY_POKE_ADDRESS+(POKE_STRUCT_SIZE*p), sizeof(pokemon_t));
    }

    int currentPokemon = 0;

    while(true){
        u32 keys = getInput();
        if(keys & KEY_DOWN){currentPokemon++; consoleClear();}
        if(keys & KEY_UP){currentPokemon--; consoleClear();}
        if(keys & KEY_START){break;}
        if(currentPokemon < 0){currentPokemon = pCount-1;}
        if(currentPokemon >= pCount){currentPokemon = 0;}

        //as y'all can see, i love using ANSI sequences...
        printf(LINE(1) "             /---------------------\\           \x1b[36m%d\x1b[0m/\x1b[36m%d\x1b[0m", currentPokemon+1, pCount);
        printf(LINE(2) "             | Stats for \x1b[32m%s \x1b[0m\x1b[2;36H|", pokemonNamesList[party[currentPokemon].speciesID]);
        printf(LINE(3) "             \\---------------------/");
        
        printf(LINE(5) "/------STATS------\\");
        printf(LINE(6) "|   Level: \x1b[36m%d\x1b[0m     \x1b[6;19H|", party[currentPokemon].level);
        printf(LINE(7) "|      HP: \x1b[36m%hu\x1b[0m/\x1b[36m%hu\x1b[0m \x1b[7;19H|", swap16(party[currentPokemon].currentHP), swap16(party[currentPokemon].maxHP));
        printf(LINE(8) "|  STATUS: \x1b[36m%s\x1b[0m      \x1b[8;19H|", getStatusConditionName(party[currentPokemon].statusCondition));
        printf(LINE(9) "|      No. \x1b[36m%03d\x1b[0m    \x1b[9;19H|", normalPokedexIDs[party[currentPokemon].speciesID]); //No. = Pokédex number in game
        printf(LINE(10)"|     IID: \x1b[36m%03d\x1b[0m    \x1b[10;19H|", party[currentPokemon].speciesID);  //IID = Internal ID used in memory
        printf(LINE(11)"\\-----------------/");
        
        printf(LINE(13) "/-----STATS-----\\");                //swapping endianness of 16bit values 
        printf(LINE(14) "| ATTACK  : \x1b[36m%03hu\x1b[0m |", swap16(party[currentPokemon].Attack));
        printf(LINE(15) "| DEFENSE : \x1b[36m%03hu\x1b[0m |", swap16(party[currentPokemon].Defense));
        printf(LINE(16) "| SPEED   : \x1b[36m%03hu\x1b[0m |", swap16(party[currentPokemon].Speed));
        printf(LINE(17) "| SPECIAL : \x1b[36m%03hu\x1b[0m |", swap16(party[currentPokemon].Special));
        printf(LINE(18) "\\---------------/");

        printf(LINE(20) "/-----MOVES-----\\");
        printf(LINE(21) "| \x1b[36m%d\x1b[0m\x1b[21;9H| PP:\x1b[36m%02d\x1b[0m |", party[currentPokemon].move1, removeUpperBitsOfPPValue(party[currentPokemon].Move1PPValue));
        printf(LINE(22) "| \x1b[36m%d\x1b[0m\x1b[22;9H| PP:\x1b[36m%02d\x1b[0m |", party[currentPokemon].move2, removeUpperBitsOfPPValue(party[currentPokemon].Move2PPValue));
        printf(LINE(23) "| \x1b[36m%d\x1b[0m\x1b[23;9H| PP:\x1b[36m%02d\x1b[0m |", party[currentPokemon].move3, removeUpperBitsOfPPValue(party[currentPokemon].Move3PPValue));
        printf(LINE(24) "| \x1b[36m%d\x1b[0m\x1b[24;9H| PP:\x1b[36m%02d\x1b[0m |", party[currentPokemon].move4, removeUpperBitsOfPPValue(party[currentPokemon].Move4PPValue));
        printf(LINE(25) "\\---------------/");

        printf(LINE(29) "--------------------------------------------------");
        printf(LINE(30) "\x1b[33mUP\x1b[0m/\x1b[33mDOWN\x1b[0m : Previous/Next Pokemon  |  \x1b[33mSTART\x1b[0m: Go back");

        flushFramebufferAndWaitForVBlank();
        
    }

    consoleClear();
}

