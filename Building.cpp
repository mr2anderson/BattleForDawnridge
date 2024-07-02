/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "Building.hpp"


Building::Building(uint32_t x, uint32_t y, uint32_t maxHp, bool full, const Player* playerPtr) : Unit(x, y, full * maxHp + (!full) * 1, maxHp, playerPtr) {
	this->currentLevel = 1;
	this->upgradeMovesLeft = 0;
}
void Building::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Unit::draw(target, states);
	if (this->exist()) {
		this->drawCurrentLevel(target, states);
		if (this->upgrading()) {
			this->drawUpgrading(target, states);
		}
	}
}
GameObjectResponse Building::processRegeneration() {
	this->addHp(this->getRegenerationSpeed());
	return GameObjectResponse();
}
GameObjectResponse Building::decreaseUpgradeMovesLeft() {
	GameObjectResponse response;
	if (this->upgradeMovesLeft == 0) {
		return response;
	}

	this->upgradeMovesLeft = this->upgradeMovesLeft - 1;
	if (this->upgradeMovesLeft == 0) {
		this->currentLevel = this->currentLevel + 1;
		MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", this->getUpgradeFinishDescription());
		response.popUpWindows.push(window);
	}

	return response;
}
uint32_t Building::getCurrentLevel() const {
	return this->currentLevel;
}
bool Building::upgrading() const {
	return (this->upgradeMovesLeft > 0);
}
GameObjectResponse Building::handleUpgradeStart() const {
	GameObjectResponse response;
	MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", this->getUpgradeStartDescription());
	response.popUpWindows.push(window);
	return response;
}
GameObjectResponse Building::handleUpgrading() const {
	GameObjectResponse response;
	MessageWindow *window = new MessageWindow(this->getNewWindowSoundName(), "click", this->getBusyWithUpgradingDescription());
	response.popUpWindows.push(window);
	return response;
}
void Building::upgrade(uint32_t t) {
	this->upgradeMovesLeft = t;
}
void Building::drawCurrentLevel(sf::RenderTarget& target, sf::RenderStates states) const {
	std::string s;
	while (s.size() < this->getCurrentLevel()) {
		s = s + "#";
	}

	sf::Text text;
	text.setFont(*FontStorage::get()->get("1"));
	text.setString(s);
	text.setCharacterSize(12);
	text.setFillColor(sf::Color(75, 0, 130));
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(64 * this->getX(), 64 * this->getY());
	target.draw(text, states);
}
void Building::drawUpgrading(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setFillColor(UI_COLOR);
	rect.setSize(sf::Vector2f(64, 64 / 2));
	rect.setPosition(64 * this->getX(), 64 * this->getY() + 64 / 2);
	target.draw(rect, states);

	sf::Sprite sprite;
	sprite.setScale(0.5, 0.5);
	sprite.setTexture(*TextureStorage::get()->get("upgrade_icon"));
	sprite.setPosition(64 * this->getX(), 64 * this->getY() + 64 / 2);
	target.draw(sprite, states);

	sf::Text text;
	text.setFont(*FontStorage::get()->get("1"));
	text.setString(std::to_string(this->upgradeMovesLeft));
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(64 * this->getX() + 64 / 2, 64 * this->getY() + 64 / 2);
	target.draw(text, states);
}