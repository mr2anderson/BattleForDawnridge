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



#include <sstream>
#include "ColorTheme.hpp"
#include "TextureStorage.hpp"
#include "FontStorage.hpp"


#pragma once


class Label : public sf::Drawable {
public:
	Label();
	Label(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::wstring& message, uint32_t charSize);
	Label(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::string& picture, const std::wstring& message, uint32_t charSize);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:
	uint32_t getX() const;
	uint32_t getY() const;
	uint32_t getW() const;
	uint32_t getH() const;
private:
	sf::RectangleShape rect;
	sf::Sprite sprite;
	sf::Text text;
};