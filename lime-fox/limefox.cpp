// limefox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "limefoxlib.h"
#include "speedmeter.h"
#include "randomvolume.h"
#include "typeowipe.h"
#include "softtouch.h"
#include "typemeter.h"

#define MAX_BYTE = 255;

#define CENTER 0
#define LEFT 1
#define RIGHT 2

#define DECAY 30

const int FPS = 120;
const int DELAY = 1000 / FPS;
const int MAXR = 255;
const int MAXG = 255;
const int MAXB = 255;

int firefly();
int volume();

void setUp();
void reset();

KEY_COLOR keyColor(0,0,0);
COLOR_MATRIX ColorMatrix;
KEYS_MATRIX KeyMatrix;

int main() {
	printf("=====LIME-FOX=====\n\n");
	printf("Press number:\n");
	printf("[1] speedmeter\n");
	printf("[2] firefly\n");
	printf("[3] volume\n");
	printf("[4] random volume\n");
	printf("[5] type o wipe\n");
	printf("[6] soft touch\n");
	printf("[7] type meter\n");

	setUp();

	switch (_getch()) {
		case '1': speedmeter();
			break;
		case '2': firefly();
			break;
		case '3': volume();
			break;
		case '4': limefox_random_volume();
			break;
		case '5': limefox_type_o_wipe();
			break;
		case '6': limefox_soft_touch();
			break;
		default: limefox_type_meter();
			break;
	}

	reset();

	return 0;
}

void callBack(int iRow, int iColumn, bool bPressed) {
	KeyMatrix.Keys[iRow][iColumn].isPressed = bPressed;
	if (bPressed) {
		printf("row:%d;column:%d;press:%d\n", iRow, iColumn, bPressed);
		limefox_setKeyColorWhite(&ColorMatrix, iRow, iColumn, 255);
		SetAllLedColor(ColorMatrix);
	}
}

int firefly() {
	srand(time(NULL));
	SetKeyCallBack(callBack);
	while (true) {
		for (int row = 0; row < S_MAX_LED_ROW; row++) {
			for (int column = 0; column < S_MAX_LED_COLUMN; column++) {
				if (KeyMatrix.Keys[row][column].isPressed) {
					limefox_setKeyColorWhite(&ColorMatrix, row, column, 255);
				}
				if (ColorMatrix.KeyColor[row][column].r == 0) {
					continue;
				}
				int horizontal = rand() % 3;
				int newRow = row, newColumn = column;
				switch (horizontal)
				{
					case CENTER:
						newColumn = column;
						newRow = row - 1;
						break;
					case LEFT:
						newColumn = column - 1;
						newRow = row - 1; 
						break;
					case RIGHT:
						newColumn = column + 1;
						newRow = row - 1;
						break;
					default:
						break;
				}
				BYTE colorTemp = ColorMatrix.KeyColor[row][column].r;
				limefox_setKeyColorWhite(&ColorMatrix, row, column, 0);
				if (newColumn >= 0 && newColumn < S_MAX_LED_COLUMN && newRow >= 0 & newRow < S_MAX_LED_ROW) {
					limefox_setKeyColorWhite(&ColorMatrix, newRow, newColumn, colorTemp);
				}
				if (KeyMatrix.Keys[row][column].isPressed) {
					limefox_setKeyColorWhite(&ColorMatrix, row, column, 255);
				}
			}
		}
		
		if (_kbhit()) {
			break;
		}
		SetAllLedColor(ColorMatrix);
		Sleep(200);
	}  
	return 0;
}

int volume()
{ 
	BYTE expectR = 0, expectG = 0, expectB = 0;
	float volume = 1;
	float oldVolume = 1;
	float maxVolume = 1;
	float minVolume = 1;
	long count = 0;

	while (true) {
		oldVolume = volume;
		volume = GetNowVolumePeekValue();
		if (volume > maxVolume) {
			maxVolume = volume;
		}
		if (volume < minVolume) {
			minVolume = volume;
		}
		if (count % 5 == 0) {
			maxVolume = limefox_average(maxVolume, volume);
			minVolume = limefox_average(minVolume, volume);
		}
		
		printf("Plug: %s\n", IsDevicePlug() ? "true" : "false");
		limefox_volume_visualization("Max:     ", maxVolume);
		limefox_volume_visualization("Current: ", GetNowVolumePeekValue());
		limefox_volume_visualization("Min:     ", minVolume);
		limefox_printRGB(keyColor);

		expectB = limefox_notExceed(maxVolume * 1.5 * MAXB, MAXB);
		expectR = 0;
		expectG = limefox_notExceed(volume * 1.5 * MAXG, MAXG);

		if (expectB >= 254) {
			expectR = ((maxVolume * 1.5 * MAXB) - expectB) * 10;
		}

		keyColor.r = limefox_average(keyColor.r, expectR);
		keyColor.g = limefox_average(keyColor.g, expectG);
		keyColor.b = limefox_average(keyColor.b, expectB);

		SetFullLedColor(keyColor.r, keyColor.g, keyColor.b);

		if (_kbhit()) {
			break;
		}
		Sleep(DELAY);
		system("cls");
		count++;
	}
    return 0;
}

void setUp() {
	SetControlDevice(DEV_MKeys_S);
	EnableLedControl(true);
	EnableKeyInterrupt(true);
}

void reset() {
	EnableLedControl(false);
	EnableKeyInterrupt(false);
	SetKeyCallBack(NULL);
}

