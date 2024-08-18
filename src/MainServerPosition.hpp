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


#include <SFML/Network.hpp>
#include <boost/optional.hpp>
#include <cstdint>


#pragma once


class MainServerPosition {
public:
    static MainServerPosition* get() {
        if (MainServerPosition::singletone == nullptr) {
            MainServerPosition::singletone = new MainServerPosition();
        }
        return MainServerPosition::singletone;
    }

    void load();
    sf::IpAddress getIP() const;
    uint16_t getPort() const;
private:
    MainServerPosition() = default;
    MainServerPosition(const MainServerPosition& copy);
    static MainServerPosition* singletone;

    boost::optional<sf::IpAddress> ip;
    boost::optional<uint16_t> port;
};