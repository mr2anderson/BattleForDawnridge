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


#include "LabelWithImage.hpp"


LabelWithImage::LabelWithImage() = default;
LabelWithImage::LabelWithImage(int32_t x, int32_t y, uint32_t sumW, uint32_t size, std::shared_ptr<const IDynamicString> textureName, const StringLcl &message, std::optional<sf::IntRect> rect) : RectangularUiElement(x, y, sumW, size) {
    this->image = Image(x, y, size, textureName, rect);
    this->label = Label(x + size + 5, y, sumW - this->image.getW() - 5, size, message, false);
}
void LabelWithImage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(this->image, states);
    target.draw(this->label, states);
}
void LabelWithImage::setX(int32_t newX) {
    this->RectangularUiElement::setX(newX);
    this->image.setX(newX);
    this->label.setX(newX);
}
void LabelWithImage::setY(int32_t newY) {
    this->RectangularUiElement::setY(newY);
    this->image.setY(newY);
    this->label.setY(newY);
}