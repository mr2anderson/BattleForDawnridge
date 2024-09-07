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


static constexpr uint32_t MALE_NAMES = 21;
static constexpr uint32_t MALE_SURNAMES = 21;
static constexpr uint32_t FEMALE_NAMES = 16;
static constexpr uint32_t FEMALE_SURNAMES = 21;


Name::Name() {
    this->seed = GlobalRandomGenerator32::get().gen();
}
StringLcl Name::toString(Gender gender) const {
    uint32_t name = (this->seed >> 24) & 0xFF;
    uint32_t surName = (this->seed >> 16) & 0xFF;

    switch (gender) {
        case Gender::Male:
            return StringLcl::BOLD() + StringLcl("{name_male_" + std::to_string(name % MALE_NAMES) + "} ") + StringLcl::BOLD() + StringLcl("{surname_male_" + std::to_string(surName % MALE_SURNAMES) + "} ");
        case Gender::Female:
            return StringLcl::BOLD() + StringLcl("{name_female_" + std::to_string(name % FEMALE_NAMES) + "} ") + StringLcl::BOLD() + StringLcl("{surname_female_" + std::to_string(surName % FEMALE_SURNAMES) + "} ");
        default:
            return {};
    }
}