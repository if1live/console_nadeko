// Ŭnicode please 
#include "snake.h"
#include "level.h"

int Snake::high_score_ = 0;

Snake::Snake()
	: dir_(DIRECTION_RIGHT),
	max_length_(SNAKE_INITIAL_MAX_LENGTH),
	length_(0),
	head_(-1),
	running_(true),
	score_(0),
	age_(0)
{
	std::fill(body_.begin(), body_.end(), -1);

	position_t center = Position::encodeCoord(BOARD_HEIGHT / 2, BOARD_WIDTH / 2);
	pushHead(center);
}

void Snake::pushHead(const position_t &pos)
{
	int prev_head = head_;
	int next_head = (prev_head + 1) % SNAKE_MAX_LENGTH;
	int prev_length = length_;
	int next_length = prev_length + 1;


	if (next_length > max_length_) {
		int prev_tail = (next_head - prev_length + SNAKE_MAX_LENGTH) % SNAKE_MAX_LENGTH;
		body_[prev_tail] = -1;
		next_length = max_length_;
	}

	body_[next_head] = pos;

	head_ = next_head;
	length_ = next_length;
}

void Snake::update(Level &lv)
{
	if (running_ == false) {
		return;
	}

	age_++;

	int move_table[][2] {
		{ 0, -1 },	// up
		{ 1, 0 },	// right
		{ 0, 1 },	// down
		{ -1, 0 },	// left
	};

	Position pos = Position::decode(getHead());
	int delta_x = move_table[dir_][0];
	int delta_y = move_table[dir_][1];
	pos.x += delta_x;
	pos.y += delta_y;

	if (pos.x < 0 || pos.x >= BOARD_WIDTH || pos.y < 0 || pos.y >= BOARD_HEIGHT) {
		Event evt(EVT_GAME_OVER);
		this->onEvent(evt);
		lv.onEvent(evt);
		return;
	}

	// 사과 먹은거 확인
	if (getHead() == lv.apple_pos()) {
		Event evt(EVT_GET_ITEM_APPLE);
		this->onEvent(evt);

		// 사과 재배치
		lv.resetApplePosition(*this);
	}

	pushHead(pos.encode());

	auto tail_list = getTail();
	for (position_t tail_pos : tail_list) {
		if (getHead() == tail_pos) {
			Event evt(EVT_GAME_OVER);
			this->onEvent(evt);
			lv.onEvent(evt);
			return;
		}
	}
}

void Snake::onEvent(const Event &evt)
{
	switch (evt.evt) {
	case EVT_TURN_LEFT:
		turnLeft();
		break;
	case EVT_TURN_RIGHT:
		turnRight();
		break;
	case EVT_GET_ITEM_APPLE:
		getApple();
		break;
	case EVT_GAME_OVER:
		running_ = false;
		updateHighScore(score_);
		break;
	case EVT_GAME_QUIT:
		exit(0);
		break;
	default:
		;	// pass
	}
}

void Snake::turnLeft()
{
	int curr = (int)dir_;
	dir_ = static_cast<direction_t>((curr + 4 - 1) % 4);
}

void Snake::turnRight()
{
	int curr = (int)dir_;
	dir_ = static_cast<direction_t>((curr + 1) % 4);
}

void Snake::getApple()
{
	max_length_ += SNAKE_APPLE_LENGTH;
	score_ += 1;
}

position_t Snake::getHead() const
{
	return body_[head_];
}

Snake::body_t Snake::getTail() const
{
	body_t tail;
	for (int i = 1; i < length_; ++i) {
		int idx = (head_ - i + SNAKE_MAX_LENGTH) % SNAKE_MAX_LENGTH;
		position_t pos = body_[idx];
		tail.push_back(pos);
	}
	return tail;
}

int Snake::getHighScore()
{
	return high_score_;
}

int Snake::updateHighScore(int score)
{
	if (high_score_ < score) {
		high_score_ = score;
	}
	return high_score_;
}