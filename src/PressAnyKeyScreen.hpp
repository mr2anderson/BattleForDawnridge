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


#include <cstdint>
#include "TextureStorage.hpp"
#include "MusicStorage.hpp"
#include "FontStorage.hpp"
#include "ColorTheme.hpp"


#pragma once


class PressAnyKeyScreen {
public:
	static PressAnyKeyScreen* get() {
		if (PressAnyKeyScreen::singletone == nullptr) {
			PressAnyKeyScreen::singletone = new PressAnyKeyScreen();
		}
		return PressAnyKeyScreen::singletone;
	}
	int32_t run(sf::RenderWindow &window);
private:
	PressAnyKeyScreen() = default;
	PressAnyKeyScreen(const PressAnyKeyScreen& copy) = delete;
	static PressAnyKeyScreen* singletone;

	int32_t wait(sf::RenderWindow &window);
};