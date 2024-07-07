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


#include "PopUpElement.hpp"
#include "Button.hpp"
#include "GameActionWindowComponent.hpp"
#include "LabelWithImage.hpp"


#pragma once


class GameActionWindow : public PopUpElement {
public:
	GameActionWindow(const std::string &soundName1, const std::string &soundName2, const std::vector<GameActionWindowComponent>& components);

	Event run(uint32_t windowW, uint32_t windowH) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	Event click() override;
	void update() override;
private:
	std::string soundName1;
	std::string soundName2;
	std::vector<GameActionWindowComponent> components;
	std::vector<Button> buttons;

	void makeButtons(uint32_t windowW, uint32_t windowH);
};