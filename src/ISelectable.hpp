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
#include <SFML/Graphics.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include "MapState.hpp"
#include "Events.hpp"
#include "ArchiveType.hpp"


#pragma once


class ISelectable : public std::enable_shared_from_this<ISelectable> {
public:
    ISelectable(); // For boost serialization
	virtual ~ISelectable();

	Events unselect(MapState* state, uint32_t x, uint32_t y, uint8_t button);
    virtual Events onUnselect(MapState* state, uint32_t x, uint32_t y, uint8_t button);
    virtual std::shared_ptr<sf::Drawable> getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const;
protected:
    template<typename T> std::shared_ptr<T> getThis() {
        return std::static_pointer_cast<T>(this->shared_from_this());
    }
    template<typename T> std::shared_ptr<const T> getThis() const {
        return std::static_pointer_cast<T>(this->shared_from_this());
    }
private:
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive& ar, const unsigned int version) {
        
    }
};


BOOST_CLASS_EXPORT_KEY(ISelectable)