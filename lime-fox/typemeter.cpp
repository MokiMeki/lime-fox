#include "stdafx.h"
#include "limefoxlib.h"

#define IDLE_R 0
#define IDLE_G 0
#define IDLE_B 0

#define IDLE_TIMEOUT 30
#define IDLE_PAINT_DELAY 50
#define CLEAR_AT_START true
#define RANDOM_COLOR true

static COLOR_MATRIX ColorMatrix = COLOR_MATRIX();

int count = 0;
int pressedKey = 0;
int idleTime = 0;
BYTE randR = 50;
BYTE randG = 255;
BYTE randB = 50;

void paintNext() {
	if (count == 0) {
		if (CLEAR_AT_START) {
			for (int i = 0; i < S_MAX_LED_COLUMN; i++) {
				limefox_setKeyColor(&ColorMatrix, 0, i, IDLE_R, IDLE_G, IDLE_B);
			}
		}
		if (RANDOM_COLOR) {
			do {
				randR = rand() % 255;
				randG = rand() % 255;
				randB = rand() % 255;
			} while (randR < 200 && randG < 200 && randB < 200);
		}
	}
	if (count == 10 || count == 5) {
		count++;
	}
	limefox_setKeyColor(&ColorMatrix, 0, count, randR, randG, randB);
	count++;
	if (count >= S_MAX_LED_COLUMN) {
		count = 0;
	}
}

void type_meter_callback(int iRow, int iColumn, bool bPressed) {
	if (bPressed) {
		pressedKey++;
		paintNext();
	}
	else {
		pressedKey--;
	}
}

int limefox_type_meter()
{
	srand(time(NULL));
	SetKeyCallBack(type_meter_callback);
	system("cls");
	printf("Type Meter\n");
	while (true) {
		system("cls");
		printf("pressedKey %d", pressedKey);
		SetAllLedColor(ColorMatrix);
		if (pressedKey <= 0) {
			pressedKey = 0;
			idleTime++;
		}
		else {
			idleTime = 0;
		}

		if (idleTime >= IDLE_TIMEOUT) {
			pressedKey = 0;
			while (pressedKey <= 0) {
				paintNext();
				SetAllLedColor(ColorMatrix);
				Sleep(IDLE_PAINT_DELAY);
			}
			idleTime = 0;
		}

		Sleep(50);
		if (_kbhit()) {
			break;
		}
	}
	return 0;
}
