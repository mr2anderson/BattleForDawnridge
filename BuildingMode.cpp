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
#include "FirstTimeTipsTable.hpp"
#include "EnableCursorEvent.hpp"
#include "DisableCursorEvent.hpp"
#include "Building.hpp"
#include "UnselectEvent.hpp"
#include "ResetHighlightEvent.hpp"


BuildingMode::BuildingMode() = default;
BuildingMode::BuildingMode(std::shared_ptr<const Building> b, uint32_t playerId) {
	this->b = b;
	this->playerId = playerId;
}
Events BuildingMode::start(MapState *state) {
	Events events;

	Events startEvent = this->getHighlightEvent(state);
	startEvent.add(std::make_shared<SelectEvent>(this));
	startEvent.add(std::make_shared<DisableCursorEvent>());

	if (FirstTimeTipsTable::get()->wasDisplayed("building_mode_guide")) {
		events = events + startEvent;
	}
	else {
		FirstTimeTipsTable::get()->markAsDisplayed("building_mode_guide");

		startEvent.add(std::make_shared<PlaySoundEvent>("click"));

		std::shared_ptr<WindowButton> buildingModeGuide = std::make_shared<WindowButton>(*Texts::get()->get("building_mode_guide"), *Texts::get()->get("OK"), startEvent);
		events.add(std::make_shared<CreateEEvent>(buildingModeGuide));
	}

	return events;
}
std::shared_ptr<sf::Drawable> BuildingMode::getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const {
	sf::Sprite sprite;
	sprite.setTexture(*Textures::get()->get(this->b->getTextureName()));
	sprite.setTextureRect(this->b->getTextureRect());
	sprite.setPosition(mouseX / 64 * 64, mouseY / 64 * 64);
	return std::make_shared<sf::Sprite>(sprite);
}
Events BuildingMode::unselect(MapState *state, uint32_t x, uint32_t y, uint8_t button) {
	Events clickSoundEvent;
	clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events exitEvent = clickSoundEvent;
	exitEvent.add(std::make_shared<UnselectEvent>());
	exitEvent.add(std::make_shared<EnableCursorEvent>());
	exitEvent.add(std::make_shared<ResetHighlightEvent>());

	if (button == sf::Mouse::Button::Right) {
		return exitEvent;
	}

	Building* clonedB = this->b->cloneBuilding();
	clonedB->setX(x);
	clonedB->setY(y);
	clonedB->changePlayer(this->playerId);

	if (!this->inMap(state, clonedB)) {
		delete clonedB;
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("not_in_map"), *Texts::get()->get("OK"), clickSoundEvent);
		exitEvent.add(std::make_shared<CreateEEvent>(w));
		return exitEvent;
	}
	if (!this->empty(state, clonedB)) {
		delete clonedB;
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("place_occupied"), *Texts::get()->get("OK"), clickSoundEvent);
		exitEvent.add(std::make_shared<CreateEEvent>(w));
		return exitEvent;
	}
	if (!this->controlled(state, clonedB)) {
		delete clonedB;
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("too_far_from_roads"), *Texts::get()->get("OK"), clickSoundEvent);
		exitEvent.add(std::make_shared<CreateEEvent>(w));
		return exitEvent;
	}

	exitEvent.add(std::make_shared<PlaySoundEvent>(clonedB->getSoundName()));
	exitEvent.add(std::make_shared<BuildEvent>(clonedB));
	exitEvent.add(std::make_shared<SubResourcesEvent>(clonedB->getCost()));

	return exitEvent;
}
Events BuildingMode::getHighlightEvent(MapState *state) const {
	Events result;
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		Building* b = state->getCollectionsPtr()->getBuilding(i);
		if (b->exist() and b->getPlayerId() == this->playerId and (b->isOrigin() or b->isActiveConductor())) {
			result = result + b->getHighlightEvent(state);
		}
	}
	return result;
}
bool BuildingMode::inMap(MapState *state, Building *clonedB) const {
	return
			(clonedB->getX() + clonedB->getSX() - 1 < state->getMapSizePtr()->getWidth() and
			clonedB->getY() + clonedB->getSY() - 1 < state->getMapSizePtr()->getHeight());
}
bool BuildingMode::empty(MapState* state, Building *clonedB) const {
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalGOs(); i = i + 1) {
		GO* go = state->getCollectionsPtr()->getGO(i);
		if (!go->exist()) {
			continue;
		}
		if (clonedB->intersects(go)) {
			return false;
		}
	}
	return true;
}
bool BuildingMode::controlled(MapState* state, Building *clonedB) const {
	uint32_t x = clonedB->getX();
	uint32_t y = clonedB->getY();
	uint32_t sx = clonedB->getSX();
	uint32_t sy = clonedB->getSY();

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		Building* b = state->getCollectionsPtr()->getBuilding(i);
		if (b->exist() and b->getPlayerId() == this->playerId and b->allowBuilding(state, x, y, sx, sy)) {
			return true;
		}
	}

	return false;
}