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


#include "Spell.hpp"
#include "SpellPreview.hpp"
#include "SpellEffect.hpp"
#include "Building.hpp"
#include "ImageFlyingE.hpp"
#include "FocusOnEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "DecreaseSpellCreationMovesLeftEvent.hpp"
#include "EnableCursorEvent.hpp"
#include "DisableCursorEvent.hpp"
#include "UnselectEvent.hpp"
#include "Warrior.hpp"
#include "SelectEvent.hpp"
#include "SubResourcesEvent.hpp"
#include "MarkSpellAsUsedEvent.hpp"


Spell::Spell() = default;
Spell::Spell(uint32_t playerId) {
	this->creationMovesLeft = boost::none;
	this->used = false;
	this->playerId = playerId;
}
bool Spell::isReady() const {
	return (this->getCreationMovesLeft() == 0);
}
uint32_t Spell::getCreationMovesLeft() const {
	return this->creationMovesLeft.value_or(this->getCreationTime());
}
Events Spell::newMove(const Building *father) {
	if (this->isReady()) {
		return Events();
	}

	Events events;
	events.add(std::make_shared<FocusOnEvent>(father->getX(), father->getY(), father->getSX(), father->getSY()));
	events.add(std::make_shared<PlaySoundEvent>(father->getSoundName()));
	
	std::shared_ptr<ImageFlyingE> f = std::make_shared<ImageFlyingE>(this->getTextureName(), father->getX(), father->getY(), father->getSX(), father->getSY());
	events.add(std::make_shared<CreateEEvent>(f));

	events.add(std::make_shared<DecreaseSpellCreationMovesLeftEvent>(this));

	return events;
}
void Spell::decreaseCreationMovesLeft() {
	this->creationMovesLeft = this->getCreationMovesLeft() - 1;
}
Events Spell::use() {
	Events response;
	
	response.add(std::make_shared<PlaySoundEvent>("click"));

	response.add(std::make_shared<DisableCursorEvent>());

	response.add(std::make_shared<SelectEvent>(this));

	return response;
}
bool Spell::wasUsed() const {
	return this->used;
}
void Spell::markAsUsed() {
	this->used = true;
}
uint32_t Spell::getPlayerId() const {
	return this->playerId;
}
Events Spell::onUnselect(MapState* state, uint32_t x, uint32_t y, uint8_t button) {
	Events events;

	if (button == sf::Mouse::Button::Right) {
		return events;
	}

	events.add(std::make_shared<MarkSpellAsUsedEvent>(this));

	std::shared_ptr<SpellEffect> effect = std::make_shared<SpellEffect>(this->getTextureName(), x, y);
	events.add(std::make_shared<CreateEEvent>(effect));

	events.add(std::make_shared<PlaySoundEvent>(this->getSoundName()));

	events = events + this->changeMap(state, x, y);

	return events;
}
std::shared_ptr<sf::Drawable> Spell::getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const {
	uint32_t x = mouseX / 64;
	uint32_t y = mouseY / 64;
	
	std::shared_ptr<SpellPreview> preview = std::make_shared<SpellPreview>(this->getRadius(), this->getPreviewColor(), x, y);

	return preview;
}


BOOST_CLASS_EXPORT_IMPLEMENT(Spell)