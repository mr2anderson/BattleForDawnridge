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


#include "TryToCollectEvent.hpp"


TryToCollectEvent::TryToCollectEvent(uint32_t playerId, ResourcePoint* rp, uint32_t value) {
	this->id = playerId;
	this->rp = rp;
	this->value = value;
}
uint32_t TryToCollectEvent::getPlayerId() const {
	return this->id;
}
ResourcePoint* TryToCollectEvent::getResourcePoint() {
	return this->rp;
}
uint32_t TryToCollectEvent::getValue() {
	return this->value;
}