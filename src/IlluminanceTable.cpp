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


#include "IlluminanceTable.hpp"


IlluminanceTable::IlluminanceTable() = default;


void IlluminanceTable::createRender(uint32_t windowW, uint32_t windowH, const sf::ContextSettings& parentSettings) {
	this->render = std::make_unique<sf::RenderTexture>();
	this->render->create(windowW, windowH, parentSettings);
}


void IlluminanceTable::newFrame(const sf::View& view) {
	this->view = view;

	this->render->clear(sf::Color(0, 0, 0, 100));
	this->render->setView(view);
}


void IlluminanceTable::add(std::shared_ptr<const IWithLightSource> i) {
	if (i->getLightSource()->inView(this->view)) {
		sf::RenderStates states;
		states.blendMode = sf::BlendNone;

		std::shared_ptr<sf::Drawable> drawable = i->getLightSource()->getDrawable();
		this->render->draw(*drawable, states);
	}
}


void IlluminanceTable::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->render->display();

	sf::Sprite sprite;
	sprite.setTexture(this->render->getTexture());
	sprite.setPosition(this->view.getCenter().x - this->view.getSize().x / 2, this->view.getCenter().y - this->view.getSize().y / 2);
	target.draw(sprite, states);
}