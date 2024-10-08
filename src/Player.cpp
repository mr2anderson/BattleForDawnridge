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
#include "Parameters.hpp"



Player::Player() = default;
Player::Player(uint32_t id) {
	this->id = id;
	this->resources = Parameters::get().getResources("start_resources");
}
uint32_t Player::getId() const {
	return this->id;
}
bool operator==(const Player &a, const Player &b) {
	if (a.getId() == b.getId()) {
		return true;
	}
	return false;
}
bool operator!=(const Player &a, const Player &b) {
	return !(a == b);
}
bool operator<(const Player &a, const Player &b) {
    return a.getId() < b.getId();
}
void Player::addResource(const Resource& resource, uint32_t limit) {
	this->resources.plus(resource, limit);
}
void Player::subResource(const Resource &resource) {
	this->resources.minus(resource);
}
void Player::addResources(const Resources& resources, const Resources& limit) {
	this->resources.plus(resources, limit);
}
void Player::subResources(const Resources& resources) {
	this->resources.minus(resources);
}
void Player::limitResources(const Resources& limit) {
	this->resources.limit(limit);
}
int32_t Player::getResource(const std::string& id) const {
	return this->resources.get(id);
}
Resources Player::getResources() const {
	return this->resources;
}
bool Player::IS_POSITIVE(uint32_t id) {
    return (id % 2 == 1);
}
bool Player::IS_NEGATIVE(uint32_t id) {
    return !IS_POSITIVE(id);
}