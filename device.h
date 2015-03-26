// Ŭnicode please

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "globals.h"
#include <array>

class Renderer {
public:
	enum {
		WIDTH = (BOARD_WIDTH + 2) * 2, 	// 전각문자=2배
		HEIGHT = BOARD_HEIGHT + 2,
		WIDTH_WITH_NEWLINE = WIDTH + 1,
	};
	typedef std::array<unsigned char, HEIGHT * WIDTH_WITH_NEWLINE> framebuffer_t;

public:
	Renderer();
	void render(const Level &board, const Snake &snake);
	void clear();
	void gotoxy(int y, int x);

private:
	int toCoord(int y, int x) const;

	void fillBorder(framebuffer_t *buffer);
	void fillLevel(const Level &lv, framebuffer_t *buffer);
	void fillSnake(const Snake &snake, framebuffer_t *buffer);
	void renderBuffer(const framebuffer_t &buffer) const;
	
	framebuffer_t prev_buffer_;
};


class GameDevice {
private:
	GameDevice() {}

public:
	static GameDevice &getInstance();

public:
	void setUp();
	void cleanUp();

	Event getKeyEvent();
	void sleep(int ms);
};


#endif