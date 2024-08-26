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


#include <boost/serialization/shared_ptr.hpp>
#include "IAreaControllerSpec.hpp"


#pragma once


class WarriorProducerSpec : public IAreaControllerSpec {
public:
	WarriorProducerSpec();

	bool hasError(MapSize mapSize, uint32_t totalPlayers) const override {
		if (this->IAreaControllerSpec::hasError(mapSize, totalPlayers)) {
			return true;
		}
		return !this->currentProducingOK(mapSize, totalPlayers);
	}

	Events startProducing(std::shared_ptr<Warrior>  w);
	void decreaseCurrentProducingMovesLeft();
	void stopProducing();
	Events getActiveNewMoveEvent(std::shared_ptr<const Building> b, MapState* state) override;
	std::vector<BuildingHorizontalSelectionWindowComponent> getComponents(std::shared_ptr<const Building> b, MapState* state) override;
	boost::optional<BuildingShortInfo> getShortInfo(std::shared_ptr<const Building> b) const override;
	uint32_t getRadius() const override;
    sf::Color getHighlightColor(uint32_t playerId) const override;
    uint8_t getHighlightType() const override;
	virtual std::vector<std::shared_ptr<Warrior> > getWarriorsToProduce(uint32_t playerId) = 0;
    virtual std::string getProducingIconName() const = 0;
    virtual std::string getWaitingIconName() const = 0;
private:
	std::shared_ptr<Warrior>  currentProducing;
	uint32_t currentProducingMovesLeft;
	bool producing;

	std::tuple<uint32_t, uint32_t> getNewWarriorPosition(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy, uint32_t playerId, MapState* state);

	bool currentProducingOK(MapSize mapSize, uint32_t totalPlayers) const;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<IAreaControllerSpec>(*this);
        ar & this->currentProducing;
        ar & this->currentProducingMovesLeft;
        ar & this->producing;
    }
};


BOOST_CLASS_EXPORT_KEY(WarriorProducerSpec)