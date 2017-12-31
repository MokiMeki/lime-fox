// limefox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CMSDK\SDKDLL.h"
#include "stdio.h"
#include "conio.h"
#include <windows.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define S_MAX_LED_ROW		6
#define S_MAX_LED_COLUMN	18

#define CENTER 0
#define LEFT 1
#define RIGHT 2

#define DECAY 30

const int FPS = 60;
const int DELAY = 1000 / FPS;
const int MAXR = 0;
const int MAXG = 125;
const int MAXB = 255;

struct KEY {
	bool isPressed;
};

struct KEYS_MATRIX {
	KEY Keys[S_MAX_LED_ROW][S_MAX_LED_COLUMN];
};

int firefly();
int volume();


void setUp();
void reset();
BYTE average(BYTE a, BYTE b);
float average(float a, float b);
float safeDivide(float a, float b);
void volumeVisualization(const char* text, float volume);
void setKeyColorWhite(COLOR_MATRIX * colorMatrix, int row, int column, BYTE rgb);
void setKeyColor(COLOR_MATRIX * colorMatrix, int row, int column, BYTE r, BYTE g, BYTE b);
void printRGB();
KEY_COLOR keyColor(0,0,0);
COLOR_MATRIX ColorMatrix;
KEYS_MATRIX KeyMatrix;

int main() {
	//return firefly();
	return volume();
}

void callBack(int iRow, int iColumn, bool bPressed) {
	KeyMatrix.Keys[iRow][iColumn].isPressed = bPressed;
	if (bPressed) {
		printf("row:%d;column:%d;press:%d\n", iRow, iColumn, bPressed);
		setKeyColorWhite(&ColorMatrix, iRow, iColumn, 255);
		SetAllLedColor(ColorMatrix);
	}
}

int firefly() {
	srand(time(NULL));
	setUp();
	SetKeyCallBack(callBack);
	while (true) {
		for (int row = 0; row < S_MAX_LED_ROW; row++) {
			for (int column = 0; column < S_MAX_LED_COLUMN; column++) {
				if (KeyMatrix.Keys[row][column].isPressed) {
					setKeyColorWhite(&ColorMatrix, row, column, 255);
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
				setKeyColorWhite(&ColorMatrix, row, column, 0);
				if (newColumn >= 0 && newColumn < S_MAX_LED_COLUMN && newRow >= 0 & newRow < S_MAX_LED_ROW) {
					setKeyColorWhite(&ColorMatrix, newRow, newColumn, colorTemp);
				}
				if (KeyMatrix.Keys[row][column].isPressed) {
					setKeyColorWhite(&ColorMatrix, row, column, 255);
				}
			}
		}
		
		if (_kbhit()) {
			break;
		}
		SetAllLedColor(ColorMatrix);
		Sleep(200);
	}  
	reset();
	return 0;
}

int volume()
{ 
	BYTE expectR, expectG, expectB;
	float volume = 1;
	float oldVolume = 1;
	float maxVolume = 1;
	float minVolume = 1;
	long count = 0;

	setUp();

	while (true) {
		oldVolume = volume;
		volume = GetNowVolumePeekValue();
		if (volume > maxVolume) {
			maxVolume = volume;
		}
		if (volume < minVolume) {
			minVolume = volume;
		}
		if (count % 30 == 0) {
			maxVolume = average(maxVolume, volume);
			minVolume = average(minVolume, volume);
		}
		
		printf("Plug: %s\n", IsDevicePlug() ? "true" : "false");
		volumeVisualization("Max:     ", maxVolume);
		volumeVisualization("Current: ", GetNowVolumePeekValue());
		volumeVisualization("Min:     ", minVolume);
		printRGB();
		//printf("Count: %d\n", count);

		expectR = safeDivide(maxVolume, minVolume) * volume * MAXR;
		expectG = safeDivide(maxVolume, minVolume) * volume * MAXG;
		expectB = safeDivide(maxVolume, minVolume) * volume * MAXB;

		keyColor.r = average(keyColor.r, expectR);
		keyColor.g = average(keyColor.g, expectG);
		keyColor.b = average(keyColor.b, expectB);

		SetFullLedColor(keyColor.r, keyColor.g, keyColor.b);

		if (_kbhit()) {
			break;
		}
		Sleep(DELAY);
		system("cls");
		count++;
	}
	reset();
    return 0;
}

void setKeyColorWhite(COLOR_MATRIX* colorMatrix, int row, int column, BYTE rgb) {
	setKeyColor(colorMatrix, row, column, rgb, rgb, rgb);
}

void setKeyColor(COLOR_MATRIX* colorMatrix, int row, int column, BYTE r, BYTE g, BYTE b) {
	(*colorMatrix).KeyColor[row][column].r = r;
	(*colorMatrix).KeyColor[row][column].g = g;
	(*colorMatrix).KeyColor[row][column].b = b;
}

void printRGB() {
	printf("R: %d;G: %d;B: %d\n", keyColor.r, keyColor.g, keyColor.b);
}

void volumeVisualization(const char* text, float volume) {
	float step = 0.025;
	float max = 1;
	float count = step;
	printf(text);
	while (count < max && volume > count) {
		printf("|");
		count += step;
	}
	printf("\n");
}


float safeDivide(float a, float b) {
	if (b == 0) {
		b = 0.000001;
	}
	return a / b;
}


BYTE average(BYTE a, BYTE b) {
	return (a + b) / 2;
}

float average(float a, float b) {
	return (a + b) / 2;
}

void setUp() {
	SetControlDevice(DEV_MKeys_S);
	EnableLedControl(true);
	EnableKeyInterrupt(true);
}

void reset() {
	EnableLedControl(false);
	EnableKeyInterrupt(false);
}
