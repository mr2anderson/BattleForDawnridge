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



#include "Label.hpp"


#pragma once


class Button : public Label {
public:
	Button();
	Button(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::wstring& message, uint32_t charSize);
	Button(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const std::string &picture, const std::wstring& message, uint32_t charSize);
	bool click(uint32_t x, uint32_t y) const;
};