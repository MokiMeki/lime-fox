#pragma once
void limefox_volume_visualization(const char* text, float volume);

BYTE limefox_average(BYTE a, BYTE b);
float limefox_average(float a, float b);

void limefox_printRGB(KEY_COLOR keyColor);

float limefox_notExceed(float expression, float notExceed);

float limefox_safeDivide(float a, float b);

void limefox_setKeyColorWhite(COLOR_MATRIX* colorMatrix, int row, int column, BYTE rgb);

void limefox_setKeyColor(COLOR_MATRIX* colorMatrix, int row, int column, BYTE r, BYTE g, BYTE b);

float limefox_limit(float expression, float min, float max);