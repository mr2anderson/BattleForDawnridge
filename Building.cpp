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
#include "Texts.hpp"
#include "ImageFlyingE.hpp"
#include "HPFlyingE.hpp"
#include "PlayerPointerCircle.hpp"
#include "WindowButton.hpp"
#include "CreateEEvent.hpp"
#include "AddHpEvent.hpp"
#include "WindowTwoButtons.hpp"
#include "DestroyEvent.hpp"
#include "ImageFlyingE.hpp"
#include "DecreaseBurningMovesLeftEvent.hpp"
#include "SubHpEvent.hpp"
#include "SetFireEvent.hpp"
#include "FocusOnEvent.hpp"


Building::Building() = default;
Building::Building(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	Unit(x, y, 1, playerId, units) {
	this->burningMovesLeft = 0;
}
void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Unit::draw(target, states);
	if (this->exist() and this->burningMovesLeft > 0) {
		target.draw(this->fire, states);
	}
}
Events Building::hit(Damage d, const std::optional<std::string> &direction) {
	uint32_t dPoints = d.getHpLoss(this->getDefence());
	uint32_t hpPointsAfterOperation;
	if (dPoints >= this->getHP()) {
		hpPointsAfterOperation = 0;
	}
	else {
		hpPointsAfterOperation = this->getHP() - dPoints;
	}

	Events response;

	if (hpPointsAfterOperation == 0) {
		response.add(std::make_shared<PlaySoundEvent>("destroy"));
	}
	else {
		response.add(std::make_shared<PlaySoundEvent>("fire"));
	}

	if (hpPointsAfterOperation == 0) {
		response.add(std::make_shared<DestroyEvent>(this));
	}
	else {
		response.add(std::make_shared<SetFireEvent>(this));
	}
	
	std::shared_ptr<HPFlyingE> hpFlyingE = std::make_shared<HPFlyingE>(hpPointsAfterOperation, this->getMaxHP(), false, this->getX(), this->getY(), this->getSX(), this->getSY());
	response.add(std::make_shared<CreateEEvent>(hpFlyingE));
	
	response.add(std::make_shared<SubHpEvent>(this, dPoints));

	return response;
}
bool Building::works() const {
	return this->exist();
}
Events Building::destroy() {
    Events response;
    response.add(std::make_shared<PlaySoundEvent>("destroy"));
	this->subHp(this->getHP());
	return response;
}
void Building::setFire() {
	this->fire = Fire(this->getX(), this->getY(), this->getSX(), this->getSY());
	this->burningMovesLeft = 3;
}
void Building::decreaseBurningMovesLeft() {
	this->burningMovesLeft = this->burningMovesLeft - 1;
}
HorizontalSelectionWindowComponent Building::getHpInfoComponent() const {
	std::string textureName;
	std::wstring secondLine;
	if (this->burningMovesLeft == 0) {
		textureName = "shield_icon";
		secondLine = *Texts::get()->get("building_speed") + std::to_wstring(this->getRegenerationSpeed()) + *Texts::get()->get("p_per_move") + L". " + *Texts::get()->get("everything_is_alright");
	}
	else {
		textureName = "fire1";
		secondLine = *Texts::get()->get("building_on_fire") + std::to_wstring(this->burningMovesLeft);
	}

	HorizontalSelectionWindowComponent component = {
		textureName,
		*Texts::get()->get("hp") + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L" (" + this->getDefence().getReadable() + L")\n" +
		secondLine,
		false,
		Events()
	};
	return component;
}
HorizontalSelectionWindowComponent Building::getDestroyComponent() {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events destroyEvent = clickSoundEvent;
	destroyEvent.add(std::make_shared<DestroyEvent>(this));
	std::shared_ptr<WindowTwoButtons> verify = std::make_shared<WindowTwoButtons>(*Texts::get()->get("verify_destroy"), *Texts::get()->get("yes"), *Texts::get()->get("no"), destroyEvent, clickSoundEvent);
	Events createVerify = clickSoundEvent;
	createVerify.add(std::make_shared<CreateEEvent>(verify));
	HorizontalSelectionWindowComponent component = {
		"destroy_icon",
		*Texts::get()->get("destroy_this_building"),
		true,
		createVerify
	};
	return component;
}
Events Building::regenerate() {
	Events events;
	if (this->burningMovesLeft == 0) {
		if (this->getHP() < this->getMaxHP()) {
			events.add(std::make_shared<FocusOnEvent>(this->getX(), this->getY(), this->getSX(), this->getSY()));
			std::shared_ptr<HPFlyingE> element = std::make_shared<HPFlyingE>(std::min(this->getMaxHP(), this->getHP() + this->getRegenerationSpeed()), this->getMaxHP(), true, this->getX(), this->getY(), this->getSX(), this->getSY());
			events.add(std::make_shared<PlaySoundEvent>("regeneration"));
			events.add(std::make_shared<CreateEEvent>(element));
			events.add(std::make_shared<AddHpEvent>(this, this->getRegenerationSpeed()));
			if (this->getHP() + this->getRegenerationSpeed() >= this->getMaxHP()) {
				std::shared_ptr<ImageFlyingE> element2 = std::make_shared<ImageFlyingE>("hammer_icon", this->getX(), this->getY(), this->getSX(), this->getSY());
				events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
				events.add(std::make_shared<CreateEEvent>(element2));
			}
		}
	}
	else {
		events.add(std::make_shared<DecreaseBurningMovesLeftEvent>(this));
	}
	return events;
}
sf::Color Building::getTextureColor() const {
	if (this->burningMovesLeft == 0) {
		return this->Unit::getTextureColor();
	}
	return sf::Color(100, 100, 100);
}
std::shared_ptr<HPPointer> Building::getHPPointer() const {
	return std::make_shared<HPPointer>(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY());
}
bool Building::warriorCanStay(uint32_t warriorPlayerId) const {
	return true;
}
uint32_t Building::getWarriorMovementCost(uint32_t warriorPlayerId) const {
	return 1;
}
std::shared_ptr<PlayerPointer> Building::getPlayerPointer() const {
    return std::make_shared<PlayerPointerCircle>(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY());
}