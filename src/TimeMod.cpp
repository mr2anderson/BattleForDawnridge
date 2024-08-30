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


#include "TimeMod.hpp"


TimeMod::TimeMod(TimeMod::Type type) {
    this->type = type;
}
int32_t TimeMod::getPercentDelta(bool positive) const {
    if (this->type == Type::NeutralTime) {
        return 0;
    }
    if (this->type == Type::PositiveTime) {
        if (positive) {
            return 25;
        }
        return -25;
    }
    if (this->type == Type::NegativeTime) {
        if (positive) {
            return -25;
        }
        return 25;
    }
    return 0;
}