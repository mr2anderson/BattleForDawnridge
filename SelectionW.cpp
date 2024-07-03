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


#include "SelectionW.hpp"


SelectionW::SelectionW(const std::string &soundName1, const std::string &soundName2, const std::vector<SelectionWComponent> &components) : 
	PopUpW(soundName1, soundName2) {
	this->components = components;
}
void SelectionW::run(uint32_t windowW, uint32_t windowH) {
	this->playSound1();
	this->makeButtons(windowW, windowH);
}
void SelectionW::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		target.draw(std::get<Button>(this->buttons[i]), states);
	}
}
PopUpWEvent SelectionW::click(uint32_t x, uint32_t y) const {
	for (const auto& t : buttons) {
		if (std::get<bool>(t) and std::get<Button>(t).click(x, y)) {
			PopUpWEvent event(true);
			event.gEvent = std::get<GEvent>(t);
			this->playSound2();
			return event;
		}
	}
	return PopUpWEvent(false);
}
void SelectionW::makeButtons(uint32_t windowW, uint32_t windowH) {
	this->buttons.resize(this->components.size());
	for (uint32_t i = 0; i < this->components.size(); i = i + 1) {
		std::string pictureName = this->components[i].pictureName;
		std::wstring message = this->components[i].message;
		bool clickable = this->components[i].clickable;
		GEvent event = this->components[i].gEvent;

		Button button(10, windowH - (64 + 10) * (i + 1), windowW - 20, 64, pictureName, message);
		this->buttons[i] = std::make_tuple(button, clickable, event);
	}
}