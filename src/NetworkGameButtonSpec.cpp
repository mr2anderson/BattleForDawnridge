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
#include "WindowTwoButtons.hpp"
#include "StartNetworkGameEvent.hpp"
#include "CreateEEvent.hpp"
#include "WindowButton.hpp"
#include "LoadNetworkGameEvent.hpp"
#include "DeleteSaveEvent.hpp"
#include "PlaySoundEvent.hpp"
#include "WindowEntry.hpp"
#include "RoomID.hpp"
#include "ConnectToRoomEvent.hpp"




NetworkGameButtonSpec::NetworkGameButtonSpec() = default;
NetworkGameButtonSpec::NetworkGameButtonSpec(uint32_t index) : MenuButtonSpec(index) {

}
NetworkGameButtonSpec::NetworkGameButtonSpec(uint32_t index, const std::vector<std::string>& mapNames, const std::set<std::string>& saveNames) : MenuButtonSpec(index) {
    this->mapNames = mapNames;
    this->saveNames = saveNames;
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
    networkGameWindowComponents.emplace_back(
            "room_id_icon",
            StringLcl("{connect}"),
            true,
            this->getConnectEvent()
    );
    networkGameWindowComponents.emplace_back(
        "save_icon",
        StringLcl("{choose_save}"),
        true,
        this->getChooseSaveEvent()
    );
    networkGameWindowComponents.emplace_back(
        "battle_icon",
        StringLcl("{choose_map}"),
        true,
        this->getChooseMapEvent()
    );

    std::shared_ptr<HorizontalSelectionWindow> networkGameWindow = std::make_shared<HorizontalSelectionWindow>(networkGameWindowComponents);

    Events createNetworkGameWindowEvent = clickEvent;
    createNetworkGameWindowEvent.add(std::make_shared<CreateEEvent>(networkGameWindow));

    return createNetworkGameWindowEvent;
}




Events NetworkGameButtonSpec::getChooseMapEvent() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::vector<HorizontalSelectionWindowComponent> chooseLevelWindowComponents;
    chooseLevelWindowComponents.emplace_back(
        "exit_icon",
        StringLcl("{exit}"),
        true,
        clickEvent
    );
    for (const auto& mapName : this->mapNames) {
        chooseLevelWindowComponents.emplace_back(
            mapName,
            StringLcl("{" + mapName + "_description}"),
            true,
            this->getChooseMapEvent(mapName)
        );
    }


    std::shared_ptr<HorizontalSelectionWindow> chooseLevelWindow = std::make_shared<HorizontalSelectionWindow>(chooseLevelWindowComponents);

    Events createChooseLevelWindowEvent = clickEvent;
    createChooseLevelWindowEvent.add(std::make_shared<CreateEEvent>(chooseLevelWindow));

    return createChooseLevelWindowEvent;
}




Events NetworkGameButtonSpec::getChooseSaveEvent() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::shared_ptr<PopUpElement> window;
    if (this->saveNames.empty()) {
        window = std::make_shared<WindowButton>(StringLcl("{you_do_not_have_any_saves_yet}"), StringLcl("{OK}"), clickEvent);
    }
    else {
        std::vector<HorizontalSelectionWindowComponent> components;
        components.emplace_back(
            "exit_icon",
            StringLcl("{exit}"),
            true,
            clickEvent
        );
        for (const auto& save : this->saveNames) {

            Events loadSaveEvent = clickEvent;
            loadSaveEvent.add(std::make_shared<LoadNetworkGameEvent>(save));
            std::shared_ptr<WindowTwoButtons> loadSaveVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl("{load}") + save + "?", StringLcl("{yes}"), StringLcl("{no}"), loadSaveEvent, clickEvent);
            Events createLoadSaveVerifyWindowEvent = clickEvent;
            createLoadSaveVerifyWindowEvent.add(std::make_shared<CreateEEvent>(loadSaveVerifyWindow));

            Events deleteSaveEvent = clickEvent;
            deleteSaveEvent.add(std::make_shared<DeleteSaveEvent>(save));
            std::shared_ptr<WindowTwoButtons> deleteSaveVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl("{delete}") + save + "?", StringLcl("{yes}"), StringLcl("{no}"), deleteSaveEvent, clickEvent);
            Events createDeleteSaveVerifyWindowEvent = clickEvent;
            createDeleteSaveVerifyWindowEvent.add(std::make_shared<CreateEEvent>(deleteSaveVerifyWindow));

            std::shared_ptr<WindowTwoButtons> chooseActionWindow = std::make_shared<WindowTwoButtons>(StringLcl("{save}") + save, StringLcl("{load}"), StringLcl("{delete}"), createLoadSaveVerifyWindowEvent, createDeleteSaveVerifyWindowEvent);
            Events createChooseActionWindowEvent = clickEvent;
            createChooseActionWindowEvent.add(std::make_shared<CreateEEvent>(chooseActionWindow));

            components.emplace_back(
                "save_icon",
                StringLcl("{save}") + save,
                true,
                createChooseActionWindowEvent
            );
        }
        window = std::make_shared<HorizontalSelectionWindow>(components, 48, 6);
    }

    Events result = clickEvent;
    result.add(std::make_shared<CreateEEvent>(window));

    return result;
}






Events NetworkGameButtonSpec::getConnectEvent() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::shared_ptr<EntrySettings> settings = std::make_shared<EntrySettings>();
    settings->setMaxLen(RoomID::READABLE_LEN);
    settings->setRequireMaxLen(true);
    settings->regDigits();
    settings->regLetters(EntrySettings::LETTER_REGISTRATION_TYPE::AS_UPPER_CASE);

    std::shared_ptr<std::string> result = std::make_shared<std::string>();

    Events connectToRoomEvent = clickEvent;
    connectToRoomEvent.add(std::make_shared<ConnectToRoomEvent>(result));

    std::shared_ptr<PopUpElement> window = std::make_shared<WindowEntry>(StringLcl("{enter_room_id}"), StringLcl("{connect2}"), StringLcl("{cancel}"), result, settings, connectToRoomEvent, clickEvent);

    Events createWindowEvent = clickEvent;
    createWindowEvent.add(std::make_shared<CreateEEvent>(window));
    return createWindowEvent;
}




Events NetworkGameButtonSpec::getChooseMapEvent(const std::string& mapName) const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events startGameOnRidgeEvent = clickEvent;
    startGameOnRidgeEvent.add(std::make_shared<StartNetworkGameEvent>(mapName));

    std::shared_ptr<WindowTwoButtons> startGameOnRidgeVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl(mapName + "_verify}"), StringLcl("{yes}"), StringLcl("{no}"), startGameOnRidgeEvent, clickEvent);

    Events createStartGameOnRidgeVerifyWindow = clickEvent;
    createStartGameOnRidgeVerifyWindow.add(std::make_shared<CreateEEvent>(startGameOnRidgeVerifyWindow));

    return createStartGameOnRidgeVerifyWindow;
}