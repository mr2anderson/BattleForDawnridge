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


#include "HPGO.hpp"
#include "SelectionW.hpp"


#pragma once


class ResourcePoint : public HPGO {
public:
	ResourcePoint();
	ResourcePoint(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t size);

	Event newMove(const Player& player) override;
	virtual std::string getResourceType() const = 0;
protected:
	virtual std::wstring getDescription() const = 0;
private:
	Event getGameObjectResponse(const Player& player) override;
	Event getSelectionWindow();
};