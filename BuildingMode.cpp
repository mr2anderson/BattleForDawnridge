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


#include "BuildingMode.hpp"


BuildingMode::BuildingMode(Building* b, sf::View *view, std::vector<GO*>* go, std::vector<TerritoryB*>* tb, Player *player) {
	this->b = b->clone();
	this->view = view;
	this->go = go;
	this->tb = tb;
	this->player = player;
}
BuildingMode::~BuildingMode() {
	if (!this->returnedPtr) {
		delete this->b;
	}
}
void BuildingMode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*this->b);
}
Events BuildingMode::run(uint32_t windowW2, uint32_t windowH2) {
	this->returnedPtr = false;
	this->windowW = windowW2;
	this->windowH = windowH2;
	return this->getHighlightEvent() + this->CameraDependentPopUpElement::run(windowW, windowH);
}
void BuildingMode::update() {
	uint32_t mouseX = sf::Mouse::getPosition().x;
	uint32_t mouseY = sf::Mouse::getPosition().y;

	this->b->setX((mouseX + this->view->getCenter().x - this->windowW / 2) / 32);
	this->b->setY((mouseY + this->view->getCenter().y - this->windowH / 2) / 32);
}
Events BuildingMode::click() {
	this->finish();
	if (!this->empty()) {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("place_occupied"), *Texts::get()->get("OK"));
		Events uiEvent;
		uiEvent.add(std::make_shared<CreateEEvent>(w));
		return this->getHighlightEvent() + uiEvent;
	}
	if (!this->controlled()) {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("too_far_from_roads"), *Texts::get()->get("OK"));
		Events uiEvent;
		uiEvent.add(std::make_shared<CreateEEvent>(w));
		return this->getHighlightEvent() + uiEvent;
	}
	Events gEvent = this->getHighlightEvent();
	gEvent.add(std::make_shared<BuildEvent>(this->b));
	gEvent.add(std::make_shared<SubResourcesEvent>(this->b->getCost()));
	gEvent.add(std::make_shared<PlaySoundEvent>(this->b->getSoundName()));
	this->returnedPtr = true;
	return gEvent;
}
bool BuildingMode::empty() const {
	sf::IntRect rect1;
	rect1.left = this->b->getX();
	rect1.top = this->b->getY();
	rect1.width = this->b->getSX();
	rect1.height = this->b->getSY();

	for (uint32_t i = 0; i < go->size(); i = i + 1) {
		if (!go->at(i)->exist()) {
			continue;
		}
		sf::IntRect rect2;
		rect2.left = go->at(i)->getX();
		rect2.top = go->at(i)->getY();
		rect2.width = go->at(i)->getSX();
		rect2.height = go->at(i)->getSY();
		if (rect1.intersects(rect2)) {
			return false;
		}
	}

	return true;
}
bool BuildingMode::controlled() const {
	uint32_t x = this->b->getX();
	uint32_t y = this->b->getY();
	uint32_t sx = this->b->getSX();
	uint32_t sy = this->b->getSY();

	for (uint32_t i = 0; i < tb->size(); i = i + 1) {
		if (tb->at(i)->allowBuilding(x, y, sx, sy, this->player)) {
			return true;
		}
	}

	return false;
}
Events BuildingMode::getHighlightEvent() const {
	Events result;
	for (uint32_t i = 0; i < tb->size(); i = i + 1) {
		result = result + tb->at(i)->getHighlightEvent();
	}
	return result;
}