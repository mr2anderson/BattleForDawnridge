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
		this->buttons.resize(this->components.size());
		int32_t y;
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

			Button button(std::make_shared<LabelWithImage>(94, windowH - 10 - (64 + 10) * (i + 1), windowW - 114, 64, pictureName, message), onClick);
			this->buttons[i] = button;
			if (button.getY() > (int32_t)(windowH / 2)) {
				y = button.getY() - 10;
			}
		}
		Events upEvent;
		upEvent.add(std::make_shared<MoveGameActionWindowUpEvent>());
		upEvent.add(std::make_shared<PlaySoundEvent>("click"));
		this->up = Button(std::make_shared<Image>(20, windowH - 10 - 2 * (64 + 10), "up_icon"), upEvent);
		Events downEvent;
		downEvent.add(std::make_shared<MoveGameActionWindowDownEvent>());
		downEvent.add(std::make_shared<PlaySoundEvent>("click"));
		this->down = Button(std::make_shared<Image>(20, windowH - 10 - (64 + 10), "down_icon"), downEvent);
		this->rect = RectangularUiElement(10, y, windowW - 20, windowH - y - 10);
	}
	if (!this->soundName1.empty()) {
		event.add(std::make_shared<PlaySoundEvent>(this->soundName1));
	}
	return event;
}
void GameActionWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->rect, states);
	if (this->possibleToMoveUp()) {
		target.draw(this->up);
	}
	if (this->possibleToMoveDown()) {
		target.draw(this->down);
	}
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		if (this->show(this->buttons[i])) {
			target.draw(this->buttons[i], states);
		}
	}
}
Events GameActionWindow::click() {
	if (this->possibleToMoveUp()) {
		Events events = this->up.click();
		this->handle(events);
	}
	if (this->possibleToMoveDown()) {
		Events events = this->down.click();
		this->handle(events);
	}
	for (const auto& b : buttons) {
		if (this->show(b)) {
			Events event = b.click();
			for (uint32_t i = 0; i < event.size(); i = i + 1) {
				if (std::shared_ptr<CloseWindowEvent> e = std::dynamic_pointer_cast<CloseWindowEvent>(event.at(i))) {
					this->finish();
					return event;
				}
			}
		}
	}
	return Events();
}
void GameActionWindow::finish() {
	this->PopUpElement::finish();
	while (this->possibleToMoveDown()) {
		this->moveDown();
	}
}
bool GameActionWindow::show(const Button& button) const {
	return (button.getY() + button.getH() > this->rect.getY() and 
			button.getY() + button.getH() < this->rect.getY() + this->rect.getH());
}
bool GameActionWindow::possibleToMoveUp() const {
	return (this->buttons.front().getY() + this->buttons.front().getH() > this->rect.getY() + this->rect.getH());
}
bool GameActionWindow::possibleToMoveDown() const {
	return (this->buttons.back().getY() < this->rect.getY());
}
void GameActionWindow::moveUp() {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		this->buttons[i].setY(this->buttons[i].getY() - (64 + 10));
	}
}
void GameActionWindow::moveDown() {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		this->buttons[i].setY(this->buttons[i].getY() + (64 + 10));
	}
}
void GameActionWindow::handle(Events& events) {
	for (uint32_t i = 0; i < events.size(); i = i + 1) {
		std::shared_ptr<Event> e = events.at(i);
		if (std::shared_ptr<MoveGameActionWindowUpEvent> moveUp = std::dynamic_pointer_cast<MoveGameActionWindowUpEvent>(e)) {
			this->handleMoveUpEvent(moveUp);
		}
		else if (std::shared_ptr<MoveGameActionWindowDownEvent> moveDown = std::dynamic_pointer_cast<MoveGameActionWindowDownEvent>(e)) {
			this->handleMoveDownEvent(moveDown);
		}
		else if (std::shared_ptr<PlaySoundEvent> playSound = std::dynamic_pointer_cast<PlaySoundEvent>(e)) {
			this->handlePlaySoundEvent(playSound);
		}
	}
}
void GameActionWindow::handleMoveUpEvent(std::shared_ptr<MoveGameActionWindowUpEvent> e) {
	this->moveUp();
}
void GameActionWindow::handleMoveDownEvent(std::shared_ptr<MoveGameActionWindowDownEvent> e) {
	this->moveDown();
}
void GameActionWindow::handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e) {
	SoundQueue::get()->push(Sounds::get()->get(e->getSoundName()));
}