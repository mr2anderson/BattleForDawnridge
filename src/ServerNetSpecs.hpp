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
 * room_id (std::string)
 * code (uint8_t)
 * additional_code_info
 */
namespace SERVER_NET_SPECS {
    namespace CODES { // enum is not used cuz it is implementation-defined
        static constexpr uint8_t ERROR = 0;
        namespace ERROR_CODES {
            static constexpr uint8_t ROOM_ALREADY_EXIST = 0;
            static constexpr uint8_t INVALID_DATA = 1;
            static constexpr uint8_t FULL_ROOM = 2;
            static constexpr uint8_t UNKNOWN_ROOM_ID = 3;
            static constexpr uint8_t OTHER = 4;
        }
        static constexpr uint8_t WORLD_STATE = 1;
        static constexpr uint8_t SOUND = 2;
        static constexpr uint8_t FOCUS = 3;
        static constexpr uint8_t RETURN_TO_MENU = 4;
        static constexpr uint8_t NOT_YOUR_MOVE = 5;
    }
};