#include "calc.hpp"
#include "trig_functions.hpp"
#include "fps_functions.hpp"
#include "draw_functions.hpp"

#ifndef PC
	APP_NAME("ponggers 2: electric boogaloo")
	APP_DESCRIPTION("pong tech demo thing, left/right arrows to move")
	APP_AUTHOR("InterChan")
	APP_VERSION("6900")
#endif

#define P_WIDTH 60
#define P_HEIGHT 8
#define P_SPEED 7
#define B_SIZE 16
#define B_STARTINGSPEED 12
#define B_BOUNCEANGLE 100
#define B_COOLDOWN 10
#define B_TRAILAMOUNT 10

int lerp(int a, int b, int t) { // t is in percent since no float
	return a + t * (b - a) / 100;
}

int clamp(int a, int min, int max) {
	if (a < min) {
		return min;
	} else if (a > max) {
		return max;
	}
	return a;
}

void rect(int x, int y, int w, int h, uint16_t c) {
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			setPixel(x + i, y + j, c);
		}
	}
}

//The actual main
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
	bool cpTargetSet = false;
	int cpTarget = ballX;
	
	// load the textures and fonts
	LOAD_FONT_PTR("fnt\\7x8", f_7x8);
	LOAD_TEXTURE_PTR("paddle", t_paddle);
	LOAD_TEXTURE_PTR("paddle2", t_paddle2);
	LOAD_TEXTURE_PTR("ball", t_ball);
	LOAD_TEXTURE_PTR("ball_trail", t_ball_trail);
	
	// game starting screen
	for (int i = 0; i < 100; i+=5) {
		fillScreen(color(22, 22, 22));
		DRAW_TEXTURE(t_ball, lerp(0, ballX, i), lerp(0, ballY, i));
		draw_font_shader(f_7x8, "ponggers2", 16, 128, color(255, 60, 0), 0, 0, 4, color(122, 222, 222));
		LCD_Refresh();
	}
	
	bool game_running = true;
	while (game_running) {
		uint32_t key1, key2;
		getKey(&key1, &key2);
		if (testKey(key1, key2, KEY_CLEAR)) game_running = false;
		if (testKey(key1, key2, KEY_LEFT)) youX -= P_SPEED;
		if (testKey(key1, key2, KEY_RIGHT)) youX += P_SPEED;
		youX = clamp(youX, 0, width - P_WIDTH);
		
		// cp logics
		if (!cpTargetSet) {
			int a = ballX + ballVelX * (ballY - P_HEIGHT) / -ballVelY; // where the ball would go if there were no walls
			int b = (a % (width-B_SIZE) + (width-B_SIZE)) % (width-B_SIZE); // modulus of the position
			int c = (a - b) / (width-B_SIZE); // how many screen widths variable "b" is offset by
			cpTarget = b;
			if (c & 1) { // if the screen width offset is odd, reflect the ball position
				cpTarget = width - cpTarget - B_SIZE;
			}
			cpTarget += B_SIZE / 2;
			// println("a = %d", a);
			// println("b = %d", b);
			// println("c = %d", c);
			// println("cpTarget = %d", cpTarget);
			// for (int i = 0; i < 40; i++) {
			// 	LCD_Refresh();
			// }
			cpTargetSet = true;
		}
		if (cpTarget < (cpX + P_WIDTH / 2)) cpX -= P_SPEED;
		if (cpTarget > (cpX + P_WIDTH / 2)) cpX += P_SPEED;
		cpX = clamp(cpX, 0, width - P_WIDTH);
		
		// ball physics
		ballX += ballVelX;
		ballY += ballVelY;
		if (ballX < 0) {
			ballX = -ballX;
			ballVelX = -ballVelX;
		}
		if (ballX > width - B_SIZE) {
			ballX = -ballX + 2 * (width - B_SIZE);
			ballVelX = -ballVelX;
		}
		if (ballCooldown > 0) {
			ballCooldown--;
		}
		if (ballCooldown <= 0) {
			// at your edge
			if (ballY + B_SIZE >= height - P_HEIGHT) {
				if (ballX + B_SIZE > youX && ballX < youX + P_WIDTH) {
					int i = ((ballX + B_SIZE / 2) - (youX + P_WIDTH / 2)) * B_BOUNCEANGLE / P_WIDTH;
					ballVelX = SIN(i, ballSpeed);
					ballVelY = -COS(i, ballSpeed);
					// ballY = -ballY + 2 * (height - P_HEIGHT - B_SIZE);
					ballY = height - P_HEIGHT - B_SIZE;
					ballSpeed += 2;
					ballCooldown = 0;
					cpTargetSet = false;
				} else {
					ballCooldown = B_COOLDOWN;
				}
			}
			// at cp's edge
			if (ballY <= P_HEIGHT) {
				if (ballX + B_SIZE > cpX && ballX < cpX + P_WIDTH) {
					int i = ((ballX + B_SIZE / 2) - (cpX + P_WIDTH / 2)) * B_BOUNCEANGLE / P_WIDTH;
					ballVelX = SIN(i, ballSpeed);
					ballVelY = COS(i, ballSpeed);
					// ballY = -ballY + 2 * P_HEIGHT;
					ballY = P_HEIGHT;
					ballCooldown = 0;
				} else {
					ballCooldown = B_COOLDOWN;
				}
			}
		}
		// reset ball after it goes offscreen
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
		
		fps10_update();
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
	
	// game ending screen
	for (int i = 0; i < 30; i+=2) {
		fillScreen(color(240, 240, 240));
		draw_font_shader(f_7x8, "ponggers2", 16, 248, color(50, 45, 45), 0, 0, 3, i);
		LCD_Refresh();
	}
	
	// freeing the stuff
	free(f_7x8);
	free(t_paddle);
	free(t_paddle2);
	free(t_ball);
	free(t_ball_trail);
}
