/*
* raw(?) I/O
*/
#include "io.h"
#include "common.h"
#include "display.h"
void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // 행, 열 반대로 전달
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printc(POSITION pos, char ch, int color) {
	if (color >= 0) {
		set_color(color);
	}
	gotoxy(pos);
	printf("%c", ch);
}

void printObject(POSITION pos, char ch, int color, int size) {

}

KEY get_key(int byte) {
	//if (!_kbhit()) {  // 입력된 키가 있는지 확인
	//	return k_none;
	//}

	//int byte = _getch();    // 입력된 키를 전달 받기
	//int byte = getInputKey();
	//switch (byte) {
	//case 'q': return k_quit;  // 'q'를 누르면 종료
	//case 'c': return k_command;
	//case 32: return k_space;
	//case 27: return k_esc;
	if (byte == 'q') {
		return k_quit;
	}
	if (byte == 224)
	{
		byte = _getch();  // MSB 224가 입력 되면 1바이트 더 전달 받기
		switch (byte) {
		case 72: return k_up;
		case 75: return k_left;
		case 77: return k_right;
		case 80: return k_down;
		default: return k_undef;

		}
	}

	else {
		return k_undef;
	}
}

int getInputKey(void) {
	if (!_kbhit()) {  // 입력된 키가 있는지 확인
		return 0;
	}

	int byte = _getch();

	return byte;


}
