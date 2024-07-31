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


#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include "HorizontalSelectionWindow.hpp"
#include "MapState.hpp"
#include "ArchiveType.hpp"


#pragma once


class Warrior;


class GO : public sf::Drawable {
public:
	GO();
	GO(uint32_t x, uint32_t y);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    float getXInPixels() const;
    float getYInPixels() const;
	uint32_t getX() const;
	uint32_t getY() const;
	virtual uint32_t getSX() const = 0;
	virtual uint32_t getSY() const = 0;
	void setX(uint32_t newX);
	void setY(uint32_t newY);
	virtual bool warriorCanStay(const Warrior *w) const;
	virtual uint32_t getWarriorMovementCost(const Warrior *w) const;
    virtual bool isUltraHighObstacle(uint32_t playerId) const;
    virtual bool isHighObstacle(uint32_t playerId) const;
    virtual bool isLowObstacle(uint32_t playerId) const;
	virtual bool exist() const;
	Events click(MapState *state, uint32_t currentPlayerId, uint8_t button, uint32_t mouseX, uint32_t mouseY);
    virtual void newFrame(MapState *state, uint32_t playerId);
	bool intersects(GO* go) const;

	virtual Events newMove(MapState *state, uint32_t currentPlayerId);
	virtual uint8_t getDrawingPriority() const;
	virtual uint8_t getClickPriority() const;
	virtual std::string getTextureName() const = 0;
	virtual sf::IntRect getTextureRect() const;
	virtual sf::Color getTextureColor() const;
	virtual std::string getSoundName() const = 0;
	virtual std::wstring getDescription() const = 0;

	enum PRIORITY {
		LOW,
		DEFAULT,
		HIGH,
		HIGHEST
	};
protected:
	virtual Events getResponse(MapState *state, uint32_t currentPlayerId, uint32_t button) = 0;
    virtual float getOffsetX() const;
    virtual float getOffsetY() const;

	HorizontalSelectionWindowComponent getExitComponent() const;
	HorizontalSelectionWindowComponent getDescriptionComponent() const;
private:
	uint32_t x, y;

	void drawTexture(sf::RenderTarget& target, sf::RenderStates states) const;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->x;
        ar & this->y;
    }
};


BOOST_CLASS_EXPORT_KEY(GO)