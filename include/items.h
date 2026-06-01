#include "load.h"

//setting the compiler alignement to 1 byte, so no padding will be added
#pragma pack(push, 1)

typedef struct item_t{
    uint8_t itemID;
    uint8_t amount;
}item_t;

#pragma pack(pop)

void editBagItems(unsigned char* savebuffer, item_t *bag, save_t *save);