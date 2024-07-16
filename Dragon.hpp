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
#include "Selectable.hpp"
#include "DragonFlame.hpp"
#include "Resources.hpp"


#pragma once


class Dragon : public Selectable {
public:
	Dragon();
	Dragon(uint32_t playerId);
	virtual Dragon* clone() const = 0;

	void resetRecoverMoves();
	void decreaseRecoverMovesLeft();
	uint32_t getRecoverMovesLeft() const;
	uint32_t getPlayerId() const;

	virtual std::wstring getDescriptionText() const = 0;
	virtual std::wstring getRaiseText() const = 0;
	virtual std::wstring getRestText() const = 0;
	virtual std::wstring getAttackText() const = 0;
	virtual std::string getTextureName() const = 0;
	virtual Resources getCost() const = 0;
	virtual uint32_t getRecoverTime() const = 0;
	virtual std::string getInfoSoundName() const = 0;
protected:
	virtual std::string getAttackSoundName() const = 0;
	virtual std::shared_ptr<DragonFlame> getFlameType() const = 0;
	virtual Events doAttack(uint32_t centerX, uint32_t centerY) = 0;
	std::shared_ptr<DragonFlame> getFlame(uint32_t centerX, uint32_t centerY) const;
private:
	uint32_t playerId;
	std::optional<uint32_t> recoverMovesLeft;

	Events unselect(uint32_t mouseX, uint32_t mouseY, uint8_t button) override;
	std::shared_ptr<sf::Drawable> getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const override;
};