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


#include "ExitButtonSpec.hpp"
#include "WindowTwoButtons.hpp"
#include "PlaySoundEvent.hpp"
#include "CloseMenuEvent.hpp"
#include "CreateEEvent.hpp"


ExitButtonSpec::ExitButtonSpec() = default;
ExitButtonSpec::ExitButtonSpec(uint32_t index) : MenuButtonSpec(index) {

}
StringLcl ExitButtonSpec::getString() const {
    return { "{exit}" };
}
Events ExitButtonSpec::getEvents() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events exitEvent;
    exitEvent.add(std::make_shared<CloseMenuEvent>());

    std::shared_ptr<WindowTwoButtons> confirmExitWindow = std::make_shared<WindowTwoButtons>(StringLcl("{confirm_exit}"), StringLcl("{yes}"), StringLcl("{no}"), exitEvent, clickEvent);
    Events createConfirmExitWindowEvent = clickEvent;
    createConfirmExitWindowEvent.add(std::make_shared<CreateEEvent>(confirmExitWindow));

    return createConfirmExitWindowEvent;
}