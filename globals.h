// Ŭnicode please

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

const int BOARD_WIDTH = 35;
const int BOARD_HEIGHT = 18;

// 게임 처음 시작할때 뱀의 길이
const int SNAKE_INITIAL_MAX_LENGTH = 2;
// 사과를 먹으면 늘어나는 길이
const int SNAKE_APPLE_LENGTH = 5;
const int SNAKE_MAX_LENGTH = 512;

const int KEY_TURN_LEFT = 'a';
const int KEY_TURN_RIGHT = 'd';
const int KEY_QUIT = 'q';
const int KEY_RESET = 'r';

typedef enum {
	// 시계방향으로 순서대로
	// +1하면 시계방향, -1하면 반시계 방향
	DIRECTION_UP = 0,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
} direction_t;

typedef enum {
	TILE_EMPTY,
	TILE_APPLE,
	TILE_HEAD,
	TILE_TAIL,
} tile_t;

typedef enum {
	EVT_NULL = 0,
	EVT_GAME_OVER,
	EVT_GAME_QUIT,
	EVT_GAME_RESET,
	EVT_TURN_LEFT,
	EVT_TURN_RIGHT,
	EVT_GET_ITEM_APPLE,
} evt_t;

typedef int position_t;


struct Position {
	Position(int y, int x) : y(y), x(x) {}

	int x;
	int y;

	int encode();
	static int encodeCoord(int y, int x);
	static Position decode(int code);
};

struct Event {
	Event() : evt(EVT_NULL) {}
	Event(evt_t evt) : evt(evt) {}

	evt_t evt;
};


class Snake;
class Level;

#endif // __GLOBALS_H__