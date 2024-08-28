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
#include "ClosePopUpElementEvent.hpp"
#include "MoveHorizontalSelectionWindowUpEvent.hpp"
#include "MoveHorizontalSelectionWindowDownEvent.hpp"
#include "PlaySoundEvent.hpp"


static constexpr uint32_t COMPONENTS_IN_FRAME = 9;


HorizontalSelectionWindow::HorizontalSelectionWindow() = default;
HorizontalSelectionWindow::HorizontalSelectionWindow(const std::vector<HorizontalSelectionWindowComponent> &components, uint32_t componentSize, uint32_t marginSize) {
	this->components = components;
	this->componentSize = componentSize;
	this->marginSize = marginSize;
	this->offset = 0;
}
void HorizontalSelectionWindow::onRestart() {
	this->offset = 0;
}
void HorizontalSelectionWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	HorizontalSelectionWindowStructure structure = this->getUIStructure(target.getSize().x, target.getSize().y);
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
	Events result;

	HorizontalSelectionWindowStructure structure = this->getStructure(windowW, windowH);

	if (structure.buttonUp.has_value()) {
		Events events = structure.buttonUp.value().click(mouseX, mouseY);
		result = result + events;
	}

	if (structure.buttonDown.has_value()) {
		Events events = structure.buttonDown.value().click(mouseX, mouseY);
		result = result + events;
	}

	for (const auto& b : structure.contentButtons) {
		Events event = b.click(mouseX, mouseY);
        result = result + event;
	}

	return result;
}
bool HorizontalSelectionWindow::possibleToMoveUp() const {
	return (this->offset + COMPONENTS_IN_FRAME < this->components.size());
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


HorizontalSelectionWindowStructure HorizontalSelectionWindow::getUIStructure(uint32_t windowW, uint32_t windowH) const {
    HorizontalSelectionWindowStructure structure;

    structure.contentButtons.resize(COMPONENTS_IN_FRAME);
    for (uint32_t i = 0; i < structure.contentButtons.size(); i = i + 1) {
        HorizontalSelectionWindowComponent component = this->components.at(i + this->offset);
        std::string pictureName = component.pictureName;
        StringLcl message = component.message;
        bool clickable = component.clickable;
        Events onClick = component.gEvent;
        boost::optional<IntRectSerializable> rect = component.rect;
        if (clickable) {
            onClick.add(std::make_shared<ClosePopUpElementEvent>());
        }

        Button button(std::make_shared<LabelWithImage>(3 * this->marginSize + this->componentSize, windowH - this->marginSize - (this->componentSize + this->marginSize) * (i + 1), windowW - (5 * this->marginSize + this->componentSize), this->componentSize, pictureName, message, rect), onClick);
        structure.contentButtons.at(i) = button;
    }

    if (this->possibleToMoveUp()) {
        structure.buttonUp = Button(std::make_shared<Image>(2 * this->marginSize, windowH - this->marginSize - 2 * (this->componentSize + this->marginSize), this->componentSize, "up_icon"), Events());
    }

    if (this->possibleToMoveDown()) {
        structure.buttonDown = Button(std::make_shared<Image>(2 * this->marginSize, windowH - this->marginSize - (this->componentSize + this->marginSize), this->componentSize, "down_icon"), Events());
    }

    structure.rect = RectangularUiElement(this->marginSize, structure.contentButtons.back().getY() - this->marginSize, windowW - 2 * this->marginSize, windowH - (structure.contentButtons.back().getY() - this->marginSize) - this->marginSize);

    return structure;
}
HorizontalSelectionWindowStructure HorizontalSelectionWindow::getStructure(uint32_t windowW, uint32_t windowH) {
	HorizontalSelectionWindowStructure structure = this->getUIStructure(windowW, windowH);

    if (this->possibleToMoveUp()) {
        Events upEvent;
        upEvent.add(std::make_shared<MoveHorizontalSelectionWindowUpEvent>(this->getThis<HorizontalSelectionWindow>()));
        upEvent.add(std::make_shared<PlaySoundEvent>("click", true));
        structure.buttonUp->add(upEvent);
    }

    if (this->possibleToMoveDown()) {
        Events downEvent;
        downEvent.add(std::make_shared<MoveHorizontalSelectionWindowDownEvent>(this->getThis<HorizontalSelectionWindow>()));
        downEvent.add(std::make_shared<PlaySoundEvent>("click", true));
        structure.buttonDown->add(downEvent);
    }

    return structure;
}


BOOST_CLASS_EXPORT_IMPLEMENT(HorizontalSelectionWindow)