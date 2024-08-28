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


#include "IEntrySettings.hpp"


#pragma once


class EntryNumSettings : public IEntrySettings {
public:
	EntryNumSettings();

	void setMax(uint64_t max);

    bool ok(std::shared_ptr<std::string> dstPtr) const override;
	void mod(std::shared_ptr<std::string> dstPtr, sf::Keyboard::Key key) override;
private:
	uint64_t max;
};