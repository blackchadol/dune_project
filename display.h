/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// ǥ���� ���� ����. ���� ���� �״µ�, ���ⲯ �߰��ϰų� �����ϱ�
#define COLOR_DEFAULT	15
#define COLOR_CURSOR	0xFF
#define COLOR_RESOURCE  0x0F

#define STATUS_WIDTH 40
#define STATUS_HEIGHT 18

#define SYSTEM_MESSAGE_WIDTH 60
#define SYSTEM_MESSAGE_HEIGHT 9

#define COMMAND_WIDTH 40
#define COMMAND_HEIGHT 9

// ������ �ڿ�, ��, Ŀ���� ǥ��
// ������ ȭ�鿡 ǥ���� ����� ���⿡ �߰��ϱ�
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor,
	bool is_cursor_2x2,
	bool was_cursor_2x2
);
void displayUnit(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], POSITION pos, int setColor, int size, int layer, char displayChar);
#endif
void init_colorMap();

void init_status();
void insert_status_message(const char* format, ...);
void init_system_message();
void insert_system_message(const char* format, ...);
void init_command();
void insert_command_message(const char* format, ...);
void setColor(POSITION pos, int color);
void display_system_message();
void display_status();
void display_command();
void display_cursor(CURSOR cursor, bool is_cursor_2x2, bool was_cursor_2x2);
void toDefaultColorEmpty(POSITION pos, char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);