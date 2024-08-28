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


#include "RectangularUiElement.hpp"
#include "StringLcl.hpp"


#pragma once


class Label : public RectangularUiElement { // This class does not let to use different char sizes and colors in order to have same ui style
public:
	Label();
	Label(int32_t x, int32_t y, uint32_t w, uint32_t h, const StringLcl &str, bool center = true, bool colorDigits = true);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	StringLcl message;
	bool center;
	bool colorDigits;

	friend class boost::serialization::access;
	template<class Archive> void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<RectangularUiElement>(*this);
		ar& this->message;
		ar& this->center;
		ar& this->colorDigits;
	}
};


BOOST_CLASS_EXPORT_KEY(Label)