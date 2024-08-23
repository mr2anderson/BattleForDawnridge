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


#include <memory>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include "Map.hpp"
#include "PopUpElement.hpp"
#include "HighlightTable.hpp"
#include "ISelectable.hpp"
#include "ResourceBar.hpp"
#include "RectangularUiElement.hpp"
#include "ArchiveType.hpp"


#pragma once


struct WorldState {
	WorldState();
	WorldState(const std::string& mapName);

	Map map;
	std::vector<bool> playerIsActive;
	uint32_t currentPlayerId;
	std::shared_ptr<PopUpElement> element;
	uint32_t move;
	HighlightTable highlightTable;
	ISelectable* selected;
	bool curcorVisibility;
	ResourceBar resourceBar;
	std::vector<std::shared_ptr<const RectangularUiElement>> buttonBases;

	template<class Archive> void serialize(Archive& ar, const unsigned int version) {
		ar& this->map;
		ar& this->playerIsActive;
		ar& this->currentPlayerId;
		ar& this->element;
		ar& this->move;
		ar& this->highlightTable;
		ar& this->selected;
		ar& this->curcorVisibility;
		ar& this->resourceBar;
		ar& this->buttonBases;
	}
};