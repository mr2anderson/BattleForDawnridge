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
#include <iostream>


SelectionW::SelectionW(const std::string &soundName1, const std::string &soundName2, const std::vector<SelectionWComponent> &components) : 
	PopUpElement() {
	this->soundName1 = soundName1;
	this->soundName2 = soundName2;
	this->components = components;
}
Events SelectionW::run(uint32_t windowW, uint32_t windowH) {
	this->makeButtons(windowW, windowH);

	UIEvent soundEvent;
	soundEvent.playSound.push_back(this->soundName1);

	return this->PopUpElement::run(windowW, windowH) + Events(GEvent(), soundEvent);
}
void SelectionW::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		target.draw(std::get<Button>(this->buttons[i]), states);
	}
}
Events SelectionW::click(uint32_t x, uint32_t y) {
	for (const auto& t : buttons) {
		if (std::get<Button>(t).click(x, y)) {
			if (std::get<bool>(t)) {
				this->finish();
			}
			return std::get<Events>(t);
		}
	}
	return Events();
}
void SelectionW::makeButtons(uint32_t windowW, uint32_t windowH) {
	this->buttons.resize(this->components.size());
	for (uint32_t i = 0; i < this->components.size(); i = i + 1) {
		std::string pictureName = this->components[i].pictureName;
		std::wstring message = this->components[i].message;
		bool clickable = this->components[i].clickable;
		bool sound = this->components[i].sound;
		GEvent gEvent = this->components[i].gEvent;

		UIEvent uiEvent;
		if (sound) {
			uiEvent.playSound.push_back(this->soundName2);
		}

		Button button(10, windowH - (64 + 10) * (i + 1), windowW - 20, 64, pictureName, message);
		this->buttons[i] = std::make_tuple(button, clickable, Events(gEvent, uiEvent));
	}
}