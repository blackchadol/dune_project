/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"
#include <stdarg.h>

int colorMap [MAP_HEIGHT][MAP_WIDTH]; // �� ���̾��� ���� �ʱ�ȭ
void init_colorMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			colorMap[i][j] = COLOR_DEFAULT;
		}
	}
}

char status_window[STATUS_HEIGHT][STATUS_WIDTH];  // �������� ���ڿ� ���� ���� 
char status_frontbuf[STATUS_HEIGHT][STATUS_WIDTH] = { 0 };  // ����â ����Ʈ����
char status_backbuf[STATUS_HEIGHT][STATUS_WIDTH] = { 0 };   // ����â �����

char system_message[SYSTEM_MESSAGE_HEIGHT][SYSTEM_MESSAGE_WIDTH];
char system_message_frontbuf[SYSTEM_MESSAGE_HEIGHT][SYSTEM_MESSAGE_WIDTH] = { 0 };
char system_message_backbuf[SYSTEM_MESSAGE_HEIGHT][SYSTEM_MESSAGE_WIDTH] = { 0 };

char command_message[COMMAND_HEIGHT][COMMAND_WIDTH];
char command_message_frontbuf[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };
char command_message_backbuf[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };


// ����� ������� �»��(topleft) ��ǥ
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
	// ���̾ ��ȿ���� Ȯ��
	if (layer < 0 || layer > 1) {
		printf("Invalid layer: %d\n", layer);
		return;
	}
	// ũ�⿡ ���� ���� ǥ��
	if (size == 2) {

		// 2x2 ũ�� ����
		if (pos.row < MAP_HEIGHT - 1 && pos.column < MAP_WIDTH - 1) {
			map[layer][pos.row][pos.column] = displayChar; // ���� ��� ǥ��
			map[layer][pos.row + 1][pos.column] = '.'; // �Ʒ��� ĭ ����
			map[layer][pos.row][pos.column + 1] = '.'; // ���� ĭ ����
			map[layer][pos.row + 1][pos.column + 1] = '.'; // ���� �ϴ� ����
			
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					colorMap[pos.row+i][pos.column+j] = setColor;
				}
			}
	

		}
	
	else { // size == 1
		// 1x1 ũ�� ����
		if (pos.row < MAP_HEIGHT && pos.column < MAP_WIDTH) {
			map[layer][pos.row][pos.column] = displayChar; // ���� ĭ ǥ��
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

// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
// 2x2 Ŀ�� ǥ�� �Լ�
void display_cursor(CURSOR cursor, bool is_cursor_2x2) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	// ���� Ŀ�� ���� ���� (1x1 �Ǵ� 2x2 ��� ����)
	if (is_cursor_2x2) {
		// 2x2 Ŀ�� �����
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
		// ���� 1x1 Ŀ�� ����
		char ch = frontbuf[prev.row][prev.column];
		printc(padd(map_pos, prev), ch, colorMap[prev.row][prev.column]);
	}

	// ���ο� Ŀ�� ���
	if (is_cursor_2x2) {
		// 2x2 Ŀ�� ǥ��
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
		// ���� 1x1 Ŀ�� ǥ��
		char ch = frontbuf[curr.row][curr.column];
		printc(padd(map_pos, curr), ch, COLOR_CURSOR);
	}
}

void init_status() {
	// ����â �迭 �ʱ�ȭ (���� ���ڷ� �ʱ�ȭ)

		// ����â �׵θ� ����
		for (int i = 0; i < STATUS_HEIGHT; i++) {
			for (int j = 0; j < STATUS_WIDTH; j++) {
				// ���, �ϴ�, �¿� �׵θ�
				if (i == 0 || i == STATUS_HEIGHT - 1 || j == 0 || j == STATUS_WIDTH -1) {
					status_window[i][j] = '#';  // ����â �׵θ� '#'
				}
				else {
					// ���δ� �������� ä��
					status_window[i][j] = ' ';
				}
			}
		}
	
}

void display_status() {
	// ����â�� �� ���� ���, ����� �κи� ����
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
				printc(pos, status_backbuf[i][j], COLOR_DEFAULT);  // ����â�� ���� ����
			}
			status_frontbuf[i][j] = status_backbuf[i][j];  // ����� �κи� ���ŵ� ���·� ����
		}
	}

	
}

void insert_status_message(const char* format, ...) {
	va_list args;
	va_start(args, format);

	// ����â�� �� ����, ���� �޽����� ���� �б�
	for (int i = 1; i < STATUS_HEIGHT - 2; i++) {
		memcpy(&status_window[i][1], &status_window[i + 1][1], STATUS_WIDTH - 2);
		// �׵θ��� �����ϰ� ���� �޽����� �̵�
	}

	// �� �޽����� �� ���� ���θ� �������� �ʱ�ȭ
	memset(&status_window[STATUS_HEIGHT - 2][1], ' ', STATUS_WIDTH - 2);

	// ���ο� �޽��� �߰� (�� �� ���ο�)
	int message_length = vsnprintf(&status_window[STATUS_HEIGHT - 2][1], STATUS_WIDTH - 2, format, args);
	va_end(args);

	// �޽��� �� ���� ���� ������ �������� ä���
	if (message_length < STATUS_WIDTH - 2) {
		memset(&status_window[STATUS_HEIGHT - 2][1 + message_length], ' ', STATUS_WIDTH - 2 - message_length);
	}
}




void init_system_message() {
	// ����â �׵θ� ����
	for (int i = 0; i < SYSTEM_MESSAGE_HEIGHT; i++) {
		for (int j = 0; j < SYSTEM_MESSAGE_WIDTH; j++) {
			// ���, �ϴ�, �¿� �׵θ�
			if (i == 0 || i == SYSTEM_MESSAGE_HEIGHT - 1 || j == 0 || j == SYSTEM_MESSAGE_WIDTH - 1) {
				system_message[i][j] = '#';  // ����â �׵θ� '#'
			}
			else {
				// ���δ� �������� ä��
				system_message[i][j] = ' ';
			}
		}
	}

}
	
void display_system_message() {
	// ����â�� �� ���� ���, ����� �κи� ����
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
				printc(pos, system_message_backbuf[i][j], COLOR_DEFAULT);  // ����â�� ���� ����
			}
			system_message_frontbuf[i][j] = system_message_backbuf[i][j];  // ����� �κи� ���ŵ� ���·� ����
		}
	}


}

void insert_system_message(const char* format, ...) {
	va_list args;
	va_start(args, format);  // ���� ���� ��� ����

	// �ý��� �޽���â�� �� ����, �� ���� �޽����� �����ϰ� ������ �޽����� ���� �б�
	for (int i = 1; i < SYSTEM_MESSAGE_HEIGHT - 2; i++) {
		// �� ���� �� �پ� ���� �о
		memcpy(&system_message[i][1], &system_message[i + 1][1], SYSTEM_MESSAGE_WIDTH - 2);
	}

	// ���ο� �޽��� �߰� (�� �ؿ�)
	int message_length = vsnprintf(&system_message[SYSTEM_MESSAGE_HEIGHT - 2][1], SYSTEM_MESSAGE_WIDTH - 2, format, args);

	va_end(args);  // ���� ���� ��� ó�� ��

	// �޽��� �� ���� ���� ������ �������� ä���
	if (message_length < SYSTEM_MESSAGE_WIDTH - 2) {
		memset(&system_message[SYSTEM_MESSAGE_HEIGHT - 2][1 + message_length], ' ', SYSTEM_MESSAGE_WIDTH - 2 - message_length);
	}
}


void init_command() {
	// ���â �׵θ� ����
	for (int i = 0; i < COMMAND_HEIGHT; i++) {
		for (int j = 0; j < COMMAND_WIDTH; j++) {
			// ���, �ϴ�, �¿� �׵θ�
			if (i == 0 || i == COMMAND_HEIGHT - 1 || j == 0 || j == COMMAND_WIDTH - 1) {
				command_message[i][j] = '#';  // ���â �׵θ� '#'
			}
			else {
				// ���δ� �������� ä��
				command_message[i][j] = ' ';
			}
		}
	}
}

void display_command() {
	// ���â�� �� ���� ���, ����� �κи� ����
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
				printc(pos, command_message_backbuf[i][j], COLOR_DEFAULT);  // ���â�� ���� ����
			}
			command_message_frontbuf[i][j] = command_message_backbuf[i][j];  // ����� �κи� ���ŵ� ���·� ����
		}
	}
}

void insert_command_message(const char* format, ...) {
	va_list args;
	va_start(args, format); // ���� ���� ��� ����
	char buffer[COMMAND_WIDTH * 2]; // �ʺ񺸴� �˳��� ũ�� ����
	// ���â�� �� ����, ���� �޽����� ���� �б�
	for (int i = 1; i < COMMAND_HEIGHT - 2; i++) {
		memcpy(&command_message[i][1], &command_message[i + 1][1], COMMAND_WIDTH - 2);
		// �׵θ��� �����ϰ� ���� �޽����� �̵�
	}

	// �� �޽����� �� ���� ���θ� �������� �ʱ�ȭ
	memset(&command_message[COMMAND_HEIGHT - 2][1], ' ', COMMAND_WIDTH - 2);

	// ���ο� �޽��� �߰� (�� �� ���ο�)
	int message_length = vsnprintf(&command_message[COMMAND_HEIGHT - 2][1], COMMAND_WIDTH - 2, format, args);
	va_end(args); // ���� ���� ��� ó�� ��

	// �޽��� �� ���� ���� ������ �������� ä���
	if (message_length < COMMAND_WIDTH - 2) {
		memset(&command_message[COMMAND_HEIGHT - 2][1 + message_length], ' ', COMMAND_WIDTH - 2 - message_length);
	}
}



void setColor(POSITION pos , int color) {
	colorMap[pos.row][pos.column] = color;
}