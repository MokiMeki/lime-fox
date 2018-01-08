#include "stdafx.h"
#include "limefoxlib.h"

#define S_MAX_LED_ROW		6
#define S_MAX_LED_COLUMN	18

#define MAXR 255
#define MAXG 255
#define MAXB 255

#define FPS 120;

const long DELAY = 1000 / FPS;

void random_color(COLOR_MATRIX* ColorMatrix, int times, BYTE r, BYTE g, BYTE b);

int limefox_random_volume() {
	COLOR_MATRIX ColorMatrix = COLOR_MATRIX();
	BYTE expectR = 0, expectG = 0, expectB = 0;
	float volume = 1;
	float oldVolume = 1;
	float maxVolume = 1;
	float minVolume = 1;
	long count = 0;

	for (int i = 0; i < S_MAX_LED_ROW; i++) {
		for (int j = 0; j < S_MAX_LED_COLUMN; j++) {
			limefox_setKeyColorWhite(&ColorMatrix, i, j, 0);
		}
	}

	srand(time(NULL));

	while (true) {
		oldVolume = volume;
		volume = GetNowVolumePeekValue();

		for (int i = 0; i < S_MAX_LED_ROW; i++) {
			for (int j = 0; j < S_MAX_LED_COLUMN; j++) {
				ColorMatrix.KeyColor[i][j].r *= 0.85;
				ColorMatrix.KeyColor[i][j].g *= 0.85;
				ColorMatrix.KeyColor[i][j].b *= 0.85;
			}
		}

		if (volume > maxVolume) {
			maxVolume = volume;
			random_color(&ColorMatrix, volume*10, 0, MAXG, MAXB);
		}
		if (volume < minVolume) {
			minVolume = volume;
		}
		maxVolume = limefox_average(maxVolume, volume);
		minVolume = limefox_average(minVolume, volume);
		printf("volume %f \n", volume);
		printf("Plug: %s\n", IsDevicePlug() ? "true" : "false");
		limefox_volume_visualization("Max:     ", maxVolume);
		limefox_volume_visualization("Current: ", GetNowVolumePeekValue());
		limefox_volume_visualization("Min:     ", minVolume);

		for (int i = 0; i < S_MAX_LED_ROW; i++) {
			for (int j = 0; j < S_MAX_LED_COLUMN; j++) {
				printf("%3d ",ColorMatrix.KeyColor[i][j].b);
			}
			printf("\n");
		}

		SetAllLedColor(ColorMatrix);

		if (_kbhit()) {
			break;
		}
		//Sleep(DELAY);
		system("cls");
		count++;
	}
	return 0;
}

void random_color(COLOR_MATRIX* ColorMatrix, int times, BYTE r, BYTE g, BYTE b) {
	for (int i = 0; i < times; i++) {
		int row = rand() % S_MAX_LED_ROW;
		int column = rand() % S_MAX_LED_COLUMN;

		limefox_setKeyColor(ColorMatrix, row, column, r, g, b);
	}
}
