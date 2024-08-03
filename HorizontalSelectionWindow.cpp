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


#include "HorizontalSelectionWindow.hpp"
#include "SoundQueue.hpp"
#include "Sounds.hpp"
#include "CloseWindowEvent.hpp"



HorizontalSelectionWindow::HorizontalSelectionWindow(const std::vector<HorizontalSelectionWindowComponent> &components, uint32_t componentSize) {
	this->components = components;
	this->componentSize = componentSize;
	this->inited = false;
}
void HorizontalSelectionWindow::run(uint32_t windowW, uint32_t windowH) {
	Events event;
	if (!this->inited) {
		this->inited = true;
		this->buttons.resize(this->components.size());
		int32_t y;
		for (uint32_t i = 0; i < this->components.size(); i = i + 1) {
			std::string pictureName = this->components.at(i).pictureName;
			std::wstring message = this->components.at(i).message;
			bool clickable = this->components.at(i).clickable;
			Events onClick = this->components.at(i).gEvent;
			std::optional<sf::IntRect> rect = this->components.at(i).rect;
			if (clickable) {
				onClick.add(std::make_shared<CloseWindowEvent>());
			}

			Button button(std::make_shared<LabelWithImage>(30 + this->componentSize, windowH - 10 - (this->componentSize + 10) * (i + 1), windowW - (50 + this->componentSize), this->componentSize, pictureName, message, rect), onClick);
			this->buttons.at(i) = button;
			if (button.getY() > (int32_t)(windowH / 2)) {
				y = button.getY() - 10;
			}
		}
		Events upEvent;
		upEvent.add(std::make_shared<MoveHorizontalSelectionWindowUpEvent>());
		upEvent.add(std::make_shared<PlaySoundEvent>("click"));
		this->up = Button(std::make_shared<Image>(20, windowH - 10 - 2 * (this->componentSize + 10), this->componentSize, "up_icon"), upEvent);
		Events downEvent;
		downEvent.add(std::make_shared<MoveHorizontalSelectionWindowDownEvent>());
		downEvent.add(std::make_shared<PlaySoundEvent>("click"));
		this->down = Button(std::make_shared<Image>(20, windowH - 10 - (this->componentSize + 10), this->componentSize, "down_icon"), downEvent);
		this->rect = std::make_unique<RectangularUiElement>(10, y, windowW - 20, windowH - y - 10);
	}
}
void HorizontalSelectionWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*this->rect.get(), states);
	if (this->possibleToMoveUp()) {
		target.draw(this->up);
	}
	if (this->possibleToMoveDown()) {
		target.draw(this->down);
	}
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		if (this->show(this->buttons.at(i))) {
			target.draw(this->buttons.at(i), states);
		}
	}
}
Events HorizontalSelectionWindow::click() {
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
void HorizontalSelectionWindow::finish() {
	this->PopUpElement::finish();
}
bool HorizontalSelectionWindow::show(const Button& button) const {
	return (button.getY() + button.getH() > this->rect->getY() and 
			button.getY() + button.getH() < this->rect->getY() + this->rect->getH());
}
bool HorizontalSelectionWindow::possibleToMoveUp() const {
	return (this->buttons.back().getY() < this->rect->getY());
}
bool HorizontalSelectionWindow::possibleToMoveDown() const {
	return (this->buttons.front().getY() + this->buttons.front().getH() > this->rect->getY() + this->rect->getH());
}
void HorizontalSelectionWindow::moveUp() {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		this->buttons.at(i).setY(this->buttons.at(i).getY() + (this->componentSize + 10));
	}
}
void HorizontalSelectionWindow::moveDown() {
	for (uint32_t i = 0; i < this->buttons.size(); i = i + 1) {
		this->buttons.at(i).setY(this->buttons.at(i).getY() - (this->componentSize + 10));
	}
}
void HorizontalSelectionWindow::handle(Events& events) {
	for (uint32_t i = 0; i < events.size(); i = i + 1) {
		std::shared_ptr<Event> e = events.at(i);
		if (std::shared_ptr<MoveHorizontalSelectionWindowUpEvent> moveUp = std::dynamic_pointer_cast<MoveHorizontalSelectionWindowUpEvent>(e)) {
			this->handleMoveUpEvent(moveUp);
		}
		else if (std::shared_ptr<MoveHorizontalSelectionWindowDownEvent> moveDown = std::dynamic_pointer_cast<MoveHorizontalSelectionWindowDownEvent>(e)) {
			this->handleMoveDownEvent(moveDown);
		}
		else if (std::shared_ptr<PlaySoundEvent> playSound = std::dynamic_pointer_cast<PlaySoundEvent>(e)) {
			this->handlePlaySoundEvent(playSound);
		}
	}
}
void HorizontalSelectionWindow::handleMoveUpEvent(std::shared_ptr<MoveHorizontalSelectionWindowUpEvent> e) {
	this->moveUp();
}
void HorizontalSelectionWindow::handleMoveDownEvent(std::shared_ptr<MoveHorizontalSelectionWindowDownEvent> e) {
	this->moveDown();
}
void HorizontalSelectionWindow::handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e) {
	SoundQueue::get()->push(Sounds::get()->get(e->getSoundName()));
}