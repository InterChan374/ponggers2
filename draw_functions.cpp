// https://github.com/InterChan374/cool-classpad-functions

// See draw_functions.hpp for info

#include "draw_functions.hpp"

inline uint16_t uint8to16(uint8_t highByte, uint8_t lowByte) {
	return (highByte << 8) | lowByte;
}

uint16_t *load_texture(const char *texturepath) {
	char concatpath[128];
	#ifdef PATH_PREFIX
		strcpy(concatpath, PATH_PREFIX);
	#endif
	strcat(concatpath, texturepath);
	int fd = open(concatpath, OPEN_READ);
	if (fd > -1) {
		uint16_t info[2];
		read(fd, info, 4);
		uint16_t w = info[0];
		uint16_t h = info[1];
		uint16_t *result = (uint16_t*)malloc(w*h*2+4);
		lseek(fd, 0, SEEK_SET);
		read(fd, result, w*h*2+4);
		close(fd);
		return result;
	}
	return 0;
}

void draw_texture_shader(uint16_t *texturepointer, int16_t x, int16_t y, uint16_t shaderID, int shaderArg) {
	uint16_t w = texturepointer[0];
	uint16_t h = texturepointer[1];
	int k = 2;
	for (int16_t j = 0; j < h; j++) {
		for (int16_t i = 0; i < w; i++) {
			shader(x, y, w, h, i, j, texturepointer[k], shaderID, shaderArg);
			k++;
		}
	}
}

uint8_t *load_font(const char *fontpath) {
	char concatpath[128];
	#ifdef PATH_PREFIX
		strcpy(concatpath, PATH_PREFIX);
	#else
		strcpy(concatpath, "\\fls0\\res\\");
	#endif
	strcat(concatpath, fontpath);
	int fd = open(concatpath, OPEN_READ);
	if (fd > -1) {
		uint16_t info[4];
		read(fd, info, 4);
		uint16_t w = info[0];
		uint16_t h = info[1];
		uint8_t *result = (uint8_t*)malloc(95*w*h/8+5);
		lseek(fd, 0, SEEK_SET);
		read(fd, result, (95*w*h/8)+5);
		close(fd);
		return result;
	}
	return 0;
}

void draw_font_shader(uint8_t *fontpointer, const char *text, int16_t x, int16_t y, uint16_t color, uint16_t wrapLength, int16_t lineSpacing, uint16_t shaderID, int shaderArg) {
	uint16_t w = uint8to16(fontpointer[0], fontpointer[1]);
	uint16_t h = uint8to16(fontpointer[2], fontpointer[3]);
	uint16_t newlines = 0;
	uint16_t chars_since_newline = 0;
	for (uint16_t textchar = 0; textchar < strlen(text); textchar++) { // repeat for each character in text
		if (text[textchar] == 10) {
			newlines++;
			chars_since_newline = 0;
		} else if (text[textchar] >= 32 && text[textchar] <= 126) {
			if (wrapLength > 0 && (w+CHAR_SPACING)*(chars_since_newline+1) > wrapLength) {
				newlines++;
				chars_since_newline = 0;
			}
			int current_byte = (text[textchar]-32)*w*h/8 + 4;
			uint8_t current_bit = 128 >> (((text[textchar]-32)*w*h)%8);
			for (int charbit = 0; charbit < w*h; charbit++) { // read bits of the target font character and draw pixels
				if (fontpointer[current_byte] & current_bit) {
					shader(x, y, w, h, charbit % w + (w+CHAR_SPACING)*chars_since_newline, charbit / w + (h+lineSpacing)*newlines, color, shaderID, shaderArg);
				}
				current_bit >>= 1;
				if (current_bit < 1) {
					current_bit = 128;
					current_byte++;
				}
			}
			chars_since_newline++;
		}

	}
}
