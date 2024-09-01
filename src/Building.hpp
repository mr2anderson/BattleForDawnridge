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


#include <boost/serialization/vector.hpp>
#include "Unit.hpp"
#include "Fire.hpp"
#include "IBuildingSpec.hpp"


#pragma once


class Building : public Unit {
public:
	Building();
	Building(uint32_t x, uint32_t y, uint32_t playerId);
	Building(const Building& building);
	virtual std::shared_ptr<Building>  createSameTypeBuilding() const = 0;

	bool hasError(MapSize mapSize, uint32_t totalPlayers) const override {
		if (this->Unit::hasError(mapSize, totalPlayers)) {
			return true;
		}
		for (auto spec : this->specs) {
			if (spec == nullptr or spec->hasError(mapSize, totalPlayers)) {
				return true;
			}
		}
		return false;
	}

	Events getHighlightEvent(MapState *state, uint8_t type) const;
	Resources getLimit() const;
	uint32_t getPopulationLimit() const;
	bool isVictoryCondition() const;
	bool isOrigin() const;
	bool isActiveConductor() const;
	bool works() const;
    bool wasWithFullHP() const;
    NewMoveMainPriority getNewMoveMainPriority() const override;
    void addHp(uint32_t delta) override;
	bool connectedTo(MapState* state, std::shared_ptr<GO> go) const;
	bool connectedToOrigin(MapState* state) const;
	Events destroy(MapState *state);
	void decreaseBurningMovesLeft();
	void setFire();
protected:
	void addSpec(std::shared_ptr<IBuildingSpec> spec);
	virtual uint32_t getRegenerationSpeed() const = 0;
private:
    bool _wasWithFullHP;
	uint32_t burningMovesLeft;
	std::vector<std::shared_ptr<IBuildingSpec>> specs;

	bool isUltraHighObstacle(uint32_t playerId) const override;
	bool isHighObstacle(uint32_t playerId) const override;
	bool isLowObstacle(uint32_t playerId) const override;
	HorizontalSelectionWindowComponent getHpInfoComponent() const;
	HorizontalSelectionWindowComponent getDestroyComponent();
    HorizontalSelectionWindowComponent getBuildingOfEnemyComponent();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void drawShortInfos(sf::RenderTarget& target, sf::RenderStates states) const;
	Events hit(Damage d) override;
	Events processRegeneration();
    Events newMove(MapState* state, uint32_t playerId) override;
    Events getResponse(MapState *state, uint32_t playerId, uint32_t button) override;
	sf::Color getTextureColor() const override;
	bool warriorCanStay(std::shared_ptr<const Warrior> w) const override;
	uint32_t getWarriorMovementCost(std::shared_ptr<Warrior> w) const override;
    std::shared_ptr<PlayerPointer> getPlayerPointer() const override;
	std::shared_ptr<ILightSource> getLightSource() const override;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version) {
        ar & boost::serialization::base_object<Unit>(*this);
        ar & this->_wasWithFullHP;
        ar & this->burningMovesLeft;
        ar & this->specs;
    }
};


BOOST_CLASS_EXPORT_KEY(Building)