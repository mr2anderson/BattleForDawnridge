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


Caravan::Caravan(uint32_t x, uint32_t y, const Player *playerPtr) : HpSensitiveBuilding(x, y, 10000, false, playerPtr) {}
void Caravan::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->Building::draw(target, states);
	if (this->currentTrade.movesLeft != 0) {
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
}
GameObjectResponse Caravan::doTrade(const Trade& trade) {
	assert(this->works() and !this->busy());
	this->currentTrade = trade;
	GameObjectResponse response;
	response.gameEvent = GameEvent();
	response.gameEvent.value().startTrade = trade.sell;
	MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", L"Сделка начата!\nВаши ресурсы забрал караван, обмен будет выполнен через несколько ходов. Детали сделки:\n" + trade.getReadableInfo());
	response.popUpWindows.push(window);
	return response;
}
GameObjectResponse Caravan::newMove(const Player& currentPlayer, uint32_t windowW, uint32_t windowH) {
	GameObjectResponse response;
	if (!this->belongTo(&currentPlayer)) {
		return response;
	}
	response = this->processRegeneration();
	if (!this->works() or !this->busy()) {
		return response;
	}
	this->currentTrade.movesLeft = this->currentTrade.movesLeft - 1;
	if (this->currentTrade.movesLeft == 0) {
		if (!response.gameEvent.has_value()) {
			response.gameEvent = GameEvent();
		}
		response.gameEvent.value().finishTrade = this->currentTrade.buy;
		MessageWindow* window = new MessageWindow(this->getNewWindowSoundName(), "click", L"Сделка окончена!\nРесурсы были получены. Детали сделки:\n" + this->currentTrade.getReadableInfo());
		response.popUpWindows.push(window);
	}
	return response;
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
	return L"Караван не готов\nДождитесь конца строительства.";
}
std::string Caravan::getTextureName() const {
	return "caravan";
}
bool Caravan::busy() const {
	return this->currentTrade.movesLeft != 0;
}
GameObjectResponse Caravan::getGameObjectResponse(const Player& player, uint32_t windowW, uint32_t windowH) {
	if (this->belongTo(&player)) {
		if (!this->works()) {
			return this->handleDoesNotWork();
		}
		if (this->busy()) {
			GameObjectResponse response;
			MessageWindow* window = new MessageWindow("horse", "click", L"Караван занят\nДетали сделки:\n" + this->currentTrade.getReadableInfo());
			response.popUpWindows.push(window);
			return response;
		}
		std::vector<std::tuple<std::string, std::wstring, bool, GameEvent>> data;
		data.emplace_back("exit", L"Покинуть", true, GameEvent());
		data.emplace_back("caravan", L"Караваны позволяют обменивать ресурсы.", false, GameEvent());
		GameEvent gameEventTrade;

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("gold", 1000), Resource("food", 100000), 5));
		data.emplace_back("food", L"Купить 100k еды за 1k золота (5 ходов)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("gold", 1000), Resource("wood", 100000), 5));
		data.emplace_back("wood", L"Купить 100k древесины за 1k золота (5 ходов)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("gold", 1000), Resource("stone", 20000), 5));
		data.emplace_back("stone", L"Купить 20k камня за 1k золота (5 ходов)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("gold", 1000), Resource("iron", 20000), 5));
		data.emplace_back("iron", L"Купить 20k железа за 1k золота (5 ходов)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("food", 100000), Resource("gold", 1000), 5));
		data.emplace_back("gold", L"Купить 1000 золота за 100k еды (5 ходов)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("wood", 100000), Resource("gold", 1000), 5));
		data.emplace_back("gold", L"Купить 1000 золота за 100k дерева (5 ходов)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("stone", 20000), Resource("gold", 1000), 5));
		data.emplace_back("gold", L"Купить 1000 золота за 20k камня (5 ходов)", true, gameEventTrade);

		gameEventTrade.tryToTrade = std::make_tuple(this, Trade(Resource("iron", 20000), Resource("gold", 1000), 5));
		data.emplace_back("gold", L"Купить 1000 золота за 20k железа (5 ходов)", true, gameEventTrade);

		SelectWindow* window = new SelectWindow(this->getNewWindowSoundName(), "click", data);
		GameObjectResponse response;
		response.popUpWindows.push(window);
		return response;
	}
	return this->getUnitOfEnemyResponse();
}