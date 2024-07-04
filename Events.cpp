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


#include "Events.hpp"


Events::Events() = default;
Events::Events(const GEvent& gEvent, const UIEvent& uiEvent) {
	this->gEvent = gEvent;
	this->uiEvent = uiEvent;
}
Events operator +(const Events& a, const Events& b) {
	Events c = a;

	c.gEvent = c.gEvent + b.gEvent;
	c.uiEvent = c.uiEvent + b.uiEvent;

	return c;
}