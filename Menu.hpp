/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Textures.hpp"
#include "Sounds.hpp"
#include "SoundQueue.hpp"
#include "Playlist.hpp"
#include "ColorTheme.hpp"
#include "Button.hpp"


#pragma once


class Menu {
public:
	static Menu* get() {
		if (Menu::singletone == nullptr) {
			Menu::singletone = new Menu();
		}
		return Menu::singletone;
	}

	int32_t run(sf::RenderWindow& window);
private:
	Menu() {
		this->graphicsInited = false;
	}
	Menu(const Menu& copy) = delete;
	static Menu* singletone;

	bool graphicsInited;
	Label license;
	Button start2on1pc;
	Button exit;
	sf::Text title;

	void initGraphics(uint32_t windowW, uint32_t windowH);
	void drawEverything(sf::RenderWindow& window);
};