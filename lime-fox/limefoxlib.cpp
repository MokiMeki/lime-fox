#include "stdafx.h"

void limefox_volume_visualization(const char* text, float volume) {
	float step = 0.025;
	float max = 1;
	float count = step;
	printf(text);
	printf("[");
	while (count < max) {
		if (volume > count) {
			printf("|");
		}
		else {
			printf(" ");
		}
		count += step;
	}
	printf("]\n");
}

BYTE limefox_average(BYTE a, BYTE b) {
	return (a + b) / 2;
}

float limefox_average(float a, float b) {
	return (a + b) / 2;
}

void limefox_printRGB(KEY_COLOR keyColor) {
	printf("R: %3d;G: %3d;B: %3d\n", keyColor.r, keyColor.g, keyColor.b);
}

float limefox_notExceed(float expression, float notExceed) {
	if (expression > notExceed) {
		return notExceed;
	}
	return expression;
}

float limefox_safeDivide(float a, float b) {
	if (b == 0) {
		b = 0.000001;
	}
	return a / b;
}

void limefox_setKeyColor(COLOR_MATRIX* colorMatrix, int row, int column, BYTE r, BYTE g, BYTE b) {
	(*colorMatrix).KeyColor[row][column].r = r;
	(*colorMatrix).KeyColor[row][column].g = g;
	(*colorMatrix).KeyColor[row][column].b = b;
}

void limefox_setKeyColorWhite(COLOR_MATRIX* colorMatrix, int row, int column, BYTE rgb) {
	limefox_setKeyColor(colorMatrix, row, column, rgb, rgb, rgb);
}
