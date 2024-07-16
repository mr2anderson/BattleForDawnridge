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


#include "DragonFlame.hpp"
#include "Textures.hpp"


DragonFlame::DragonFlame() = default;
void DragonFlame::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (uint32_t x = this->getXMin(); x <= this->getXMax(); x = x + 1) {
		for (uint32_t y = this->getYMin(); y <= this->getYMax(); y = y + 1) {
			sf::RectangleShape shape;
			shape.setPosition(64 * x, 64 * y);
			shape.setSize(sf::Vector2f(64, 64));
			shape.setFillColor(this->getCurrentColor());
			target.draw(shape, states);
		}
	}
}
void DragonFlame::run(uint32_t windowW, uint32_t windowH) {
	this->timer.restart();
}
Events DragonFlame::click() {
	return Events();
}
void DragonFlame::update() {
	if (this->timer.getElapsedTime().asSeconds() >= this->getAnimationTime()) {
		this->finish();
	}
}
uint32_t DragonFlame::getXMin() const {
	if (this->getRadius() >= this->centerX) {
		return 0;
	}
	return this->centerX - this->getRadius();
}
uint32_t DragonFlame::getXMax() const {
	return this->centerX + this->getRadius();
}
uint32_t DragonFlame::getYMin() const {
	if (this->getRadius() >= this->centerY) {
		return 0;
	}
	return this->centerY - this->getRadius();
}
uint32_t DragonFlame::getYMax() const {
	return this->centerY + this->getRadius();
}
void DragonFlame::setCenterX(uint32_t newCenterX) {
	this->centerX = newCenterX;
}
void DragonFlame::setCenterY(uint32_t newCenterY) {
	this->centerY = newCenterY;
}
std::shared_ptr<DragonFlamePreview> DragonFlame::getPreview() const {
	std::shared_ptr<DragonFlamePreview> preview = this->getPreviewType();

	for (uint32_t x = this->getXMin(); x <= this->getXMax(); x = x + 1) {
		for (uint32_t y = this->getYMin(); y <= this->getYMax(); y = y + 1) {
			preview->addCellUnderTheFlame(x, y);
		}
	}

	return preview;
}
sf::Color DragonFlame::getCurrentColor() const {
	uint8_t component = 255 - std::min((uint32_t)255, (uint32_t)(255 * std::abs(this->timer.getElapsedTime().asSeconds() - this->getAnimationTime() / 2) / (this->getAnimationTime() / 2)));
	return sf::Color(0, 0, 0, component);
}