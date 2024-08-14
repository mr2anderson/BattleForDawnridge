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


#include <set>
#include <filesystem>
#include "LocalGameButtonSpec.hpp"
#include "HorizontalSelectionWindow.hpp"
#include "WindowTwoButtons.hpp"
#include "StartGameEvent.hpp"
#include "CreateEEvent.hpp"
#include "WindowButton.hpp"
#include "LoadGameEvent.hpp"
#include "DeleteSaveEvent.hpp"
#include "Root.hpp"
#include "PlaySoundEvent.hpp"




LocalGameButtonSpec::LocalGameButtonSpec() = default;
LocalGameButtonSpec::LocalGameButtonSpec(uint32_t index) : MenuButtonSpec(index) {

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
    chooseLevelWindowComponents.emplace_back(
          "ridge",
        StringLcl("{ridge_description}"),
        true,
        this->getChooseRidgeEvent()
    );

    std::shared_ptr<HorizontalSelectionWindow> chooseLevelWindow = std::make_shared<HorizontalSelectionWindow>(chooseLevelWindowComponents);

    Events createChooseLevelWindowEvent = clickEvent;
    createChooseLevelWindowEvent.add(std::make_shared<CreateEEvent>(chooseLevelWindow));

    return createChooseLevelWindowEvent;
}




Events LocalGameButtonSpec::getChooseSaveEvent() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::set<std::string> saves;
    if (std::filesystem::is_directory(USERDATA_ROOT + "/saves")) {
        for (const auto& entry : std::filesystem::directory_iterator(USERDATA_ROOT + "/saves")) {
            saves.insert(entry.path().filename().string());
        }
    }

    std::shared_ptr<PopUpElement> window;
    if (saves.empty()) {
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
        for (const auto& save : saves) {

            Events loadSaveEvent = clickEvent;
            loadSaveEvent.add(std::make_shared<LoadGameEvent>(save));
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




Events LocalGameButtonSpec::getChooseRidgeEvent() const {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Events startGameOnRidgeEvent = clickEvent;
    startGameOnRidgeEvent.add(std::make_shared<StartGameEvent>("ridge"));

    std::shared_ptr<WindowTwoButtons> startGameOnRidgeVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl("{local_ridge_verify}"), StringLcl("{yes}"), StringLcl("{no}"), startGameOnRidgeEvent, clickEvent);

    Events createStartGameOnRidgeVerifyWindow = clickEvent;
    createStartGameOnRidgeVerifyWindow.add(std::make_shared<CreateEEvent>(startGameOnRidgeVerifyWindow));

    return createStartGameOnRidgeVerifyWindow;
}