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
#include "LocalGameButtonSpec.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "WindowTwoButtons.hpp"
#include "StartLocalGameEvent.hpp"
#include "CreateEEvent.hpp"
#include "WindowButton.hpp"
#include "LoadLocalGameEvent.hpp"
#include "DeleteSaveEvent.hpp"
#include "Root.hpp"
#include "PlaySoundEvent.hpp"




LocalGameButtonSpec::LocalGameButtonSpec() = default;
LocalGameButtonSpec::LocalGameButtonSpec(uint32_t index) : MenuButtonSpec(index) {

}
LocalGameButtonSpec::LocalGameButtonSpec(uint32_t index, const std::vector<std::string>& mapNames, const std::set<std::string>& saveNames) : MenuButtonSpec(index) {
    this->mapNames = mapNames;
    this->saveNames = saveNames;
}
StringLcl LocalGameButtonSpec::getString() const {
    return { "{local_game}" };
}




Events LocalGameButtonSpec::getEvents() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::vector<HorizontalSelectionWindowComponent> localGameWindowComponent;
    localGameWindowComponent.emplace_back(
          "exit_icon",
        StringLcl("{exit}"),
        true,
        clickEvent
    );
    localGameWindowComponent.emplace_back(
          "save_icon",
        StringLcl("{choose_save}"),
        true,
        this->getChooseSaveEvent()
    );
    localGameWindowComponent.emplace_back(
          "battle_icon",
        StringLcl("{choose_map}"),
        true,
        this->getChooseMapEvent()
    );

    std::shared_ptr<HorizontalSelectionWindow> localGameWindow = std::make_shared<HorizontalSelectionWindow>(localGameWindowComponent);

    Events createLocalGameWindowEvent = clickEvent;
    createLocalGameWindowEvent.add(std::make_shared<CreateEEvent>(localGameWindow));

    return createLocalGameWindowEvent;
}




Events LocalGameButtonSpec::getChooseMapEvent() const {
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




Events LocalGameButtonSpec::getChooseSaveEvent() const {
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
            loadSaveEvent.add(std::make_shared<LoadLocalGameEvent>(save));
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




Events LocalGameButtonSpec::getChooseMapEvent(const std::string &mapName) const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events startGameOnRidgeEvent;
    startGameOnRidgeEvent.add(std::make_shared<StartLocalGameEvent>(mapName));

    std::shared_ptr<WindowTwoButtons> startGameOnRidgeVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl(mapName + "_verify}"), StringLcl("{yes}"), StringLcl("{no}"), startGameOnRidgeEvent, clickEvent);

    Events createStartGameOnRidgeVerifyWindow = clickEvent;
    createStartGameOnRidgeVerifyWindow.add(std::make_shared<CreateEEvent>(startGameOnRidgeVerifyWindow));

    return createStartGameOnRidgeVerifyWindow;
}