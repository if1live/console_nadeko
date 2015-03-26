// Ŭnicode please

#ifndef __GAME_H__
#define __GAME_H__

#include "globals.h"

#include <vector>


class Level {
public:
	Level();

public:
	const position_t &apple_pos() const { return apple_pos_; }
	void resetApplePosition(const Snake &snake);

	void onEvent(const Event &evt);

private:
	void resetApplePosition(const std::vector<position_t> &ignore_list);
private:
	position_t apple_pos_;
};

#endif // __GAME_H__
