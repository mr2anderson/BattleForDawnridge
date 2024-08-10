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



#include "IButtonSpec.hpp"


#pragma once


class Button : public sf::Drawable {
public:
	Button();
	Button(std::shared_ptr<RectangularUiElement> element, const Events &onClick);
	Button(const IButtonSpec& spec);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Events click(uint32_t mouseX, uint32_t mouseY) const;
	int32_t getX() const;
	int32_t getY() const;
	uint32_t getW() const;
	uint32_t getH() const;
private:
    std::shared_ptr<RectangularUiElement> element;
    Events onClick;
};