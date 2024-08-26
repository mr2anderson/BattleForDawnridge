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


#include "ClueManager.hpp"
#include "Locales.hpp"
#include "GlobalRandomGenerator32.hpp"


const uint32_t ClueManager::TOTAL_CLUES = 36;


const uint32_t ClueManager::W = 475;
const uint32_t ClueManager::H = 80;


ClueManager::ClueManager() {
	this->currentClue = GlobalRandomGenerator32::get().gen() % TOTAL_CLUES;
}
Label ClueManager::getClueLabel(uint32_t windowW, uint32_t windowH) {
	return Label(windowW - W - 20, windowH - H - 20, W, H, StringLcl("{clue" + std::to_string(this->currentClue) + "}"));
}
void ClueManager::nextClue() {
	this->currentClue = (this->currentClue + 1) % TOTAL_CLUES;
}