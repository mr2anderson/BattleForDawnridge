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


#include <queue>
#include "Maps.hpp"
#include "SoundQueue.hpp"
#include "Playlist.hpp"
#include "ColorTheme.hpp"
#include "WindowButton.hpp"
#include "Sounds.hpp"


#pragma once


class Menu {
public:
	static Menu* get() {
		if (Menu::singletone == nullptr) {
			Menu::singletone = new Menu();
		}
		return Menu::singletone;
	}

	Map* run(sf::RenderWindow& window);
private:
	Menu() {
		this->graphicsInited = false;
	}
	Menu(const Menu& copy) = delete;
	static Menu* singletone;

	bool graphicsInited;
    bool exit;
    bool startGame;
    uint32_t windowW, windowH;
    std::queue<std::shared_ptr<PopUpElement>> elements;
	std::vector<Button> buttons;
	sf::Text title;

	void initGraphics(uint32_t windowW, uint32_t windowH);
	void drawEverything(sf::RenderWindow& window);
    void addElement(std::shared_ptr<PopUpElement> e);
    void removeFinishedElements();
    void prepareToExit();
    bool handleButtonsClick();

    void handleEvent(const Event &e);

    void handleSoundEvent(const Event &e);
    void handleCreateEEvent(const Event &e);
    void handleExitEvent(const Event &e);
    void handleStartGameEvent(const Event &e);
};