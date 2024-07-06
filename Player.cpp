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


#include "Player.hpp"
#include <iostream>


Player::Player() = default;
Player::Player(uint32_t id) {
	this->id = id;
	this->bar.plus(Resource("food", 20000));
	this->bar.plus(Resource("wood", 20000));
}
uint32_t Player::getId() const {
	return this->id;
}
bool operator==(Player a, Player b) {
	if (a.getId() == b.getId()) {
		return true;
	}
	return false;
}
bool operator!=(Player a, Player b) {
	return !(a == b);
}
const ResourceBar* Player::getConstResourceBarPtr() const {
	return &this->bar;
}
void Player::addResource(const Resource &resource) {
	this->bar.plus(resource);
}
void Player::subResource(const Resource &resource) {
	this->bar.minus(resource);
}
void Player::addResources(const Resources& resources) {
	this->bar.plus(resources);
}
void Player::subResources(const Resources& resources) {
	this->bar.minus(resources);
}
int32_t Player::getResource(const std::string& id) const {
	return this->bar.get(id);
}
Resources Player::getResources() const {
	return this->bar.getResources();
}