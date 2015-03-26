// Ŭnicode please

#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "globals.h"
#include <array>
#include <vector>

class Snake {
public:
	typedef std::vector<position_t> body_t;
public:
	Snake();

	void update(Level &level);
	void onEvent(const Event &evt);

public:
	position_t getHead() const;
	body_t getTail() const;
	direction_t direction() const { return dir_; }
	bool running() const { return running_; }
	int score() const { return score_; }
	int age() const { return age_; }
public:
	static int getHighScore();
	static int updateHighScore(int score);

private:
	void turnLeft();
	void turnRight();
	void getApple();

private:
	void pushHead(const position_t &pos);

private:
	std::array<position_t, SNAKE_MAX_LENGTH> body_;
	int head_;
	int max_length_;
	int length_;

	direction_t dir_;

	bool running_;
	int score_;
	int age_;

	
	static int high_score_;
};

#endif