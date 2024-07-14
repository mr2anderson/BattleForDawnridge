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


#include "AreaB.hpp"


#pragma once


class AreaBLandscapeSensible : public AreaB {
public:
    AreaBLandscapeSensible();
    AreaBLandscapeSensible(uint32_t x, uint32_t y, uint32_t playerId, std::shared_ptr<GOCollection<Unit>> units, std::shared_ptr<GOCollection<GO>> go, uint32_t mapW, uint32_t mapH);

    bool inRadius(uint32_t x, uint32_t y, uint32_t sx, uint32_t sy) override;
    Events getHighlightEvent() override;
protected:
    std::shared_ptr<GOCollection<GO>> getGO();
    uint32_t getMapW() const;
    uint32_t getMapH() const;
    std::map<std::tuple<uint32_t, uint32_t>, uint32_t> getAvailable();

    virtual bool ignoreUltraHighObstacles() const = 0;
    virtual bool ignoreHighObstacles() const = 0;
    virtual bool ignoreLowObstacles() const = 0;
private:
    std::shared_ptr<GOCollection<GO>> go;
    uint32_t mapW, mapH;
    uint64_t prevHash;
    std::map<std::tuple<uint32_t, uint32_t>, uint32_t> prevAvailable;

    uint64_t getHash() const;
};