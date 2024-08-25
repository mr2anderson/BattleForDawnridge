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


#include "SaveGameButtonSpec.hpp"
#include "SaveGameEvent.hpp"
#include "CreateEEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "WindowButton.hpp"
#include "WindowTwoButtons.hpp"



SaveGameButtonSpec::SaveGameButtonSpec() = default;
SaveGameButtonSpec::SaveGameButtonSpec(uint32_t index) : MainScreenButtonSpec(index) {

}
std::string SaveGameButtonSpec::getTextureName() const {
	return   "save_icon";
}
StringLcl SaveGameButtonSpec::getString() const {
	return StringLcl("{save_game}");
}
Events SaveGameButtonSpec::getEvents() const {
	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

	Events saveGameEvent = clickEvent;
	saveGameEvent.add(std::make_shared<SaveGameEvent>());

	std::shared_ptr<WindowTwoButtons> saveGameVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl{ "{verify_save_game}" }, StringLcl{ "{yes}" }, StringLcl{ "{no}" }, saveGameEvent, clickEvent);

	Events createSaveGameVerifyWindowEvent = clickEvent;
	createSaveGameVerifyWindowEvent.add(std::make_shared<CreateEEvent>(saveGameVerifyWindow));

	return createSaveGameVerifyWindowEvent;
}