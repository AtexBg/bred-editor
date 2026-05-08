#include <stdio.h>

char gbchar2char(int hex) {
    if (hex >= 0x80 && hex <= 0x99){
        return 'A' + (hex - 0x80);
    }

    if (hex >= 0xA0 && hex <= 0xB9){
        return 'a' + (hex - 0xA0);
    }

    if (hex >= 0xF6 && hex <= 0xFF){
        return '0' + (hex - 0xF6);
    }

    //TODO: add more chars, refer to this link :
    //https://datacrystal.tcrf.net/wiki/Pok%C3%A9mon_Red_and_Blue/TBL
        
        
    switch(hex) {
        case 0xF4: return ',';
        case 0x50: return '\0'; // end of string
        case 0x7F: return ' ';
        case 0x00: return ' ';

        default:
            return '?';
    }
}
unsigned char char2gbchar(char letter) {
    if (letter >= 'A' && letter <= 'Z') return 0x80 + (letter - 'A');
    if (letter >= 'a' && letter <= 'z') return 0xA0 + (letter - 'a');
    //0xF6 to 0xFF is 0 to 9
    if (letter >= '0' && letter <= '9') return 0xF6 + (letter - '0');
    if (letter == ' ') return 0x7F;
    
    return 0xE6; //char 0xE6 = '?'
}

void encodeGBString(const char *input, unsigned char *output) {
    for (int i = 0; i < 7; i++) {
        if (input[i] == '\0') {
            output[i] = 0x50; //0x50 = end of text in charset
        
        }
        output[i] = char2gbchar(input[i]);
    }
}
