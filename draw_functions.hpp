// https://github.com/InterChan374/cool-classpad-functions

// these functions allow you to read and display the converted textures and fonts converted with convert_textures.py and convert_fonts.py
// remember to free() the texture pointers when you stop using them (for example when you exit to the main menu you can free the ingame textures)
// refer to this example below on using the functions:
/*

uint16_t *playerSprite = load_texture("GameName\\player");					// for the path, if you #define PATH_PREFIX (see line 24) then you can omit the game name and just have "player"
LOAD_FONT_PTR("fonts\\default", menuFont);							// just a shorthand, refer to "#define" section below. note that load_texture is uint16_t* while load_font is uint8_t*

draw_texture_shader(playerSprite, 60, 90, 2, 1);							// draws the loaded texture at (60, 90) using shader #2 with "1" as the argument which shows the second frame of the texture
draw_font_shader(menuFont, "Nice font!", 40, 180, 0xFFFF, 150, 2, 4, 3);	// displays "Nice font!" with the loaded font at (40, 180) with rgb565 color 0xFFFF, wrapping at 150 pixels horizontally from 40, with 2 pixel gap between lines vertically, using shader 4 which is image scale at 3x

LCD_Refresh();

free(playerSprite);
free(menuFont);

*/

#pragma once

// edit PATH_PREFIX to change what gets appended to the texture/font path when you load them, or remove it to disable the appending
#define PATH_PREFIX "\\fls0\\usr\\"

#include <sdk/os/file.hpp>
#include <sdk/os/mem.hpp>
#include <sdk/os/string.hpp>
#include "shaders.hpp"

#define LOAD_TEXTURE_PTR(path, pointer) uint16_t *pointer = load_texture(path)
#define LOAD_FONT_PTR(path, pointer) uint8_t *pointer = load_font(path)
#define DRAW_TEXTURE(texturepointer, x, y) draw_texture_shader(texturepointer, x, y, 1, 0)
#define DRAW_TEXTURE_FRAME(texturepointer, x, y, frame) draw_texture_shader(texturepointer, x, y, 2, frame)
#define DRAW_FONT(fontpointer, text, x, y, color) draw_font_shader(fontpointer, text, x, y, color, 0, 1, 0, 0)

inline uint16_t uint8to16(uint8_t, uint8_t);

uint16_t *load_texture(const char *);

void draw_texture_shader(uint16_t *, int16_t, int16_t, uint16_t, int);

uint8_t *load_font(const char *);

#define CHAR_SPACING 1
void draw_font_shader(uint8_t *, const char *, int16_t, int16_t, uint16_t, uint16_t, int16_t, uint16_t, int);
