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


#include <SFML/Graphics.hpp>
#include <cstdint>


#pragma once


class BattleScreen {
public:
	static BattleScreen* get() {
		if (BattleScreen::singletone == nullptr) {
			BattleScreen::singletone = new BattleScreen();
		}
		return BattleScreen::singletone;
	}
	int32_t run(sf::RenderWindow& window);
private:
	BattleScreen() = default;
	BattleScreen(const BattleScreen& copy) = delete;
	static BattleScreen* singletone;
};