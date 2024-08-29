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


#include <boost/algorithm/string.hpp>
#include <iomanip>
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
#include "EntryStrSettings.hpp"




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




static std::string TO_STRING(int32_t val, uint32_t len) {
    std::stringstream ss;
    ss << std::setw(len) << std::setfill('0') << val;
    return ss.str();
}
static StringLcl SAVE_NAME(const std::string& save, bool upper) {
    std::string delimiters("-.");
    std::vector<std::string> parts;
    boost::split(parts, save, boost::is_any_of(delimiters));
    if (parts.size() != 5) {
        return StringLcl("{unknown_save}");
    }
    int32_t month;
    int32_t day;
    int32_t hour;
    int32_t minute;
    try {
        month = std::stoi(parts.at(0));
        day = std::stoi(parts.at(1));
        hour = std::stoi(parts.at(2));
        minute = std::stoi(parts.at(3));
    }
    catch (std::exception&) {
        return StringLcl("{unknown_save}");
    }
    if (month <= 0 or day <= 0 or hour < 0 or minute < 0 or month > 12 or day > 31 or hour > 23 or minute > 59) {
        return StringLcl("{unknown_save}");
    }
    std::string startKey;
    if (upper) {
        startKey = "save_upper";
    }
    else {
        startKey = "save";
    }
    return StringLcl("{" + startKey + "}") + TO_STRING(day, 2) + " " + StringLcl("{month" + std::to_string(month) + "}") + " " + StringLcl("{in}") + " " + TO_STRING(hour, 2) + ":" + TO_STRING(minute, 2);
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

            StringLcl saveNameUpper = SAVE_NAME(save, true);
            StringLcl saveName = SAVE_NAME(save, false);

            Events loadSaveEvent;
            loadSaveEvent.add(std::make_shared<LoadNetworkGameEvent>(save));
            std::shared_ptr<WindowTwoButtons> loadSaveVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl("{load}") + saveName + "?", StringLcl("{yes}"), StringLcl("{no}"), loadSaveEvent, clickEvent);
            Events createLoadSaveVerifyWindowEvent = clickEvent;
            createLoadSaveVerifyWindowEvent.add(std::make_shared<CreateEEvent>(loadSaveVerifyWindow));

            Events deleteSaveEvent = clickEvent;
            deleteSaveEvent.add(std::make_shared<DeleteSaveEvent>(save));
            std::shared_ptr<WindowTwoButtons> deleteSaveVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl("{delete}") + saveName + "?", StringLcl("{yes}"), StringLcl("{no}"), deleteSaveEvent, clickEvent);
            Events createDeleteSaveVerifyWindowEvent = clickEvent;
            createDeleteSaveVerifyWindowEvent.add(std::make_shared<CreateEEvent>(deleteSaveVerifyWindow));

            std::shared_ptr<WindowTwoButtons> chooseActionWindow = std::make_shared<WindowTwoButtons>(saveNameUpper, StringLcl("{load}"), StringLcl("{delete}"), createLoadSaveVerifyWindowEvent, createDeleteSaveVerifyWindowEvent);
            Events createChooseActionWindowEvent = clickEvent;
            createChooseActionWindowEvent.add(std::make_shared<CreateEEvent>(chooseActionWindow));

            components.emplace_back(
                "save_icon",
                saveNameUpper,
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

    std::shared_ptr<EntryStrSettings> settings = std::make_shared<EntryStrSettings>();
    settings->setMaxLen(RoomID::READABLE_LEN);
    settings->setMinLen(RoomID::READABLE_LEN);
    settings->regDigits();
    settings->regLetters(EntryStrSettings::LETTER_REGISTRATION_TYPE::AS_UPPER_CASE);

    std::shared_ptr<std::string> result = std::make_shared<std::string>();

    Events connectToRoomEvent;
    connectToRoomEvent.add(std::make_shared<ConnectToRoomEvent>(result));

    std::shared_ptr<PopUpElement> window = std::make_shared<WindowEntry>(StringLcl("{enter_room_id}"), StringLcl("{connect2}"), StringLcl("{cancel}"), result, settings, connectToRoomEvent, clickEvent);

    Events createWindowEvent = clickEvent;
    createWindowEvent.add(std::make_shared<CreateEEvent>(window));
    return createWindowEvent;
}




Events NetworkGameButtonSpec::getChooseMapEvent(const std::string& mapName) const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events startGameEvent;
    startGameEvent.add(std::make_shared<StartNetworkGameEvent>(mapName));

    std::shared_ptr<WindowTwoButtons> startGameOnRidgeVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl(mapName + "_verify}"), StringLcl("{yes}"), StringLcl("{no}"), startGameEvent, clickEvent);

    Events createStartGameOnRidgeVerifyWindow = clickEvent;
    createStartGameOnRidgeVerifyWindow.add(std::make_shared<CreateEEvent>(startGameOnRidgeVerifyWindow));

    return createStartGameOnRidgeVerifyWindow;
}