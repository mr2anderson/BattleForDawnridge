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


#include "Effect.hpp"
#include "Locales.hpp"
#include "SubHpEvent.hpp"
#include "CreateEEvent.hpp"
#include "ArchiveType.hpp"


Effect::Effect() = default;
Effect::Effect(uint32_t x, uint32_t y, uint32_t playerId) : HPGO(x, y, boost::none) {
	this->playerId = playerId;
}
uint32_t Effect::getSX() const { // Config file is not used cuz engine supports only (1, 1) effects
	return 1;
}
uint32_t Effect::getSY() const {
	return 1;
}
uint32_t Effect::getPlayerId() const {
    return this->playerId;
}
uint8_t Effect::getClickPriority() const {
	return GO::PRIORITY::LOW;
}
uint8_t Effect::getDrawingPriority() const {
	return GO::PRIORITY::HIGHEST;
}
uint32_t Effect::getMaxHP() const {
	return this->getLifetime();
}
Events Effect::newMove(MapState* state, uint32_t playerId) {
	if (!this->exist()) {
		return Events();
	}

	Events events;

	if (this->playerId == playerId) {
		events.add(std::make_shared<SubHpEvent>(this, 1));
		if (this->getHP() == 1) {
			return events;
		}
	}

	events = events + this->getActiveNewMoveEvent(state, playerId);

	return events;
}
Events Effect::getResponse(MapState* state, uint32_t playerId, uint32_t button) {
	if (!this->exist()) {
		return Events();
	}

	std::vector<HorizontalSelectionWindowComponent> components;
	components.push_back(this->getExitComponent());
	components.push_back(this->getDescriptionComponent());
	components.push_back(this->getEffectComponent());

	std::shared_ptr<HorizontalSelectionWindow> w = std::make_shared<HorizontalSelectionWindow>(components);

	Events events;
	events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));
	events.add(std::make_shared<CreateEEvent>(w));

	return events;
}
HorizontalSelectionWindowComponent Effect::getEffectComponent() const {
	return {
		this->getTextureName(),
        *Locales::get()->get("effect_description") + std::to_wstring(this->getHP()),
		false,
		Events(),
		this->getTextureRect()
	};
}


BOOST_CLASS_EXPORT_IMPLEMENT(Effect)