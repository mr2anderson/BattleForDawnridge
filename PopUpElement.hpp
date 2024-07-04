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


#include <SFML/Graphics.hpp>
#include "Events.hpp"


#pragma once


class PopUpElement : public sf::Drawable {
public:
	PopUpElement();

	void addOnStartGEvent(const GEvent& event);
	bool finished() const;
	virtual void update() = 0;

	virtual Events run(uint32_t windowW, uint32_t windowH);
	virtual Events click(uint32_t x, uint32_t y) = 0;
protected:
	void finish();
private:
	GEvent onStart;
	bool _finished;
};