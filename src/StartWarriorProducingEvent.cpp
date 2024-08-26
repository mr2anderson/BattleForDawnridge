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


#include "StartWarriorProducingEvent.hpp"


StartWarriorProducingEvent::StartWarriorProducingEvent(std::shared_ptr<const Building>  b, std::shared_ptr<WarriorProducerSpec> spec, std::shared_ptr<Warrior>  w) {
	this->b = b;
	this->spec = spec;
	this->w = w;
}
std::shared_ptr<const Building>  StartWarriorProducingEvent::getBuilding() const {
	return this->b;
}
std::shared_ptr<WarriorProducerSpec> StartWarriorProducingEvent::getSpec() {
	return this->spec;
}
std::shared_ptr<Warrior>  StartWarriorProducingEvent::getWarrior() {
	return this->w;
}