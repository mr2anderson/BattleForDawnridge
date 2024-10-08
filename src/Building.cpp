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


#include "Building.hpp"
#include "PlaySoundEvent.hpp"
#include "Locales.hpp"
#include "ImageFlyingE.hpp"
#include "HPFlyingE.hpp"
#include "BuildingStatePointer.hpp"
#include "CreateEEvent.hpp"
#include "AddHpEvent.hpp"
#include "WindowTwoButtons.hpp"
#include "DestroyEvent.hpp"
#include "DecreaseBurningMovesLeftEvent.hpp"
#include "SubHpEvent.hpp"
#include "SetFireEvent.hpp"
#include "FocusOnEvent.hpp"
#include "ResetHighlightEvent.hpp"
#include "ConductionGraph.hpp"
#include "Parameters.hpp"
#include "IAreaControllerSpec.hpp"
#include "EmptyLightSource.hpp"
#include "ColorTheme.hpp"


Building::Building() = default;
Building::Building(uint32_t x, uint32_t y, uint32_t playerId) :
	Unit(x, y, 1, playerId) {
    this->_wasWithFullHP = false;
	this->burningMovesLeft = 0;
}
Building::Building(const Building& building) {
	*this = building;
	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		this->specs.at(i) = building.specs.at(i)->clone();
	}
}
Events Building::getHighlightEvent(MapState *state, uint8_t type) const {
	Events highlightEvent;

	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		highlightEvent = highlightEvent + this->specs.at(i)->getHighlightEvent(this->getThis<Building>(), state, type);
	}

	return highlightEvent;
}
Resources Building::getLimit() const {
	Resources limit;

	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		limit.plus(this->specs.at(i)->getLimit(this->getThis<Building>()));
	}

	return limit;
}
uint32_t Building::getPopulationLimit() const {
	uint32_t populationLimit = 0;

	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		populationLimit = populationLimit + this->specs.at(i)->getPopulationLimit(this->getThis<Building>());
	}

	return populationLimit;
}
bool Building::isVictoryCondition() const {
	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		if (this->specs.at(i)->isVictoryCondition()) {
			return true;
		}
	}

	return false;
}
bool Building::isOrigin() const {
	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		if (this->specs.at(i)->isOrigin()) {
			return true;
		}
	}

	return false;
}
bool Building::isActiveConductor() const {
	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		if (this->specs.at(i)->isActiveConductor(this->getThis<Building>())) {
			return true;
		}
	}

	return false;
}
bool Building::works() const {
	return (this->getHP() == this->getMaxHP());
}
bool Building::wasWithFullHP() const {
    return this->_wasWithFullHP;
}
NewMoveMainPriority Building::getNewMoveMainPriority() const {
    NewMoveMainPriority priority = this->Unit::getNewMoveMainPriority();

    for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
        priority = std::max(priority, this->specs.at(i)->getNewMoveMainPriority());
    }

    return priority;
}
void Building::addHp(uint32_t delta) {
    this->Unit::addHp(delta);
    if (this->getHP() == this->getMaxHP()) {
        this->_wasWithFullHP = true;
    }
}
bool Building::connectedTo(MapState* state, std::shared_ptr<GO> go) const {
	ConductionGraph g;

	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		std::shared_ptr<Building>  b = state->getCollectionsPtr()->getBuilding(i);
		if (b->isActiveConductor() and b->getPlayerId() == this->getPlayerId()) {
			g.addConductor(b->getX(), b->getY(), b->getSX(), b->getSY());
		}
	}

	g.addDestination(go->getX(), go->getY(), go->getSX(), go->getSY());

	return g.connectedToDestination(this->getX(), this->getY(), this->getSX(), this->getSY());
}
bool Building::connectedToOrigin(MapState* state) const {
	for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
		std::shared_ptr<Building>  b = state->getCollectionsPtr()->getBuilding(i);
		if (b->works() and b->isOrigin() and b->getPlayerId() == this->getPlayerId()) {
			if (this->connectedTo(state, b)) {
				return true;
			}
		}
	}

	return false;
}
Events Building::destroy(MapState *state) {
    Events response;
	response.add(std::make_shared<SubHpEvent>(this->getThis<Building>(), this->getHP()));

	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		response = response + this->specs.at(i)->getEventOnDestroy(this->getThis<Building>(), state);
	}

	return response;
}
void Building::decreaseBurningMovesLeft() {
	this->burningMovesLeft = this->burningMovesLeft - 1;
}
void Building::setFire() {
	this->burningMovesLeft = Parameters::get().getInt("fire_lifetime");
}
void Building::addSpec(std::shared_ptr<IBuildingSpec> spec) {
	this->specs.push_back(spec);
}
uint32_t Building::getWarriorMovementCost(std::shared_ptr<Warrior> w) const {
	uint32_t cost = 1;

	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		uint32_t specMovementCost = this->specs.at(i)->getWarriorMovementCost(this->getThis<Building>(), w);
		cost = std::max(cost, specMovementCost);
	}

	return cost;
}
bool Building::warriorCanStay(std::shared_ptr<const Warrior> w) const {
	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		if (!this->specs.at(i)->warriorCanStay(this->getThis<Building>(), w)) {
			return false;
		}
	}

	return true;
}
bool Building::isUltraHighObstacle(uint32_t playerId) const {
	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		if (this->specs.at(i)->isUltraHighObstacle(this->getThis<Building>(), playerId)) {
			return true;
		}
	}

	return false;
}
bool Building::isHighObstacle(uint32_t playerId) const {
	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		if (this->specs.at(i)->isHighObstacle(this->getThis<Building>(), playerId)) {
			return true;
		}
	}

	return false;
}
bool Building::isLowObstacle(uint32_t playerId) const {
	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		if (this->specs.at(i)->isLowObstacle(this->getThis<Building>(), playerId)) {
			return true;
		}
	}

	return false;
}
HorizontalSelectionWindowComponent Building::getHpInfoComponent() const {
	std::string textureName;
	StringLcl secondLine;
	if (this->burningMovesLeft == 0) {
		textureName =  "shield_icon";
		secondLine = StringLcl("{building_speed}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->getRegenerationSpeed()) + "{p_per_move}. {everything_is_alright}";
	}
	else {
		textureName = "fire";
		secondLine = StringLcl("{building_on_fire}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + std::to_string(this->burningMovesLeft);
	}

	HorizontalSelectionWindowComponent component = {
		textureName,
		StringLcl("{hp}") + StringLcl::COLOR(COLOR_THEME::STATE_COLOR(this->getHP(), this->getMaxHP())) + std::to_string(this->getHP()) + " / " + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_BEST) + std::to_string(this->getMaxHP()) + " " + this->getDefence().getReadable() + StringLcl("\n") +
        secondLine,
		false,
		Events()
	};
	return component;
}
HorizontalSelectionWindowComponent Building::getDestroyComponent() {
	Events resetHighlightEvent;
	resetHighlightEvent.add(std::make_shared<ResetHighlightEvent>());

    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events destroyEvent;
    destroyEvent.add(std::make_shared<PlaySoundEvent>("destroy"));
	destroyEvent.add(std::make_shared<DestroyEvent>(this->getThis<Building>()));
	std::shared_ptr<WindowTwoButtons> verify = std::make_shared<WindowTwoButtons>(StringLcl("{verify_destroy}"), StringLcl("{yes}"), StringLcl("{no}"), resetHighlightEvent + destroyEvent, resetHighlightEvent + clickSoundEvent);
	Events createVerify = clickSoundEvent;
	createVerify.add(std::make_shared<CreateEEvent>(verify));
	HorizontalSelectionWindowComponent component = {
		  "destroy_icon",
		StringLcl("{destroy_this_building}"),
		true,
		createVerify
	};
	return component;
}
HorizontalSelectionWindowComponent Building::getBuildingOfEnemyComponent() {
    return {
          "lord_icon",
        StringLcl("{building_of_enemy}"),
        false,
        Events()
    };
}
void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Unit::draw(target, states);
	if (this->exist()) {
		this->drawShortInfos(target, states);
	}
}
void Building::drawShortInfos(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!this->works()) {
        if (this->burningMovesLeft == 0) {
            uint32_t delta = this->getMaxHP() - (this->getHP() - 1 * (this->getHP() == 1));
            uint32_t movesToRegenerate = delta / this->getRegenerationSpeed() + (delta % this->getRegenerationSpeed() != 0);
            BuildingShortInfo bsi(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY(), "hammer_icon", std::to_string(movesToRegenerate));
            target.draw(bsi, states);
        }
        return;
    }

	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		boost::optional<BuildingShortInfo> bsi = this->specs.at(i)->getShortInfo(this->getThis<Building>());
		if (bsi.has_value()) {
			target.draw(bsi.value(), states);
		}
	}
}
Events Building::hit(Damage damage) {
    uint32_t d = damage.getHpLoss(this->getDefence());

	uint32_t hpPointsAfterOperation;
	if (d >= this->getHP()) {
		hpPointsAfterOperation = 0;
	}
	else {
		hpPointsAfterOperation = this->getHP() - d;
	}

	Events response;

    response.add(std::make_shared<PlaySoundEvent>("building_hit"));
	if (hpPointsAfterOperation == 0) {
		response.add(std::make_shared<PlaySoundEvent>("destroy"));
        response.add(std::make_shared<DestroyEvent>(this->getThis<Building>()));
	}
	else {
        if (this->burningMovesLeft == 0) {
            response.add(std::make_shared<PlaySoundEvent>("fire"));
        }
        response.add(std::make_shared<SetFireEvent>(this->getThis<Building>()));
	}

	std::shared_ptr<HPFlyingE> hpFlyingE = std::make_shared<HPFlyingE>(d, false, this->getX(), this->getY(), this->getSX(), this->getSY());
	response.add(std::make_shared<CreateEEvent>(hpFlyingE));

	response.add(std::make_shared<SubHpEvent>(this->getThis<Building>(), d));

	return response;
}
Events Building::newMove(MapState* state, uint32_t playerId) {
	if (!this->belongTo(playerId) or !this->exist()) {
		return Events();
	}

	Events event = this->processRegeneration();

	for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
		event = event + this->specs.at(i)->getActiveNewMoveEvent(this->getThis<Building>(), state);
	}

	return event;
}
Events Building::processRegeneration() {
	Events events;
	if (this->burningMovesLeft == 0) {
		if (this->getHP() < this->getMaxHP()) {
            uint32_t toHeal = this->getRegenerationSpeed() - 1 * (this->getHP() == 1);
			events.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));
			std::shared_ptr<HPFlyingE> element = std::make_shared<HPFlyingE>(std::min(toHeal, this->getMaxHP() - this->getHP()), true, this->getX(), this->getY(), this->getSX(), this->getSY());
			events.add(std::make_shared<PlaySoundEvent>("regeneration"));
			events.add(std::make_shared<CreateEEvent>(element));
			events.add(std::make_shared<AddHpEvent>(this->getThis<Building>(), toHeal));
			if (this->getHP() + toHeal >= this->getMaxHP()) {
				std::shared_ptr<ImageFlyingE> element2 = std::make_shared<ImageFlyingE>("hammer_icon", this->getX(), this->getY(), this->getSX(), this->getSY());
				events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
				events.add(std::make_shared<CreateEEvent>(element2));
			}
		}
	}
	else {
		events.add(std::make_shared<DecreaseBurningMovesLeftEvent>(this->getThis<Building>()));
	}
	return events;
}
Events Building::getResponse(MapState *state, uint32_t playerId, uint32_t button) {
	if (!this->exist()) {
		return Events();
	}

    Events response;

	std::vector<HorizontalSelectionWindowComponent> components;
    components.push_back(this->getExitComponent());
	if (button == sf::Mouse::Button::Left) {
		if (!this->belongTo(playerId)) {
			components.push_back(this->getBuildingOfEnemyComponent());
		}
		components.push_back(this->getDescriptionComponent());
		components.push_back(this->getHpInfoComponent());
		if (this->belongTo(playerId)) {
			components.push_back(this->getDestroyComponent());
			for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
				std::vector<BuildingHorizontalSelectionWindowComponent> specComponents = this->specs.at(i)->getComponents(this->getThis<Building>(), state);
				for (uint32_t j = 0; j < specComponents.size(); j = j + 1) {
					components.push_back(specComponents[j].component);
				}
			}
			response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
		}
		else {
			for (uint32_t i = 0; i < this->specs.size(); i = i + 1) {
				std::vector<BuildingHorizontalSelectionWindowComponent> specComponents = this->specs.at(i)->getComponents(this->getThis<Building>(), state);
				for (uint32_t j = 0; j < specComponents.size(); j = j + 1) {
					if (specComponents[j].showToEnemies) {
						components.push_back(specComponents[j].component);
					}
				}
			}
			response.add(std::make_shared<PlaySoundEvent>("click"));
		}
	}
	else {
		response.add(std::make_shared<PlaySoundEvent>("click"));
	}

    response = response + this->getHighlightEvent(state, IAreaControllerSpec::HIGHLIGHT_TYPE::UNIVERSAL);

	std::shared_ptr<HorizontalSelectionWindow> w = std::make_shared<HorizontalSelectionWindow>(components);
	response.add(std::make_shared<CreateEEvent>(w));

	return response;
}
sf::Color Building::getTextureColor() const {
	if (this->getHP() == this->getMaxHP()) {
		return this->Unit::getTextureColor();
	}
    if (this->burningMovesLeft > 0) {
        return {75, 75, 75};
    }
	return {200, 200, 200};
}
std::shared_ptr<PlayerPointer> Building::getPlayerPointer() const {
    return std::make_shared<BuildingStatePointer>(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY(), this->getHP(), this->getMaxHP());
}
std::shared_ptr<ILightSource> Building::getLightSource() const {
	if (this->works()) {
		return this->Unit::getLightSource();
	}
	return std::make_shared<EmptyLightSource>();
}


BOOST_CLASS_EXPORT_IMPLEMENT(Building)