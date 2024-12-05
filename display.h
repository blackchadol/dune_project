/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// 표시할 색상 정의. 대충 맞춰 뒀는데, 취향껏 추가하거나 변경하기
#define COLOR_DEFAULT	15
#define COLOR_CURSOR	0xFF
#define COLOR_RESOURCE  0x0F

#define STATUS_WIDTH 40
#define STATUS_HEIGHT 18

#define SYSTEM_MESSAGE_WIDTH 60
#define SYSTEM_MESSAGE_HEIGHT 9

#define COMMAND_WIDTH 40
#define COMMAND_HEIGHT 9

// 지금은 자원, 맵, 커서만 표시
// 앞으로 화면에 표시할 내용들 여기에 추가하기
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