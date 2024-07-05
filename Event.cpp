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



#include "Event.hpp"


Event::Event() = default;
Event operator+(const Event &a, const Event& b) {
	Event c = a;

	c.tryToAttack.insert(c.tryToAttack.end(), b.tryToAttack.begin(), b.tryToAttack.end());
	c.tryToTrade.insert(c.tryToTrade.end(), b.tryToTrade.begin(), b.tryToTrade.end());
	c.subResource.insert(c.subResource.end(), b.subResource.begin(), b.subResource.end());
	c.addResource.insert(c.addResource.end(), b.addResource.begin(), b.addResource.end());
	c.subResources.insert(c.subResources.end(), b.subResources.begin(), b.subResources.end());
	c.addResources.insert(c.addResources.end(), b.addResources.begin(), b.addResources.end());
	c.changeHighlight.insert(c.changeHighlight.end(), b.changeHighlight.begin(), b.changeHighlight.end());
	c.collect.insert(c.collect.end(), b.collect.begin(), b.collect.end());
	c.tryToUpgrade.insert(c.tryToUpgrade.end(), b.tryToUpgrade.begin(), b.tryToUpgrade.end());
	c.addHp.insert(c.addHp.end(), b.addHp.begin(), b.addHp.end());
	c.decreaseUpgradeMovesLeft.insert(c.decreaseUpgradeMovesLeft.end(), b.decreaseUpgradeMovesLeft.begin(), b.decreaseUpgradeMovesLeft.end());
	c.increaseLevel.insert(c.increaseLevel.end(), b.increaseLevel.begin(), b.increaseLevel.end());
	c.decreaseCurrentTradeMovesLeft.insert(c.decreaseCurrentTradeMovesLeft.end(), b.decreaseCurrentTradeMovesLeft.begin(), b.decreaseCurrentTradeMovesLeft.end());
	c.tryToBuild.insert(c.tryToBuild.end(), b.tryToBuild.begin(), b.tryToBuild.end());
	c.build.insert(c.build.end(), b.build.begin(), b.build.end());
	c.playSound.insert(c.playSound.end(), b.playSound.begin(), b.playSound.end());
	c.createE.insert(c.createE.end(), b.createE.begin(), b.createE.end());

	return c;
}