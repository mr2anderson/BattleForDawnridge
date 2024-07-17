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



#include <SFML/Graphics.hpp>
#include "LoadingError.hpp"


#pragma once


class LoadingScreen {
public:
	static LoadingScreen* get() {
		if (LoadingScreen::singletone == nullptr) {
			LoadingScreen::singletone = new LoadingScreen();
		}
		return LoadingScreen::singletone;
	}

	bool run(sf::RenderWindow &window);
private:
	LoadingScreen() = default;
	LoadingScreen(const LoadingScreen& copy) = delete;
	static LoadingScreen* singletone;

	void setBaseScreen(sf::RenderWindow &window);
	bool loadBase(sf::RenderWindow &window);
	void setNormalScreen(sf::RenderWindow& window);
	bool loadAll(sf::RenderWindow &window);
    void loadingError(LoadingError *e, sf::RenderWindow &window);
	void setCursor(sf::RenderWindow& window);
};