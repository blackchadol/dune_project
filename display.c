/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"
#include <stdarg.h>

int colorMap [MAP_HEIGHT][MAP_WIDTH]; // 각 레이어의 색상 초기화
void init_colorMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			colorMap[i][j] = COLOR_DEFAULT;
		}
	}
}

char status_window[STATUS_HEIGHT][STATUS_WIDTH];  // 마지막은 문자열 끝을 위한 
char status_frontbuf[STATUS_HEIGHT][STATUS_WIDTH] = { 0 };  // 상태창 프론트버퍼
char status_backbuf[STATUS_HEIGHT][STATUS_WIDTH] = { 0 };   // 상태창 백버퍼

char system_message[SYSTEM_MESSAGE_HEIGHT][SYSTEM_MESSAGE_WIDTH];
char system_message_frontbuf[SYSTEM_MESSAGE_HEIGHT][SYSTEM_MESSAGE_WIDTH] = { 0 };
char system_message_backbuf[SYSTEM_MESSAGE_HEIGHT][SYSTEM_MESSAGE_WIDTH] = { 0 };

char command_message[COMMAND_HEIGHT][COMMAND_WIDTH];
char command_message_frontbuf[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };
char command_message_backbuf[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };


// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor, bool is_cursor_2x2);
void display_status();
void init_status();
void insert_status_message(const char* format, ...);
void init_system_message();
void display_system_message();
void insert_system_message(const char* format, ...);
void init_command();
void display_command();
void insert_command_message(const char* format, ...);
void setColor(POSITION pos, int color);
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor,
	bool is_cursor_2x2)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor, is_cursor_2x2);
	display_system_message();
	display_status();
	display_command();
	// ...
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}

void displayUnit(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], POSITION pos, int setColor, int size, int layer, char displayChar) {
	// 레이어가 유효한지 확인
	if (layer < 0 || layer > 1) {
		printf("Invalid layer: %d\n", layer);
		return;
	}
	// 크기에 따라 유닛 표시
	if (size == 2) {

		// 2x2 크기 유닛
		if (pos.row < MAP_HEIGHT - 1 && pos.column < MAP_WIDTH - 1) {
			map[layer][pos.row][pos.column] = displayChar; // 좌측 상단 표시
			map[layer][pos.row + 1][pos.column] = '.'; // 아래쪽 칸 비우기
			map[layer][pos.row][pos.column + 1] = '.'; // 우측 칸 비우기
			map[layer][pos.row + 1][pos.column + 1] = '.'; // 우측 하단 비우기
			
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					colorMap[pos.row+i][pos.column+j] = setColor;
				}
			}
	

		}
	
	else { // size == 1
		// 1x1 크기 유닛
		if (pos.row < MAP_HEIGHT && pos.column < MAP_WIDTH) {
			map[layer][pos.row][pos.column] = displayChar; // 단일 칸 표시
			colorMap[pos.row][pos.column] = setColor;
		}
	}

	
}

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				POSITION pos = { i, j };
				int color = colorMap[i][j];

				printc(padd(map_pos, pos), backbuf[i][j], color);
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}

// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
// 2x2 커서 표시 함수
void display_cursor(CURSOR cursor, bool is_cursor_2x2) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	// 기존 커서 영역 복구 (1x1 또는 2x2 모두 지원)
	if (is_cursor_2x2) {
		// 2x2 커서 지우기
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				POSITION prev_cell = { prev.row + i, prev.column + j };
				if (prev_cell.row < MAP_HEIGHT && prev_cell.column < MAP_WIDTH) {
					char ch = frontbuf[prev_cell.row][prev_cell.column];
					printc(padd(map_pos, prev_cell), ch, colorMap[prev_cell.row][prev_cell.column]);
				}
			}
		}
	}
	else {
		// 기존 1x1 커서 복구
		char ch = frontbuf[prev.row][prev.column];
		printc(padd(map_pos, prev), ch, colorMap[prev.row][prev.column]);
	}

	// 새로운 커서 출력
	if (is_cursor_2x2) {
		// 2x2 커서 표시
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				POSITION curr_cell = { curr.row + i, curr.column + j };
				if (curr_cell.row < MAP_HEIGHT && curr_cell.column < MAP_WIDTH) {
					char ch = frontbuf[curr_cell.row][curr_cell.column];
					printc(padd(map_pos, curr_cell), ch, COLOR_CURSOR);
				}
			}
		}
	}
	else {
		// 기존 1x1 커서 표시
		char ch = frontbuf[curr.row][curr.column];
		printc(padd(map_pos, curr), ch, COLOR_CURSOR);
	}
}

void init_status() {
	// 상태창 배열 초기화 (공백 문자로 초기화)

		// 상태창 테두리 설정
		for (int i = 0; i < STATUS_HEIGHT; i++) {
			for (int j = 0; j < STATUS_WIDTH; j++) {
				// 상단, 하단, 좌우 테두리
				if (i == 0 || i == STATUS_HEIGHT - 1 || j == 0 || j == STATUS_WIDTH -1) {
					status_window[i][j] = '#';  // 상태창 테두리 '#'
				}
				else {
					// 내부는 공백으로 채움
					status_window[i][j] = ' ';
				}
			}
		}
	
}

void display_status() {
	// 상태창을 한 번에 출력, 변경된 부분만 갱신
	for (int i = 0; i < STATUS_HEIGHT; i++) {
		for (int j = 0; j < STATUS_WIDTH; j++) {

			if (status_window[i][j] >= 0) {
				status_backbuf[i][j] = status_window[i][j];
			}
		}
	}


	for (int i = 0; i < STATUS_HEIGHT; i++) {
		for (int j = 0; j < STATUS_WIDTH; j++) {
			
			if (status_frontbuf[i][j] != status_backbuf[i][j]) {
				POSITION pos = { i + 1, j + 60 };
				printc(pos, status_backbuf[i][j], COLOR_DEFAULT);  // 상태창에 색상 적용
			}
			status_frontbuf[i][j] = status_backbuf[i][j];  // 변경된 부분만 갱신된 상태로 저장
		}
	}

	
}

void insert_status_message(const char* format, ...) {
	va_list args;
	va_start(args, format);

	// 상태창이 꽉 차면, 내부 메시지만 위로 밀기
	for (int i = 1; i < STATUS_HEIGHT - 2; i++) {
		memcpy(&status_window[i][1], &status_window[i + 1][1], STATUS_WIDTH - 2);
		// 테두리를 제외하고 내부 메시지만 이동
	}

	// 새 메시지가 들어갈 줄의 내부를 공백으로 초기화
	memset(&status_window[STATUS_HEIGHT - 2][1], ' ', STATUS_WIDTH - 2);

	// 새로운 메시지 추가 (맨 밑 내부에)
	int message_length = vsnprintf(&status_window[STATUS_HEIGHT - 2][1], STATUS_WIDTH - 2, format, args);
	va_end(args);

	// 메시지 끝 이후 남은 공간을 공백으로 채우기
	if (message_length < STATUS_WIDTH - 2) {
		memset(&status_window[STATUS_HEIGHT - 2][1 + message_length], ' ', STATUS_WIDTH - 2 - message_length);
	}
}




void init_system_message() {
	// 상태창 테두리 설정
	for (int i = 0; i < SYSTEM_MESSAGE_HEIGHT; i++) {
		for (int j = 0; j < SYSTEM_MESSAGE_WIDTH; j++) {
			// 상단, 하단, 좌우 테두리
			if (i == 0 || i == SYSTEM_MESSAGE_HEIGHT - 1 || j == 0 || j == SYSTEM_MESSAGE_WIDTH - 1) {
				system_message[i][j] = '#';  // 상태창 테두리 '#'
			}
			else {
				// 내부는 공백으로 채움
				system_message[i][j] = ' ';
			}
		}
	}

}
	
void display_system_message() {
	// 상태창을 한 번에 출력, 변경된 부분만 갱신
	for (int i = 0; i < SYSTEM_MESSAGE_HEIGHT; i++) {
		for (int j = 0; j < SYSTEM_MESSAGE_WIDTH; j++) {

			if (system_message[i][j] >= 0) {
				system_message_backbuf[i][j] = system_message[i][j];
			}
		}
	}


	for (int i = 0; i < SYSTEM_MESSAGE_HEIGHT; i++) {
		for (int j = 0; j < SYSTEM_MESSAGE_WIDTH; j++) {
			if (system_message_frontbuf[i][j] != system_message_backbuf[i][j]) {
				POSITION pos = { i + 21, j};
				printc(pos, system_message_backbuf[i][j], COLOR_DEFAULT);  // 상태창에 색상 적용
			}
			system_message_frontbuf[i][j] = system_message_backbuf[i][j];  // 변경된 부분만 갱신된 상태로 저장
		}
	}


}

void insert_system_message(const char* format, ...) {
	va_list args;
	va_start(args, format);  // 가변 인자 목록 시작

	// 시스템 메시지창이 꽉 차면, 맨 위의 메시지를 제거하고 나머지 메시지를 위로 밀기
	for (int i = 1; i < SYSTEM_MESSAGE_HEIGHT - 2; i++) {
		// 각 줄을 한 줄씩 위로 밀어냄
		memcpy(&system_message[i][1], &system_message[i + 1][1], SYSTEM_MESSAGE_WIDTH - 2);
	}

	// 새로운 메시지 추가 (맨 밑에)
	int message_length = vsnprintf(&system_message[SYSTEM_MESSAGE_HEIGHT - 2][1], SYSTEM_MESSAGE_WIDTH - 2, format, args);

	va_end(args);  // 가변 인자 목록 처리 끝

	// 메시지 끝 이후 남은 공간을 공백으로 채우기
	if (message_length < SYSTEM_MESSAGE_WIDTH - 2) {
		memset(&system_message[SYSTEM_MESSAGE_HEIGHT - 2][1 + message_length], ' ', SYSTEM_MESSAGE_WIDTH - 2 - message_length);
	}
}


void init_command() {
	// 명령창 테두리 설정
	for (int i = 0; i < COMMAND_HEIGHT; i++) {
		for (int j = 0; j < COMMAND_WIDTH; j++) {
			// 상단, 하단, 좌우 테두리
			if (i == 0 || i == COMMAND_HEIGHT - 1 || j == 0 || j == COMMAND_WIDTH - 1) {
				command_message[i][j] = '#';  // 명령창 테두리 '#'
			}
			else {
				// 내부는 공백으로 채움
				command_message[i][j] = ' ';
			}
		}
	}
}

void display_command() {
	// 명령창을 한 번에 출력, 변경된 부분만 갱신
	for (int i = 0; i < COMMAND_HEIGHT; i++) {
		for (int j = 0; j < COMMAND_WIDTH; j++) {
			if (command_message[i][j] >= 0) {
				command_message_backbuf[i][j] = command_message[i][j];
			}
		}
	}

	for (int i = 0; i < COMMAND_HEIGHT; i++) {
		for (int j = 0; j < COMMAND_WIDTH; j++) {
			if (command_message_frontbuf[i][j] != command_message_backbuf[i][j]) {
				POSITION pos = { i + 21, j + 61 };
				printc(pos, command_message_backbuf[i][j], COLOR_DEFAULT);  // 명령창에 색상 적용
			}
			command_message_frontbuf[i][j] = command_message_backbuf[i][j];  // 변경된 부분만 갱신된 상태로 저장
		}
	}
}

void insert_command_message(const char* format, ...) {
	va_list args;
	va_start(args, format); // 가변 인자 목록 시작
	char buffer[COMMAND_WIDTH * 2]; // 너비보다 넉넉한 크기 설정
	// 명령창이 꽉 차면, 내부 메시지만 위로 밀기
	for (int i = 1; i < COMMAND_HEIGHT - 2; i++) {
		memcpy(&command_message[i][1], &command_message[i + 1][1], COMMAND_WIDTH - 2);
		// 테두리를 제외하고 내부 메시지만 이동
	}

	// 새 메시지가 들어갈 줄의 내부를 공백으로 초기화
	memset(&command_message[COMMAND_HEIGHT - 2][1], ' ', COMMAND_WIDTH - 2);

	// 새로운 메시지 추가 (맨 밑 내부에)
	int message_length = vsnprintf(&command_message[COMMAND_HEIGHT - 2][1], COMMAND_WIDTH - 2, format, args);
	va_end(args); // 가변 인자 목록 처리 끝

	// 메시지 끝 이후 남은 공간을 공백으로 채우기
	if (message_length < COMMAND_WIDTH - 2) {
		memset(&command_message[COMMAND_HEIGHT - 2][1 + message_length], ' ', COMMAND_WIDTH - 2 - message_length);
	}
}



void setColor(POSITION pos , int color) {
	colorMap[pos.row][pos.column] = color;
}