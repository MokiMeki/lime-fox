#include "stdafx.h"
#include "sys\timeb.h"

#define S_MAX_LED_ROW		6
#define S_MAX_LED_COLUMN	18

#define FPS 120
#define DELAY 1000 / FPS

#define EFFECT_ROWS {0,1,2,3,4,5}

static float average(float a, float b);
static void setUp();
static void reset();

int keyPressed = 0, speed = 0; //speed = key per sec
time_t firstKeyStreamedTime, lastKeyPressedTime, currentTime;

static void callBack(int iRow, int iColumn, bool bPressed) {
	if (bPressed) {
		keyPressed++;
		lastKeyPressedTime = time(NULL);
		if (firstKeyStreamedTime == 0) {
			firstKeyStreamedTime = time(NULL);
		}
	}
}

int speedmeter() {	
	setUp();
	SetKeyCallBack(callBack);
	while (true) {
		currentTime = time(NULL);
		if (currentTime - lastKeyPressedTime > 1) {
			keyPressed = 0;
			firstKeyStreamedTime = 0;
		}
		if (currentTime - firstKeyStreamedTime != 0) {
			speed = average(speed, keyPressed / (currentTime - firstKeyStreamedTime));
		}
		printf("FirstKey Time: %I64d\n", firstKeyStreamedTime);
		printf("Current Time: %I64d\n", currentTime);
		printf("LastKey Time: %I64d\n", lastKeyPressedTime);
		printf("Speed: %d\n", speed);

		if (_kbhit()) {
			break;
		}
		Sleep(DELAY);
		system("cls");
	}
	

	reset();
	return 0;
}

static float average(float a, float b) {
	return (a + b) / 2;
}

static void setUp() {
	SetControlDevice(DEV_MKeys_S);
	EnableLedControl(true);
	EnableKeyInterrupt(true);
}

static void reset() {
	EnableLedControl(false);
	EnableKeyInterrupt(false);
}