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


#include "WindowEntry.hpp"
#include "CloseWindowEvent.hpp"


WindowEntry::WindowEntry() {
    this->dst = nullptr;
    this->settings = nullptr;
}
WindowEntry::WindowEntry(const StringLcl& message, const StringLcl &buttonText1, const StringLcl &buttonText2, std::shared_ptr<std::string> dst, std::shared_ptr<EntrySettings> settings, const Events &events1, const Events &events2, uint32_t w, uint32_t h) {
    this->w = w;
    this->h = h;
    this->dst = dst;
    this->settings = settings;
    this->events1 = events1;
    this->events2 = events2;
    this->events1.add(std::make_shared<CloseWindowEvent>());
    this->events2.add(std::make_shared<CloseWindowEvent>());
    this->message = message;
    this->buttonText1 = buttonText1;
    this->buttonText2 = buttonText2;
}
void WindowEntry::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(this->getLabel(target.getSize().x, target.getSize().y), states);
    if (this->settings == nullptr or this->dst == nullptr or this->settings->ok(this->dst)) {
        target.draw(this->getButton1(target.getSize().x, target.getSize().y), states);
    }
    target.draw(this->getButton2(target.getSize().x, target.getSize().y), states);
}
Events WindowEntry::click(uint32_t mouseX, uint32_t mouseY, uint32_t windowW, uint32_t windowH) {
    Events event = this->getButton1(windowW, windowH).click(mouseX, mouseY);
    for (uint32_t i = 0; i < event.size(); i = i + 1) {
        if (std::shared_ptr<CloseWindowEvent> closeWindowEvent = std::dynamic_pointer_cast<CloseWindowEvent>(event.at(i))) {
            event.erase(i);
            this->finish();
            return event;
        }
    }

    event = this->getButton2(windowW, windowH).click(mouseX, mouseY);
    for (uint32_t i = 0; i < event.size(); i = i + 1) {
        if (std::shared_ptr<CloseWindowEvent> closeWindowEvent = std::dynamic_pointer_cast<CloseWindowEvent>(event.at(i))) {
            event.erase(i);
            this->finish();
            return event;
        }
    }

    return Events();
}
void WindowEntry::keyPressed(sf::Keyboard::Key key) {
    if (this->dst != nullptr and this->settings != nullptr) {
        this->settings->mod(this->dst, key);
    }
}


Label WindowEntry::getLabel(uint32_t windowW, uint32_t windowH) const {
    StringLcl currentMessage = this->message;
    if (this->dst != nullptr) {
        currentMessage = currentMessage + "\n";
        if (this->dst->empty()) {
            currentMessage = currentMessage + "{nothing_is_entered}";
        }
        else {
            currentMessage = currentMessage + *this->dst;
        }
    }
    return Label((windowW - this->w) / 2, (windowH - this->h) / 2, this->w, this->h, currentMessage);
}


static const uint32_t BUTTON_W = 95;
static const uint32_t BUTTON_H = 30;


Button WindowEntry::getButton1(uint32_t windowW, uint32_t windowH) const {
    return Button(std::make_shared<Label>((windowW - this->w) / 2 + this->w / 5 + 5, (windowH - this->h) / 2 + h - BUTTON_H - 15, BUTTON_W, BUTTON_H, this->buttonText1), this->events1);
}


Button WindowEntry::getButton2(uint32_t windowW, uint32_t windowH) const {
    return Button(std::make_shared<Label>((windowW - this->w) / 2 + this->w - BUTTON_W - this->w / 5 - 5, (windowH - this->h) / 2 + h - BUTTON_H - 15, BUTTON_W, BUTTON_H, this->buttonText2), this->events2);
}


BOOST_CLASS_EXPORT_IMPLEMENT(WindowEntry)