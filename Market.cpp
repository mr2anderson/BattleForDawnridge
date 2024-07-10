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
Market::Market(uint32_t x, uint32_t y, uint32_t playerId) : 
	HpSensitiveB(x, y, 3, 3, 15000, playerId),
	Building(x, y, 3, 3, 15000, playerId) {

}
Building* Market::cloneBuilding() const {
	return new Market(*this);
}
void Market::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->HpSensitiveB::draw(target, states);
	if (this->exist() and this->currentTrade.movesLeft != 0) {
		this->drawCurrentTradeShortInfo(target, states);
	}
}
Events Market::doTrade(const Trade& trade) {
	this->currentTrade = trade;

	Events gEvent;
	gEvent.add(std::make_shared<SubResourceEvent>(trade.sell));

	std::shared_ptr<WindowButton> window = std::make_shared<WindowButton>(this->getSoundName(), "click",
		*Texts::get()->get("trade_started") + L'\n' +
		trade.getReadableInfo(), *Texts::get()->get("OK"));
	
	window->addOnStartGEvent(gEvent);
	Events responce;
	responce.add(std::make_shared<CreateEEvent>(window));
	return responce;
}
void Market::decreaseCurrentTradeMovesLeft() {
	this->currentTrade.movesLeft = this->currentTrade.movesLeft - 1;
}
Events Market::newMove(uint32_t playerId) {
	if (!this->belongTo(playerId) or !this->exist()) {
		return Events();
	}
	Events response = this->regenerate();
	if (this->works()) {
		response = response + this->handleCurrentTrade();
	}
	return response;
}
Resources Market::getCost() const {
	Resources cost;
	cost.plus(Resource("stone", 25000));
	return cost;
}
std::string Market::getTextureName() const {
	return "market";
}
std::string Market::getSoundName() const {
	return "gold";
}
std::wstring Market::getDescription() const {
	return *Texts::get()->get("market_description");
}
HorizontalSelectionWindowComponent Market::getTradeComponent(std::shared_ptr<TryToTradeEvent> gameEventTrade) const {
	Events events;
	events.add(gameEventTrade);
	HorizontalSelectionWindowComponent component = {
		gameEventTrade->getTrade().buy.type + "_icon",
		*Texts::get()->get("buy") + gameEventTrade->getTrade().buy.getReadableInfo() +
		*Texts::get()->get("for") + gameEventTrade->getTrade().sell.getReadableInfo(),
		true,
		false,
		events
	};
	return component;
}
HorizontalSelectionWindowComponent Market::getBusyWithTradeComponent() const {
	HorizontalSelectionWindowComponent component = {
		"trade_icon",
		*Texts::get()->get("market_is_busy") + this->currentTrade.getReadableInfo(),
		false,
		false,
		Events()
	};
	return component;
}
uint32_t Market::getRegenerationSpeed() const {
	return 10000;
}
std::wstring Market::getUpperCaseReadableName() const {
	return *Texts::get()->get("market_upper_case_readable_name");
}
bool Market::busy() const {
	return this->currentTrade.movesLeft != 0;
}
uint32_t Market::getTradeStartTime() const {
	return 8;
}
void Market::drawCurrentTradeShortInfo(sf::RenderTarget& target, sf::RenderStates states) const {
	BuildingShortInfo info(this->getX(), this->getY(), this->getSX(), this->getSY(), this->currentTrade.buy.type + "_icon", std::to_string(this->currentTrade.movesLeft));
	target.draw(info, states);
}
Events Market::handleCurrentTrade() {
	if (!this->busy()) {
		return Events();
	}
	Events responce;
	Events decreaseEvent;
	decreaseEvent.add(std::make_shared<DecreaseCurrentTradeMovesLeftEvent>(this));
	std::shared_ptr<FlyingE> element = std::make_shared<FlyingE>("trade_icon", this->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
	responce.add(std::make_shared<CreateEEvent>(element));
	element->addOnStartGEvent(decreaseEvent);
	if (this->currentTrade.movesLeft == 1) {
		Events gEvent;
		gEvent.add(std::make_shared<AddResourceEvent>(this->currentTrade.buy));
		element = std::make_shared<FlyingE>(this->currentTrade.buy.type + "_icon", this->getSoundName(), this->getX(), this->getY(), this->getSX(), this->getSY());
		element->addOnStartGEvent(gEvent);
		responce.add(std::make_shared<CreateEEvent>(element));
	}
	return responce;
}
Events Market::getSelectionW() {
	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getHpInfoComponent());
	components.push_back(this->getDestroyComponent());
	if (this->repairing()) {
		components.push_back(this->getBusyWithRepairingComponent());
	}
	if (this->busy()) {
		components.push_back(this->getBusyWithTradeComponent());
	}
	if (this->works() and !this->busy()) {
		for (const auto& a : { std::make_tuple("gold", 10000, "food", 10000),
		std::make_tuple("gold", 10000, "wood", 10000),
		std::make_tuple("gold", 10000, "stone", 10000),
		std::make_tuple("gold", 10000, "iron", 10000),
		std::make_tuple("food", 10000, "gold", 10000),
		std::make_tuple("wood", 10000, "gold", 10000),
		std::make_tuple("stone", 10000, "gold", 10000),
		std::make_tuple("iron", 10000, "gold", 10000) }) {
			std::shared_ptr<TryToTradeEvent> tryToTradeEvent = std::make_shared<TryToTradeEvent>(this, Trade(Resource(std::get<0>(a), std::get<1>(a)), Resource(std::get<2>(a), std::get<3>(a)), this->getTradeStartTime()));
			components.push_back(this->getTradeComponent(tryToTradeEvent));
		}
	}

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(this->getSoundName(), "click", components);
	Events response;
	response.add(std::make_shared<CreateEEvent>(window));
	return response;
}
Events Market::getGameObjectResponse(uint32_t playerId) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(playerId)) {
		return this->getSelectionW();
	}
	return this->getUnitOfEnemyResponse();
}