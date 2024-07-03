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


#include "Market.hpp"


Market::Market() = default;
Market::Market(uint32_t x, uint32_t y, const Player *playerPtr) : 
	UpgradeableHpSensitiveB(x, y, 2, 2, 30000, playerPtr),
	Building(x, y, 2, 2, 30000, playerPtr) {

}
void Market::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Building::draw(target, states);
	if (this->exist() and this->currentTrade.movesLeft != 0) {
		this->drawCurrentTradeShortInfo(target, states);
	}
}
GOR Market::doTrade(const Trade& trade) {
	this->currentTrade = trade;

	GOR response;
	response.gEvent.subResource.push_back(trade.sell);

	MessageW* window = new MessageW(this->getNewWindowSoundName(), "click", 
		L"СДЕЛКА НАЧАТА\n"
		"Ваши ресурсы забрал караван, обмен будет выполнен через несколько ходов. Детали сделки:\n" + 
		trade.getReadableInfo());
	response.popUpWindows.push(window);

	return response;
}
GOR Market::newMove(const Player& currentPlayer) {
	if (!this->belongTo(&currentPlayer) or !this->exist()) {
		return GOR();
	}
	GOR response = this->handleCurrentUpgrade();
	if (this->upgrading()) {
		return response;
	}
	response = response + this->regenerate();
	if (!this->works()) {
		return response;
	}
	return response + this->handleCurrentTrade();
}
Resources Market::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 30000));
	return cost;
}
uint32_t Market::getRegenerationSpeed() const {
	return 10000;
}
std::string Market::getNewWindowSoundName() const {
	return "horse";
}
std::string Market::getTextureName() const {
	return "market";
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
	rect.setSize(sf::Vector2f(64, 64 / 2));
	rect.setPosition(64 * this->getX(), 64 * this->getY() + 64 * this->getSY() - 64 / 2);
	target.draw(rect, states);

	sf::Sprite sprite;
	sprite.setScale(0.5, 0.5);
	sprite.setTexture(*Textures::get()->get(this->currentTrade.buy.type));
	sprite.setPosition(64 * this->getX(), 64 * this->getY() + 64 * this->getSY() - 64 / 2);
	target.draw(sprite, states);

	sf::Text text;
	text.setFont(*Fonts::get()->get("1"));
	text.setString(std::to_string(this->currentTrade.movesLeft));
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(64 * this->getX(), 64 * this->getY() + 64 * this->getSY() - 64 / 2);
	target.draw(text, states);
}
GOR Market::handleCurrentTrade() {
	if (!this->busy()) {
		return GOR();
	}
	this->currentTrade.movesLeft = this->currentTrade.movesLeft - 1;
	if (this->currentTrade.movesLeft == 0) {
		GOR response;
		response.gEvent.addResource.push_back(this->currentTrade.buy);
		MessageW* window = new MessageW(this->getNewWindowSoundName(), "click", 
			L"СДЕЛКА ЗАВЕРШЕНА\n"
			"Ресурсы были получены. Детали сделки:\n"
			+ this->currentTrade.getReadableInfo());
		response.popUpWindows.push(window);
		return response;
	}
	return GOR();
}
GOR Market::getSelectionW() {
	std::vector<SelectionWComponent> components;
	components.emplace_back("exit_icon", L"Покинуть", true, GEvent());
	components.emplace_back("market", 
		L"Рынки позволяют обменивать ресурсы.\n" +
		this->getReadableHpInfo(), false, GEvent());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		GEvent gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		components.emplace_back("upgrade_icon", 
			L"Улучшить рынок за " + this->getUpgradeCost().getReadableInfo() + L"\n"
			"Улучшение уменьшит число ходов для одной сделки с " + std::to_wstring(this->getTradeStartTime()) + L" до " + std::to_wstring(GET_TRADE_START_TIME(this->getCurrentLevel())) + L".", true, gameEventUpgrade);
	}

	for (const auto& a : { std::make_tuple("gold", 100, "food", 50000),
		std::make_tuple("gold", 100, "wood", 50000),
		std::make_tuple("gold", 100, "stone", 50000),
		std::make_tuple("gold", 100, "iron", 50000),
		std::make_tuple("food", 50000, "gold", 100),
		std::make_tuple("wood", 50000, "gold", 100),
		std::make_tuple("stone", 50000, "gold", 100),
		std::make_tuple("iron", 50000, "gold", 100) }) {
		GEvent gameEventTrade;
		gameEventTrade.tryToTrade.emplace_back(this, Trade(Resource(std::get<0>(a), std::get<1>(a)), Resource(std::get<2>(a), std::get<3>(a)), this->getTradeStartTime()));
		this->addTrade(components, gameEventTrade);
	}

	SelectionW* window = new SelectionW(this->getNewWindowSoundName(), "click", components);
	GOR response;
	response.popUpWindows.push(window);
	return response;
}
void Market::addTrade(std::vector<SelectionWComponent>& components, const GEvent& gameEventTrade) {
	components.emplace_back(std::get<Trade>(gameEventTrade.tryToTrade.back()).buy.type + "_icon",
		L"Купить " + std::get<Trade>(gameEventTrade.tryToTrade.back()).buy.getReadableInfo() +
		L" за " + std::get<Trade>(gameEventTrade.tryToTrade.back()).sell.getReadableInfo(), true, gameEventTrade);
}
GOR Market::handleBusyWithTrade() const {
	GOR response;
	MessageW* window = new MessageW(this->getNewWindowSoundName(), "click",
		L"РЫНОК ЗАНЯТ\n"
		"Детали сделки:\n" +
		this->currentTrade.getReadableInfo());
	response.popUpWindows.push(window);
	return response;
}
GOR Market::getGameObjectResponse(const Player& player) {
	if (!this->exist()) {
		return GOR();
	}
	if (this->belongTo(&player)) {
		if (!this->works()) {
			return this->handleDoesNotWork();
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