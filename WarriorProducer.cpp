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


#include "WarriorProducer.hpp"
#include "CouldntFindNewWarriorPosition.hpp"
#include "DecreaseCurrentProducingMovesLeftEvent.hpp"
#include "TryToProduceEvent.hpp"
#include "WarriorProducingFinishedEvent.hpp"
#include "BuildingShortInfo.hpp"
#include "FlyingE.hpp"
#include "Texts.hpp"


WarriorProducer::WarriorProducer() = default;
WarriorProducer::WarriorProducer(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH) :
	HpSensitiveB(x, y, sx, sy, maxHp, playerId, units),
	AreaBLandscapeInsensible(x, y, sx, sy, maxHp, playerId, units),
	Building(x, y, sx, sy, maxHp, playerId, units) {
	this->go = go;
	this->mapW = mapW;
	this->mapH = mapH;
	this->producing = false;
}
void WarriorProducer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Building::draw(target, states);
	if (this->exist() and this->busy()) {
		this->drawCurrentProducingShortInfo(target, states);
	}
}
Events WarriorProducer::startProducing(std::shared_ptr<Warrior> w) {
	this->currentProducing = w;
	this->currentProducingMovesLeft = w->getTimeToProduce();
	this->producing = true;

	std::shared_ptr<WindowButton> producingStartedWindow = std::make_shared<WindowButton>(this->currentProducing->getSoundName(), "click", *Texts::get()->get("producing_started") + std::to_wstring(this->currentProducingMovesLeft), *Texts::get()->get("OK"));
	Events response;
	response.add(std::make_shared<CreateEEvent>(producingStartedWindow));
	return response;
} 
void WarriorProducer::decreaseCurrentProducingMovesLeft() {
	this->currentProducingMovesLeft = this->currentProducingMovesLeft - 1;
}
void WarriorProducer::stopProducing() {
	this->producing = false;
}
Events WarriorProducer::newMove(uint32_t playerId) {
	if (this->belongTo(playerId) and this->exist()) {
		Events response = this->regenerate();
		if (this->works()) {
			response = response + this->handleCurrentProducing();
		}
		return response;
	}
	return Events();
}
std::shared_ptr<GOCollection<GO>> WarriorProducer::getGO() {
	return this->go;
}
uint32_t WarriorProducer::getMapW() const {
	return this->mapW;
}
uint32_t WarriorProducer::getMapH() const {
	return this->mapH;
}
std::vector<HorizontalSelectionWindowComponent> WarriorProducer::getProduceComponents() {
	std::vector<std::shared_ptr<Warrior>> toProduce = this->getWarriorsToProduce();
	std::vector<HorizontalSelectionWindowComponent> components(toProduce.size());
	for (uint32_t i = 0; i < components.size(); i = i + 1) {
		std::shared_ptr<Warrior> w = toProduce.at(i);
		Events tryToProduceEvent;
		tryToProduceEvent.add(std::make_shared<TryToProduceEvent>(this, w));
		HorizontalSelectionWindowComponent component = {
			w->getTextureName(),
			w->getDescription() + L"\n" +
			*Texts::get()->get("cost") + w->getCost().getReadableInfo() + L". " + *Texts::get()->get("time_to_produce") + std::to_wstring(w->getTimeToProduce()),
			true,
			false,
			tryToProduceEvent + this->getHighlightEvent()
		};
		components.at(i) = component;
	}
	return components;
}
HorizontalSelectionWindowComponent WarriorProducer::getBusyWithProducingComponent() {
	std::wstring text;
	if (this->currentProducingMovesLeft > 0) {
		text = *Texts::get()->get("producing_in_progress") + std::to_wstring(this->currentProducingMovesLeft);
	}
	else {
		text = *Texts::get()->get("couldnt_find_new_warrior_position");
	}
	HorizontalSelectionWindowComponent component = {
		currentProducing->getTextureName(),
		text,
		false,
		false,
		Events()
	};
	return component;
}
bool WarriorProducer::busy() const {
	return this->producing;
}
void WarriorProducer::drawCurrentProducingShortInfo(sf::RenderTarget& target, sf::RenderStates states) const {
	std::string text;
	if (this->currentProducingMovesLeft > 0) {
		text = std::to_string(this->currentProducingMovesLeft);
	}
	else {
		text = "!";
	}
	BuildingShortInfo info(this->getX(), this->getY(), this->getSX(), this->getSY(), this->currentProducing->getTextureName(), text);
	target.draw(info, states);
}
Events WarriorProducer::handleCurrentProducing() {
	if (!this->busy()) {
		return Events();
	}
	Events response;
	if (this->currentProducingMovesLeft != 0) {
		std::shared_ptr<FlyingE> flyingE = std::make_shared<FlyingE>("producing_icon", this->currentProducing->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
		response.add(std::make_shared<CreateEEvent>(flyingE));
		response.add(std::make_shared<DecreaseCurrentProducingMovesLeftEvent>(this));
	}
	if (this->currentProducingMovesLeft != 1) {
		return response;
	}
	try {
		uint32_t x, y;
		std::tie(x, y) = this->getNewWarriorPosition();
		this->currentProducing->setX(x);
		this->currentProducing->setY(y);
		response.add(std::make_shared<WarriorProducingFinishedEvent>(this, this->currentProducing));
		return response;
	}
	catch (CouldntFindNewWarriorPosition& e) {
		return response;
	}
}
std::tuple<uint32_t, uint32_t> WarriorProducer::getNewWarriorPosition() {
	for (uint32_t x = this->getAreaXMin(); x <= this->getAreaXMax(); x = x + 1) {
		for (uint32_t y = this->getAreaYMin(); y <= this->getAreaYMax(); y = y + 1) {
			if (this->inRadius(x, y, this->currentProducing->getSX(), this->currentProducing->getSY()) and this->currentProducing->canStay(x, y)) {
				return std::make_tuple(x, y);
			}
		}
	}
	throw CouldntFindNewWarriorPosition();
}
Events WarriorProducer::getSelectionW() {
	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getDestroyComponent());
	if (this->repairing()) {
		components.push_back(this->getBusyWithRepairingComponent());
	}
	if (this->works()) {
		if (this->busy()) {
			components.push_back(this->getBusyWithProducingComponent());
		}
		else {
			std::vector<HorizontalSelectionWindowComponent> produceComponents = this->getProduceComponents();
			components.insert(components.end(), produceComponents.begin(), produceComponents.end());
		}
	}
	
	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(this->getSoundName(), "click", components);
	Events createWindowEvent;
	createWindowEvent.add(std::make_shared<CreateEEvent>(window));
	return createWindowEvent;
}
Events WarriorProducer::getGameObjectResponse(uint32_t playerId) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(playerId)) {
		return this->getSelectionW() + this->getHighlightEvent();
	}
	return Events();
}