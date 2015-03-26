// Ŭnicode please 
#include "globals.h"
#include "device.h"
#include "snake.h"
#include "level.h"

#include <memory>

void reset_game(std::unique_ptr<Renderer> &renderer, std::unique_ptr<Level> &lv, std::unique_ptr<Snake> &snake)
{
	renderer = std::unique_ptr<Renderer>(new Renderer());
	lv = std::unique_ptr<Level>(new Level());
	snake = std::unique_ptr<Snake>(new Snake());
	lv->resetApplePosition(*snake);

	renderer->clear();
}

int main()
{
	std::unique_ptr<Renderer> foo;
	///////////////////
	// set up singleton
	GameDevice &device = GameDevice::getInstance();
	device.setUp();
	///////////////////

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Level> lv;
	std::unique_ptr<Snake> snake;
	reset_game(renderer, lv, snake);

	while(true) {
		Event evt = device.getKeyEvent();
		if (evt.evt == EVT_GAME_RESET) {
			reset_game(renderer, lv, snake);
			continue;
		}

		snake->onEvent(evt);
		snake->update(*lv);

		renderer->render(*lv, *snake);

		device.sleep(200);
	}

	/////////////////////
	// clean up singleton
	device.cleanUp();

	return 0;
}
