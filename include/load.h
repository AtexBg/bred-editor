#include <stdint.h> //uint8/16_t

#ifndef LOAD_H
#define LOAD_H

typedef struct save_t{
    struct {
        unsigned char player[0xB];       // 0x2598
        unsigned char rival[0xB];        // 0x25F6
    }name;

    uint8_t money[3];                      // 0x25F3
    unsigned char badges;           // 0x2602
    uint16_t playerID;         // 0x2605

    struct {
        unsigned char playerX;          // 0x260D
        unsigned char playerY;          // 0x260E
        unsigned char direction;        // 0x27D6
    }playerPos;
    
    unsigned char state;            // 0x29AC

    struct {
        unsigned char hours;            // 0x2CED
        unsigned char mins;          // 0x2CEF
        unsigned char secs;           // 0x2CF0
    }playtime;

    uint8_t checksum;                   //0x3523
    bool hasUnsavedChanges;     //not in the savefile itself
} save_t;


void loadValues(unsigned char *saveFile, save_t *save);
int money2int(uint8_t money[3]);
void int2money(int interger, int *money);


#endif