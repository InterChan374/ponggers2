// shaders.hpp template by InterChan

// this file's function goes with draw_functions.hpp
// you can add case clauses for custom shaders, and the available variables allow for fancy effects
// "x" and "y" coordinates represent where the top-left corner of the texture is
// "w" and "h" are the width and height of the texture
// "i" and "j" are the x and y offsets from the top-left corner, for the current pixel to be drawn
// when using draw_texture_shader() you can include extra information with "shaderArg"

#pragma once

#include <sdk/calc/calc.hpp>

#define TRANSPARENCY_COLOR 0xF81F // (255, 0, 255) or #FF00FF

void shader(int16_t x, int16_t y, int16_t w, int16_t h, int16_t i, int16_t j, uint16_t color, uint16_t shaderID, int shaderArg) {
	switch (shaderID) {
		default: case 0:
			// no effects
			setPixel(x + i, y + j, color);
			break;
		case 1:
			// cutout (default shader, used by DRAW_TEXTURE)
			if (color != TRANSPARENCY_COLOR) {
				setPixel(x + i, y + j, color);
			}
			break;
		case 2:
			// cutout + frame selection (for textures containing multiple "frames", useful for rotation, animations and texture variants. see texture_rotator.py)
			// (this uses the texture width as the height of one frame, so make sure the frames are square)
			if (j / w == shaderArg) {
				shader(x, y - shaderArg * w, w, h, i, j, color, 1, 0);
			}
			break;
		case 3:
			// for opacity shader, just get the LCD pixel's color and multiply with texture color???
			break;
		case 4:
			// scaling
			for (int b = 0; b < shaderArg; b++) {
				for (int a = 0; a < shaderArg; a++) {
					shader(x, y, w, h, i * shaderArg + a, j * shaderArg + b, color, 1, 0);
				}
			}
			break;
		case 5:
			// drop shadow for each pixel with color shaderArg
			if (color != TRANSPARENCY_COLOR) {
				setPixel(x + i, y + j, color);
				setPixel(x + i + 1, y + j + 1, (uint16_t)shaderArg);
			}
			break;
	}
}
