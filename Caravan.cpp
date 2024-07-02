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


#include "Caravan.hpp"


const Resources Caravan::UPGRADE_COSTS[Caravan::TOTAL_LEVELS - 1] = {
	Resources({{"wood", 35000}}),
	Resources({{"wood", 35000}}),
};
const uint32_t Caravan::UPGRADE_MOVES[Caravan::TOTAL_LEVELS - 1] = {
	2,
	2,
};
const uint32_t Caravan::LEVEL_TRADE_TIME[Caravan::TOTAL_LEVELS] = {
	5,
	4,
	3
};


Caravan::Caravan(uint32_t x, uint32_t y, const Player *playerPtr) : HpSensitiveBuilding(x, y, 10000, false, playerPtr) {}
void Caravan::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Building::draw(target, states);
	if (this->exist() and this->currentTrade.movesLeft != 0 and !this->upgrading()) {
		this->drawCurrentTradeShortInfo(target, states);
	}
}
GameObjectResponse Caravan::doTrade(const Trade& trade) {
	this->currentTrade = trade;

	GameObjectResponse response;
	response.gameEvent = GameEvent();
	response.gameEvent.value().subResource.push_back(trade.sell);

	MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", L"Сделка начата!\nВаши ресурсы забрал караван, обмен будет выполнен через несколько ходов. Детали сделки:\n" + trade.getReadableInfo());
	response.popUpWindows.push(window);

	return response;
}
GameObjectResponse Caravan::newMove(const Player& currentPlayer) {
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
GameObjectResponse Caravan::upgrade() {
	GameObjectResponse response;
	response.gameEvent = GameEvent();
	response.gameEvent.value().subResources.push_back(UPGRADE_COSTS[this->getCurrentLevel() - 1]);

	this->Building::upgrade(UPGRADE_MOVES[this->getCurrentLevel() - 1]);

	response = response + this->handleUpgradeStart();

	return response;
}
Resources Caravan::getCost() const {
	Resources cost;
	cost.plus(Resource("wood", 25000));
	return cost;
}
GameObjectResponse Caravan::currentTradeNewMove() {
	GameObjectResponse response;
	response.gameEvent = GameEvent();

	this->currentTrade.movesLeft = this->currentTrade.movesLeft - 1;
	if (this->currentTrade.movesLeft == 0) {
		if (!response.gameEvent.has_value()) {
			response.gameEvent = GameEvent();
		}
		response.gameEvent.value().addResource.push_back(this->currentTrade.buy);
		MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", L"Сделка окончена!\nРесурсы были получены. Детали сделки:\n" + this->currentTrade.getReadableInfo());
		response.popUpWindows.push(window);
	}

	return response;
}
bool Caravan::busy() const {
	return this->currentTrade.movesLeft != 0;
}
GameObjectResponse Caravan::getSelectWindow() {
	std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
	data.emplace_back("exit", L"Покинуть", true, GameEvent());
	data.emplace_back("caravan", L"Караваны позволяют обменивать ресурсы.", false, GameEvent());

	if (this->getCurrentLevel() < TOTAL_LEVELS) {
		GameEvent gameEventUpgrade;
		gameEventUpgrade.tryToUpgrade.emplace_back(this, UPGRADE_COSTS[this->getCurrentLevel() - 1]);
		data.emplace_back("upgrade", L"Увеличить скорость сделок за " + UPGRADE_COSTS[this->getCurrentLevel() - 1].getReadableInfo(), true, gameEventUpgrade);
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
		gameEventTrade.tryToTrade.emplace_back(this, Trade(Resource(std::get<0>(a), std::get<1>(a)), Resource(std::get<2>(a), std::get<3>(a)), LEVEL_TRADE_TIME[this->getCurrentLevel() - 1]));
		this->addTrade(data, gameEventTrade);
	}

	SelectWindow* window = new SelectWindow(this->getNewWindowSoundName(), "click", data);
	GameObjectResponse response;
	response.popUpWindows.push(window);
	return response;
}
void Caravan::addTrade(std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>>& data, const GameEvent& gameEventTrade) {
	data.emplace_back(std::get<Trade>(gameEventTrade.tryToTrade.back()).sell.type, 
		L"Купить " + std::get<Trade>(gameEventTrade.tryToTrade.back()).buy.getReadableInfo() +
		L" за " + std::get<Trade>(gameEventTrade.tryToTrade.back()).sell.getReadableInfo(), true, gameEventTrade);
}
void Caravan::drawCurrentTradeShortInfo(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::RectangleShape rect;
	rect.setFillColor(UI_COLOR);
	rect.setSize(sf::Vector2f(64, 64 / 2));
	rect.setPosition(64 * this->getX(), 64 * this->getY() + 64 / 2);
	target.draw(rect, states);

	sf::Sprite sprite;
	sprite.setScale(0.5, 0.5);
	sprite.setTexture(*TextureStorage::get()->get(this->currentTrade.buy.type));
	sprite.setPosition(64 * this->getX(), 64 * this->getY() + 64 / 2);
	target.draw(sprite, states);

	sf::Text text;
	text.setFont(*FontStorage::get()->get("1"));
	text.setString(std::to_string(this->currentTrade.movesLeft));
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1);
	text.setPosition(64 * this->getX() + 64 / 2, 64 * this->getY() + 64 / 2);
	target.draw(text, states);
}
uint32_t Caravan::getRegenerationSpeed() const {
	return 2000;
}
std::string Caravan::getNewWindowSoundName() const {
	return "horse";
}
std::wstring Caravan::getBuildingFinishedStr() const {
	return L"Караван построен!\nБлагодаря Вашим рабочим, караван готов к работе.";
}
std::wstring Caravan::getIsNotBuiltYetStr() const {
	return L"Караван еще не построен\nДождитесь конца строительства.";
}
std::string Caravan::getTextureName() const {
	return "caravan";
}
std::wstring Caravan::getUpgradeStartDescription() const {
	return L"Начато улучшение каравана\nПодождите, пока оно закончится.";
}
std::wstring Caravan::getUpgradeFinishDescription() const {
	return L"Улучшение каравана завершено!\nТеперь Вы снова можете его использовать.";
}
std::wstring Caravan::getBusyWithUpgradingDescription() const {
	return L"Караван не доступен\nПодождите, пока будет завершено улучшение.";
}
GameObjectResponse Caravan::getGameObjectResponse(const Player& player) {
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
			MessageWindow* window = new MessageWindow("horse", "click", L"Караван занят\nДетали сделки:\n" + this->currentTrade.getReadableInfo());
			response.popUpWindows.push(window);
			return response;
		}
		return this->getSelectWindow();
	}
	return this->getUnitOfEnemyResponse();
}