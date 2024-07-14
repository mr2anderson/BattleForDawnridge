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


#include <ctime>
#include "ClueManager.hpp"
#include "Texts.hpp"


ClueManager* ClueManager::singletone = nullptr;


const uint32_t ClueManager::TOTAL_CLUES = 17;


const uint32_t ClueManager::W = 475;
const uint32_t ClueManager::H = 80;


ClueManager::ClueManager() {
	this->currentClue = time(nullptr) % TOTAL_CLUES;
	this->regenerateLabel();
}
Label ClueManager::getClueLabel(uint32_t windowW, uint32_t windowH) {
	label.setX(windowW - label.getW() - 20);
	label.setY(windowH - label.getH() - 20);

	return label;
}
void ClueManager::nextClue() {
	this->currentClue = (this->currentClue + 1) % TOTAL_CLUES;
	this->regenerateLabel();
}
void ClueManager::regenerateLabel() {
	this->label = Label(0, 0, W, H, *Texts::get()->get("clue" + std::to_string(this->currentClue)));
}