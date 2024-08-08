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
#include <optional>


#pragma once


class PublicIP {
public:
    static PublicIP* get() {
        if (PublicIP::singletone == nullptr) {
            PublicIP::singletone = new PublicIP();
        }
        return PublicIP::singletone;
    }

    void load();
    sf::IpAddress getIp() const;
private:
    PublicIP() = default;
    PublicIP(const PublicIP& copy);
    static PublicIP* singletone;

    std::optional<sf::IpAddress> ip;
};