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


#include "EndTurnButtonSpec.hpp"
#include "ChangeMoveEvent.hpp"
#include "WindowTwoButtons.hpp"
#include "CreateEEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "StaticString.hpp"


EndTurnButtonSpec::EndTurnButtonSpec() = default;
EndTurnButtonSpec::EndTurnButtonSpec(uint32_t index) : MainScreenButtonSpec(index) {

}
std::shared_ptr<IDynamicString> EndTurnButtonSpec::getTextureName() const {
	return std::make_shared<StaticString>("new_turn_icon");
}
StringLcl EndTurnButtonSpec::getString() const {
	return StringLcl("{new_move}");
}
Events EndTurnButtonSpec::getEvents() const {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events endMoveEvent = clickEvent;
	endMoveEvent.add(std::make_shared<ChangeMoveEvent>());

	std::shared_ptr<WindowTwoButtons> confirmEndMoveWindow = std::make_shared<WindowTwoButtons>(StringLcl("{confirm_end_move}"), StringLcl("{yes}"), StringLcl("{no}"), endMoveEvent, clickEvent);
	Events createConfirmEndMoveWindowEvent = clickEvent;
	createConfirmEndMoveWindowEvent.add(std::make_shared<CreateEEvent>(confirmEndMoveWindow));

	return createConfirmEndMoveWindowEvent;
}