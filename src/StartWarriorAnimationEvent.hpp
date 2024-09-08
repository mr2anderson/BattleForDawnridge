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


#include <string>
#include <memory>
#include "Event.hpp"


#pragma once


class Warrior;


class StartWarriorAnimationEvent : public Event {
public:
	StartWarriorAnimationEvent(std::shared_ptr<Warrior>  w, const std::string& animation, bool urgent = false);

	std::shared_ptr<Warrior> getWarrior();
	std::string getAnimation() const;
    bool isUrgent() const override;
private:
	std::shared_ptr<Warrior>  w;
	std::string animation;
    bool urgent;
};