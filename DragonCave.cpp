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


#include "DragonCave.hpp"
#include "BuildingShortInfo.hpp"
#include "RedDragon.hpp"
#include "Texts.hpp"
#include "TryToRaiseDragonEvent.hpp"
#include "WindowButton.hpp"
#include "CreateEEvent.hpp"
#include "SelectEvent.hpp"
#include "ImageFlyingE.hpp"
#include "WindowTwoButtons.hpp"
#include "DecreaseDragonRecoverMovesLeftEvent.hpp"
#include "FirstTimeTipsTable.hpp"


DragonCave::DragonCave() = default;
DragonCave::DragonCave(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Building(x, y, playerId, units) {
	this->dragon = nullptr;
}
DragonCave::~DragonCave() {
	if (this->dragon != nullptr) {
		delete this->dragon;
	}
}
void DragonCave::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Building::draw(target, states);
	if (this->exist()) {
		this->drawCurrentDragonBuildingShortInfo(target, states);
	}
}
Events DragonCave::setDragon(Dragon *newDragon) {
	this->dragon = newDragon;

	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("dragon_set"), *Texts::get()->get("OK"), clickEvent);
	Events response;
	response.add(std::make_shared<PlaySoundEvent>(this->dragon->getInfoSoundName()));
	response.add(std::make_shared<CreateEEvent>(w));
	return response;
}
Events DragonCave::processDecreaseDragonRecoverMovesLeft() {
	Events events;

	if (this->dragon != nullptr and this->dragon->getRecoverMovesLeft() > 0) {
		std::shared_ptr<ImageFlyingE> flyingE = std::make_shared<ImageFlyingE>(this->dragon->getTextureName(), this->getX(), this->getY(), this->getSX(), this->getSY());
		events.add(std::make_shared<PlaySoundEvent>(this->dragon->getInfoSoundName()));
		events.add(std::make_shared<CreateEEvent>(flyingE));
		events.add(std::make_shared<DecreaseDragonRecoverMovesLeftEvent>(this->dragon));
	}

	return events;
}
std::vector<HorizontalSelectionWindowComponent> DragonCave::getDragonCaveComponents() {
	std::vector<HorizontalSelectionWindowComponent> components;

	if (this->dragon == nullptr) {
		components.push_back(this->getRaiseComponent(std::make_shared<RedDragon>(this->getUnits(), this->getPlayerId())));
	}
	else {
		if (this->dragon->getRecoverMovesLeft() == 0) {
			components.push_back(this->getAttackComponent());
		}
		else {
			components.push_back(this->getRestComponent());
		}
	}

	return components;
}
HorizontalSelectionWindowComponent DragonCave::getRaiseComponent(std::shared_ptr<Dragon> newDragon) {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events raiseEvent;
	raiseEvent.add(std::make_shared<TryToRaiseDragonEvent>(this, newDragon));

	std::shared_ptr<WindowTwoButtons> w = std::make_shared<WindowTwoButtons>(*Texts::get()->get("raise_dragon_verify"), *Texts::get()->get("yes"), *Texts::get()->get("no"), raiseEvent, clickEvent);
	Events raiseEventVerify = clickEvent;
	raiseEventVerify.add(std::make_shared<CreateEEvent>(w));

	HorizontalSelectionWindowComponent component;
	component = {
		newDragon->getTextureName(),

		newDragon->getRaiseText() + L"\n" +
		newDragon->getDescriptionText() + L"\n" +
		*Texts::get()->get("cost") + newDragon->getCost().getReadableInfo(),

		true,
		raiseEventVerify
	};
	return component;
}
HorizontalSelectionWindowComponent DragonCave::getAttackComponent() {
	Events attackEvent;
	attackEvent.add(std::make_shared<SelectEvent>(this->dragon));

	Events componentEvent;

	if (FirstTimeTipsTable::get()->wasDisplayed("dragon_guide")) {
		componentEvent = attackEvent;
	}
	else {
		FirstTimeTipsTable::get()->markAsDisplayed("dragon_guide");
		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("dragon_guide"), *Texts::get()->get("OK"), attackEvent);
		componentEvent.add(std::make_shared<PlaySoundEvent>("click"));
		componentEvent.add(std::make_shared<CreateEEvent>(w));
	}

	HorizontalSelectionWindowComponent component;
	component = {
		this->dragon->getTextureName(),

		this->dragon->getAttackText() + L"\n" +
		this->dragon->getDescriptionText(),

		true,
		componentEvent
	};
	return component;
}
HorizontalSelectionWindowComponent DragonCave::getRestComponent() {
	HorizontalSelectionWindowComponent component;
	component = {
		this->dragon->getTextureName(),

		this->dragon->getRestText() + std::to_wstring(this->dragon->getRecoverMovesLeft()) + L"\n" +
		this->dragon->getDescriptionText(),

		false,
		Events()
	};
	return component;
}
void DragonCave::drawCurrentDragonBuildingShortInfo(sf::RenderTarget& target, sf::RenderStates states) const {
	if (this->dragon == nullptr) {
		return;
	}

	std::string text;
	if (this->dragon->getRecoverMovesLeft() == 0) {
		text = "!";
	}
	else {
		text = std::to_string(this->dragon->getRecoverMovesLeft());
	}

	BuildingShortInfo bsi(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY(), this->dragon->getTextureName(), text);
	target.draw(bsi, states);
}