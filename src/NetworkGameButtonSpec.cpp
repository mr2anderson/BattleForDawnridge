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


#include "NetworkGameButtonSpec.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "IsServerTable.hpp"
#include "WindowButton.hpp"


NetworkGameButtonSpec::NetworkGameButtonSpec() = default;
NetworkGameButtonSpec::NetworkGameButtonSpec(uint32_t index) : MenuButtonSpec(index) {

}
StringLcl NetworkGameButtonSpec::getString() const {
    return { "{network_game}" };
}




Events NetworkGameButtonSpec::getEvents() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::vector<HorizontalSelectionWindowComponent> networkGameWindowComponents;
    networkGameWindowComponents.emplace_back(
          "exit_icon",
        StringLcl("{exit}"),
        true,
        clickEvent
    );

    std::shared_ptr<HorizontalSelectionWindow> networkGameWindow = std::make_shared<HorizontalSelectionWindow>(networkGameWindowComponents);
    Events createNetworkGameWindowEvent = clickEvent;
    createNetworkGameWindowEvent.add(std::make_shared<CreateEEvent>(networkGameWindow));

    return createNetworkGameWindowEvent;
}