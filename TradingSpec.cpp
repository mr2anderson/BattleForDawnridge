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
#include "Locales.hpp"
#include "CreateEEvent.hpp"
#include "SubResourceEvent.hpp"
#include "FocusOnEvent.hpp"
#include "ImageFlyingE.hpp"
#include "DecreaseCurrentTradeMovesLeftEvent.hpp"
#include "AddResourceEvent.hpp"
#include "Building.hpp"
#include "DoTradeEvent.hpp"


TradingSpec::TradingSpec() = default;
Events TradingSpec::doTrade(const Building *b, const Trade& trade) {
	Events clickSoundEvent;
	clickSoundEvent.add(std::make_shared<PlaySoundEvent>("click"));

	this->currentTrade = trade;

	Events response;

	response.add(std::make_shared<PlaySoundEvent>(b->getSoundName()));

	std::shared_ptr<WindowButton> window = std::make_shared<WindowButton>(
            *Locales::get()->get("trade_started") + L'\n' +
            trade.getReadableInfo(), *Locales::get()->get("OK"), clickSoundEvent);
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

		Resources limit;
		for (uint32_t i = 0; i < state->getCollectionsPtr()->totalBuildings(); i = i + 1) {
			Building* warehouse = state->getCollectionsPtr()->getBuilding(i);
			if (warehouse->exist() and warehouse->getPlayerId() == b->getPlayerId()) {
				limit.plus(warehouse->getLimit());
			}
		}

		responce.add(std::make_shared<AddResourceEvent>(this->currentTrade.buy, limit));
	}

	return responce;
}
std::vector<BuildingHorizontalSelectionWindowComponent> TradingSpec::getComponents(const Building *b, MapState* state) {
	std::vector<BuildingHorizontalSelectionWindowComponent> components;

	if (b->works()) {
		if (this->busy()) {
			components.emplace_back(
				HorizontalSelectionWindowComponent("trade_icon",
                *Locales::get()->get("trading_building_is_busy") + this->currentTrade.getReadableInfo(),
				false,
				Events()),
                true
			);
		}
		else {
			Resources playerResources = state->getPlayersPtr()->getPlayerPtr(b->getPlayerId())->getResources();
			std::vector<Trade> trades = this->getTrades();
			for (const auto& trade : trades) {
				components.push_back(this->getTradeComponent(b, playerResources, trade));
			}
		}
	}
	else {
		components.emplace_back(
			HorizontalSelectionWindowComponent("hammer_icon",
			*Locales::get()->get("this_building_cant_do_trades_if_hp_isnt_full"),
			false,
			Events()),
            true
		);
	}

	return components;
}
std::optional<BuildingShortInfo> TradingSpec::getShortInfo(const Building *b) const {
    if (!b->works()) {
        return std::nullopt;
    }

    std::string pictureName;
    std::string text;

    if (this->busy()) {
        pictureName = this->currentTrade.buy.type + "_icon";
        text = std::to_string(this->currentTrade.movesLeft);
    }
    else {
        pictureName = "trade_icon";
        text = "...";
    }

    return BuildingShortInfo(b->getXInPixels(), b->getYInPixels(), b->getSX(), b->getSY(), pictureName, text);
}
bool TradingSpec::busy() const {
	return this->currentTrade.movesLeft > 0;
}
BuildingHorizontalSelectionWindowComponent TradingSpec::getTradeComponent(const Building* b, const Resources& playerResources, const Trade& trade) {
	Events events;
	if (playerResources.get(trade.sell.type) >= trade.sell.n) {
		events.add(std::make_shared<DoTradeEvent>(b, this, trade));
	}
	else {
		Events clickEvent;
		clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

		std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Locales::get()->get("no_resources_for_trade"), *Locales::get()->get("OK"), clickEvent);
		events = events + clickEvent;
		events.add(std::make_shared<CreateEEvent>(w));
	}

	BuildingHorizontalSelectionWindowComponent component = {
		HorizontalSelectionWindowComponent(trade.buy.type + "_icon",
        *Locales::get()->get("buy") + trade.buy.getReadableInfo() +
        *Locales::get()->get("for") + trade.sell.getReadableInfo(),
		true,
		events),
        false
	};

	return component;
}