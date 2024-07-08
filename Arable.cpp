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


#include "Arable.hpp"


Arable::Arable() = default;
Arable::Arable(uint32_t x, uint32_t y, std::shared_ptr<Player> playerPtr) :
	UpgradeableB(x, y, 3, 3, 2000, playerPtr),
	Building(x, y, 3, 3, 2000, playerPtr) {

}
Building* Arable::cloneBuilding() const {
	return new Arable(*this);
}
Events Arable::newMove(std::shared_ptr<Player> player) {
	if (this->exist() and this->belongTo(player)) {
		this->collected = false;
		return this->handleCurrentUpgrade();
	}
	return Events();
}
Resources Arable::getCost() const {
	return Resources({ Resource("wood", 10000) });
}
std::string Arable::getTextureName() const {
	return "arable";
}
std::string Arable::getSoundName() const {
	return "food";
}
std::wstring Arable::getDescription() const {
	return *Texts::get()->get("arable_description");
}
bool Arable::alreadyCollected() const {
	return this->collected;
}
uint32_t Arable::collect() const {
	return GET_COLLECTION_SPEED(this->getCurrentLevel() - 1);
}
uint32_t Arable::GET_COLLECTION_SPEED(uint32_t i) {
	uint32_t collectionSpeed[UpgradeableB::TOTAL_LEVELS] = {
		1000,
		3000,
		9000
	};
	return collectionSpeed[i];
}
GameActionWindowComponent Arable::getUpgradeComponent() {
	Events gameEventUpgrade;
	gameEventUpgrade.add(std::make_shared<TryToUpgradeEvent>(this));
	GameActionWindowComponent component = {
		"upgrade_icon",
		*Texts::get()->get("upgrade_for") + this->getUpgradeCost().getReadableInfo() + L'\n' +
		*Texts::get()->get("upgrade_will_increase_collection_speed_from") + std::to_wstring(GET_COLLECTION_SPEED(this->getCurrentLevel() - 1)) + *Texts::get()->get("to") + std::to_wstring(GET_COLLECTION_SPEED(this->getCurrentLevel())) + L".",
		true,
		false,
		gameEventUpgrade
	};
	return component;
}
uint32_t Arable::getRegenerationSpeed() const {
	return 1000;
}
std::wstring Arable::getUpperCaseReadableName() const {
	return *Texts::get()->get("arable_upper_case_readable_name");
}
Resources Arable::getUpgradeCost() const {
	Resources upgradeCosts[UpgradeableB::TOTAL_LEVELS - 1] = {
		 Resources({ Resource("wood", 30000) }),
		 Resources({ Resource("wood", 80000) })
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Arable::getUpgradeTime() const {
	uint32_t upgradeTime[UpgradeableB::TOTAL_LEVELS - 1] = {
		2,
		3
	};
	return upgradeTime[this->getCurrentLevel() - 1];
}
Events Arable::getSelectionW() {
	std::vector<GameActionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	if (this->upgrading()) {
		components.push_back(this->getBusyWithUpgradingComponent());
	}
	if (this->works()) {
		components.push_back(this->getUpgradeComponent());
	}

	std::shared_ptr<GameActionWindow> window = std::make_shared<GameActionWindow>(this->getSoundName(), "click", components);
	Events windowEvent;
	windowEvent.add(std::make_shared<CreateEEvent>(window));
	return windowEvent;
}
Events Arable::getGameObjectResponse(std::shared_ptr<Player> player) {
	if (this->exist()) {
		if (this->belongTo(player)) {
			return this->getSelectionW();
		}
		return this->getUnitOfEnemyResponse();
	}
	return Events();
}