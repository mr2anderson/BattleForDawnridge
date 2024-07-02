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



#include "GameEvent.hpp"


GameEvent::GameEvent() = default;
GameEvent operator+(const GameEvent &a, const GameEvent& b) {
	GameEvent c = a;
	c.tryToAttack.insert(c.tryToAttack.end(), b.tryToAttack.begin(), b.tryToAttack.end());
	c.tryToTrade.insert(c.tryToTrade.end(), b.tryToTrade.begin(), b.tryToTrade.end());
	c.startTrade.insert(c.startTrade.end(), b.startTrade.begin(), b.startTrade.end());
	c.finishTrade.insert(c.finishTrade.end(), b.finishTrade.begin(), b.finishTrade.end());
	c.changeHighlight.insert(c.changeHighlight.end(), b.changeHighlight.begin(), b.changeHighlight.end());
	c.collect.insert(c.collect.end(), b.collect.begin(), b.collect.end());
	return c;
}