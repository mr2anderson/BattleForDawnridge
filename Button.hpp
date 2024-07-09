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



#include <memory>
#include "RectangularUIElement.hpp"
#include "FlyingE.hpp"


#pragma once


class Button : public sf::Drawable {
public:
	Button();
	Button(std::shared_ptr<RectangularUiElement> element, const Events &onClick);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Events click() const;
	void setX(int32_t x);
	void setY(int32_t y);
	int32_t getX() const;
	int32_t getY() const;
	uint32_t getW() const;
	uint32_t getH() const;
private:
    std::shared_ptr<RectangularUiElement> element;
    Events onClick;
};