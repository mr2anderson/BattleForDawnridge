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


#include "Dragon.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "ResetDragonRecoverMovesLeftEvent.hpp"


Dragon::Dragon() {
	this->recoverMovesLeft = std::nullopt;
}
Dragon::Dragon(uint32_t playerId) {
	this->recoverMovesLeft = std::nullopt;
	this->playerId = playerId;
}
void Dragon::resetRecoverMoves() {
	this->recoverMovesLeft = this->getRecoverTime();
}
void Dragon::decreaseRecoverMovesLeft() {
	this->recoverMovesLeft = this->getRecoverMovesLeft() - 1;
}
uint32_t Dragon::getRecoverMovesLeft() const {
	return this->recoverMovesLeft.value_or(this->getRecoverTime());
}
uint32_t Dragon::getPlayerId() const {
	return this->playerId;
}
std::shared_ptr<DragonFlame> Dragon::getFlame(uint32_t centerX, uint32_t centerY) const {
	std::shared_ptr<DragonFlame> flame = this->getFlameType();

	flame->setCenterX(centerX);
	flame->setCenterY(centerY);

	return flame;
}
Events Dragon::unselect(uint32_t x, uint32_t y, uint8_t button) {
	if (button == sf::Mouse::Button::Right) {
		Events events = this->Selectable::unselect(x, y, button);
		events.add(std::make_shared<PlaySoundEvent>("click"));
		return events;
	}

	Events events = this->Selectable::unselect(x, y, button);
	events.add(std::make_shared<PlaySoundEvent>(this->getAttackSoundName()));
	events.add(std::make_shared<ResetDragonRecoverMovesLeftEvent>(this));
	events.add(std::make_shared<CreateEEvent>(this->getFlame(x, y)));
	events = events + this->doAttack(x, y);
	return events;
}
std::shared_ptr<sf::Drawable> Dragon::getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const {
	return this->getFlame(mouseX / 64, mouseY / 64)->getPreview();
}