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


#include "WarriorProducerSpec.hpp"
#include "WindowButton.hpp"
#include "Warrior.hpp"
#include "Locales.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "ImageFlyingE.hpp"
#include "FocusOnEvent.hpp"
#include "DecreaseCurrentProducingMovesLeftEvent.hpp"
#include "WarriorProducingFinishedEvent.hpp"
#include "CouldntFindNewWarriorPosition.hpp"
#include "ResetHighlightEvent.hpp"
#include "Building.hpp"
#include "SubResourcesEvent.hpp"
#include "StartWarriorProducingEvent.hpp"
#include "HighlightColors.hpp"


WarriorProducerSpec::WarriorProducerSpec() {
	this->producing = false;
}
Events WarriorProducerSpec::startProducing(std::shared_ptr<Warrior> w) {
	this->currentProducing = w;
	this->currentProducingMovesLeft = w->getTimeToProduce();
	this->producing = true;

	Events clickSoundEvent;
	clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	std::shared_ptr<WindowButton> producingStartedWindow = std::make_shared<WindowButton>(StringLcl("{producing_started}" + std::to_string(this->currentProducingMovesLeft)), StringLcl("{OK}"), clickSoundEvent);
	Events response;
	response.add(std::make_shared<PlaySoundEvent>(this->currentProducing->getSoundName()));
	response.add(std::make_shared<CreateEEvent>(producingStartedWindow));
	return response;
}
void WarriorProducerSpec::decreaseCurrentProducingMovesLeft() {
	this->currentProducingMovesLeft = this->currentProducingMovesLeft - 1;
}
void WarriorProducerSpec::stopProducing() {
	this->producing = false;
}
Events WarriorProducerSpec::getActiveNewMoveEvent(const Building *b, MapState* state) {
	if (!b->works() or !this->producing) {
		return Events();
	}

	Events response;
	if (this->currentProducingMovesLeft != 0) {
		std::shared_ptr<ImageFlyingE> flyingE = std::make_shared<ImageFlyingE>(this->getProducingIconName(), b->getX(), b->getY(), b->getSX(), b->getSY());

		response.add(std::make_shared<FocusOnEvent>(b->getX(), b->getY(), b->getSX(), b->getSY()));
		response.add(std::make_shared<PlaySoundEvent>(this->currentProducing->getSoundName()));
		response.add(std::make_shared<CreateEEvent>(flyingE));
		response.add(std::make_shared<DecreaseCurrentProducingMovesLeftEvent>(this));
	}
	if (this->currentProducingMovesLeft > 1) {
		return response;
	}

    uint32_t freeSpace = 0;
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
        Building *building = state->getCollectionsPtr()->getBuilding(i);
        if (building->exist() and building->getPlayerId() == b->getPlayerId()) {
            freeSpace = freeSpace + building->getPopulationLimit();
        }
    }
    for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
        Warrior *warrior = state->getCollectionsPtr()->getWarrior(i);
        if (warrior->exist() and warrior->getPlayerId() == b->getPlayerId()) {
            if (freeSpace >= warrior->getPopulation()) {
                freeSpace = freeSpace - warrior->getPopulation();
            }
            else {
                freeSpace = 0;
                break;
            }
        }
    }

    if (this->currentProducing->getPopulation() > freeSpace) {
        return response;
    }

	try {
		uint32_t newX, newY;
		std::tie(newX, newY) = this->getNewWarriorPosition(b->getX(), b->getY(), b->getSX(), b->getSY(), b->getPlayerId(), state);
		this->currentProducing->setX(newX);
		this->currentProducing->setY(newY);

		std::shared_ptr<ImageFlyingE> flyingE = std::make_shared<ImageFlyingE>(this->currentProducing->getTextureName(), b->getX(), b->getY(), b->getSX(), b->getSY());

		response.add(std::make_shared<FocusOnEvent>(b->getX(), b->getY(), b->getSX(), b->getSY()));
		response.add(std::make_shared<PlaySoundEvent>(this->currentProducing->getSoundName()));
		response.add(std::make_shared<CreateEEvent>(flyingE));
		response.add(std::make_shared<WarriorProducingFinishedEvent>(this, this->currentProducing));

		return response;
	}
	catch (CouldntFindNewWarriorPosition&) {
		return response;
	}
}
std::vector<BuildingHorizontalSelectionWindowComponent> WarriorProducerSpec::getComponents(const Building *b, MapState* state) {
	std::vector<BuildingHorizontalSelectionWindowComponent> components;

	if (b->works()) {
		if (this->producing) {
			StringLcl text;
			if (this->currentProducingMovesLeft > 0) {
				text = StringLcl("{producing_in_progress}" + std::to_string(this->currentProducingMovesLeft));
			}
			else {
				text = StringLcl("{couldnt_place_warrior}");
			}
			components.emplace_back(
				HorizontalSelectionWindowComponent(currentProducing->getTextureName(),
				text,
				false,
				Events()),
                true
			);
		}
		else {
			std::vector<std::shared_ptr<Warrior>> toProduce = this->getWarriorsToProduce(b->getPlayerId());

			uint32_t population = 0;
			for (uint32_t i = 0; i < state->getCollectionsPtr()->totalWarriors(); i = i + 1) {
				Warrior* w = state->getCollectionsPtr()->getWarrior(i);
				if (w->exist() and w->getPlayerId() == b->getPlayerId()) {
					population = population + w->getPopulation();
				}
			}

			uint32_t populationLimit = 0;
			for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
				Building* populationLimitIncreaser = state->getCollectionsPtr()->getBuilding(i);
				if (populationLimitIncreaser->exist() and populationLimitIncreaser->getPlayerId() == b->getPlayerId()) {
					populationLimit = populationLimit + populationLimitIncreaser->getPopulationLimit();
				}
			}

			for (uint32_t i = 0; i < toProduce.size(); i = i + 1) {
				std::shared_ptr<Warrior> w = toProduce.at(i);

				Events produceEvent;
				produceEvent.add(std::make_shared<ResetHighlightEvent>());
				
				if (!(state->getPlayersPtr()->getPlayerPtr(b->getPlayerId())->getResources() >= w->getCost())) {
					Events clickEvent;
					clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

					std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{no_resources_for_producing}"), StringLcl("{OK}"), clickEvent);
					produceEvent = produceEvent + clickEvent;
					produceEvent.add(std::make_shared<CreateEEvent>(w));
				}
				else if (population + w->getPopulation() > populationLimit) {
					Events clickEvent;
					clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

					std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(StringLcl("{population_limit}"), StringLcl("{OK}"), clickEvent);
					produceEvent = produceEvent + clickEvent;
					produceEvent.add(std::make_shared<CreateEEvent>(w));
				}
				else {
					produceEvent.add(std::make_shared<SubResourcesEvent>(w->getCost()));
					produceEvent.add(std::make_shared<StartWarriorProducingEvent>(b, this, w));
				}

				components.emplace_back(
					HorizontalSelectionWindowComponent(w->getTextureName(),
                    w->getDescription() + StringLcl("\n") +
                    StringLcl("{hp}") + StringLcl(std::to_string(w->getMaxHP())) + StringLcl(" (") + w->getDefence().getReadable() + StringLcl(")\n") +
                    w->getSpecialInfoString() + StringLcl("\n") +
                    StringLcl("{population}") + StringLcl(std::to_string(w->getPopulation())) + StringLcl(". ") + StringLcl("{cost}") + w->getCost().getReadableInfo() + StringLcl(". ") + StringLcl("{time_to_produce}") + StringLcl(std::to_string(w->getTimeToProduce())),
					true,
					produceEvent),
                    false
				);
			}
		}
	}
	else {
		components.emplace_back(
			HorizontalSelectionWindowComponent(  "hammer_icon",
			StringLcl("{does_not_train_if_hp_isnt_full}"),
			false,
			Events()),
            true
		);
	}

	return components;
}
boost::optional<BuildingShortInfo> WarriorProducerSpec::getShortInfo(const Building *b) const {
	if (!b->works()) {
		return boost::none;
	}

    std::string pictureName;
	std::string text;

    if (this->producing) {
        pictureName = this->currentProducing->getTextureName();
        if (this->currentProducingMovesLeft > 0) {
            text = std::to_string(this->currentProducingMovesLeft);
        }
        else {
            text = "!";
        }
    }
	else {
        pictureName = this->getWaitingIconName();
        text = "...";
    }

	return BuildingShortInfo(b->getXInPixels(), b->getYInPixels(), b->getSX(), b->getSY(), pictureName, text);
}
uint32_t WarriorProducerSpec::getRadius() const {
	return 1;
}
sf::Color WarriorProducerSpec::getHighlightColor(uint32_t playerId) const {
    return HighlightColors::get()->getWarriorProducingColor(playerId);
}
uint8_t WarriorProducerSpec::getHighlightType() const {
    return IAreaControllerSpec::HIGHLIGHT_TYPE::OTHER;
}
std::tuple<uint32_t, uint32_t> WarriorProducerSpec::getNewWarriorPosition(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, MapState* state) {
	HashTableMapPosition<uint32_t> available = this->getAvailable(x, y, sx, sy, playerId, state);
	for (const auto& a : available) {
		if (this->currentProducing->canStay(state, std::get<0>(a.first), std::get<1>(a.first))) {
			return a.first;
		}
	}
	throw CouldntFindNewWarriorPosition();
}


BOOST_CLASS_EXPORT_IMPLEMENT(WarriorProducerSpec)