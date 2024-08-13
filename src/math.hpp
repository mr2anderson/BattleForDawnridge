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


namespace bfdlib {
    namespace math {
        template<typename T> constexpr T pow(T base, uint32_t exponent) { // what the hell why should i implement fucking pow function by myself
            T result = 1;

            while (exponent) {
                if (exponent % 2 == 0) {
                    exponent = exponent / 2;
                    base = base * base;
                }
                else {
                    exponent = exponent - 1;
                    result = result * base;
                }
            }

            return result;
        }
        template<typename T> constexpr T subu(T a, T b) {
            if (b > a) {
                return 0;
            }
            return a - b;
        }
    }
}