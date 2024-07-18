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


#include "TradingSpec.hpp"
#include "PlaySoundEvent.hpp"
#include "WindowButton.hpp"
#include "Texts.hpp"
#include "CreateEEvent.hpp"
#include "SubResourceEvent.hpp"
#include "FocusOnEvent.hpp"
#include "ImageFlyingE.hpp"
#include "DecreaseCurrentTradeMovesLeftEvent.hpp"
#include "AddResourceEvent.hpp"
#include "TryToTradeEvent.hpp"
#include "Building.hpp"


TradingSpec::TradingSpec() = default;
Events TradingSpec::doTrade(const Building *b, const Trade& trade) {
	Events clickSoundEvent;
	clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	this->currentTrade = trade;

	Events response;

	response.add(std::make_shared<PlaySoundEvent>(b->getSoundName()));

	std::shared_ptr<WindowButton> window = std::make_shared<WindowButton>(
		*Texts::get()->get("trade_started") + L'\n' +
		trade.getReadableInfo(), *Texts::get()->get("OK"), clickSoundEvent);
	response.add(std::make_shared<CreateEEvent>(window));

	response.add(std::make_shared<SubResourceEvent>(trade.sell));

	return response;
}
void TradingSpec::decreaseCurrentTradeMovesLeft() {
	this->currentTrade.movesLeft = this->currentTrade.movesLeft - 1;
}
Events TradingSpec::getActiveNewMoveEvent(const Building *b, MapState* state) {
	if (!b->works() or !this->busy()) {
		return Events();
	}

	Events responce;

	responce.add(std::make_shared<FocusOnEvent>(b->getX(), b->getY(), b->getSX(), b->getSY()));

	responce.add(std::make_shared<PlaySoundEvent>(b->getSoundName()));

	std::shared_ptr<ImageFlyingE> element = std::make_shared<ImageFlyingE>("trade_icon", b->getX(), b->getY(), b->getSX(), b->getSY());
	responce.add(std::make_shared<CreateEEvent>(element));

	responce.add(std::make_shared<DecreaseCurrentTradeMovesLeftEvent>(this));

	if (this->currentTrade.movesLeft == 1) {
		responce.add(std::make_shared<PlaySoundEvent>(b->getSoundName()));

		element = std::make_shared<ImageFlyingE>(this->currentTrade.buy.type + "_icon", b->getX(), b->getY(), b->getSX(), b->getSY());
		responce.add(std::make_shared<CreateEEvent>(element));

		responce.add(std::make_shared<AddResourceEvent>(this->currentTrade.buy));
	}

	return responce;
}
static HorizontalSelectionWindowComponent GET_TRADE_COMPONENT(std::shared_ptr<TryToTradeEvent> e) {
	Events events;
	events.add(e);

	HorizontalSelectionWindowComponent component = {
		e->getTrade().buy.type + "_icon",
		*Texts::get()->get("buy") + e->getTrade().buy.getReadableInfo() +
		*Texts::get()->get("for") + e->getTrade().sell.getReadableInfo(),
		true,
		events
	};

	return component;
}
std::vector<HorizontalSelectionWindowComponent> TradingSpec::getComponents(const Building *b, MapState* state) {
	std::vector<HorizontalSelectionWindowComponent> components;

	if (b->works()) {
		if (this->busy()) {
			components.emplace_back(
				"trade_icon",
				*Texts::get()->get("trading_building_is_busy") + this->currentTrade.getReadableInfo(),
				false,
				Events()
			);
		}
		else {
			std::vector<Trade> trades = this->getTrades();
			for (const auto& trade : trades) {
				std::shared_ptr<TryToTradeEvent> tryToTradeEvent = std::make_shared<TryToTradeEvent>(b, this, trade);
				components.push_back(GET_TRADE_COMPONENT(tryToTradeEvent));
			}
		}
	}
	else {
		components.emplace_back(
			"hammer_icon",
			*Texts::get()->get("this_building_cant_do_trades_if_hp_isnt_full"),
			false,
			Events()
		);
	}

	return components;
}
std::optional<BuildingShortInfo> TradingSpec::getShortInfo(const Building *b) const {
	if (this->busy()) {
		return BuildingShortInfo(b->getXInPixels(), b->getYInPixels(), b->getSX(), b->getSY(), this->currentTrade.buy.type + "_icon", std::to_string(this->currentTrade.movesLeft));
	}
	return std::nullopt;
}
bool TradingSpec::busy() const {
	return this->currentTrade.movesLeft > 0;
}