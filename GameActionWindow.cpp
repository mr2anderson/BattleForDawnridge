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


#include "GameActionWindow.hpp"



GameActionWindow::GameActionWindow(const std::string &soundName1, const std::string &soundName2, const std::vector<GameActionWindowComponent> &components) {
	this->soundName1 = soundName1;
	this->soundName2 = soundName2;
	this->components = components;
	this->inited = false;
}
Events GameActionWindow::run(uint32_t windowW, uint32_t windowH) {
	Events event = this->CameraIndependentPopUpElement::run(windowW, windowH);
	if (!this->inited) {
		this->inited = true;
		this->makeButtons(windowW, windowH);
	}
	if (!this->soundName1.empty()) {
		event.add(std::make_shared<PlaySoundEvent>(this->soundName1));
	}
	return event;
}
void GameActionWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		target.draw(this->buttons[i], states);
	}
}
Events GameActionWindow::click() {
	for (const auto& b : buttons) {
        Events event = b.click();
		for (uint32_t i = 0; i < event.size(); i = i + 1) {
			if (std::shared_ptr<CloseWindowEvent> e = std::dynamic_pointer_cast<CloseWindowEvent>(event.at(i))) {
				this->finish();
				return event;
			}
		}
	}
	return Events();
}
void GameActionWindow::update() {

}
void GameActionWindow::makeButtons(uint32_t windowW, uint32_t windowH) {
	this->buttons.resize(this->components.size());
	for (uint32_t i = 0; i < this->components.size(); i = i + 1) {
		std::string pictureName = this->components[i].pictureName;
		std::wstring message = this->components[i].message;
		bool clickable = this->components[i].clickable;
		bool sound = this->components[i].sound;
		Events onClick = this->components[i].gEvent;
        if (clickable) {
			onClick.add(std::make_shared<CloseWindowEvent>());
        }
		if (sound and !this->soundName2.empty()) {
			onClick.add(std::make_shared<PlaySoundEvent>(this->soundName2));
		}

		Button button(std::make_shared<LabelWithImage>(10, windowH - (64 + 10) * (i + 1), windowW - 20, 64, pictureName, message), onClick);
		this->buttons[i] = button;
	}
}