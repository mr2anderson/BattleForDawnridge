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


#include "PopUpWindow.hpp"


PopUpWindow::PopUpWindow(uint32_t windowW, uint32_t windowH) {
	this->windowW = windowW;
	this->windowH = windowH;
}
uint32_t PopUpWindow::getWindowW() const {
	return this->windowW;
}
uint32_t PopUpWindow::getWindowH() const {
	return this->windowH;
}