#include "stdafx.h"
#include "limefoxlib.h"

#define IDLE_R 50
#define IDLE_G 200
#define IDLE_B 255
#define PRESSED_R 255
#define PRESSED_G 0
#define PRESSED_B 0

static KEYS_MATRIX KeyMatrix = KEYS_MATRIX();
static COLOR_MATRIX ColorMatrix = COLOR_MATRIX();

void key_press_callback(int iRow, int iColumn, bool bPressed) {
	if (bPressed) {
		int beforeColumn = limefox_limit(iColumn - 1, 0, S_MAX_LED_COLUMN);
		int afterColumn = limefox_limit(iColumn + 1, 0, S_MAX_LED_COLUMN);
		int beforeRow = limefox_limit(iRow - 1, 0, S_MAX_LED_ROW);
		int afterRow = limefox_limit(iRow + 1, 0, S_MAX_LED_ROW);

		limefox_setKeyColor(&ColorMatrix, iRow, iColumn, PRESSED_R, PRESSED_G, PRESSED_B);
		limefox_setKeyColor(&ColorMatrix, iRow, beforeColumn, PRESSED_R, PRESSED_G, PRESSED_B);
		limefox_setKeyColor(&ColorMatrix, iRow, afterColumn, PRESSED_R, PRESSED_G, PRESSED_B);
		limefox_setKeyColor(&ColorMatrix, beforeRow, iColumn, PRESSED_R, PRESSED_G, PRESSED_B);
		limefox_setKeyColor(&ColorMatrix, beforeRow, beforeColumn, PRESSED_R, PRESSED_G, PRESSED_B);
		limefox_setKeyColor(&ColorMatrix, beforeRow, afterColumn, PRESSED_R, PRESSED_G, PRESSED_B);
		limefox_setKeyColor(&ColorMatrix, afterRow, iColumn, PRESSED_R, PRESSED_G, PRESSED_B);
		limefox_setKeyColor(&ColorMatrix, afterRow, beforeColumn, PRESSED_R, PRESSED_G, PRESSED_B);
		limefox_setKeyColor(&ColorMatrix, afterRow, afterColumn, PRESSED_R, PRESSED_G, PRESSED_B);
	}	
	KeyMatrix.Keys[iRow][iColumn].isPressed = bPressed;
}

int limefox_soft_touch() {
	SetKeyCallBack(key_press_callback);
	system("cls");
	printf("Soft Touch\n");
	while (true) {
		for (int i = 0; i < S_MAX_LED_ROW; i++) {
			for (int j = 0; j < S_MAX_LED_COLUMN; j++) {
				if (!KeyMatrix.Keys[i][j].isPressed) {
					ColorMatrix.KeyColor[i][j].r = limefox_average_with_factor(ColorMatrix.KeyColor[i][j].r, IDLE_R, 10);
					ColorMatrix.KeyColor[i][j].g = limefox_average_with_factor(ColorMatrix.KeyColor[i][j].g, IDLE_G, 10);
					ColorMatrix.KeyColor[i][j].b = limefox_average_with_factor(ColorMatrix.KeyColor[i][j].b, IDLE_B, 10);
				}
			}
		}
		SetAllLedColor(ColorMatrix);
		Sleep(50);
		if (_kbhit()) {
			break;
		}
	}
	return 0;
}
