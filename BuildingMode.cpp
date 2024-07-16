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
#include "BuildEvent.hpp"
#include "SubResourcesEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "Texts.hpp"
#include "WindowButton.hpp"
#include "CreateEEvent.hpp"
#include "Textures.hpp"
#include "SelectEvent.hpp"


BuildingMode::BuildingMode(std::shared_ptr<const Building> b, std::shared_ptr<GOCollection<GO>> go, std::shared_ptr<GOCollection<TerritoryB>> tb, uint32_t playerId, uint32_t mapW, uint32_t mapH) {
	this->b = b;
	this->view = view;
	this->go = go;
	this->tb = tb;
	this->playerId = playerId;
	this->mapW = mapW;
	this->mapH = mapH;
}
Events BuildingMode::start() {
	Events events = this->getHighlightEvent();

	events.add(std::make_shared<SelectEvent>(this));

	return events;
}
std::shared_ptr<sf::Drawable> BuildingMode::getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const {
	sf::Sprite sprite;
	sprite.setTexture(*Textures::get()->get(this->b->getTextureName()));
	sprite.setTextureRect(this->b->getTextureRect());
	sprite.setPosition(mouseX / 64 * 64, mouseY / 64 * 64);
	return std::make_shared<sf::Sprite>(sprite);
}
Events BuildingMode::unselect(uint32_t x, uint32_t y, uint8_t button) {
	Events clickSoundEvent;
	clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	if (button == sf::Mouse::Button::Right) {
		delete this;
		return this->Selectable::unselect(x, y, button) + this->getHighlightEvent() + clickSoundEvent;
	}

	Building* clonedB = this->b->cloneBuilding();
	clonedB->setX(x);
	clonedB->setY(y);
	clonedB->changePlayer(this->playerId);

	if (!this->inMap(clonedB)) {
		delete clonedB;
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("not_in_map"), *Texts::get()->get("OK"), clickSoundEvent);
		Events uiEvent = clickSoundEvent;
		uiEvent.add(std::make_shared<CreateEEvent>(w));
		delete this;
		return this->Selectable::unselect(x, y, button) + this->getHighlightEvent() + uiEvent;
	}
	if (!this->empty(clonedB)) {
		delete clonedB;
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("place_occupied"), *Texts::get()->get("OK"), clickSoundEvent);
		Events uiEvent = clickSoundEvent;
		uiEvent.add(std::make_shared<CreateEEvent>(w));
		delete this;
		return this->Selectable::unselect(x, y, button) + this->getHighlightEvent() + uiEvent;
	}
	if (!this->controlled(clonedB)) {
		delete clonedB;
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("too_far_from_roads"), *Texts::get()->get("OK"), clickSoundEvent);
		Events uiEvent = clickSoundEvent;
		uiEvent.add(std::make_shared<CreateEEvent>(w));
		delete this;
		return this->Selectable::unselect(x, y, button) + this->getHighlightEvent() + uiEvent;
	}

	Events gEvent = this->getHighlightEvent();
	gEvent.add(std::make_shared<PlaySoundEvent>(clonedB->getSoundName()));
	gEvent.add(std::make_shared<BuildEvent>(clonedB));
	gEvent.add(std::make_shared<SubResourcesEvent>(clonedB->getCost()));

	delete this;
	return this->Selectable::unselect(x, y, button) + gEvent;
}
Events BuildingMode::getHighlightEvent() const {
	Events result;
	for (uint32_t i = 0; i < tb->size(); i = i + 1) {
		if (tb->at(i)->works() and tb->at(i)->getPlayerId() == this->playerId) {
			result = result + tb->at(i)->getHighlightEvent();
		}
	}
	return result;
}
bool BuildingMode::inMap(Building *clonedB) const {
	return
			(clonedB->getX() + clonedB->getSX() - 1 < this->mapW and
			clonedB->getY() + clonedB->getSY() - 1 < this->mapH);
}
bool BuildingMode::empty(Building *clonedB) const {
	for (uint32_t i = 0; i < go->size(); i = i + 1) {
		if (!go->at(i)->exist()) {
			continue;
		}
		if (clonedB->intersects(go->at(i))) {
			return false;
		}
	}
	return true;
}
bool BuildingMode::controlled(Building *clonedB) const {
	uint32_t x = clonedB->getX();
	uint32_t y = clonedB->getY();
	uint32_t sx = clonedB->getSX();
	uint32_t sy = clonedB->getSY();

	for (uint32_t i = 0; i < tb->size(); i = i + 1) {
		if (tb->at(i)->allowBuilding(x, y, sx, sy, this->playerId)) {
			return true;
		}
	}

	return false;
}