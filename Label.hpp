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


#include <sstream>
#include "RectangularUIElement.hpp"
#include "Fonts.hpp"


#pragma once


class Label : public RectangularUiElement {
public:
	Label();
	Label(uint32_t x, uint32_t y, uint32_t w, uint32_t h, std::wstring message, bool center = true);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Text text;

	void initText();
	std::wstring putNLs(const std::wstring& message, uint32_t w);
	std::wstring centerLines(const std::wstring& message);
	uint32_t getLongestLineWidth(const std::wstring& message);
	std::wstring centerLines(const std::wstring& message, uint32_t maxWidth);
};