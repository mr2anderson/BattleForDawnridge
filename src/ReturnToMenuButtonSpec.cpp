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


#include "ReturnToMenuButtonSpec.hpp"
#include "WindowTwoButtons.hpp"
#include "ReturnToMenuEvent.hpp"
#include "CreateEEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "StaticString.hpp"


ReturnToMenuButtonSpec::ReturnToMenuButtonSpec() = default;
ReturnToMenuButtonSpec::ReturnToMenuButtonSpec(uint32_t index) : MainScreenButtonSpec(index) {

}
std::shared_ptr<IDynamicString> ReturnToMenuButtonSpec::getTextureName() const {
	return std::make_shared<StaticString>("to_menu_icon");
}
StringLcl ReturnToMenuButtonSpec::getString() const {
	return StringLcl("{to_menu}");
}
Events ReturnToMenuButtonSpec::getEvents() const {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events returnToMenuEvent = clickEvent;
	returnToMenuEvent.add(std::make_shared<ReturnToMenuEvent>());

	std::shared_ptr<WindowTwoButtons> confirmReturnToMenuWindow = std::make_shared<WindowTwoButtons>(StringLcl("{confirm_return_to_menu}"), StringLcl("{yes}"), StringLcl("{no}"), returnToMenuEvent, clickEvent);

	Events createConfirmReturnToMenuWindowEvent = clickEvent;
	createConfirmReturnToMenuWindowEvent.add(std::make_shared<CreateEEvent>(confirmReturnToMenuWindow));

	return createConfirmReturnToMenuWindowEvent;
}