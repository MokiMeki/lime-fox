#include "stdafx.h"
#include "sys\timeb.h"
#include "limefoxlib.h"

#define FPS 240
#define DELAY 1000 / FPS

static boolean timeout = false;
static long count = 0;
static long lastKeyCount = 0;
static long lastKeyCount2 = 0;
static long lastKeyCount3 = 0;
static BYTE color = 0;
//static COLOR_MATRIX ColorMatrix = COLOR_MATRIX();


static void callBack(int iRow, int iColumn, bool bPressed) {
	if (bPressed) {
		long averageCount = (count + lastKeyCount + lastKeyCount2 + lastKeyCount3) / 4;
		long divide = limefox_limit(limefox_safeDivide(255, averageCount/4), 0, 255);
		color = limefox_average_with_factor((float)color, divide, 2);
		timeout = false;
		lastKeyCount3 = lastKeyCount2;
		lastKeyCount2 = lastKeyCount;
		lastKeyCount = count;
		count = 0;
	}
}

int speedmeter() {	
	SetKeyCallBack(callBack);
	while (true) {
		if (!timeout) {
			count++;
		}
		if (count > 50) {
			timeout = true;
			count = 0;
		}
		if (color > 0){
			color = limefox_limit(color - 3, 0, 255);
		}
		if (_kbhit()) {
			break;
		}
		SetFullLedColor(limefox_limit(color,0,255), limefox_limit(200 - limefox_limit(color/2,0,200), 0, 255), limefox_limit(200 - limefox_limit(color/2,0,200), 0, 255));
		Sleep(DELAY);
		system("cls");
	}
	
	return 0;
}