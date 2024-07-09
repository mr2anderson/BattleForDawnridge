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


#include "Windmill.hpp"


Windmill::Windmill() = default;
Windmill::Windmill(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Arable>> arables) :
	HpSensitiveB(x, y, 3, 3, 20000, playerId),
	AreaB(x, y, 3, 3, 20000, playerId),
	Building(x, y, 3, 3, 20000, playerId) {
	this->arables = arables;
}
Building* Windmill::cloneBuilding() const {
	return new Windmill(*this);
}
Events Windmill::newMove(uint32_t playerId) {
	if (this->exist() and this->belongTo(playerId)) {
		Events response = this->regenerate();
		if (this->works()) {
			response = response + this->getCollectEvents();
		}
		return response;
	}
	return Events();
}
Resources Windmill::getCost() const {
	return Resources({Resource("wood", 10000)});
}
std::string Windmill::getTextureName() const {
	return "windmill";
}
std::string Windmill::getSoundName() const {
	return "food";
}
std::wstring Windmill::getDescription() const {
	return *Texts::get()->get("windmill_description");
}
uint32_t Windmill::getRadius() const {
	return 4;
}
uint32_t Windmill::getRegenerationSpeed() const {
	return 10000;
}
std::wstring Windmill::getUpperCaseReadableName() const {
	return *Texts::get()->get("windmill_upper_case_readable_name");
}
Events Windmill::getSelectionW() {
	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	if (this->repairing()) {
		components.push_back(this->getBusyWithRepairingComponent());
	}
	components.push_back(this->getBuildArableComponent());

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(this->getSoundName(), "click", components);
	Events createWindowEvent;
	createWindowEvent.add(std::make_shared<CreateEEvent>(window));
	return createWindowEvent;
}
Events Windmill::getGameObjectResponse(uint32_t playerId) {
	if (this->exist()) {
		if (this->belongTo(playerId)) {
			return this->getSelectionW() + this->getHighlightEvent();
		}
		return this->getUnitOfEnemyResponse();
	}
	return Events();
}
HorizontalSelectionWindowComponent Windmill::getBuildArableComponent() const {
	Events event = this->getHighlightEvent();
	event.add(std::make_shared<TryToBuildEvent>(std::make_shared<Arable>(0, 0, 0)));
	HorizontalSelectionWindowComponent component = {
		Arable().getTextureName(),
		Arable().getDescription() + L"\n" + 
		*Texts::get()->get("cost") + Arable().getCost().getReadableInfo(),
		true,
		true,
		event
	};
	return component;
}
Events Windmill::getCollectEvents() {
	Events events;
	for (uint32_t i = 0; i < this->arables->size(); i = i + 1) {
		Arable* arable = this->arables->at(i);
		if (!arable->works() or arable->getPlayerId() != this->getPlayerId() or arable->alreadyCollected() or !this->inRadius(arable)) {
			continue;
		}
		uint32_t n = arable->collect();
		std::shared_ptr<FlyingE> flyingE = std::make_shared<FlyingE>("food_icon", "food", arable->getX(), arable->getY(), arable->getSX(), arable->getSY());
		Events collectEvent;
		collectEvent.add(std::make_shared<AddResourceEvent>(Resource("food", n)));
		flyingE->addOnStartGEvent(collectEvent);
		events.add(std::make_shared<CreateEEvent>(flyingE));
	}
	return events;
}