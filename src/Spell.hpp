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


#include <boost/optional.hpp>
#include <boost/serialization/base_object.hpp>
#include "ISelectable.hpp"
#include "Resources.hpp"


#pragma once


class Building;


class Spell : public ISelectable {
public:
    Spell();
	Spell(uint32_t playerId);
	virtual Spell* clone() const = 0;

	bool isReady() const;
	uint32_t getCreationMovesLeft() const;
	Events newMove(const Building* father);
	void decreaseCreationMovesLeft();

	Events use();
	bool wasUsed() const;
	void markAsUsed();
	virtual StringLcl getDescription() const = 0;
	virtual Resources getCost() const = 0;
	virtual uint32_t getCreationTime() const = 0;
	virtual std::string getTextureName() const = 0;
protected:
	uint32_t getPlayerId() const;
	virtual sf::Color getPreviewColor() const = 0;
	virtual uint32_t getRadius() const = 0;
	virtual std::string getSoundName() const = 0;
	virtual Events changeMap(MapState* state, uint32_t centerX, uint32_t centerY) = 0;
private:
	boost::optional<uint32_t> creationMovesLeft;
	bool used;
	uint32_t playerId;

	Events unselect(MapState* state, uint32_t x, uint32_t y, uint8_t button) override;
	std::shared_ptr<sf::Drawable> getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const override;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->creationMovesLeft;
        ar & this->used;
        ar & this->playerId;
    }
};


BOOST_CLASS_EXPORT_KEY(Spell)