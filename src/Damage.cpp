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


#include "Damage.hpp"
#include "Locales.hpp"
#include "ColorTheme.hpp"


Damage::Damage() = default;
Damage::Damage(uint32_t points, uint8_t type, uint8_t spec) {
	this->points = points;
	this->type = type;
    this->spec = spec;
}
Damage operator*(double k, Damage damage) {
	return Damage(damage.points * k, damage.type, damage.spec);
}
uint32_t Damage::getHpLoss(Defence defence) const {
	double k;
	switch (this->type) {
			case TYPE::CUT: {
				k = defence.getCut();
				break;
			}	
			case TYPE::CRUSH: {
				k = defence.getCrush();
				break;
			}
			case TYPE::STAB: {
				k = defence.getStab();
				break;
			}
            case TYPE::SERVICE:
                k = 1;
                break;
	}
	return k * this->points;
}
uint32_t Damage::getPoints() const {
	return this->points;
}
StringLcl Damage::getReadable() const {
	StringLcl result = this->getTypeReadable() + " ";
	result = result + StringLcl::COLOR(COLOR_THEME::STATE_COLOR_NEUTRAL) + StringLcl(std::to_string(this->points));
	return result;
}
StringLcl Damage::getTypeReadable() const {
	StringLcl result;
	switch (this->type) {
	case TYPE::CUT: {
		result = result + StringLcl("{cut}");
		break;
	}
	case TYPE::CRUSH: {
		result = result + StringLcl("{crush}");
		break;
	}
	case TYPE::STAB: {
		result = result + StringLcl("{stab}");
		break;
	}
	}

    if (this->hasSpec(SPEC::POISON)) {
        result = result + " {poison}";
    }

	return result;
}
bool Damage::hasSpec(uint8_t val) const {
    return (this->spec & val);
}