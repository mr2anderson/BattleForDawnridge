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
#include "ResourcePoint.hpp"
#include "Forest.hpp"
#include "Stone.hpp"
#include "Iron.hpp"
#include "Mountains.hpp"
#include "Unit.hpp"
#include "Castle.hpp"
#include "TerritoryB.hpp"
#include "TerritoryConductorB.hpp"
#include "TerritoryOriginB.hpp"
#include "CouldntOpenMap.hpp"
#include "GOCollection.hpp"
#include "Water.hpp"


#pragma once


class Map : public sf::Drawable {
public:
    Map(const std::string &path);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    uint32_t getW() const;
    uint32_t getH() const;
    uint32_t getPlayersNumber() const;
    std::shared_ptr<Player> getPlayer(uint32_t i);
    std::shared_ptr<GOCollection<GO>> getGO();
    std::shared_ptr<GOCollection<ResourcePoint>> getResourcePoints();
    std::shared_ptr<GOCollection<Unit>> getUnits();
    std::shared_ptr<GOCollection<TerritoryB>> getTbs();
    std::shared_ptr<GOCollection<TerritoryConductorB>> getTcbs();
    std::shared_ptr<GOCollection<TerritoryOriginB>> getTobs();
    void add(GO* object);
private:
    uint32_t w, h;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<GOCollection<GO>> go;
    std::shared_ptr<GOCollection<ResourcePoint>> resourcePoints;
    std::shared_ptr<GOCollection<Unit>> units;
    std::shared_ptr<GOCollection<TerritoryB>> tbs;
    std::shared_ptr<GOCollection<TerritoryConductorB>> tcbs;
    std::shared_ptr<GOCollection<TerritoryOriginB>> tobs;
};