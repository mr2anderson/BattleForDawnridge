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


WindowButton::WindowButton(const std::string& soundName1, const std::string& soundName2, const std::wstring& message, const std::wstring &buttonText, uint32_t w, uint32_t h) :
	PopUpElement() {
    this->w = w;
    this->h = h;
    this->soundName1 = soundName1;
    this->soundName2 = soundName2;
	this->message = message;
    this->buttonText = buttonText;
    this->inited = false;
}
Event WindowButton::run(uint32_t windowW, uint32_t windowH) {
    Event soundEvent1 = this->PopUpElement::run(windowW, windowH);
    soundEvent1.addPlaySoundEvent(this->soundName1);

    if (!this->inited) {
        this->inited = true;

        uint32_t buttonW = 75;
        uint32_t buttonH = 30;

        Event onClick;
        onClick.addPlaySoundEvent(this->soundName2);
        onClick.addCloseThisWindowEvent();

        this->label = Label((windowW - this->w) / 2, (windowH - this->h) / 2, this->w, this->h, message);
        this->button = Button(std::make_shared<Label>((windowW - this->w) / 2 + (this->w - buttonW) / 2, (windowH - this->h) / 2 + h - buttonH - 15, buttonW, buttonH, buttonText), onClick);
    }

    return soundEvent1;
}
void WindowButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(label, states);
	target.draw(button, states);
}
Event WindowButton::click() {
    Event event = this->button.click();
    if (event.getCloseThisWindowEvent()) {
        this->finish();
    }
	return event;
}
void WindowButton::update() {

}