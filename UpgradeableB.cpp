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


#include "UpgradeableB.hpp"


UpgradeableB::UpgradeableB() = default;
UpgradeableB::UpgradeableB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, const Player* playerPtr) : 
	Building(x, y, sx, sy, maxHp, playerPtr) {
	this->currentLevel = 1;
	this->upgradeMovesLeft = 0;
}
void UpgradeableB::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Building::draw(target, states);
	if (this->exist()) {
		this->drawCurrentLevel(target, states);
		if (this->upgrading()) {
			this->drawUpgrading(target, states);
		}
	}
}
GOR UpgradeableB::upgrade() {
	GOR response;
	GEvent gEvent;
	gEvent.subResources.push_back(this->getUpgradeCost());
	response.events.push_back(gEvent);
	this->upgradeMovesLeft = this->getUpgradeTime();
	response = response + this->handleUpgradeStart();
	return response;
}
uint32_t UpgradeableB::getCurrentLevel() const {
	return this->currentLevel;
}
bool UpgradeableB::upgrading() const {
	return (this->upgradeMovesLeft > 0);
}
GOR UpgradeableB::handleCurrentUpgrade() {
	if (this->upgradeMovesLeft == 0) {
		return GOR();
	}
	this->upgradeMovesLeft = this->upgradeMovesLeft - 1;
	if (this->upgradeMovesLeft == 0) {
		return this->handleUpgradeEnd();
	}
	return GOR();
}
GOR UpgradeableB::handleBusyWithUpgrading() const {
	GOR response;
	MessageW* window = new MessageW(this->getNewWindowSoundName(), "click",
		this->getUpperCaseReadableName() + L": ИДЕТ УЛУЧШЕНИЕ\n"
		"Подождите, пока оно не закончится\n"
		L"Число ходов до конца улучшения: " + std::to_wstring(this->upgradeMovesLeft));
	response.windows.push(window);
	return response;
}
GOR UpgradeableB::handleUpgradeStart() const {
	GOR response;
	MessageW* window = new MessageW(this->getNewWindowSoundName(), "click",
		this->getUpperCaseReadableName() + L": НАЧАТО УЛУЧШЕНИЕ\n"
		"Подождите, пока оно не закончится\n"
		L"Число ходов до конца улучшения: " + std::to_wstring(this->upgradeMovesLeft));
	response.windows.push(window);
	return response;
}
GOR UpgradeableB::handleUpgradeEnd() {
	this->currentLevel = this->currentLevel + 1;
	GOR response;
	MessageW* window = new MessageW(this->getNewWindowSoundName(), "click",
		this->getUpperCaseReadableName() + L": УЛУЧШЕНИЕ ЗАВЕРШЕНО\n"
		"Теперь Вы снова можете использовать это здание.");
	response.windows.push(window);
	return response;
}
void UpgradeableB::drawCurrentLevel(sf::RenderTarget& target, sf::RenderStates states) const {
	std::string s;
	while (s.size() < this->getCurrentLevel()) {
		s = s + "#";
	}

	sf::Text text;
	text.setFont(*Fonts::get()->get("1"));
	text.setString(s);
	text.setCharacterSize(12);
	text.setFillColor(sf::Color(75, 0, 130));
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(64 * this->getX(), 64 * this->getY());
	target.draw(text, states);
}
void UpgradeableB::drawUpgrading(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setFillColor(UI_COLOR);
	rect.setSize(sf::Vector2f(64, 64 / 2));
	rect.setPosition(64 * this->getX(), 64 * this->getY() + 64 * this->getSY() - 64 / 2);
	target.draw(rect, states);

	sf::Sprite sprite;
	sprite.setScale(0.5, 0.5);
	sprite.setTexture(*Textures::get()->get("upgrade_icon"));
	sprite.setPosition(64 * this->getX(), 64 * this->getY() + 64 * this->getSY() - 64 / 2);
	target.draw(sprite, states);

	sf::Text text;
	text.setFont(*Fonts::get()->get("1"));
	text.setString(std::to_string(this->upgradeMovesLeft));
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(64 * this->getX() + 64 / 2, 64 * this->getY() + 64 * this->getSY() - 64 / 2);
	target.draw(text, states);
}