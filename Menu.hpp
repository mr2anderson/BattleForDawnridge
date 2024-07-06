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


#include "Textures.hpp"
#include "Sounds.hpp"
#include "SoundQueue.hpp"
#include "Playlist.hpp"
#include "ColorTheme.hpp"
#include "SimpleWindow.hpp"


#pragma once


class Menu {
public:
	static Menu* get() {
		if (Menu::singletone == nullptr) {
			Menu::singletone = new Menu();
		}
		return Menu::singletone;
	}

	std::string run(sf::RenderWindow& window);
private:
	Menu() {
		this->graphicsInited = false;
	}
	Menu(const Menu& copy) = delete;
	static Menu* singletone;

	bool graphicsInited;
    PopUpElement *element;
	Button start2on1pcButton;
    Button supportButton;
    Button creditsButton;
    Button licenseButton;
	Button exitButton;
	sf::Text title;

	void initGraphics(uint32_t windowW, uint32_t windowH);
	void drawEverything(sf::RenderWindow& window);

    void handleEvent(const Event &e);

    void handleSoundEvent(const Event &e);
};