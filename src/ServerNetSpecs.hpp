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


#pragma once


// Package format:
/*
 * room_id (sf::Uint64)
 * code (uint8_t)
 * additional_code_info
 */
namespace SERVER_NET_SPECS {
    namespace CODES { // enum is not used cuz it is implementation-defined
        static constexpr uint8_t OK = 0;
        static constexpr uint8_t WORLD_UI_STATE = 1;
        static constexpr uint8_t SOUND = 2;
        static constexpr uint8_t FOCUS = 3;
        static constexpr uint8_t RETURN_TO_MENU = 4;
        static constexpr uint8_t SAVE = 5;
    }
	namespace PORTS {
        static constexpr uint16_t SEND = 2089;
        static constexpr uint16_t RECEIVE = 2090;
    }
};