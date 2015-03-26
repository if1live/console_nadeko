// Ŭnicode please
#include "device.h"
#include "globals.h"
#include <conio.h>
#include <Windows.h>
#include <ctime>

#include "level.h"
#include "snake.h"

const char *WALL_HORIZONTAL = "─";
const char *WALL_VERTICAL = "│";
const char *WALL_TOP_LEFT = "┌";
const char *WALL_TOP_RIGHT = "┐";
const char *WALL_BOTTOM_LEFT = "└";
const char *WALL_BOTTOM_RIGHT = "┘";

const char *HEAD = "＠";
const char *TAIL = "■";
const char *APPLE = "♡";
const char *EMPTY = "  ";

Renderer::Renderer()
{
	std::fill(prev_buffer_.begin(), prev_buffer_.end(), ' ');
	int buffer_width = BOARD_WIDTH + 2;
	int buffer_height = HEIGHT;
	for (int y = 0; y < buffer_height; ++y) {
		int newline_idx = toCoord(y, buffer_width);
		prev_buffer_[newline_idx] = '\0';
	}
	
	//fillBorder(&prev_buffer_);
}

int Renderer::toCoord(int y, int x) const
{
	return (y * WIDTH_WITH_NEWLINE) + (x * 2);
}

void Renderer::fillBorder(framebuffer_t *buffer)
{
	int buffer_width = BOARD_WIDTH + 2;
	int buffer_height = HEIGHT;

	// reset buffer
	std::fill(buffer->begin(), buffer->end(), ' ');
	for (int y = 0; y < buffer_height; ++y) {
		if (y == 0 || y == (buffer_height - 1)) {
			for (int x = 0; x < buffer_width; ++x) {
				int idx = toCoord(y, x);
				memcpy(buffer->data() + idx, WALL_HORIZONTAL, 2);
			}
		}
		int left_wall_idx = toCoord(y, 0);
		int right_wall_idx = toCoord(y, buffer_width - 1);
		memcpy(buffer->data() + left_wall_idx, WALL_VERTICAL, 2);
		memcpy(buffer->data() + right_wall_idx, WALL_VERTICAL, 2);

		if (y == 0) {
			int left_idx = toCoord(y, 0);
			int right_idx = toCoord(y, buffer_width - 1);
			memcpy(buffer->data() + left_idx, WALL_TOP_LEFT, 2);
			memcpy(buffer->data() + right_idx, WALL_TOP_RIGHT, 2);
		}
		if (y == (HEIGHT - 1)) {
			int left_idx = toCoord(y, 0);
			int right_idx = toCoord(y, buffer_width - 1);
			memcpy(buffer->data() + left_idx, WALL_BOTTOM_LEFT, 2);
			memcpy(buffer->data() + right_idx, WALL_BOTTOM_RIGHT, 2);
		}

		int newline_idx = toCoord(y, buffer_width);
		(*buffer)[newline_idx] = '\0';
	}
}

void Renderer::fillLevel(const Level &lv, framebuffer_t *buffer)
{
	Position apple_pos = Position::decode(lv.apple_pos());
	int idx = toCoord(apple_pos.y + 1, apple_pos.x + 1);
	memcpy(buffer->data() + idx, APPLE, 2);
}

void Renderer::fillSnake(const Snake &snake, framebuffer_t *buffer)
{
	position_t head_pos = snake.getHead();
	Position pos = Position::decode(head_pos);
	int head_idx = toCoord(pos.y + 1, pos.x + 1);
	memcpy(buffer->data() + head_idx, HEAD, 2);

	auto tail = snake.getTail();
	for (auto tail_pos : tail) {
		Position pos = Position::decode(tail_pos);
		int tail_idx = toCoord(pos.y + 1, pos.x + 1);
		memcpy(buffer->data() + tail_idx, TAIL, 2);
	}
}
void Renderer::renderBuffer(const framebuffer_t &buffer) const
{
	for (int y = 0; y < HEIGHT; ++y) {
		int idx = toCoord(y, 0);
		const unsigned char *ptr = buffer.data() + idx;
		printf("%s\n", ptr);
	}
}

void Renderer::render(const Level &lv, const Snake &snake)
{
	gotoxy(1, 1);
	printf("%c=Left / %c = Right / %c=Reset / %c=Quit\n",
		KEY_TURN_LEFT, KEY_TURN_RIGHT, KEY_RESET, KEY_QUIT);

	gotoxy(2, 1);
	printf("Turn = %d, Score = %d / HighScore = %d\n", snake.age(), snake.score(), Snake::getHighScore());

	framebuffer_t curr_buffer;
	fillBorder(&curr_buffer);
	fillLevel(lv, &curr_buffer);
	fillSnake(snake, &curr_buffer);

	// prev와 curr의 차이를 찾아서 변경지점만 렌더링
	for (int y = 0; y < HEIGHT; ++y) {
		for(int x = 0; x < BOARD_WIDTH + 2; ++x) {
			int coord = toCoord(y, x);
			unsigned char curr = curr_buffer[coord];
			unsigned char prev = prev_buffer_[coord];
			if (curr != prev) {
				unsigned char buffer[3];
				memcpy(buffer, curr_buffer.data() + coord, 2);
				buffer[2] = '\0';

				gotoxy(y + 3, x * 2 + 1);
				printf("%s", buffer);
			}
		}
	}
	
	//renderBuffer(prev_buffer_);
	std::copy(curr_buffer.begin(), curr_buffer.end(), prev_buffer_.begin());

	if (snake.running() == false) {
		gotoxy(HEIGHT + 3, 1);
		printf("Game Over. Score=%d, Restart=%c\n", snake.score(), KEY_RESET);
	}
}

void Renderer::clear()
{
	system("cls");
}

void Renderer::gotoxy(int y, int x)
{
	COORD pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

////////////////////////
Event GameDevice::getKeyEvent()
{
	if (kbhit()) {
		int c = getch();
		switch (c) {
		case KEY_TURN_LEFT:
			return Event(EVT_TURN_LEFT);
		case KEY_TURN_RIGHT:
			return Event(EVT_TURN_RIGHT);
		case KEY_QUIT:
			return Event(EVT_GAME_QUIT);
		case KEY_RESET:
			return Event(EVT_GAME_RESET);
		}
	}

	Event evt;
	return evt;
}
void GameDevice::sleep(int ms)
{
	Sleep(ms);
}

GameDevice &GameDevice::getInstance()
{
	static GameDevice ctx;
	return ctx;
}

void GameDevice::setUp()
{
	srand((unsigned int)time(NULL));
}

void GameDevice::cleanUp()
{

}