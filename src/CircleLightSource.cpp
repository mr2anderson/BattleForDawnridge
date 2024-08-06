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


#include "CircleLightSource.hpp"


CircleLightSource::CircleLightSource(float centerX, float centerY, float maxRadius) {
	this->centerX = centerX;
	this->centerY = centerY;
	this->maxRadius = maxRadius;
}
std::shared_ptr<sf::Drawable> CircleLightSource::getDrawable() const {
	sf::CircleShape shape;
	shape.setOrigin(this->getRadius(), this->getRadius());
	shape.setPosition(this->centerX, this->centerY);
	shape.setRadius(this->getRadius());
	shape.setPointCount(360);
	shape.setFillColor(sf::Color::Transparent);
	return std::make_shared<sf::CircleShape>(shape);
}
bool CircleLightSource::inView(const sf::View& view) const {
	sf::FloatRect thisRect;
	thisRect.left = this->centerX - this->maxRadius;
	thisRect.top = this->centerY - this->maxRadius;
	thisRect.width = 2 * this->maxRadius;
	thisRect.height = 2 * this->maxRadius;

	sf::FloatRect viewRect;
	viewRect.left = view.getCenter().x - view.getSize().x / 2;
	viewRect.top = view.getCenter().y - view.getSize().y / 2;
	viewRect.width = view.getSize().x;
	viewRect.height = view.getSize().y;

	return thisRect.intersects(viewRect);
}
float CircleLightSource::getMaxRadius() const {
	return this->maxRadius;
}