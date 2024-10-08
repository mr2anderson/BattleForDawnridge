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


#include <boost/optional.hpp>
#include <boost/serialization/optional.hpp>
#include "GO.hpp"


#pragma once


class HPGO : public GO {
public:
	HPGO();
	HPGO(uint32_t x, uint32_t y, boost::optional<uint32_t> currentHp);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void addHp(uint32_t n);
	void subHp(uint32_t n);
	uint32_t getHP() const;
	void setMaxHp();
	bool exist() const override;
    virtual uint32_t getMaxHP() const = 0;
private:
	boost::optional<uint32_t> currentHp;

    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<GO>(*this);
        ar & this->currentHp;
    }
};


BOOST_CLASS_EXPORT_KEY(HPGO)