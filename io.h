/*
* raw(?) I/O
*/
#ifndef _IO_H_
#define _IO_H_

#include "common.h"

KEY get_key(int byte);
void printc(POSITION pos, char ch, int color);
void gotoxy(POSITION pos);
void set_color(int color);
int getInputKey(void);
#endif

