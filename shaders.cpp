// https://github.com/InterChan374/cool-classpad-functions

// this file's function goes with draw_functions.hpp
// you can add case clauses for custom shaders, and the available variables allow for fancy effects
// "screenX" and "screenY" coordinates represent where the top-left corner of the texture is
// "textureWidth" and "textureHeight" are the width and height of the texture
// "offsetX" and "offsetY" are the offsets from the top-left corner, for the current pixel to be drawn
// when using draw_texture_shader() you can include extra information by passing an array of the correct size to shaderArgs[] (depending on which shader you're using)

#include "shaders.hpp"

#define TRANSPARENCY_COLOR 0xF81F // RGB (255, 0, 255) or #FF00FF

// #define THING texturepointer[k]

void draw_texture_shader(uint16_t *texturepointer, int16_t screenX, int16_t screenY, uint16_t shaderID, int shaderArgs[]) {
// int16_t textureWidth, int16_t textureHeight
	uint16_t textureWidth = texturepointer[0];
	uint16_t textureHeight = texturepointer[1];
	int k = 2;
	switch (shaderID) {
		default: case 0:
			// no effects
			for (int16_t offsetY = 0; offsetY < textureHeight; offsetY++) {
				for (int16_t offsetX = 0; offsetX < textureWidth; offsetX++) {
					setPixel(screenX + offsetX, screenY + offsetY, texturepointer[k]);
					// draw_texture_shader(screenX, screenY, textureWidth, textureHeight, offsetX, offsetY, texturepointer[k], shaderID, shaderArgs[]);
					k++;
				}
			}
			break;
		case 1:
			// cutout (default shader, used by DRAW_TEXTURE)
			if (texturepointer[k] != TRANSPARENCY_COLOR) {
				setPixel(screenX + offsetX, screenY + offsetY, color);
			}
			break;
		case 2:
			// frame selection + cutout (for textures containing multiple "frames", useful for rotation, animations and texture variants. see texture_rotator.py)
			// (this uses the texture width as the height of one frame, so make sure the frames are square)
			if (offsetY / textureWidth == shaderArgs[]) {
				draw_texture_shader(screenX, screenY - shaderArgs[] * textureWidth, textureWidth, textureHeight, offsetX, offsetY, color, 1, 0);
			}
			break;
		case 3:
			// scaling of 4 + sine wavy effect, the amplitude is fixed but shaderArg alters the period
			for (int b = 0; b < 4; b++) {
				for (int a = 0; a < 4; a++) {
					setPixel(screenX + offsetX * 4 + a + SIN((offsetY*4+b) * shaderArgs[] / 2, 60), screenY + offsetY * 4 + b, color);
				}
			}
			break;
		case 4:
			// scaling of 4 + drop shadow
			for (int b = 0; b < 4; b++) {
				for (int a = 0; a < 4; a++) {
					draw_texture_shader(screenX, screenY, textureWidth, textureHeight, offsetX * 4 + a, offsetY * 4 + b, color, 5, shaderArgs[]);
				}
			}
			break;
		case 5:
			// drop shadow for each pixel with color shaderArg
			if (color != TRANSPARENCY_COLOR) {
				setPixel(screenX + offsetX, screenY + offsetY, color);
				setPixel(screenX + offsetX + 1, screenY + offsetY + 1, (uint16_t)shaderArgs[]);
			}
			break;
		
	}
}
