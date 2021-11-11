#include "calc.hpp"
#include "trig_functions.hpp"
#include "fps_functions.hpp"
#include "draw_functions.hpp"

#ifndef PC
	APP_NAME("ponggers 2: electric boogaloo")
	APP_DESCRIPTION("pong tech demo thing, left/right arrows to move")
	APP_AUTHOR("InterChan")
	APP_VERSION("69")
#endif

#define P_WIDTH 60
#define P_HEIGHT 8
#define P_SPEED 6
#define B_SIZE 16
#define B_STARTINGSPEED 10
#define B_BOUNCEANGLE 100
#define B_COOLDOWN 10
#define B_TRAILAMOUNT 10


void rect(int x, int y, int w, int h, uint16_t c) {
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			setPixel(x + i, y + j, c);
		}
	}
}

//The acutal main
void main2() {
	
	int youX = width / 2 - P_WIDTH / 2;
	int cpX = youX;
	int ballX = width / 2 - B_SIZE / 2;
	int ballY = height / 2 - B_SIZE / 2;
	int ballVelX = 0;
	int ballVelY = B_STARTINGSPEED;
	int ballSpeed = B_STARTINGSPEED;
	int ballCooldown = 0;
	int ballTrailsX[B_TRAILAMOUNT] = {0};
	int ballTrailsY[B_TRAILAMOUNT] = {0};
	for (int i = 0; i < B_TRAILAMOUNT; i++) {
		ballTrailsX[i] = ballX;
		ballTrailsY[i] = ballY;
	}
	
	// load the textures and fonts
	LOAD_FONT_PTR("ponggers2\\fnt\\7x8", f_7x8);
	LOAD_TEXTURE_PTR("ponggers2\\paddle", t_paddle);
	LOAD_TEXTURE_PTR("ponggers2\\paddle2", t_paddle2);
	LOAD_TEXTURE_PTR("ponggers2\\ball", t_ball);
	LOAD_TEXTURE_PTR("ponggers2\\ball_trail", t_ball_trail);
	
	while (true) {
		uint32_t key1, key2;
		getKey(&key1, &key2);
		if (testKey(key1, key2, KEY_CLEAR)) break;
		if (testKey(key1, key2, KEY_LEFT)) youX -= P_SPEED;
		if (testKey(key1, key2, KEY_RIGHT)) youX += P_SPEED;
		if (youX < 1) youX = 0;
		if (youX > width - P_WIDTH) youX = width - P_WIDTH;
		
		// cp logics
		if ((ballX + B_SIZE / 2) < (cpX + P_WIDTH / 2)) cpX -= P_SPEED;
		if ((ballX + B_SIZE / 2) > (cpX + P_WIDTH / 2)) cpX += P_SPEED;
		if (cpX < 1) cpX = 0;
		if (cpX > width - P_WIDTH) cpX = width - P_WIDTH;
		
		// ball physics
		ballX += ballVelX;
		ballY += ballVelY;
		if (ballX <= 0) {
			ballX = -ballX;
			ballVelX = -ballVelX;
		}
		if (ballX >= width - B_SIZE) {
			ballX = -ballX - 2 * B_SIZE + 2 * width;
			ballVelX = -ballVelX;
		}
		if (ballCooldown > 0) {
			ballCooldown--;
		} else {
			// at your edge
			if (ballY + B_SIZE > height - P_HEIGHT) {
				if (ballX + B_SIZE > youX && ballX < youX + P_WIDTH) {
					int i = ((ballX + B_SIZE / 2) - (youX + P_WIDTH / 2)) * B_BOUNCEANGLE / P_WIDTH;
					ballVelX = SIN(i, ballSpeed);
					ballVelY = -COS(i, ballSpeed);
					ballSpeed += 2;
					ballCooldown = 0;
				} else {
					ballCooldown--;
					if (ballCooldown < -2) ballCooldown = B_COOLDOWN;
				}
			}
			// at cp's edge
			if (ballY < P_HEIGHT) {
				if (ballX + B_SIZE > cpX && ballX < cpX + P_WIDTH) {
					int i = ((ballX + B_SIZE / 2) - (cpX + P_WIDTH / 2)) * B_BOUNCEANGLE / P_WIDTH;
					ballVelX = SIN(i, ballSpeed);
					ballVelY = COS(i, ballSpeed);
					ballCooldown = 0;
				} else {
					ballCooldown--;
					if (ballCooldown < -2) ballCooldown = B_COOLDOWN;
				}
			}
		}
		if (ballCooldown == 1) {
			ballSpeed = B_STARTINGSPEED;
			ballX = width / 2 - B_SIZE / 2;
			ballY = height / 2 - B_SIZE / 2;
			ballVelX = 0;
			ballVelY = ballSpeed;
		}
		
		// update ball trail arrays
		for (int i = 0; i < B_TRAILAMOUNT - 1; i++) {
			ballTrailsX[i] = ballTrailsX[i + 1];
			ballTrailsY[i] = ballTrailsY[i + 1];
		}
		ballTrailsX[B_TRAILAMOUNT - 1] = ballX;
		ballTrailsY[B_TRAILAMOUNT - 1] = ballY;
		
		fps_update();
		fps_formatted_update();
		
		// draw
		fillScreen(color(22, 22, 22));
		for (int i = 0; i < B_TRAILAMOUNT - 1; i++) {
			// rect(ballTrailsX[i], ballTrailsY[i], B_SIZE, B_SIZE, color(69, 69, 69));
			DRAW_TEXTURE(t_ball_trail, ballTrailsX[i], ballTrailsY[i]);
		}
		DRAW_TEXTURE(t_paddle, youX, height - P_HEIGHT - 8);
		DRAW_TEXTURE(t_paddle2, cpX, 0);
		DRAW_TEXTURE(t_ball, ballX, ballY);
		
		draw_font_shader(f_7x8, fps_formatted, 0, 0, 0xFFFF, 0, 0, 5, color(255, 128, 0));
		LCD_Refresh();
	}
}
