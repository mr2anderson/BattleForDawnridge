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


#include "TradingB.hpp"
#include "SubResourceEvent.hpp"
#include "AddResourceEvent.hpp"
#include "DecreaseCurrentTradeMovesLeftEvent.hpp"
#include "BuildingShortInfo.hpp"
#include "Texts.hpp"
#include "ImageFlyingE.hpp"
#include "WindowButton.hpp"
#include "CreateEEvent.hpp"


TradingB::TradingB() = default;
TradingB::TradingB(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units) :
	HpSensitiveB(x, y, playerId, units),
	Building(x, y, playerId, units) {

}
void TradingB::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	this->HpSensitiveB::draw(target, states);
	if (this->exist() and this->currentTrade.movesLeft != 0) {
		this->drawCurrentTradeShortInfo(target, states);
	}
}
Events TradingB::doTrade(const Trade& trade) {
    Events clickSoundEvent;
    clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	this->currentTrade = trade;
	Events response;
	std::shared_ptr<WindowButton> window = std::make_shared<WindowButton>(
		*Texts::get()->get("trade_started") + L'\n' +
		    trade.getReadableInfo(), *Texts::get()->get("OK"), clickSoundEvent);
    response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	response.add(std::make_shared<CreateEEvent>(window));
	response.add(std::make_shared<SubResourceEvent>(trade.sell));
	return response;
}
void TradingB::decreaseCurrentTradeMovesLeft() {
	this->currentTrade.movesLeft = this->currentTrade.movesLeft - 1;
}
Events TradingB::newMove(uint32_t playerId) {
	if (!this->belongTo(playerId) or !this->exist()) {
		return Events();
	}
	Events response = this->regenerate();
	if (this->works()) {
		response = response + this->handleCurrentTrade();
	}
	return response;
}
HorizontalSelectionWindowComponent TradingB::getTradeComponent(std::shared_ptr<TryToTradeEvent> gameEventTrade) const {
	Events events;
	events.add(gameEventTrade);
	HorizontalSelectionWindowComponent component = {
		gameEventTrade->getTrade().buy.type + "_icon",
		*Texts::get()->get("buy") + gameEventTrade->getTrade().buy.getReadableInfo() +
		*Texts::get()->get("for") + gameEventTrade->getTrade().sell.getReadableInfo(),
		true,
		events
	};
	return component;
}
HorizontalSelectionWindowComponent TradingB::getBusyWithTradeComponent() const {
	HorizontalSelectionWindowComponent component = {
		"trade_icon",
		*Texts::get()->get("trading_building_is_busy") + this->currentTrade.getReadableInfo(),
		false,
		Events()
	};
	return component;
}
bool TradingB::busy() const {
	return this->currentTrade.movesLeft != 0;
}
void TradingB::drawCurrentTradeShortInfo(sf::RenderTarget& target, sf::RenderStates states) const {
	BuildingShortInfo info(this->getXInPixels(), this->getYInPixels(), this->getSX(), this->getSY(), this->currentTrade.buy.type + "_icon", std::to_string(this->currentTrade.movesLeft));
	target.draw(info, states);
}
Events TradingB::handleCurrentTrade() {
	if (!this->busy()) {
		return Events();
	}
	Events responce;
	std::shared_ptr<ImageFlyingE> element = std::make_shared<ImageFlyingE>("trade_icon", this->getX(), this->getY(), this->getSX(), this->getSY());
    responce.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	responce.add(std::make_shared<CreateEEvent>(element));
	responce.add(std::make_shared<DecreaseCurrentTradeMovesLeftEvent>(this));
	if (this->currentTrade.movesLeft == 1) {
		element = std::make_shared<ImageFlyingE>(this->currentTrade.buy.type + "_icon", this->getX(), this->getY(), this->getSX(), this->getSY());
        responce.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
		responce.add(std::make_shared<CreateEEvent>(element));
		responce.add(std::make_shared<AddResourceEvent>(this->currentTrade.buy));
	}
	return responce;
}
Events TradingB::getSelectionW() {
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
		std::vector<Trade> trades = this->getTrades();
		for (const auto& trade : trades) {
			std::shared_ptr<TryToTradeEvent> tryToTradeEvent = std::make_shared<TryToTradeEvent>(this, trade);
			components.push_back(this->getTradeComponent(tryToTradeEvent));
		}
	}

	std::shared_ptr<HorizontalSelectionWindow> window = std::make_shared<HorizontalSelectionWindow>(components);
	Events response;
    response.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	response.add(std::make_shared<CreateEEvent>(window));
	return response;
}
Events TradingB::getGameObjectResponse(uint32_t playerId) {
	if (!this->exist()) {
		return Events();
	}
	if (this->belongTo(playerId)) {
		return this->getSelectionW();
	}
	return Events();
}