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


#include "ISelectable.hpp"
#include "FocusOnEvent.hpp"
#include "UnselectEvent.hpp"
#include "EnableCursorEvent.hpp"


ISelectable::ISelectable() = default;
ISelectable::~ISelectable() = default;
Events ISelectable::unselect(MapState *state, uint32_t x, uint32_t y, uint8_t button) {
    Events events;
    events.add(std::make_shared<FocusOnEvent>(x, y, 1, 1));
    events.add(std::make_shared<UnselectEvent>());
    events.add(std::make_shared<EnableCursorEvent>());
    events = events + this->onUnselect(state, x, y, button);
    return events;
}
Events ISelectable::onUnselect(MapState* state, uint32_t x, uint32_t y, uint8_t button) {
    return {};
}
std::shared_ptr<sf::Drawable> ISelectable::getSelectablePointer(uint32_t mouseX, uint32_t mouseY) const {
    return nullptr;
}


BOOST_CLASS_EXPORT_IMPLEMENT(ISelectable)