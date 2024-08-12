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


#include "Collection.hpp"
#include "AreaResourcePoint.hpp"
#include "ConductionResourcePoint.hpp"
#include "Unit.hpp"
#include "Building.hpp"
#include "Warrior.hpp"


#pragma once


struct CollectionsImpl {
	Collection<GO> gos;
	Collection<AreaResourcePoint> areaRps;
	Collection<ConductionResourcePoint> conductionRps;
	Collection<Unit> units;
	Collection<Building> buildings;
	Collection<Warrior> warriors;
};