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
LabelWithImage::LabelWithImage(int32_t x, int32_t y, uint32_t sumW, uint32_t size, const std::string& textureName, const StringLcl &message, boost::optional<IntRectSerializable> rect, bool colorDigits) : RectangularUiElement(x, y, sumW, size) {
    this->image = Image(x, y, size, textureName, rect);
    this->label = Label(x + size + 5, y, sumW - this->image.getW() - 5, size, message, false, colorDigits);
}
void LabelWithImage::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(this->image, states);
    target.draw(this->label, states);
}


BOOST_CLASS_EXPORT_IMPLEMENT(LabelWithImage)