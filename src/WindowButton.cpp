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


#include "WindowButton.hpp"
#include "ClosePopUpElementEvent.hpp"


WindowButton::WindowButton() = default;
WindowButton::WindowButton(const StringLcl& message, const StringLcl &buttonText, const Events &onFinish, uint32_t w, uint32_t h) {
    this->w = w;
    this->h = h;
    this->onFinish.add(std::make_shared<ClosePopUpElementEvent>());
    this->onFinish = this->onFinish + onFinish;
	this->message = message;
    this->buttonText = buttonText;
}
void WindowButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->getLabel(target.getSize().x, target.getSize().y), states);
	target.draw(this->getButton(target.getSize().x, target.getSize().y), states);
}
Events WindowButton::click(uint32_t mouseX, uint32_t mouseY, uint32_t windowW, uint32_t windowH) {
    Events event = this->getButton(windowW, windowH).click(mouseX, mouseY);
	return event;
}


Label WindowButton::getLabel(uint32_t windowW, uint32_t windowH) const {
    return Label((windowW - this->w) / 2, (windowH - this->h) / 2, this->w, this->h, this->message, true);
}


static const uint32_t BUTTON_W = 95;
static const uint32_t BUTTON_H = 30;


Button WindowButton::getButton(uint32_t windowW, uint32_t windowH) const {
    return Button(std::make_shared<Label>((windowW - this->w) / 2 + (this->w - BUTTON_W) / 2, (windowH - this->h) / 2 + h - BUTTON_H - 15, BUTTON_W, BUTTON_H, this->buttonText), this->onFinish);
}


BOOST_CLASS_EXPORT_IMPLEMENT(WindowButton)