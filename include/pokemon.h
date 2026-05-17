#include <stdint.h>

#include "load.h"

//setting the compiler alignement to 1 byte, so no padding will be added
#pragma pack(push, 1)

//this is a struct for a party pokemon, data offsetq are different for PC/daycare pokemons for some reason 
//struct is 0x2C (44) bytes in total
typedef struct pokemon_t{
                                  //offsets
    uint8_t speciesID;            //0x00
    uint16_t currentHP;           //0x01
    uint8_t unknownDummyLevelStat;//0x03
    uint8_t statusCondition;      //0x04
    uint8_t type1;                //0x05
    uint8_t type2;                //0x06
    uint8_t heldItem;             //0x07

    //moves ids
    uint8_t move1;                //0x08
    uint8_t move2;                //0x09
    uint8_t move3;                //0x0A
    uint8_t move4;                //0x0B

    uint16_t OriginalTrainerID;   //0x0C

    //exp points are stored in 3 bytes, idk how to properly use them yet
    uint8_t  EXPPoints_high;      //0x0E
    uint16_t EXPPoints_low;       //0x0F

    uint16_t HPStatExpData;        //0x11
    uint16_t AttackStatEXP;       //0x13
    uint16_t DefenseStatEXP;      //0x15
    uint16_t SpeedStatEXP;        //0x17
    uint16_t SpecialStatEXP;      //0x19

    uint16_t IV_Data;             //0x1B

    uint8_t Move1PPValue;         //0x1D
    uint8_t Move2PPValue;         //0x1E
    uint8_t Move3PPValue;         //0x1F
    uint8_t Move4PPValue;         //0x20

    uint8_t level;                //0x21
    uint16_t maxHP;               //0x22

    uint16_t Attack;              //0x24
    uint16_t Defense;             //0x26
    uint16_t Speed;               //0x28
    uint16_t Special;             //0x2A

}pokemon_t;

//restore normal compiler behaviour
#pragma pack(pop)

void loadPartyPokemonDataToStruct(unsigned char* savebuffer, save_t *save);