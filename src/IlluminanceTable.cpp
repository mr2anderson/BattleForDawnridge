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
#include "IlluminationTableSettings.hpp"


IlluminanceTable::IlluminanceTable() = default;


void IlluminanceTable::createRender(uint32_t windowW, uint32_t windowH, const sf::ContextSettings& parentSettings) {
	if (!IlluminationTableSettings::get().isEnabled()) {
		return;
	}

	this->parentSettings = parentSettings;

	this->render = std::make_unique<sf::RenderTexture>();
	this->render->create(windowW, windowH, this->parentSettings);
}


void IlluminanceTable::newFrame(const sf::View& view, sf::Color additionalFilter) {
	if (!IlluminationTableSettings::get().isEnabled()) {
		return;
	}

	this->view = view;

	if (this->view.getSize().x != this->render->getSize().x or this->view.getSize().y != this->render->getSize().y) {
		this->render = std::make_unique<sf::RenderTexture>();
		this->render->create(this->view.getSize().x, this->view.getSize().y, this->parentSettings);
	}

	this->render->clear(sf::Color(0, 0, 0, 255 - IlluminationTableSettings::get().getBrightness()));

	this->render->setView(this->render->getDefaultView());
	sf::RectangleShape filterRect;
	filterRect.setPosition(0, 0);
	filterRect.setFillColor(additionalFilter);
	filterRect.setSize(sf::Vector2f(this->render->getSize().x, this->render->getSize().y));
	this->render->draw(filterRect);

	this->render->setView(view);
}


void IlluminanceTable::add(std::shared_ptr<const IWithLightSource> i) {
	if (!IlluminationTableSettings::get().isEnabled()) {
		return;
	}

	if (i->getLightSource()->inView(this->view)) {
		sf::RenderStates states;
		states.blendMode = sf::BlendNone;

		std::shared_ptr<sf::Drawable> drawable = i->getLightSource()->getDrawable();
		this->render->draw(*drawable, states);
	}
}


void IlluminanceTable::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!IlluminationTableSettings::get().isEnabled()) {
		return;
	}

	this->render->display();

	sf::Sprite sprite;
	sprite.setTexture(this->render->getTexture());
	sprite.setPosition(this->view.getCenter().x - this->view.getSize().x / 2, this->view.getCenter().y - this->view.getSize().y / 2);
	target.draw(sprite, states);
}