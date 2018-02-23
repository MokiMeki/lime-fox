#pragma once

#define S_MAX_LED_ROW		6
#define S_MAX_LED_COLUMN	18

struct KEY {
	bool isPressed;
};

struct KEYS_MATRIX {
	KEY Keys[S_MAX_LED_ROW][S_MAX_LED_COLUMN];
};

void limefox_volume_visualization(const char* text, float volume);

BYTE limefox_average_with_factor(BYTE a, BYTE b, float aFactor);
BYTE limefox_average(BYTE a, BYTE b);
float limefox_average(float a, float b);
long limefox_average(long a, long b);

void limefox_printRGB(KEY_COLOR keyColor);

float limefox_notExceed(float expression, float notExceed);

float limefox_safeDivide(float a, float b);

void limefox_setKeyColorWhite(COLOR_MATRIX* colorMatrix, int row, int column, BYTE rgb);

void limefox_setKeyColor(COLOR_MATRIX* colorMatrix, int row, int column, BYTE r, BYTE g, BYTE b);

float limefox_limit(float expression, float min, float max);

int limefox_overflow(int expression, int max);