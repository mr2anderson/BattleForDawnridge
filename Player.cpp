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


#include "Player.hpp"


Player::Player() = default;
Player::Player(uint32_t id) {
	this->id = id;
	this->bar.plus("food", 10000);
	this->bar.plus("wood", 10000);
}
uint32_t Player::getId() const {
	return this->id;
}
const ResourceBar* Player::getConstResourceBarPtr() const {
	return &this->bar;
}
void Player::addResource(const std::string& id, int32_t value) {
	this->bar.plus(id, value);
}
void Player::subResource(const std::string& id, int32_t value) {
	this->bar.minus(id, value);
}
int32_t Player::getResource(const std::string& id) const {
	return this->bar.get(id);
}
bool operator==(const Player& a, const Player& b) {
	if (a.getId() == b.getId()) {
		return true;
	}
	return false;
}
bool operator!=(const Player& a, const Player& b) {
	return !(a == b);
}