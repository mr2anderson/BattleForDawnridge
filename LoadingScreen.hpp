/*
 *  Conquesta
 *  Copyright (C) 2023 mr2anderson
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


#include "FontStorage.hpp"


#pragma once


class LoadingScreen {
public:
	static LoadingScreen* get() {
		if (LoadingScreen::singletone == nullptr) {
			LoadingScreen::singletone = new LoadingScreen();
		}
		return LoadingScreen::singletone;
	}
	void run(sf::RenderWindow &window);
private:
	LoadingScreen() = default;
	LoadingScreen(const LoadingScreen& copy) = delete;
	static LoadingScreen* singletone;

	void setScreen(sf::RenderWindow &window);
	void load();
};