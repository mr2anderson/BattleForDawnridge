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


#pragma once


class Defence {
public:
	Defence(double cut, double stab, double crush);

	double getCut() const;
	double getStab() const;
	double getCrush() const;
	std::wstring getReadable() const;
private:
	double cut, stab, crush;
};



namespace DEFENCE {
	static Defence HUMAN = Defence(1, 1, 0.01);
	static Defence WOOD = Defence(1, 0.25, 1);
	static Defence STONE = Defence(0.1, 0.1, 1);
}