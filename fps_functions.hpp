// https://github.com/InterChan374/cool-classpad-functions

// In main loop add "fps10_update();" which updates the variable fps10
// The int "fps10" represents the fps to one decimal place, except without the decimal point so 14.2 is just 142
// The "fps_formatted" string is the FPS to one decimal place, in the format "xx.x FPS"
// Use "fps_formatted_update();" to update the string to current fps10 before displaying it
// Use "fps_debug_printf();" to display the formatted FPS in the top left corner

#pragma once

#include <sdk/os/string.hpp>
#include <sdk/os/debug.hpp>

extern uint8_t *R64CNT;
extern uint8_t prevtime;
extern int fps10;
extern char fps_formatted[];

void fps10_update();

void fps_formatted_update();

inline void fps_debug_printf();
