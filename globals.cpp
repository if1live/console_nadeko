// Ŭnicode please

#include "globals.h"

int Position::encode()
{
	return encodeCoord(y, x);
}

int Position::encodeCoord(int y, int x)
{
	int val = BOARD_WIDTH * y + x;
	return val;
}

Position Position::decode(int code)
{
	int y = code / BOARD_WIDTH;
	int x = code % BOARD_WIDTH;
	return Position(y, x);
}