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


#include <set>
#include "BuildingMode.hpp"
#include "BuildEvent.hpp"
#include "SubResourcesEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "Locales.hpp"
#include "WindowButton.hpp"
#include "CreateEEvent.hpp"
#include "Textures.hpp"
#include "SelectEvent.hpp"
#include "EnableCursorEvent.hpp"
#include "DisableCursorEvent.hpp"
#include "Building.hpp"
#include "UnselectEvent.hpp"
#include "ResetHighlightEvent.hpp"
#include "IAreaControllerSpec.hpp"
#include "Warrior.hpp"
#include "SetHighlightEvent.hpp"


BuildingMode::BuildingMode() = default;
BuildingMode::BuildingMode(std::shared_ptr<const Building>b, uint32_t playerId) {
	this->b = b;
	this->playerId = playerId;
}
Events BuildingMode::start(MapState *state) {
	Events events;

	Events startEvent = this->getHighlightEvent(state);
	startEvent.add(std::make_shared<SelectEvent>(this->getThis<ISelectable>()));
	startEvent.add(std::make_shared<DisableCursorEvent>());

	events = events + startEvent;

	return events;
}
std::shared_ptr<sf::Drawable> BuildingMode::getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const {
	sf::Sprite sprite;
	sprite.setTexture(*Textures::get().get(this->b->getTextureName()));
	sprite.setTextureRect(this->b->getTextureRect());
	sprite.setPosition(mouseX / 64 * 64, mouseY / 64 * 64);
	return std::make_shared<sf::Sprite>(sprite);
}
Events BuildingMode::onUnselect(MapState *state, uint32_t x, uint32_t y, uint8_t button) {
	Events clickSoundEvent;
	clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events exitEvent;
	exitEvent.add(std::make_shared<ResetHighlightEvent>());

	if (button == sf::Mouse::Button::Right) {
		return exitEvent;
	}

	std::shared_ptr<Building>  clonedB = this->b->createSameTypeBuilding();
	clonedB->setX(x);
	clonedB->setY(y);
	clonedB->changePlayer(this->playerId);

	if (!this->inMap(state, clonedB)) {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{not_in_map}"), StringLcl("{OK}"), clickSoundEvent);
		exitEvent = exitEvent + clickSoundEvent;
		exitEvent.add(std::make_shared<CreateEEvent>(w));
		return exitEvent;
	}
	if (!this->empty(state, clonedB)) {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{place_occupied}"), StringLcl("{OK}"), clickSoundEvent);
		exitEvent = exitEvent + clickSoundEvent;
		exitEvent.add(std::make_shared<CreateEEvent>(w));
		return exitEvent;
	}
	if (!this->controlled(state, clonedB)) {
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{too_far_from_roads}"), StringLcl("{OK}"), clickSoundEvent);
		exitEvent = exitEvent + clickSoundEvent;
		exitEvent.add(std::make_shared<CreateEEvent>(w));
		return exitEvent;
	}
    if (!this->noEnemyWarriors(state, clonedB)) {
        std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{enemy_warriors}"), StringLcl("{OK}"), clickSoundEvent);
        exitEvent = exitEvent + clickSoundEvent;
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
		std::shared_ptr<Building>  building = state->getCollectionsPtr()->getBuilding(i);
		if (building->exist()) {
            if (building->getPlayerId() == this->playerId) {
                result = result + building->getHighlightEvent(state, IAreaControllerSpec::HIGHLIGHT_TYPE::TERRITORY);
            }
		}
	}
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
        std::shared_ptr<Warrior>  w = state->getCollectionsPtr()->getWarrior(i);
        if (w->exist() and w->getPlayerId() != this->playerId and w->blockBuildingAbility()) {
            result = result + w->getMoveHighlightionEvent(state);
        }
    }
	return result;
}
bool BuildingMode::inMap(MapState *state, std::shared_ptr<const Building> clonedB) const {
	return
			(clonedB->getX() + clonedB->getSX() - 1 < state->getMapSizePtr()->getWidth() and
			clonedB->getY() + clonedB->getSY() - 1 < state->getMapSizePtr()->getHeight());
}
bool BuildingMode::empty(MapState* state, std::shared_ptr<const Building> clonedB) const {
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalGOs(); i = i + 1) {
		std::shared_ptr<GO> go = state->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
		if (!go->exist()) {
			continue;
		}
		if (clonedB->intersects(go)) {
			return false;
		}
	}
	return true;
}
bool BuildingMode::controlled(MapState* state, std::shared_ptr<const Building> clonedB) const {
	uint32_t x = clonedB->getX();
	uint32_t y = clonedB->getY();
	uint32_t sx = clonedB->getSX();
	uint32_t sy = clonedB->getSY();

    std::set<std::tuple<uint32_t, uint32_t>> ds; // splitting on small sections in order to avoid situations when area is highlighted but building is now allowed cuz it is highlighted by two (or more) different territory buildings
    for (uint32_t dx = 0; dx < sx; dx = dx + 1) {
        for (uint32_t dy = 0; dy < sy; dy = dy + 1) {
            ds.emplace(x + dx, y + dy);
        }
    }

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		std::shared_ptr<Building>  b = state->getCollectionsPtr()->getBuilding(i);
		if (b->exist() and b->getPlayerId() == this->playerId) {
			Events highlightEvent = b->getHighlightEvent(state, IAreaControllerSpec::HIGHLIGHT_TYPE::TERRITORY);
			for (uint32_t i = 0; i < highlightEvent.size(); i = i + 1) {
				std::shared_ptr<SetHighlightEvent> e = std::static_pointer_cast<SetHighlightEvent>(highlightEvent.at(i));
				std::tuple<uint32_t, uint32_t> t = std::make_tuple(e->getX(), e->getY());
				auto it = ds.find(t);
				if (it != ds.end()) {
					ds.erase(it);
					if (ds.empty()) {
						return true;
					}
				}
			}
		}
	}

    return false;
}
bool BuildingMode::noEnemyWarriors(MapState *state, std::shared_ptr<const Building> clonedB) const {
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
        std::shared_ptr<Warrior> w = state->getCollectionsPtr()->getWarrior(i);
        if (w->exist() and w->getPlayerId() != this->playerId and w->blockBuildingAbility()) {
            std::vector<std::tuple<uint32_t, uint32_t>> moves = w->getMoves(state);
            for (const auto move : moves) {
                uint32_t moveX, moveY;
                std::tie(moveX, moveY) = move;
                if (moveX >= clonedB->getX() and moveY >= clonedB->getY() and moveX < clonedB->getX() + clonedB->getSX() and moveY < clonedB->getY() + clonedB->getSY()) {
                    return false;
                }
            }
        }
    }

    return true;
}


BOOST_CLASS_EXPORT_IMPLEMENT(BuildingMode)