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


#include "UpgradeableB.hpp"


UpgradeableB::UpgradeableB() {
	this->currentLevel = 1;
	this->upgradeMovesLeft = 0;
}
UpgradeableB::UpgradeableB(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t maxHp, std::shared_ptr<Player> playerPtr) : 
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
Events UpgradeableB::startUpgrade() {
	Events events;
	events.add(std::make_shared<SubResourcesEvent>(this->getUpgradeCost()));
	this->upgradeMovesLeft = this->getUpgradeTime();
	events = events + this->handleUpgradeStart();
	return events;
}
void UpgradeableB::decreaseUpgradeMovesLeft() {
	this->upgradeMovesLeft = this->upgradeMovesLeft - 1;
}
void UpgradeableB::increaseLevel() {
	this->currentLevel = this->currentLevel + 1;
}
bool UpgradeableB::works() const {
	return this->Building::works() and !this->upgrading();
}
uint32_t UpgradeableB::getCurrentLevel() const {
	return this->currentLevel;
}
bool UpgradeableB::upgrading() const {
	return (this->upgradeMovesLeft > 0);
}
Events UpgradeableB::handleCurrentUpgrade() {
	if (this->upgradeMovesLeft == 0) {
		return Events();
	}
	Events events;
	std::shared_ptr<FlyingE> element = std::make_shared<FlyingE>("upgrade_icon", "regeneration", this->getX(), this->getY(), this->getSX(), this->getSY());
	events.add(std::make_shared<CreateEEvent>(element));
	Events event;
	event.add(std::make_shared<DecreaseUpgradeMovesLeftEvent>(this));
	element->addOnStartGEvent(event);
	if (this->upgradeMovesLeft == 1) {
		events = events + this->handleUpgradeEnd();
	}
	return events;
}
Events UpgradeableB::handleBusyWithUpgrading() const {
	Events events;
	std::shared_ptr<WindowButton> window = std::make_shared<WindowButton>(this->getSoundName(), "click",
		this->getUpperCaseReadableName() + *Texts::get()->get("upgrade_in_progress") + std::to_wstring(this->upgradeMovesLeft), *Texts::get()->get("OK"));
	events.add(std::make_shared<CreateEEvent>(window));
	return events;
}
Events UpgradeableB::handleUpgradeStart() const {
	Events events;
	std::shared_ptr<WindowButton> window = std::make_shared<WindowButton>(this->getSoundName(), "click",
		this->getUpperCaseReadableName() + *Texts::get()->get("upgrade_started") + std::to_wstring(this->upgradeMovesLeft), *Texts::get()->get("OK"));
	events.add(std::make_shared<CreateEEvent>(window));
	return events;
}
Events UpgradeableB::handleUpgradeEnd() {
	Events events;
	Events event;
	event.add(std::make_shared<IncreaseLevelEvent>(this));
	std::shared_ptr<FlyingE> element = std::make_shared<FlyingE>("upgrade_icon", this->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
	element->addOnStartGEvent(event);
	events.add(std::make_shared<CreateEEvent>(element));
	return events;
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
	text.setPosition(32 * this->getX() + 32 * this->getSX() - text.getLocalBounds().width, 32 * this->getY());
	target.draw(text, states);
}
void UpgradeableB::drawUpgrading(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setFillColor(COLOR_THEME::UI_COLOR);
	rect.setSize(sf::Vector2f(32 * this->getSX(), 32 * this->getSY() / 2));
	rect.setPosition(32 * this->getX(), 32 * this->getY() + 32 * this->getSY() / 2);
	target.draw(rect, states);

	sf::Sprite sprite;
	sprite.setTexture(*Textures::get()->get("upgrade_icon"));
	sprite.setScale(rect.getSize().y / sprite.getTexture()->getSize().x, rect.getSize().y / sprite.getTexture()->getSize().y);
	sprite.setPosition(rect.getPosition());
	target.draw(sprite, states);

	sf::Text text;
	text.setFont(*Fonts::get()->get("1"));
	text.setString(std::to_string(this->upgradeMovesLeft));
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(sprite.getPosition() + sf::Vector2f(rect.getSize().x / 2, 0));
	target.draw(text, states);
}