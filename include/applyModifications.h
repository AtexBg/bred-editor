#include "pokemon.h" //pokemon_t type
#include "items.h" //item_t 

void applyAndWriteSaveFile(save_t *save, char* saveFilePath, unsigned char *savefileBuffer, pokemon_t *party, item_t *bag, int isLoadingGbEmulator);