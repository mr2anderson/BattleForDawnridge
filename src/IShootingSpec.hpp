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


#include "IAreaControllerSpec.hpp"
#include "Projectile.hpp"
#include "Damage.hpp"


#pragma once


class IShootingSpec : public IAreaControllerSpec {
public:
    Events getActiveNewMoveEvent(std::shared_ptr<const Building> b, MapState* state) override;
    std::vector<BuildingHorizontalSelectionWindowComponent> getComponents(std::shared_ptr<const Building> b, MapState* state) override;
    uint32_t getRadius() const override;
    sf::Color getHighlightColor(uint32_t playerId) const override;
    uint8_t getHighlightType() const override;
    bool ignoreLowObstacles() const override;
    bool ignoreHighObstacles() const override;
    NewMoveMainPriority getNewMoveMainPriority() const override;
    virtual Damage getDamage() const = 0;
    virtual uint32_t getShotsNumber() const = 0;
    virtual uint32_t getShootingRadius() const = 0;
    virtual std::shared_ptr<Projectile> getProjectile() const = 0;
private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<IAreaControllerSpec>(*this);
    }
};


BOOST_CLASS_EXPORT_KEY(IShootingSpec)