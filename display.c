/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"

int colorMap [MAP_HEIGHT][MAP_WIDTH]; // �� ���̾��� ���� �ʱ�ȭ
void init_colorMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			colorMap[i][j] = COLOR_DEFAULT;
		}
	}
}

// ����� ������� �»��(topleft) ��ǥ
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);


void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	// display_system_message()
	// display_object_info()
	// display_commands()
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
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					colorMap[pos.row+i][pos.column+j] = setColor;
				}
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
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	char ch = frontbuf[prev.row][prev.column];
	printc(padd(map_pos, prev), ch, COLOR_DEFAULT);

	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}


void display_system_message() {

}


