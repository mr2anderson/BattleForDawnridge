/*
 *  Battle for Dawnridge
 *  Copyright (C) 2024 mr2anderson
 *
 *  Battle for Dawnridge is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Battle for Dawnridge is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Battle for Dawnridge.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Button.hpp"
#include "PlaySoundEvent.hpp"
#include "NextClueEvent.hpp"


#pragma once


class PAKScreen {
public:
	static PAKScreen* get() {
		if (PAKScreen::singletone == nullptr) {
			PAKScreen::singletone = new PAKScreen();
		}
		return PAKScreen::singletone;
	}

	bool run(sf::RenderWindow &window);
private:
	PAKScreen() = default;
	PAKScreen(const PAKScreen& copy) = delete;
	static PAKScreen* singletone;

	sf::Sprite s;
	sf::Text t;
	Button nextClueButton;

	void init(uint32_t windowW, uint32_t windowH);
	void drawEverything(sf::RenderWindow& window);

	void handleEvent(Events& e);
	void handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e);
	void handleNextClueEvent(std::shared_ptr<NextClueEvent> e);
};