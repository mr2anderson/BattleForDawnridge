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


#include "Market.hpp"


Market::Market() = default;
Market::Market(uint32_t x, uint32_t y, const Player *playerPtr) : 
	UpgradeableB(x, y, 3, 3, 30000, playerPtr),
	HpSensitiveB(x, y, 3, 3, 30000, playerPtr),
	Building(x, y, 3, 3, 30000, playerPtr) {

}
void Market::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->UpgradeableB::draw(target, states);
	if (this->exist() and this->currentTrade.movesLeft != 0) {
		this->drawCurrentTradeShortInfo(target, states);
	}
}
Event Market::doTrade(const Trade& trade) {
	this->currentTrade = trade;

	Event gEvent;
	gEvent.subResource.push_back(trade.sell);

	MessageW* window = new MessageW(this->getSoundName(), "click", 
		L"СДЕЛКА НАЧАТА\n"
		"Ваши ресурсы забрал караван, обмен будет выполнен через несколько ходов. Детали сделки:\n" + 
		trade.getReadableInfo());
	
	window->addOnStartGEvent(gEvent);
	Event responce;
	responce.createE.push_back(window);
	return responce;
}
void Market::decreaseCurrentTradeMovesLeft() {
	this->currentTrade.movesLeft = this->currentTrade.movesLeft - 1;
}
Event Market::newMove(const Player& currentPlayer) {
	if (!this->belongTo(&currentPlayer) or !this->exist()) {
		return Event();
	}
	Event response = this->handleCurrentUpgrade();
	if (this->upgrading()) {
		return response;
	}
	response = response + this->regenerate();
	if (this->repairing()) {
		return response;
	}
	return response + this->handleCurrentTrade();
}
bool Market::works() const {
	return this->HpSensitiveB::works() and this->UpgradeableB::works();
}
Resources Market::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 30000));
	return cost;
}
std::string Market::getTextureName() const {
	return "market";
}
std::string Market::getSoundName() const {
	return "gold";
}
std::wstring Market::getDescription() const {
	return L"Рынки позволяют обменивать ресурсы.";
}
uint32_t Market::getRegenerationSpeed() const {
	return 10000;
}
Resources Market::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 60000}}),
		Resources({{"wood", 120000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Market::getUpgradeTime() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		3,
		6
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}
std::wstring Market::getReadableName() const {
	return L"рынок";
}
bool Market::busy() const {
	return this->currentTrade.movesLeft != 0;
}
uint32_t Market::GET_TRADE_START_TIME(uint32_t level) {
	uint32_t levelTradeStartTime[TOTAL_LEVELS] = {
		6,
		4,
		1
	};
	return levelTradeStartTime[level];
}
uint32_t Market::getTradeStartTime() const {
	return GET_TRADE_START_TIME(this->getCurrentLevel() - 1);
}
void Market::drawCurrentTradeShortInfo(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setFillColor(UI_COLOR);
	rect.setSize(sf::Vector2f(32 * this->getSX(), 32 * this->getSY() / 2));
	rect.setPosition(32 * this->getX(), 32 * this->getY() + 32 * this->getSY() / 2);
	target.draw(rect, states);

	sf::Sprite sprite;
	sprite.setTexture(*Textures::get()->get(this->currentTrade.buy.type + "_icon"));
	sprite.setScale(rect.getSize().y / sprite.getTexture()->getSize().x, rect.getSize().y / sprite.getTexture()->getSize().y);
	sprite.setPosition(rect.getPosition());
	target.draw(sprite, states);

	sf::Text text;
	text.setFont(*Fonts::get()->get("1"));
	text.setString(std::to_string(this->currentTrade.movesLeft));
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(sprite.getPosition() + sf::Vector2f(rect.getSize().x / 2, 0));
	target.draw(text, states);
}
Event Market::handleCurrentTrade() {
	if (!this->busy()) {
		return Event();
	}
	Event responce;
	Event decreaseEvent;
	decreaseEvent.decreaseCurrentTradeMovesLeft.push_back(this);
	FlyingE* element = new FlyingE("trade_icon", this->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
	responce.createE.push_back(element);
	element->addOnStartGEvent(decreaseEvent);
	if (this->currentTrade.movesLeft == 1) {
		Event gEvent;
		gEvent.addResource.push_back(this->currentTrade.buy);
		element = new FlyingE(this->currentTrade.buy.type + "_icon", this->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
		element->addOnStartGEvent(gEvent);
		responce.createE.push_back(element);
	}
	return responce;
}
Event Market::getSelectionW() {
	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"Покинуть", true, true, Event());
	components.emplace_back("market", 
		this->getDescription() + L"\n" +
		this->getReadableHpInfo(), false, false, Event());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		Event gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		components.emplace_back("upgrade_icon", 
			L"Улучшить рынок за " + this->getUpgradeCost().getReadableInfo() + L"\n"
			"Улучшение уменьшит число ходов для одной сделки с " + std::to_wstring(this->getTradeStartTime()) + L" до " + std::to_wstring(GET_TRADE_START_TIME(this->getCurrentLevel())) + L".", true, false, gameEventUpgrade);
	}

	for (const auto& a : { std::make_tuple("gold", 100, "food", 50000),
		std::make_tuple("gold", 100, "wood", 50000),
		std::make_tuple("gold", 100, "stone", 50000),
		std::make_tuple("gold", 100, "iron", 50000),
		std::make_tuple("food", 50000, "gold", 100),
		std::make_tuple("wood", 50000, "gold", 100),
		std::make_tuple("stone", 50000, "gold", 100),
		std::make_tuple("iron", 50000, "gold", 100) }) {
		Event gameEventTrade;
		gameEventTrade.tryToTrade.emplace_back(this, Trade(Resource(std::get<0>(a), std::get<1>(a)), Resource(std::get<2>(a), std::get<3>(a)), this->getTradeStartTime()));
		this->addTrade(components, gameEventTrade);
	}

	SelectionW* window = new SelectionW(this->getSoundName(), "click", components);
	Event response;
	response.createE.push_back(window);
	return response;
}
void Market::addTrade(std::vector<SelectionWComponent>& components, const Event& gameEventTrade) {
	components.emplace_back(std::get<Trade>(gameEventTrade.tryToTrade.back()).buy.type + "_icon",
		L"Купить " + std::get<Trade>(gameEventTrade.tryToTrade.back()).buy.getReadableInfo() +
		L" за " + std::get<Trade>(gameEventTrade.tryToTrade.back()).sell.getReadableInfo(), true, false, gameEventTrade);
}
Event Market::handleBusyWithTrade() const {
	Event response;
	MessageW* window = new MessageW(this->getSoundName(), "click",
		L"РЫНОК ЗАНЯТ\n"
		"Детали сделки:\n" +
		this->currentTrade.getReadableInfo());
	response.createE.push_back(window);
	return response;
}
Event Market::getGameObjectResponse(const Player& player) {
	if (!this->exist()) {
		return Event();
	}
	if (this->belongTo(&player)) {
		if (this->repairing()) {
			return this->handleRepairing();
		}
		if (this->upgrading()) {
			return this->handleBusyWithUpgrading();
		}
		if (this->busy()) {
			return this->handleBusyWithTrade();
		}
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}