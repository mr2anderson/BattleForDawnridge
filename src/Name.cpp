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


#include "Name.hpp"
#include "GlobalRandomGenerator32.hpp"
#include "Locales.hpp"
#include "ColorTheme.hpp"


static constexpr uint32_t TOTAL = 31;


Name::Name() {
    this->a = GlobalRandomGenerator32::get().gen() % TOTAL;
    this->b = GlobalRandomGenerator32::get().gen() % TOTAL;
}
StringLcl Name::toString() const {
    StringLcl syl1 = "{syl" + std::to_string(a) + "}";
    StringLcl syl2 = "{syl" + std::to_string(b) + "}";

    return StringLcl::BOLD() + syl1 + "-" + syl2;
}