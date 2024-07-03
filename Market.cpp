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
	UpgradeableHpSensitiveBuilding(x, y, 2, 2, 30000, false, playerPtr),
	Building(x, y, 2, 2, 30000, false, playerPtr){}
void Market::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Building::draw(target, states);
	if (this->exist() and this->currentTrade.movesLeft != 0) {
		this->drawCurrentTradeShortInfo(target, states);
	}
}
GameObjectResponse Market::doTrade(const Trade& trade) {
	this->currentTrade = trade;

	GameObjectResponse response;
	response.gameEvent = GameEvent();
	response.gameEvent.value().subResource.push_back(trade.sell);

	MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", L"СДЕЛКА НАЧАТА\nВаши ресурсы забрал караван, обмен будет выполнен через несколько ходов. Детали сделки:\n" + trade.getReadableInfo());
	response.popUpWindows.push(window);

	return response;
}
GameObjectResponse Market::newMove(const Player& currentPlayer) {
	GameObjectResponse response;
	if (!this->belongTo(&currentPlayer) or !this->exist()) {
		return response;
	}
	response = this->decreaseUpgradeMovesLeft();
	if (this->upgrading()) {
		return response;
	}
	response = response + this->processRegeneration();
	if (!this->works() or !this->busy()) {
		return response;
	}

	response = response + this->currentTradeNewMove();

	return response;
}
Resources Market::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 30000));
	return cost;
}
GameObjectResponse Market::currentTradeNewMove() {
	GameObjectResponse response;
	response.gameEvent = GameEvent();

	this->currentTrade.movesLeft = this->currentTrade.movesLeft - 1;
	if (this->currentTrade.movesLeft == 0) {
		if (!response.gameEvent.has_value()) {
			response.gameEvent = GameEvent();
		}
		response.gameEvent.value().addResource.push_back(this->currentTrade.buy);
		MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", L"СДЕЛКА ЗАВЕРШЕНА\nРесурсы были получены. Детали сделки:\n" + this->currentTrade.getReadableInfo());
		response.popUpWindows.push(window);
	}

	return response;
}
bool Market::busy() const {
	return this->currentTrade.movesLeft != 0;
}
GameObjectResponse Market::getSelectWindow() {
	std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
	data.emplace_back("exit_icon", L"Покинуть", true, GameEvent());
	data.emplace_back("market", L"Рынки позволяют обменивать ресурсы. Защита: " + std::to_wstring(this->getHP()) + L" / " + std::to_wstring(this->getMaxHP()) + L".", false, GameEvent());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		GameEvent gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, this->getUpgradeCost());
		data.emplace_back("upgrade_icon", L"Улучшить рынок за " + this->getUpgradeCost().getReadableInfo() + L". Улучшение ускорит ход сделок.", true, gameEventUpgrade);
	}

	for (const auto& a : { std::make_tuple("gold", 100, "food", 50000),
		std::make_tuple("gold", 100, "wood", 50000),
		std::make_tuple("gold", 100, "stone", 50000),
		std::make_tuple("gold", 100, "iron", 50000),
		std::make_tuple("food", 50000, "gold", 100),
		std::make_tuple("wood", 50000, "gold", 100),
		std::make_tuple("stone", 50000, "gold", 100),
		std::make_tuple("iron", 50000, "gold", 100) }) {
		GameEvent gameEventTrade;
		gameEventTrade.tryToTrade.emplace_back(this, Trade(Resource(std::get<0>(a), std::get<1>(a)), Resource(std::get<2>(a), std::get<3>(a)), this->getTradeStartTime()));
		this->addTrade(data, gameEventTrade);
	}

	SelectWindow* window = new SelectWindow(this->getNewWindowSoundName(), "click", data);
	GameObjectResponse response;
	response.popUpWindows.push(window);
	return response;
}
void Market::addTrade(std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>>& data, const GameEvent& gameEventTrade) {
	data.emplace_back(std::get<Trade>(gameEventTrade.tryToTrade.back()).sell.type + "_icon",
		L"Купить " + std::get<Trade>(gameEventTrade.tryToTrade.back()).buy.getReadableInfo() +
		L" за " + std::get<Trade>(gameEventTrade.tryToTrade.back()).sell.getReadableInfo(), true, gameEventTrade);
}
uint32_t Market::getTradeStartTime() const {
	uint32_t levelTradeStartTime[TOTAL_LEVELS] = {
		6,
		4,
		1
	};
	return levelTradeStartTime[this->getCurrentLevel() - 1];
}
void Market::drawCurrentTradeShortInfo(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setFillColor(UI_COLOR);
	rect.setSize(sf::Vector2f(64, 64 / 2));
	rect.setPosition(64 * this->getX(), 64 * this->getY() + 64 * this->getSY() - 64 / 2);
	target.draw(rect, states);

	sf::Sprite sprite;
	sprite.setScale(0.5, 0.5);
	sprite.setTexture(*TextureStorage::get()->get(this->currentTrade.buy.type));
	sprite.setPosition(64 * this->getX(), 64 * this->getY() + 64 * this->getSY() - 64 / 2);
	target.draw(sprite, states);

	sf::Text text;
	text.setFont(*FontStorage::get()->get("1"));
	text.setString(std::to_string(this->currentTrade.movesLeft));
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(64 * this->getX(), 64 * this->getY() + 64 * this->getSY() - 64 / 2);
	target.draw(text, states);
}
uint32_t Market::getRegenerationSpeed() const {
	return 10000;
}
std::string Market::getNewWindowSoundName() const {
	return "horse";
}
std::wstring Market::getBuildingFinishedStr() const {
	return L"РЫНОК ПОСТРОЕН\nБлагодаря Вашим рабочим, рынок готов к торговле.";
}
std::wstring Market::getIsNotBuiltYetStr() const {
	return L"РЫНОК ЕЩЕ НЕ ПОСТРОЕН\nДождитесь конца строительства.";
}
std::string Market::getTextureName() const {
	return "market";
}
std::wstring Market::getUpgradeStartDescription() const {
	return L"НАЧАТО УЛУЧШЕНИЕ РЫНКА\nПодождите, пока оно закончится.";
}
std::wstring Market::getUpgradeFinishDescription() const {
	return L"УЛУЧШЕНИЕ РЫНКА ЗАВЕРШЕНО\nТеперь Вы снова можете его использовать.";
}
std::wstring Market::getBusyWithUpgradingDescription() const {
	return L"РЫНОК НЕДОСТУПЕН\nПодождите, пока будет завершено улучшение.";
}
Resources Market::getUpgradeCost() const {
	Resources upgradeCosts[TOTAL_LEVELS - 1] = {
		Resources({{"wood", 60000}}),
		Resources({{"wood", 120000}})
	};
	return upgradeCosts[this->getCurrentLevel() - 1];
}
uint32_t Market::getUpgradeMoves() const {
	uint32_t upgradeMoves[TOTAL_LEVELS - 1] = {
		3,
		6
	};
	return upgradeMoves[this->getCurrentLevel() - 1];
}
GameObjectResponse Market::getGameObjectResponse(const Player& player) {
	if (!this->exist()) {
		return GameObjectResponse();
	}
	if (this->belongTo(&player)) {
		if (!this->works()) {
			return this->handleDoesNotWork();
		}
		if (this->upgrading()) {
			return this->handleUpgrading();
		}
		if (this->busy()) {
			GameObjectResponse response;
			MessageWindow* window = new MessageWindow("horse", "click", L"РЫНОК ЗАНЯТ\nДетали сделки:\n" + this->currentTrade.getReadableInfo());
			response.popUpWindows.push(window);
			return response;
		}
		return this->getSelectWindow();
	}
	return this->getUnitOfEnemyResponse();
}