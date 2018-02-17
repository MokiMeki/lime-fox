#include "stdafx.h"
#include "limefoxlib.h"

#define MAXR 255
#define MAXG 255
#define MAXB 255

#define FPS 120

//Variations
#define STICK false

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

		printf("limit: %f\n", limefox_limit(1.3 - volume, 0.6, 0.99));
		for (int i = 0; i < S_MAX_LED_ROW; i++) {
			for (int j = 0; j < S_MAX_LED_COLUMN; j++) {
				ColorMatrix.KeyColor[i][j].r *= limefox_limit(1.3 - volume, 0.6, 0.99);
				ColorMatrix.KeyColor[i][j].g *= limefox_limit(1.3 - volume, 0.6, 0.99);
				ColorMatrix.KeyColor[i][j].b *= limefox_limit(1.3 - volume, 0.6, 0.99);
			}
		}

		if (volume > maxVolume) {
			maxVolume = volume;
			random_color(&ColorMatrix, volume*5, 0, MAXG, MAXB);
		}
		if (volume < minVolume) {
			minVolume = volume;
		}
		maxVolume = limefox_average(maxVolume, volume);
		minVolume = limefox_average(minVolume, volume);
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
		Sleep(DELAY);
		system("cls");
		count++;
	}
	return 0;
}

void random_color(COLOR_MATRIX* ColorMatrix, int times, BYTE r, BYTE g, BYTE b) {
	if (STICK) {
		times = limefox_limit(times*2, 1, 10);
		int row = rand() % S_MAX_LED_ROW;
		int column = rand() % S_MAX_LED_COLUMN;
		limefox_setKeyColor(ColorMatrix, row, column, r, g, b);

		//random position around current
		for (int i = 0; i < times; i++) {
			int rowOffset = rand() % 3 - 1;
			int columnOffset = rand() % 3 - 1;
			int resultRow = limefox_limit(row + rowOffset, 0, S_MAX_LED_ROW - 1);
			int resultColumn = limefox_limit(column + columnOffset, 0, S_MAX_LED_COLUMN - 1);
			row = resultRow;
			column = resultColumn;
			limefox_setKeyColor(ColorMatrix, resultRow, resultColumn, r *= 0.8, g *= 0.8, b *= 0.8);
		}
	}
	else {
		times = limefox_limit(times, 1, 5);
		for (int i = 0; i < times; i++) {
			int row = rand() % S_MAX_LED_ROW;
			int column = rand() % S_MAX_LED_COLUMN;
			limefox_setKeyColor(ColorMatrix, row, column, r, g, b);
		}
	}
}
