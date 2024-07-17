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


#include <optional>
#include "GO.hpp"


#pragma once


class HPGO : public GO {
public:
	HPGO();
	HPGO(uint32_t x, uint32_t y, std::optional<uint32_t> currentHp);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void addHp(uint32_t n);
	void subHp(uint32_t n);
	uint32_t getHP() const;
	void setMaxHp();
	bool exist() const override;
    virtual uint32_t getMaxHP() const = 0;
protected:
	virtual uint8_t getHPPointerOrientation() const = 0;
private:
	std::optional<uint32_t> currentHp;

	void drawHpBar(sf::RenderTarget& target, sf::RenderStates states) const;
};