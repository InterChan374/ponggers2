// https://github.com/InterChan374/cool-classpad-functions

// See fps_functions.hpp for info

#include "fps_functions.hpp"

uint8_t *R64CNT = (uint8_t*)0xA413FEC0;
uint8_t prevtime = 0;
int fps10 = 0;
char fps_formatted[9];

void fps10_update() { // update fps10 to the actual FPS
	if (*R64CNT - prevtime == 0) {
		fps10 = 1280;
	} else {
		fps10 = 1280 / ((int)((*R64CNT - prevtime) & 0b01111111));
	}
	prevtime = *R64CNT;
}

void fps_formatted_update() { // update fps_formatted to value in fps10
	int16_t dec00_1 = fps10;
	uint8_t dec01_0 = 0;
	uint8_t dec10_0 = 0;
	while (dec00_1 >= 100) { dec00_1-= 100; dec10_0++; }
	while (dec00_1 >= 10) { dec00_1-=  10; dec01_0++; }
	strcpy(fps_formatted, "  .  FPS");
	fps_formatted[0] = dec10_0 + 48;
	fps_formatted[1] = dec01_0 + 48;
	fps_formatted[3] = (uint8_t)(dec00_1 + 48);
}

inline void fps_debug_printf() {
	Debug_Printf(0,0,false,0,fps_formatted);
}
