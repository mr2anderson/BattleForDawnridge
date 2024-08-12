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


Damage::Damage() = default;
Damage::Damage(uint32_t points, uint8_t type) {
	this->points = points;
	this->type = type;
}
Damage operator*(double k, Damage damage) {
	return Damage(damage.points * k, damage.type);
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
	}
	return k * this->points;
}
StringLcl Damage::getReadable() const {
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
	result = result + StringLcl(std::to_string(this->points));
	return result;
}