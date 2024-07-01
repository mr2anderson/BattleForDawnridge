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


Caravan::Caravan(uint32_t x, uint32_t y, const Player *playerPtr) : Building(x, y, 10000, playerPtr) {}
GameObjectResponse Caravan::doTrade(const Trade& trade) {
	assert(this->works() and !this->busy());
	this->currentTrade = trade;
	GameObjectResponse response;
	response.gameEvent = GameEvent();
	response.gameEvent.value().startTrade = trade.sell;
	MessageWindow* window = new MessageWindow("horse", "click", L"Сделка начата!\nВаши ресурсы забрал караван, обмен будет выполнен через несколько ходов.");
	response.popUpWindows.push(window);
	return response;
}
GameObjectResponse Caravan::newMove(const Player& currentPlayer, uint32_t windowW, uint32_t windowH) {
	GameObjectResponse response;
	if (!this->belongTo(&currentPlayer)) {
		return response;
	}
	bool work1 = this->works();
	this->addHp(Building::REGENERATION_SPEED);
	bool work2 = this->works();
	if (work2 and !work1) {
		MessageWindow* window = new MessageWindow("horse", "click", L"Караван был достроен!\nБлагодаря усилиям Ваших рабочих, караван снова работает.");
		response.popUpWindows.push(window);
	}
	if (!this->works() or !this->busy()) {
		return response;
	}
	this->currentTrade.movesLeft = this->currentTrade.movesLeft - 1;
	if (this->currentTrade.movesLeft == 0) {
		if (!response.gameEvent.has_value()) {
			response.gameEvent = GameEvent();
		}
		response.gameEvent.value().finishTrade = this->currentTrade.buy;
		MessageWindow* window = new MessageWindow("horse", "click", L"Сделка окончена!\nРесурсы были получены.");
		response.popUpWindows.push(window);
	}
	return response;
}
std::string Caravan::getTextureName() const {
	return "caravan";
}
bool Caravan::busy() const {
	return this->currentTrade.movesLeft != 0;
}
GameObjectResponse Caravan::getGameObjectResponse(const Player& player, uint32_t windowW, uint32_t windowH) {
	GameObjectResponse response;
	if (this->belongTo(&player)) {
		if (!this->works()) {
			MessageWindow* window = new MessageWindow("horse", "click", L"Караван не достроен\nЧтобы использовать караван, он должен быть цел хотя-бы на " + std::to_wstring(Building::MINIMAL_PERCENT) + L"%.");
			response.popUpWindows.push(window);
			return response;
		}
		if (this->busy()) {
			MessageWindow* window = new MessageWindow("horse", "click", L"Караван занят\nДо конца сделки осталось ходов: " + std::to_wstring(this->currentTrade.movesLeft) + L" .");
			response.popUpWindows.push(window);
			return response;
		}
		std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
		data.emplace_back("exit", L"Покинуть", true, GameEvent());
		data.emplace_back("fort", L"Используя караваны, Вы можете обменивать ресурсы.", false, GameEvent());
		GameEvent gameEventTrade;

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("gold", 1000), Resource("food", 10000), 2));
		data.emplace_back("food", L"Купить 10k еды за 1k золота (2 хода)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("gold", 1000), Resource("wood", 10000), 2));
		data.emplace_back("wood", L"Купить 10k древесины за 1k золота (2 хода)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("gold", 1000), Resource("stone", 2000), 2));
		data.emplace_back("stone", L"Купить 2k камня за 1k золота (2 хода)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("gold", 1000), Resource("iron", 2000), 2));
		data.emplace_back("iron", L"Купить 2k железа за 1k золота (2 хода)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("food", 10000), Resource("gold", 1000), 2));
		data.emplace_back("gold", L"Купить 1000 золота за 10k еды (2 хода)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("wood", 10000), Resource("gold", 1000), 2));
		data.emplace_back("gold", L"Купить 1000 золота за 10k дерева (2 хода)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("stone", 2000), Resource("gold", 1000), 2));
		data.emplace_back("gold", L"Купить 1000 золота за 2k камня (2 хода)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("iron", 2000), Resource("gold", 1000), 2));
		data.emplace_back("gold", L"Купить 1000 золота за 2k железа (2 хода)", true, gameEventTrade);

		SelectWindow* window = new SelectWindow("horse", "click", data);
		response.popUpWindows.push(window);

		return response;
	}

	response.gameEvent = GameEvent();
	response.gameEvent.value().tryToAttack = (this);
	return response;
}