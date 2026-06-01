#include <stdio.h>
#include <stdint.h> //uint8/16_t

uint8_t calculateChecksum(unsigned char *save) <%
    uint16_t checksum = 0x00; //16bits to avoid overflow
    
    for(int i = 0x2598; i <= 0x3522; i++) <%
        checksum += save[i]; 
    %>
    
    checksum = ~checksum; //the "~" operator invert the bytes
    return checksum; //return obtained value
%>