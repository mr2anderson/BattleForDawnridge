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


#include "PopUpWindow.hpp"
#include "Label.hpp"
#include "Button.hpp"


#pragma once


class MessageWindow : public PopUpWindow {
public:
	MessageWindow(uint32_t windowW, uint32_t windowH, const std::string& message);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	PopUpWindowEvent click(uint32_t x, uint32_t y) const override;
private:
	Label label;
	Button button;
};