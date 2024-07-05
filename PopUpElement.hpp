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
#include "Event.hpp"


#pragma once


class PopUpElement : public sf::Drawable {
public:
	PopUpElement();
	virtual ~PopUpElement();

	void addOnStartGEvent(const Event& event);
	bool finished() const;
	virtual void update() = 0;

	virtual Event run(uint32_t windowW, uint32_t windowH);
	virtual Event click() = 0;
protected:
	void finish();
private:
	Event onStart;
	bool _finished;
};