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
#include "StaticString.hpp"


HorizontalSelectionWindow::HorizontalSelectionWindow(const std::vector<HorizontalSelectionWindowComponent> &components, uint32_t componentSize) {
	this->components = components;
	this->componentSize = componentSize;
	this->offset = 0;
}
void HorizontalSelectionWindow::onRestart() {
	this->offset = 0;
}
void HorizontalSelectionWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	HorizontalSelectionWindowStructure structure = this->getStructure(target.getSize().x, target.getSize().y);
	target.draw(structure.rect);
	if (structure.buttonUp.has_value()) {
		target.draw(structure.buttonUp.value());
	}
	if (structure.buttonDown.has_value()) {
		target.draw(structure.buttonDown.value());
	}
	for (uint32_t i = 0; i < structure.contentButtons.size(); i = i + 1) {
		target.draw(structure.contentButtons.at(i), states);
	}
}
Events HorizontalSelectionWindow::click(uint32_t mouseX, uint32_t mouseY, uint32_t windowW, uint32_t windowH) {
	HorizontalSelectionWindowStructure structure = this->getStructure(windowW, windowH);
	if (structure.buttonUp.has_value()) {
		Events events = structure.buttonUp.value().click(mouseX, mouseY);
		this->handle(events);
	}
	if (structure.buttonDown.has_value()) {
		Events events = structure.buttonDown.value().click(mouseX, mouseY);
		this->handle(events);
	}
	for (const auto& b : structure.contentButtons) {
		Events event = b.click(mouseX, mouseY);
		for (uint32_t i = 0; i < event.size(); i = i + 1) {
			if (std::shared_ptr<CloseWindowEvent> e = std::dynamic_pointer_cast<CloseWindowEvent>(event.at(i))) {
				this->finish();
				return event;
			}
		}
	}
	return Events();
}
bool HorizontalSelectionWindow::possibleToMoveUp(uint32_t componentsInFrame) const {
	return (this->offset + componentsInFrame < this->components.size());
}
bool HorizontalSelectionWindow::possibleToMoveDown() const {
	return this->offset > 0;
}
void HorizontalSelectionWindow::moveUp() {
	this->offset = this->offset + 1;
}
void HorizontalSelectionWindow::moveDown() {
	this->offset = this->offset - 1;
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



uint32_t HorizontalSelectionWindow::getComponentsInFrame(uint32_t windowW, uint32_t windowH) const {
	return std::min((uint32_t)this->components.size(), windowH * 3 / 4 / this->componentSize);
}
HorizontalSelectionWindowStructure HorizontalSelectionWindow::getStructure(uint32_t windowW, uint32_t windowH) const {
	HorizontalSelectionWindowStructure structure;

	structure.contentButtons.resize(this->getComponentsInFrame(windowW, windowH));
	int32_t y;
	for (uint32_t i = 0; i < structure.contentButtons.size(); i = i + 1) {
		HorizontalSelectionWindowComponent component = this->components.at(i + this->offset);
		std::shared_ptr<const IDynamicString> pictureName = component.pictureName;
		StringLcl message = component.message;
		bool clickable = component.clickable;
		Events onClick = component.gEvent;
		std::optional<sf::IntRect> rect = component.rect;
		if (clickable) {
			onClick.add(std::make_shared<CloseWindowEvent>());
		}

		Button button(std::make_shared<LabelWithImage>(30 + this->componentSize, windowH - 10 - (this->componentSize + 10) * (i + 1), windowW - (50 + this->componentSize), this->componentSize, pictureName, message, rect), onClick);
		structure.contentButtons.at(i) = button;
		if (button.getY() > (int32_t)(windowH / 2)) {
			y = button.getY() - 10;
		}
	}

	if (this->possibleToMoveUp(structure.contentButtons.size())) {
		Events upEvent;
		upEvent.add(std::make_shared<MoveHorizontalSelectionWindowUpEvent>());
		upEvent.add(std::make_shared<PlaySoundEvent>("click"));
		structure.buttonUp = Button(std::make_shared<Image>(20, windowH - 10 - 2 * (this->componentSize + 10), this->componentSize, std::make_shared<StaticString>("up_icon")), upEvent);
	}
	
	if (this->possibleToMoveDown()) {
		Events downEvent;
		downEvent.add(std::make_shared<MoveHorizontalSelectionWindowDownEvent>());
		downEvent.add(std::make_shared<PlaySoundEvent>("click"));
		structure.buttonDown = Button(std::make_shared<Image>(20, windowH - 10 - (this->componentSize + 10), this->componentSize, std::make_shared<StaticString>("down_icon")), downEvent);
	}

	structure.rect = RectangularUiElement(10, y, windowW - 20, windowH - y - 10);

	return structure;
}