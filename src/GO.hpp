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
#include "DrawingPriority.hpp"
#include "ClickPriority.hpp"
#include "NewMovePriority.hpp"
#include "MapState.hpp"
#include "IWithLightSource.hpp"
#include "UUID.hpp"
#include "ArchiveType.hpp"
#include "Filter.hpp"


#pragma once


class Warrior;


class GO : public sf::Drawable, public IWithLightSource {
public:
	GO();
	GO(uint32_t x, uint32_t y);
    virtual UUID getTypeUUID() const = 0;

    virtual bool hasError(MapSize mapSize, uint32_t totalPlayers) const {
        // Avoid checking uuid cuz server does not receive uuids from clients
        return
                (this->x + this->getSX() > mapSize.getWidth() or
                this->y + this->getSY() > mapSize.getHeight());
    }

    bool inView(const sf::View& view) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    float getXInPixels() const;
    float getYInPixels() const;
    float getCenterX() const;
    float getCenterY() const;
	uint32_t getX() const;
	uint32_t getY() const;
	virtual uint32_t getSX() const = 0;
	virtual uint32_t getSY() const = 0;
    UUID getUUID() const;
	void setX(uint32_t newX);
	void setY(uint32_t newY);
	virtual bool warriorCanStay(const Warrior *w) const;
	virtual uint32_t getWarriorMovementCost(const Warrior *w) const;
    virtual bool isUltraHighObstacle(uint32_t playerId) const;
    virtual bool isHighObstacle(uint32_t playerId) const;
    virtual bool isLowObstacle(uint32_t playerId) const;
	virtual bool exist() const;
	Events click(MapState *state, uint32_t currentPlayerId, uint8_t button, uint32_t mouseX, uint32_t mouseY);
    virtual void update(MapState *state, uint32_t playerId);
	bool intersects(GO* go) const;

	virtual Events newMove(MapState *state, uint32_t currentPlayerId);
	virtual DrawingPriority getDrawingPriority() const;
	virtual ClickPriority getClickPriority() const;
    virtual NewMoveMainPriority getNewMoveMainPriority() const;
    NewMovePriority getNewMovePriority() const;
	virtual sf::IntRect getTextureRect() const;
	virtual sf::Color getTextureColor() const;
    virtual std::string getTextureName() const = 0;
	virtual std::string getSoundName() const = 0;
	virtual StringLcl getDescription() const = 0;

    static const DrawingPriority DRAWING_PRIORITY_WARRIOR_FLYING;
    static const DrawingPriority DRAWING_PRIORITY_WARRIOR_DEFAULT;
    static const DrawingPriority DRAWING_PRIORITY_WARRIOR_VEHICLE;
    static const DrawingPriority DRAWING_PRIORITY_LOWEST;

    static const ClickPriority CLICK_PRIORITY_WARRIOR;
    static const ClickPriority CLICK_PRIORITY_LOWEST;

    static const NewMoveMainPriority NEW_MOVE_MAIN_PRIORITY_VICTORY_CONDITION;
    static const NewMoveMainPriority NEW_MOVE_MAIN_PRIORITY_WARRIOR;
    static const NewMoveMainPriority NEW_MOVE_MAIN_PRIORITY_SHOOTING;
    static const NewMoveMainPriority NEW_MOVE_MAIN_PRIORITY_TERRITORY_CONDUCTOR;
    static const NewMoveMainPriority NEW_MOVE_MAIN_PRIORITY_HOUSE;
    static const NewMoveMainPriority NEW_MOVE_MAIN_PRIORITY_WAREHOUSE;
    static const NewMoveMainPriority NEW_MOVE_MAIN_PRIORITY_LOWEST;
protected:
	virtual Events getResponse(MapState *state, uint32_t currentPlayerId, uint32_t button) = 0;
    virtual float getOffsetX() const;
    virtual float getOffsetY() const;

	HorizontalSelectionWindowComponent getExitComponent() const;
	HorizontalSelectionWindowComponent getDescriptionComponent() const;
private:
	uint32_t x, y;
    UUID uuid;

	void drawTexture(sf::RenderTarget& target, sf::RenderStates states) const;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & this->x;
        ar & this->y;
    }
};


BOOST_CLASS_EXPORT_KEY(GO)