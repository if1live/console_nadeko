// Ŭnicode please 
#include "level.h"

#include "snake.h"

Level::Level()
{
}

void Level::onEvent(const Event &evt)
{
	switch (evt.evt) {
	case EVT_GAME_OVER:
		;	// TODO
	}
}

void Level::resetApplePosition(const Snake &snake)
{
	std::vector<position_t> ignore_list;

	// 뱀이 존재하는 자리에는 사과가 생기면 안된다.
	auto tail_list = snake.getTail();
	for (position_t pos : tail_list) {
		ignore_list.push_back(pos);
	}
	position_t head = snake.getHead();
	Position head_pos = Position::decode(head);
	ignore_list.push_back(head);
	

	// 뱀의 진행방향 바로 앞에 사과가 생기면 재미없다
	// 4방향 확인은 귀찮으니 가로/세로만 확인
	// 이거로도 충분할거다
	direction_t dir = snake.direction();
	switch (dir) {
	case DIRECTION_UP: 
	case DIRECTION_DOWN:
	{
		for (int y = 0; y < BOARD_HEIGHT; ++y) {
			Position ignore(head_pos.x, y);
			ignore_list.push_back(ignore.encode());
		}
		break;
	}
	case DIRECTION_LEFT:
	case DIRECTION_RIGHT:
	{
		for (int x = 0; x < BOARD_WIDTH; ++x) {
			Position ignore(x, head_pos.y);
			ignore_list.push_back(ignore.encode());
		}
		break;
	}
	}

	resetApplePosition(ignore_list);
}

void Level::resetApplePosition(const std::vector<position_t> &ignore_list)
{
	// 임의의 점을 찍어내는걸 무식하게 구현
	// ignore_list를 제외한 모든 좌표를 생성
	// 랜덤하게 하나 뽑음
	std::vector<position_t> candidate_list;
	for (int y = 0; y < BOARD_HEIGHT; ++y) {
		for (int x = 0; x < BOARD_WIDTH; ++x) {
			position_t pos = Position::encodeCoord(y, x);
			auto found = std::find(ignore_list.begin(), ignore_list.end(), pos);
			if (found == ignore_list.end()) {
				candidate_list.push_back(pos);
			}
		}
	}

	int idx = rand() % candidate_list.size();
	apple_pos_ = candidate_list[idx];
}


