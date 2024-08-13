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


#include <cstdint>
#include <boost/optional.hpp>


#pragma once


class Ports {
public:
    static Ports* get() {
        if (Ports::singletone == nullptr) {
            Ports::singletone = new Ports();
        }
        return Ports::singletone;
    }

    uint16_t getClientSendPort() const;
    uint16_t getClientReceivePort() const;

    uint16_t getLocalServerSendPort() const;
    uint16_t getLocalServerReceivePort() const;

    uint16_t getServerSendPort() const;
    uint16_t getServerReceivePort() const;

    void load();
private:
    Ports() = default;
    Ports(const Ports& copy);

    static Ports* singletone;
    
    boost::optional<uint16_t> clientSendPort;
    boost::optional<uint16_t> clientReceivePort;

    boost::optional<uint16_t> localServerSendPort;
    boost::optional<uint16_t> localServerReceivePort;

    boost::optional<uint16_t> serverSendPort;
    boost::optional<uint16_t> serverReceivePort;
};