#include "stdafx.h"
#include "limefoxlib.h"

static KEYS_MATRIX KeyMatrix = KEYS_MATRIX();
static COLOR_MATRIX ColorMatrix = COLOR_MATRIX();
static int currentColumn = 0;


void key_callback(int iRow, int iColumn, bool bPressed) {
	if (!KeyMatrix.Keys[iRow][iColumn].isPressed && bPressed) {
		int randR = rand() % 120;
		int randG = rand() % 120;
		int randB = rand() % 120;

		int beforeColumn = limefox_overflow(currentColumn - 1, S_MAX_LED_COLUMN);
		int afterColumn = limefox_overflow(currentColumn + 1, S_MAX_LED_COLUMN);

		int rBefore = limefox_notExceed(ColorMatrix.KeyColor[0][beforeColumn].r + randR, 255);
		int gBefore = limefox_notExceed(ColorMatrix.KeyColor[0][beforeColumn].g + randG, 255);
		int bBefore = limefox_notExceed(ColorMatrix.KeyColor[0][beforeColumn].b + randB, 255);
		int rCurrnet = limefox_notExceed(ColorMatrix.KeyColor[0][currentColumn].r + randR, 255);
		int gCurrent = limefox_notExceed(ColorMatrix.KeyColor[0][currentColumn].g + randG, 255);
		int bCurrent = limefox_notExceed(ColorMatrix.KeyColor[0][currentColumn].b + randB, 255);
		int rAfter = limefox_notExceed(ColorMatrix.KeyColor[0][afterColumn].r + randR, 255);
		int gAfter = limefox_notExceed(ColorMatrix.KeyColor[0][afterColumn].g + randG, 255);
		int bAfter = limefox_notExceed(ColorMatrix.KeyColor[0][afterColumn].b + randB, 255);

		for (int i = 0; i < S_MAX_LED_ROW; i++) {
			limefox_setKeyColor(&ColorMatrix, i, beforeColumn, rBefore, gBefore, bBefore);
			limefox_setKeyColor(&ColorMatrix, i, currentColumn, rCurrnet, gCurrent, bCurrent);
			limefox_setKeyColor(&ColorMatrix, i, afterColumn, rAfter, gAfter, bAfter);
		}
		currentColumn++;
		if (currentColumn > S_MAX_LED_COLUMN) {
			currentColumn = 0;
		}
	}
	KeyMatrix.Keys[iRow][iColumn].isPressed = bPressed;
}

int limefox_type_o_wipe() {
	srand(time(NULL));
	SetKeyCallBack(key_callback);
	system("cls");
	printf("Type O Wipe\n");
	while (true) {
		for (int i = 0; i < S_MAX_LED_ROW; i++) {
			for (int j = 0; j < S_MAX_LED_COLUMN; j++) {
				ColorMatrix.KeyColor[i][j].r *= 0.99;
				ColorMatrix.KeyColor[i][j].g *= 0.99;
				ColorMatrix.KeyColor[i][j].b *= 0.99;
			}
		}
		SetAllLedColor(ColorMatrix);
		Sleep(1);
		if (_kbhit()) {
			break;
		}
	}
	return 0;
}
