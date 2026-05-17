#include <stdio.h>
#include <time.h>

int framee = 0;
int fps = 0;
int sec = 0;

int getCurrentFPS() {
    static time_t last_time = 0; //static to keeps value between calls
    time_t current_time = time(NULL);
    framee++; // increment frame counter

    if(current_time != last_time) {
        fps = framee;
        framee = 0;
        last_time = current_time;
    }
    return fps;
}
