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
#include "PopUpWindowEvent.hpp"


#pragma once


class PopUpWindow : public sf::Drawable {
public:
	PopUpWindow(uint32_t windowW, uint32_t windowH);
	virtual PopUpWindowEvent click(uint32_t x, uint32_t y) const = 0;
protected:
	uint32_t getWindowW() const;
	uint32_t getWindowH() const;
private:
	uint32_t windowW, windowH;
};