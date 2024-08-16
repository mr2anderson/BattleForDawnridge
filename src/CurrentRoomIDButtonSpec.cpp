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


#include "CurrentRoomIDButtonSpec.hpp"
#include "PlaySoundEvent.hpp"


CurrentRoomIDButtonSpec::CurrentRoomIDButtonSpec() = default;
CurrentRoomIDButtonSpec::CurrentRoomIDButtonSpec(uint32_t index) : MainScreenButtonSpec(index) {

}
CurrentRoomIDButtonSpec::CurrentRoomIDButtonSpec(uint32_t index, const std::string& value) : MainScreenButtonSpec(index) {
	this->value = value;
}
std::string CurrentRoomIDButtonSpec::getTextureName() const {
	return "room_id_icon";
}
StringLcl CurrentRoomIDButtonSpec::getString() const {
	return { "{room} " + this->value };
}
Events CurrentRoomIDButtonSpec::getEvents() const {
	Events events;

	events.add(std::make_shared<PlaySoundEvent>("wind"));

	return events;
}