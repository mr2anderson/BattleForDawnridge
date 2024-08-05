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


#include "WindowButtonImage.hpp"
#include "CloseWindowEvent.hpp"
#include "Textures.hpp"


WindowButtonImage::WindowButtonImage(const std::wstring& message, const std::wstring& buttonText, const std::string &pictureName, const Events& onFinish, uint32_t w, uint32_t h) {
    this->w = w;
    this->h = h;
    this->onFinish = onFinish;
    this->message = message;
    this->buttonText = buttonText;
    this->pictureName = pictureName;
    this->inited = false;
}
void WindowButtonImage::run(uint32_t windowW, uint32_t windowH) {
    if (!this->inited) {
        this->inited = true;

        uint32_t buttonW = 95;
        uint32_t buttonH = 30;

        Events onClick;
        onClick.add(std::make_shared<CloseWindowEvent>());
        onClick = onClick + onFinish;

        this->label = Label((windowW - this->w) / 2, (windowH - this->h) / 2, this->w, this->h, message);
        this->button = Button(std::make_shared<Label>(this->label.getX() + (this->w - buttonW) / 2, this->label.getY() + h - buttonH - 15, buttonW, buttonH, buttonText), onClick);
        sf::Texture* t = Textures::get()->get(this->pictureName);
        this->image = Image(this->label.getX() + (this->w - t->getSize().x) / 2, this->button.getY() - 15 - t->getSize().y, this->pictureName);
    }
}
void WindowButtonImage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(this->label, states);
    target.draw(this->image, states);
    target.draw(this->button, states);
}
Events WindowButtonImage::click() {
    Events event = this->button.click();
    for (uint32_t i = 0; i < event.size(); i = i + 1) {
        if (std::shared_ptr<CloseWindowEvent> closeWindowEvent = std::dynamic_pointer_cast<CloseWindowEvent>(event.at(i))) {
            this->finish();
            break;
        }
    }
    return event;
}