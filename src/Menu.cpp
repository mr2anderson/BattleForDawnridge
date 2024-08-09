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


#include <filesystem>
#include <set>
#include "Menu.hpp"
#include "Maps.hpp"
#include "WindowTwoButtons.hpp"
#include "WindowButtonImage.hpp"
#include "SoundQueue.hpp"
#include "Playlist.hpp"
#include "ColorTheme.hpp"
#include "WindowButton.hpp"
#include "Sounds.hpp"
#include "Locales.hpp"
#include "Textures.hpp"
#include "Music.hpp"
#include "FirstTimeTipsTable.hpp"
#include "LanguageAlreadyInUse.hpp"
#include "Root.hpp"
#include "StaticString.hpp"
#include "IsServerTable.hpp"
#include "ScreenAlreadyFinished.hpp"


Menu::Menu(sf::RenderWindow& window) {
    this->alreadyFinished = false;




    this->closeMenu = false;
    this->element = nullptr;




    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));




    Events startGameOnRidgeEvent = clickEvent;
    startGameOnRidgeEvent.add(std::make_shared<StartGameEvent>("ridge"));
    std::shared_ptr<WindowTwoButtons> startGameOnRidgeVerifyWindow = std::make_shared<WindowTwoButtons>(StringLcl("{local_ridge_verify}"), StringLcl("{yes}"), StringLcl("{no}"), startGameOnRidgeEvent, clickEvent);
    Events createStartGameOnRidgeVerifyWindow = clickEvent;
    createStartGameOnRidgeVerifyWindow.add(std::make_shared<CreateEEvent>(startGameOnRidgeVerifyWindow));

    std::vector<HorizontalSelectionWindowComponent> chooseLevelWindowComponents;
    chooseLevelWindowComponents.emplace_back(
        std::make_shared<StaticString>("exit_icon"),
        StringLcl("{exit}"),
        true,
        clickEvent
    );
    chooseLevelWindowComponents.emplace_back(
        std::make_shared<StaticString>("ridge"),
        StringLcl("{ridge_description}"),
        true,
        createStartGameOnRidgeVerifyWindow
    );
    std::shared_ptr<HorizontalSelectionWindow> chooseLevelWindow = std::make_shared<HorizontalSelectionWindow>(chooseLevelWindowComponents, Maps::THUMBNAIL_SIZE);
    Events createChooseLevelWindowEvent = clickEvent;
    createChooseLevelWindowEvent.add(std::make_shared<CreateEEvent>(chooseLevelWindow));




    Events createChooseSaveWindowEvent = clickEvent;
    createChooseSaveWindowEvent.add(std::make_shared<GenerateChooseSaveWindowEvent>());




    std::vector<HorizontalSelectionWindowComponent> localGameWindowComponent;
    localGameWindowComponent.emplace_back(
        std::make_shared<StaticString>("exit_icon"),
        StringLcl("{exit}"),
        true,
        clickEvent
    );
    localGameWindowComponent.emplace_back(
        std::make_shared<StaticString>("save_icon"),
        StringLcl("{choose_save}"),
        true,
        createChooseSaveWindowEvent
    );
    localGameWindowComponent.emplace_back(
        std::make_shared<StaticString>("battle_icon"),
        StringLcl("{choose_map}"),
        true,
        createChooseLevelWindowEvent
    );
    std::shared_ptr<HorizontalSelectionWindow> localGameWindow = std::make_shared<HorizontalSelectionWindow>(localGameWindowComponent);
    Events createLocalGameWindowEvent = clickEvent;
    createLocalGameWindowEvent.add(std::make_shared<CreateEEvent>(localGameWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 10, 400, 60, StringLcl("{local_game}")), createLocalGameWindowEvent);




    Events createNetworkGameSettingsWindowEvent = clickEvent;
    createNetworkGameSettingsWindowEvent.add(std::make_shared<GenerateNetworkGameSettingsWindowEvent>());




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
        createNetworkGameSettingsWindowEvent
    );
    networkGameWindowComponents.emplace_back(
        std::make_shared<StaticString>("save_icon"),
        StringLcl("{choose_save}"),
        true,
        createChooseSaveWindowEvent
    );
    networkGameWindowComponents.emplace_back(
        std::make_shared<StaticString>("battle_icon"),
        StringLcl("{choose_map}"),
        true,
        createChooseLevelWindowEvent
    );
    std::shared_ptr<HorizontalSelectionWindow> networkGameWindow = std::make_shared<HorizontalSelectionWindow>(networkGameWindowComponents);
    Events createNetworkGameWindowEvent = clickEvent;
    createNetworkGameWindowEvent.add(std::make_shared<CreateEEvent>(networkGameWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 80, 400, 60, StringLcl("{network_game}")), createNetworkGameWindowEvent);






    std::vector<HorizontalSelectionWindowComponent> educationWindowComponents;
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("exit_icon"),
        StringLcl("{leave}"),
        true,
        clickEvent
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("food_icon"),
        StringLcl("{guide_food_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("wood_icon"),
        StringLcl("{guide_wood_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("stone_icon"),
        StringLcl("{guide_stone_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("iron_icon"),
        StringLcl("{guide_iron_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("crystal_icon"),
        StringLcl("{guide_crystal_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("gold_icon"),
        StringLcl("{guide_gold_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("road_none"),
        StringLcl("{guide_road_system_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("shield_icon"),
        StringLcl("{guide_damage_mods_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("fire1"),
        StringLcl("{guide_fire_system_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("warehouse_food"),
        StringLcl("{guide_storage_system_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("wall2"),
        StringLcl("{guide_wall_system_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("forest_icon"),
        StringLcl("{guide_movement_cost_system_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("water_icon"),
        StringLcl("{guide_obstacle_system_description}"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        std::make_shared<StaticString>("helmet"),
        StringLcl("{guide_population_limit_system_description}"),
        false,
        Events()
    );
    std::shared_ptr<HorizontalSelectionWindow> educationWindow = std::make_shared<HorizontalSelectionWindow>(educationWindowComponents);
    Events createEducationWindowEvent = clickEvent;
    createEducationWindowEvent.add(std::make_shared<CreateEEvent>(educationWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 150, 400, 60, StringLcl("{education}")), createEducationWindowEvent);




    Events chooseLanguageEnglishEvent;
    chooseLanguageEnglishEvent.add(std::make_shared<ChooseLanguageEvent>("en.cfg"));
    Events chooseLanguageRussianEvent;
    chooseLanguageRussianEvent.add(std::make_shared<ChooseLanguageEvent>("ru.cfg"));
    std::vector<HorizontalSelectionWindowComponent> chooseLanguageWindowComponents;
    chooseLanguageWindowComponents.emplace_back(
        std::make_shared<StaticString>("exit_icon"),
        StringLcl("{exit}"),
        true,
        clickEvent
    );
    chooseLanguageWindowComponents.emplace_back(
        std::make_shared<StaticString>("english_icon"),
        StringLcl("English"),
        true,
        chooseLanguageEnglishEvent
    );
    chooseLanguageWindowComponents.emplace_back(
        std::make_shared<StaticString>("russian_icon"),
        StringLcl("Russian"),
        true,
        chooseLanguageRussianEvent
    );
    std::shared_ptr<HorizontalSelectionWindow> chooseLanguageWindow = std::make_shared<HorizontalSelectionWindow>(chooseLanguageWindowComponents);
    Events createChooseLanguageWindowEvent = clickEvent;
    createChooseLanguageWindowEvent.add(std::make_shared<CreateEEvent>(chooseLanguageWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 220, 400, 60, StringLcl("{language}")), createChooseLanguageWindowEvent);




    std::shared_ptr<WindowButtonImage> supportWindow = std::make_shared<WindowButtonImage>(StringLcl("{support}"), StringLcl("{close}"), "btc", clickEvent, 400, 300);
    Events supportEvent = clickEvent;
    supportEvent.add(std::make_shared<CreateEEvent>(supportWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 290, 400, 60, StringLcl("{show_support}")), supportEvent);




    std::shared_ptr<WindowButton> creditsWindow = std::make_shared<WindowButton>(StringLcl("{credits}"), StringLcl("{close}"), clickEvent, 600, 600);
    Events creditsEvent = clickEvent;
    creditsEvent.add(std::make_shared<CreateEEvent>(creditsWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 360, 400, 60, StringLcl("{show_credits}")), creditsEvent);




    std::shared_ptr<WindowButton> licenseWindow = std::make_shared<WindowButton>(StringLcl("{license}"), StringLcl("{close}"), clickEvent, 600, 600);
    Events licenseEvent = clickEvent;
    licenseEvent.add(std::make_shared<CreateEEvent>(licenseWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 430, 400, 60, StringLcl("{show_license}")), licenseEvent);




    Events exitEvent;
    exitEvent.add(std::make_shared<CloseMenuEvent>());
    std::shared_ptr<WindowTwoButtons> confirmExitWindow = std::make_shared<WindowTwoButtons>(StringLcl("{confirm_exit}"), StringLcl("{yes}"), StringLcl("{no}"), exitEvent, clickEvent);
    Events createConfirmExitWindowEvent = clickEvent;
    createConfirmExitWindowEvent.add(std::make_shared<CreateEEvent>(confirmExitWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 500, 400, 60, StringLcl("{exit}")), createConfirmExitWindowEvent);






    if (!FirstTimeTipsTable::get()->wasDisplayed("welcome")) {
        FirstTimeTipsTable::get()->markAsDisplayed("welcome");
        std::shared_ptr<WindowButton> welcomeWindow = std::make_shared<WindowButton>(StringLcl("{welcome}"), StringLcl("{OK}"), clickEvent);
        Events createWelcomeWindowEvent = clickEvent;
        createWelcomeWindowEvent.add(std::make_shared<CreateEEvent>(welcomeWindow));
        this->addEvents(createWelcomeWindowEvent);
    }
}
MenuResponse Menu::run(sf::RenderWindow& window) {
    if (this->alreadyFinished) {
        throw ScreenAlreadyFinished();
    }
    this->alreadyFinished = true;
    window.setMouseCursorVisible(true);
    Music::get()->get("menu")->play();
    Music::get()->get("menu")->setVolume(50);

	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left) {
                if (this->element == nullptr) {
                    this->addButtonClickEventToQueue();
                }
                else {
                    Events elementEvents = this->element->click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y, window.getSize().x, window.getSize().y);
                    this->addEvents(elementEvents);
                }
			}
		}
		this->drawEverything(window);
        if (this->element != nullptr) {
            this->element->update();
            if (this->element->finished()) {
                this->element = nullptr;
            }
        }
        this->processEvents();
        if (this->closeMenu) {
            Music::get()->get("menu")->stop();
            return MenuResponse(MenuResponse::TYPE::EXIT, "");
        }
        if (this->response.has_value()) {
            Music::get()->get("menu")->stop();
            return this->response.value();
        }
	}
}
void Menu::drawEverything(sf::RenderWindow &window) {
    window.clear();
    window.draw(this->bg);
	for (const auto& b : this->buttons) {
        window.draw(b);
    }
    if (this->element != nullptr) {
        window.draw(*this->element);
    }
	window.display();
}
void Menu::processEvents() {
    while (!this->events.empty()) {
        if (this->element != nullptr) {
            break;
        }
        this->handleEvent(this->events.front());
        this->events.pop();
    }
}
void Menu::addButtonClickEventToQueue() {
    for (const auto& b : this->buttons) {
        Events event = b.click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
        if (!event.empty()) {
            this->addEvents(event);
            break;
        }
    }
}
void Menu::addEvents(Events &e) {
    for (uint32_t i = 0; i < e.size(); i = i + 1) {
        if (e.at(i)->isUrgent()) {
            this->handleEvent(e.at(i));
        }
        else {
            this->events.push(e.at(i));
        }
    }
}
void Menu::handleEvent(std::shared_ptr<Event> e) {
    if (std::shared_ptr<PlaySoundEvent> playSoundEvent = std::dynamic_pointer_cast<PlaySoundEvent>(e)) {
        this->handleSoundEvent(playSoundEvent);
    }
    else if (std::shared_ptr<CreateEEvent> createEEvent = std::dynamic_pointer_cast<CreateEEvent>(e)) {
        this->handleCreateEEvent(createEEvent);
    }
    else if (std::shared_ptr<CloseMenuEvent> closeMenuEvent = std::dynamic_pointer_cast<CloseMenuEvent>(e)) {
        this->handleCloseMenuEvent(closeMenuEvent);
    }
    else if (std::shared_ptr<StartGameEvent> startGameEvent = std::dynamic_pointer_cast<StartGameEvent>(e)) {
        this->handleStartGameEvent(startGameEvent);
    }
    else if (std::shared_ptr<LoadGameEvent> loadGameEvent = std::dynamic_pointer_cast<LoadGameEvent>(e)) {
        this->handleLoadGameEvent(loadGameEvent);
    }
    else if (std::shared_ptr<GenerateChooseSaveWindowEvent> generateChooseSaveWindowEvent = std::dynamic_pointer_cast<GenerateChooseSaveWindowEvent>(e)) {
        this->handleGenerateChooseSaveWindowEvent(generateChooseSaveWindowEvent);
    }
    else if (std::shared_ptr<GenerateNetworkGameSettingsWindowEvent> generateNetworkGameWindowEvent = std::dynamic_pointer_cast<GenerateNetworkGameSettingsWindowEvent>(e)) {
        this->handleGenerateNetworkGameSettingsWindowEvent(generateNetworkGameWindowEvent);
    }
    else if (std::shared_ptr<InvertIsServerStateEvent> invertIsServerStateEvent = std::dynamic_pointer_cast<InvertIsServerStateEvent>(e)) {
        this->handleInvertIsServerStateEvent(invertIsServerStateEvent);
    }
    else if (std::shared_ptr<DeleteSaveEvent> deleteSaveEvent = std::dynamic_pointer_cast<DeleteSaveEvent>(e)) {
        this->handleDeleteSaveEvent(deleteSaveEvent);
    }
    else if (std::shared_ptr<ChooseLanguageEvent> chooseLanguageEvent = std::dynamic_pointer_cast<ChooseLanguageEvent>(e)) {
        this->handleChooseLanguageEvent(chooseLanguageEvent);
    }
}
void Menu::handleSoundEvent(std::shared_ptr<PlaySoundEvent> e) {
    SoundQueue::get()->push(Sounds::get()->get(e->getSoundName()));
}
void Menu::handleCreateEEvent(std::shared_ptr<CreateEEvent> e) {
    this->element = e->getElement();
    this->element->restart();
}
void Menu::handleCloseMenuEvent(std::shared_ptr<CloseMenuEvent> e) {
    this->closeMenu = true;
}
void Menu::handleStartGameEvent(std::shared_ptr<StartGameEvent> e) {
    this->response = MenuResponse(MenuResponse::TYPE::START_LOCAL_GAME, e->getMapName());
}
void Menu::handleLoadGameEvent(std::shared_ptr<LoadGameEvent> e) {
    this->response = MenuResponse(MenuResponse::TYPE::LOAD_LOCAL_GAME, e->getSaveName());
}
void Menu::handleGenerateChooseSaveWindowEvent(std::shared_ptr<GenerateChooseSaveWindowEvent> e) { // It is impossible to create choose save window like other windows in menu, cuz it is content might be changed during run time
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::set<std::string> saves;
    if (std::filesystem::is_directory(USERDATA_ROOT + "/saves")) {
        for (const auto & entry : std::filesystem::directory_iterator(USERDATA_ROOT + "/saves")) {
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
                std::make_shared<StaticString>("exit_icon"),
                StringLcl("{exit}"),
                true,
                clickEvent
        );
        for (const auto &save : saves) {

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
                    std::make_shared<StaticString>("save_icon"),
                    StringLcl("{save}") + save,
                    true,
                    createChooseActionWindowEvent
            );
        }
        window = std::make_shared<HorizontalSelectionWindow>(components);
    }

    Events result;
    result.add(std::make_shared<CreateEEvent>(window));
    this->addEvents(result);
}
void Menu::handleGenerateNetworkGameSettingsWindowEvent(std::shared_ptr<GenerateNetworkGameSettingsWindowEvent> e) {
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
    Events events;
    events.add(std::make_shared<CreateEEvent>(w));
    this->addEvents(events);
}
void Menu::handleInvertIsServerStateEvent(std::shared_ptr<InvertIsServerStateEvent> e) {
    IsServerTable::get()->invert();
}
void Menu::handleDeleteSaveEvent(std::shared_ptr<DeleteSaveEvent> e) {
    std::filesystem::remove(USERDATA_ROOT + "/saves/" + e->getSaveName());
}
void Menu::handleChooseLanguageEvent(std::shared_ptr<ChooseLanguageEvent> e) {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::shared_ptr<WindowButton> w;
    try {
        Locales::get()->setDefaultPath(e->getLocaleFile());
        FirstTimeTipsTable::get()->clear();
        w = std::make_shared<WindowButton>(StringLcl("{language_was_changed}"), StringLcl("{OK}"), clickEvent);
    }
    catch (LanguageAlreadyInUse&) {
        w = std::make_shared<WindowButton>(StringLcl("{language_already_in_use}"), StringLcl("{OK}"), clickEvent);
    }
    Events createWindowEvent = clickEvent;
    createWindowEvent.add(std::make_shared<CreateEEvent>(w));
    this->addEvents(createWindowEvent);
}