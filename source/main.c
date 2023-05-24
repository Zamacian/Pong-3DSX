// Simple citro2d untextured shape example
#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

int rand_range(int min, int max) {
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

int getRandomOne() {
	int rG = -2 + rand() / (RAND_MAX / (2 - -2 + 1) + 1);
	if (rG == 0) {
		rG = getRandomOne();
	} else {
		return rG;
	}
}

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Create colors
	u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF); //#FFFFFF
	u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0xFF); //#000000

	bool UP1;
	bool UP2;
	bool DOWN1;
	bool DOWN2;
	int Rect_1_Y = SCREEN_HEIGHT / 2 - 50 / 2;
	int Rect_2_Y = SCREEN_HEIGHT / 2 - 50 / 2;
	int allow_accel = 0;
	int ball_x = SCREEN_WIDTH / 2;
	int ball_y = SCREEN_HEIGHT / 2;
	int ball_x_accel = 0;
	int ball_y_accel = 0;
	int winner = 0;

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		u32 kUp = hidKeysUp();
		if (kDown & KEY_SELECT)
			break; // break in order to return to hbmenu
		if (kDown & KEY_UP) {
			UP1 = true;
		} else if (kUp & KEY_UP) {
			UP1 = false;
		}
		if (kDown & KEY_DOWN) {
			DOWN1 = true;
		} else if (kUp & KEY_DOWN) {
			DOWN1 = false;
		}
		if (kDown & KEY_X) {
			UP2 = true;
		} else if (kUp & KEY_X) {
			UP2 = false;
		}
		if (kDown & KEY_B) {
			DOWN2 = true;
		} else if (kUp & KEY_B) {
			DOWN2 = false;
		}
		if (kDown & KEY_START) {
			if (ball_x_accel == 0 && ball_y_accel == 0) {
				ball_x_accel = getRandomOne();
				ball_y_accel = getRandomOne();
			}
		}

		if (UP1 && Rect_1_Y > 10) {
			Rect_1_Y -= 3;
		} else if (DOWN1 && Rect_1_Y < SCREEN_HEIGHT - 60) {
			Rect_1_Y += 3;
		}
		if (UP2 && Rect_2_Y > 10) {
			Rect_2_Y -= 3;
		} else if (DOWN2 && Rect_2_Y < SCREEN_HEIGHT - 60) {
			Rect_2_Y += 3;
		}

		if (ball_x + 8 >= SCREEN_WIDTH - 20 && ball_y < Rect_2_Y + 50 && ball_y > Rect_2_Y) {
			if (allow_accel == 3) {
				allow_accel = 0;
				ball_y_accel ++;
				ball_x_accel = -(abs(ball_x_accel) + 1);
			} else {
				allow_accel ++;
				ball_x_accel = -(abs(ball_x_accel));
			}
		} else if (ball_x - 8 <= 20 && ball_y < Rect_1_Y + 50 && ball_y > Rect_1_Y) {
			if (allow_accel == 3) {
				allow_accel = 0;
				ball_y_accel ++;
				ball_x_accel = abs(ball_x_accel) + 1;
			} else {
				allow_accel ++;
				ball_x_accel = abs(ball_x_accel);
			}
		} else if (ball_x + 8 >= SCREEN_WIDTH - 20) {
			ball_y = SCREEN_HEIGHT / 2;
			ball_x = SCREEN_WIDTH / 2;
			ball_y_accel = 0;
			ball_x_accel = 0;
			winner = 1;
		} else if (ball_x - 8 <= 20) {
			ball_y = SCREEN_HEIGHT / 2;
			ball_x = SCREEN_WIDTH / 2;
			ball_y_accel = 0;
			ball_x_accel = 0;
			winner = 2;
		}

		if (ball_y  - 8 <= 0 && ball_y_accel < 0) {
			ball_y_accel = abs(ball_y_accel);
		} else if (ball_y + 8 >= SCREEN_HEIGHT && ball_y_accel > 0) {
			ball_y_accel = ball_y_accel * -1;
		}

		ball_x += ball_x_accel;
		ball_y += ball_y_accel;

		printf("\x1b[2;1HStart :      'Start'\x1b[K");
		printf("\x1b[3;1HExit :       'Select'\x1b[K");
		printf("\x1b[4;1HPlayer 1 :   'D-Pad'\x1b[K");
		printf("\x1b[5;1HPlayer 2 :   'X and B'\x1b[K");
		if (winner == 1) {
			printf("\x1b[6;1HLast Winner : Player 1\x1b[K");
		} else if (winner == 2) {
			printf("\x1b[6;1HLast Winner : Player 2\x1b[K");
		}
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrBlack);
		C2D_SceneBegin(top);

		C2D_DrawRectangle(10, Rect_1_Y , 0, 10, 50,
		clrWhite, clrWhite, clrWhite, clrWhite);
		C2D_DrawRectangle(SCREEN_WIDTH - 20, Rect_2_Y, 0, 10, 50,
		clrWhite, clrWhite, clrWhite, clrWhite);

		C2D_DrawCircleSolid(ball_x, ball_y, 0, 8, clrWhite);

		C3D_FrameEnd(0);
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}
