// https://github.com/InterChan374/cool-classpad-functions

// this file's function goes with draw_functions.hpp
// you can add case clauses for custom shaders, and the available variables allow for fancy effects
// "x" and "y" coordinates represent where the top-left corner of the texture is
// "w" and "h" are the width and height of the texture
// "i" and "j" are the x and y offsets from the top-left corner, for the current pixel to be drawn
// when using draw_texture_shader() you can include extra information with "shaderArg"

#pragma once

#include <sdk/calc/calc.hpp>
#include "trig_functions.hpp"

void shader(int16_t x, int16_t y, int16_t w, int16_t h, int16_t i, int16_t j, uint16_t color, uint16_t shaderID, int shaderArg);
