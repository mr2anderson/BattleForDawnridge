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


#include "WindowTwoButtons.hpp"
#include "CloseWindowEvent.hpp"
#include "PlaySoundEvent.hpp"


WindowTwoButtons::WindowTwoButtons(const std::string& soundName1, const std::string& soundName2, const std::wstring& message, const std::wstring& buttonText1, const std::wstring& buttonText2, const Events& events1, const Events& events2, uint32_t w, uint32_t h) {
    this->w = w;
    this->h = h;
    this->soundName1 = soundName1;
    this->soundName2 = soundName2;
    this->message = message;
    this->buttonText1 = buttonText1;
    this->buttonText2 = buttonText2;
    this->events1 = events1;
    this->events2 = events2;
    this->inited = false;
}
void WindowTwoButtons::addEvent1(const Events &events) {
    this->events1 = this->events1 + events;
}
void WindowTwoButtons::addEvent2(const Events& events) {
    this->events2 = this->events2 + events;
}
Events WindowTwoButtons::run(uint32_t windowW, uint32_t windowH) {
    Events soundEvent1;
    if (!this->soundName1.empty()) {
        soundEvent1.add(std::make_shared<PlaySoundEvent>(this->soundName1));
    }

    if (!this->inited) {
        this->inited = true;

        uint32_t buttonW = 95;
        uint32_t buttonH = 30;

        Events onClick;
        if (!this->soundName2.empty()) {
            onClick.add(std::make_shared<PlaySoundEvent>(this->soundName2));
        }
        onClick.add(std::make_shared<CloseWindowEvent>());

        this->label = Label((windowW - this->w) / 2, (windowH - this->h) / 2, this->w, this->h, message);
        this->button1 = Button(std::make_shared<Label>((windowW - this->w) / 2 + this->w / 5 + 5, (windowH - this->h) / 2 + h - buttonH - 15, buttonW, buttonH, buttonText1), onClick + this->events1);
        this->button2 = Button(std::make_shared<Label>((windowW - this->w) / 2 + this->w - buttonW - this->w / 5 - 5, (windowH - this->h) / 2 + h - buttonH - 15, buttonW, buttonH, buttonText2), onClick + this->events2);
    }

    return soundEvent1;
}
void WindowTwoButtons::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(label, states);
    target.draw(button1, states);
    target.draw(button2, states);
}
Events WindowTwoButtons::click() {
    Events event = this->button1.click();
    for (uint32_t i = 0; i < event.size(); i = i + 1) {
        if (std::shared_ptr<CloseWindowEvent> closeWindowEvent = std::dynamic_pointer_cast<CloseWindowEvent>(event.at(i))) {
            this->finish();
            return event;
        }
    }

    event = this->button2.click();
    for (uint32_t i = 0; i < event.size(); i = i + 1) {
        if (std::shared_ptr<CloseWindowEvent> closeWindowEvent = std::dynamic_pointer_cast<CloseWindowEvent>(event.at(i))) {
            this->finish();
            return event;
        }
    }
   
    return Events();
}