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


#include "GO.hpp"


#pragma once


class HPGO : public GO {
public:
	HPGO();
	HPGO(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t currentHp, uint32_t maxHp);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void addHp(uint32_t n);
	void subHp(uint32_t n);
	uint32_t getHP() const;
	uint32_t getMaxHP() const;
	void changeCurrentHp(uint32_t newCurrentHp);
	void changeMaxHp(uint32_t newMaxHp);
	void setMaxHp();
	std::wstring getReadableHpInfo() const;
	bool exist() const override;
private:
	uint32_t currentHp, maxHp;

	void drawHpBar(sf::RenderTarget& target, sf::RenderStates states) const;
};