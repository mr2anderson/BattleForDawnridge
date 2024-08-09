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
#include "StaticString.hpp"
#include "PlaySoundEvent.hpp"
#include "CreateEEvent.hpp"
#include "IsServerTable.hpp"
#include "WindowButton.hpp"
#include "InvertIsServerStateEvent.hpp"


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
        std::make_shared<StaticString>("exit_icon"),
        StringLcl("{exit}"),
        true,
        clickEvent
    );
    networkGameWindowComponents.emplace_back(
        std::make_shared<StaticString>("advanced_settings_icon"),
        StringLcl("{advanced_settings}"),
        true,
        this->getAdvancedSettingsWindowEvent()
    );

    std::shared_ptr<HorizontalSelectionWindow> networkGameWindow = std::make_shared<HorizontalSelectionWindow>(networkGameWindowComponents);
    Events createNetworkGameWindowEvent = clickEvent;
    createNetworkGameWindowEvent.add(std::make_shared<CreateEEvent>(networkGameWindow));

    return createNetworkGameWindowEvent;
}



Events NetworkGameButtonSpec::getAdvancedSettingsWindowEvent() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::vector<HorizontalSelectionWindowComponent> components;
    components.emplace_back(
        std::make_shared<StaticString>("exit_icon"),
        StringLcl("{exit}"),
        true,
        clickEvent
    );
    if (IsServerTable::get()->isServer()) {
        std::shared_ptr<WindowButton> doneWindow = std::make_shared<WindowButton>(StringLcl("{switched_to_client_mode}"), StringLcl("{OK}"), clickEvent);
        Events events = clickEvent;
        events.add(std::make_shared<InvertIsServerStateEvent>());
        events.add(std::make_shared<CreateEEvent>(doneWindow));
        components.emplace_back(
            std::make_shared<StaticString>("switch_to_client_icon"),
            StringLcl("{switch_to_client}"),
            true,
            events
        );
    }
    else {
        std::shared_ptr<WindowButton> doneWindow = std::make_shared<WindowButton>(StringLcl("{switched_to_server_mode}"), StringLcl("{OK}"), clickEvent);
        Events events = clickEvent;
        events.add(std::make_shared<InvertIsServerStateEvent>());
        events.add(std::make_shared<CreateEEvent>(doneWindow));
        components.emplace_back(
            std::make_shared<StaticString>("switch_to_server_icon"),
            StringLcl("{switch_to_server}"),
            true,
            events
        );
    }

    std::shared_ptr<HorizontalSelectionWindow> w = std::make_shared<HorizontalSelectionWindow>(components);
    Events events = clickEvent;
    events.add(std::make_shared<CreateEEvent>(w));
    
    return events;
}