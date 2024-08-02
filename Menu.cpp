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
#include "UTFEncoder.hpp"
#include "FirstTimeTipsTable.hpp"
#include "LanguageAlreadyInUse.hpp"
#include "Root.hpp"


Menu* Menu::singletone = nullptr;


std::tuple<uint8_t, std::string> Menu::run(sf::RenderWindow& window) {
    window.setMouseCursorVisible(true);
    this->init(window.getSize().x, window.getSize().y);
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left) {
                if (this->element == nullptr) {
                    this->addButtonClickEventToQueue();
                }
                else {
                    Events elementEvents = this->element->click();
                    this->addEvents(elementEvents);
                }
			}
		}
		this->drawEverything(window);
        this->removeFinishedElement();
        this->processEvents();
        if (this->element != nullptr) {
            this->element->update();
        }
        if (this->closeMenu) {
            return this->response;
        }
        if (!std::get<1>(this->response).empty()) {
            this->prepareToStartGame();
            return this->response;
        }
	}
}
void Menu::init(uint32_t windowW, uint32_t windowH) {
    Music::get()->get("menu")->play();
    Music::get()->get("menu")->setVolume(50);

    this->closeMenu = false;
    this->response = std::tuple<uint8_t, std::string>();
    this->windowW = windowW;
    this->windowH = windowH;
    this->element = nullptr;



    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));




    Events startGameOnRidgeEvent = clickEvent;
    startGameOnRidgeEvent.add(std::make_shared<StartGameEvent>("ridge"));

    std::shared_ptr<WindowTwoButtons> startGameOnRidgeVerifyWindow = std::make_shared<WindowTwoButtons>(*Locales::get()->get("local_ridge_verify"), *Locales::get()->get("yes"), *Locales::get()->get("no"), startGameOnRidgeEvent, clickEvent);
    Events createStartGameOnRidgeVerifyWindow = clickEvent;
    createStartGameOnRidgeVerifyWindow.add(std::make_shared<CreateEEvent>(startGameOnRidgeVerifyWindow));

    std::vector<HorizontalSelectionWindowComponent> chooseLevelWindowComponents;
    chooseLevelWindowComponents.emplace_back(
        "exit_icon",
        *Locales::get()->get("cancel"),
        true,
        clickEvent
    );
    chooseLevelWindowComponents.emplace_back(
        "ridge",
        *Locales::get()->get("ridge_description"),
        true,
        createStartGameOnRidgeVerifyWindow
    );

    std::shared_ptr<HorizontalSelectionWindow> chooseLevelWindow = std::make_shared<HorizontalSelectionWindow>(chooseLevelWindowComponents, Maps::THUMBNAIL_SIZE);
    Events createChooseLevelWindowEvent = clickEvent;
    createChooseLevelWindowEvent.add(std::make_shared<CreateEEvent>(chooseLevelWindow));
	this->buttons.emplace_back(std::make_shared<Label>(10, 10, 400, 60, *Locales::get()->get("start_game_local")), createChooseLevelWindowEvent);





    Events createChooseSaveWindowEvent = clickEvent;
    createChooseSaveWindowEvent.add(std::make_shared<GenerateChooseSaveWindowEvent>());
    this->buttons.emplace_back(std::make_shared<Label>(10, 80, 400, 60, *Locales::get()->get("load_game_local")), createChooseSaveWindowEvent);




    std::vector<HorizontalSelectionWindowComponent> educationWindowComponents;
    educationWindowComponents.emplace_back(
        "exit_icon",
        *Locales::get()->get("leave"),
        true,
        clickEvent
    );
    educationWindowComponents.emplace_back(
        "food_icon",
        *Locales::get()->get("guide_food_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "wood_icon",
        *Locales::get()->get("guide_wood_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "stone_icon",
        *Locales::get()->get("guide_stone_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "iron_icon",
        *Locales::get()->get("guide_iron_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
            "crystal_icon",
            *Locales::get()->get("guide_crystal_description"),
            false,
            Events()
    );
    educationWindowComponents.emplace_back(
        "gold_icon",
        *Locales::get()->get("guide_gold_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "road_none",
        *Locales::get()->get("guide_road_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "shield_icon",
        *Locales::get()->get("guide_damage_mods_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "fire1",
        *Locales::get()->get("guide_fire_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "warehouse_food",
        *Locales::get()->get("guide_storage_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "wall2",
        *Locales::get()->get("guide_wall_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "forest_icon",
        *Locales::get()->get("guide_movement_cost_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "water_icon",
        *Locales::get()->get("guide_obstacle_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "helmet",
        *Locales::get()->get("guide_population_limit_system_description"),
        false,
        Events()
    );

    std::shared_ptr<HorizontalSelectionWindow> educationWindow = std::make_shared<HorizontalSelectionWindow>(educationWindowComponents);
    Events createEducationWindowEvent = clickEvent;
    createEducationWindowEvent.add(std::make_shared<CreateEEvent>(educationWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 150, 400, 60, *Locales::get()->get("education")), createEducationWindowEvent);




    Events chooseLanguageEnglishEvent;
    chooseLanguageEnglishEvent.add(std::make_shared<ChooseLanguageEvent>("en.cfg"));

    Events chooseLanguageRussianEvent;
    chooseLanguageRussianEvent.add(std::make_shared<ChooseLanguageEvent>("ru.cfg"));

    std::vector<HorizontalSelectionWindowComponent> chooseLanguageWindowComponents;
    chooseLanguageWindowComponents.emplace_back(
        "exit_icon",
        *Locales::get()->get("cancel"),
        true,
        clickEvent
    );
    chooseLanguageWindowComponents.emplace_back(
        "english_icon",
        UTFEncoder::get()->utf8ToUtf16("English"),
        true,
        chooseLanguageEnglishEvent
    );
    chooseLanguageWindowComponents.emplace_back(
        "russian_icon",
        UTFEncoder::get()->utf8ToUtf16("Russian"),
        true,
        chooseLanguageRussianEvent
    );

    std::shared_ptr<HorizontalSelectionWindow> chooseLanguageWindow = std::make_shared<HorizontalSelectionWindow>(chooseLanguageWindowComponents);
    Events createChooseLanguageWindowEvent = clickEvent;
    createChooseLanguageWindowEvent.add(std::make_shared<CreateEEvent>(chooseLanguageWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 220, 400, 60, *Locales::get()->get("language")), createChooseLanguageWindowEvent);




    std::shared_ptr<WindowButtonImage> supportWindow = std::make_shared<WindowButtonImage>(*Locales::get()->get("support"), *Locales::get()->get("close"), "btc", clickEvent, 400, 300);
    Events supportEvent = clickEvent;
    supportEvent.add(std::make_shared<CreateEEvent>(supportWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 290, 400, 60,  *Locales::get()->get("show_support")), supportEvent);




    std::shared_ptr<WindowButton> creditsWindow = std::make_shared<WindowButton>(*Locales::get()->get("credits"), *Locales::get()->get("close"), clickEvent, 600, 600);
    Events creditsEvent = clickEvent;
    creditsEvent.add(std::make_shared<CreateEEvent>(creditsWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 360, 400, 60, *Locales::get()->get("show_credits")), creditsEvent);




    std::shared_ptr<WindowButton> licenseWindow = std::make_shared<WindowButton>(*Locales::get()->get("license"), *Locales::get()->get("close"), clickEvent, 600, 600);
    Events licenseEvent = clickEvent;
    licenseEvent.add(std::make_shared<CreateEEvent>(licenseWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 430, 400, 60, *Locales::get()->get("show_license")), licenseEvent);




    Events exitEvent;
    exitEvent.add(std::make_shared<CloseMenuEvent>());

    std::shared_ptr<WindowTwoButtons> confirmExitWindow = std::make_shared<WindowTwoButtons>(*Locales::get()->get("confirm_exit"), *Locales::get()->get("yes"), *Locales::get()->get("no"), exitEvent, clickEvent);
    Events createConfirmExitWindowEvent = clickEvent;
    createConfirmExitWindowEvent.add(std::make_shared<CreateEEvent>(confirmExitWindow));
	this->buttons.emplace_back(std::make_shared<Label>(10, 500, 400, 60, *Locales::get()->get("exit")), createConfirmExitWindowEvent);

    this->background.setTexture(*Textures::get()->get("menu"));
    this->background.setPosition(windowW - this->background.getLocalBounds().width, windowH - this->background.getLocalBounds().height);



    if (!FirstTimeTipsTable::get()->wasDisplayed("welcome")) {
        FirstTimeTipsTable::get()->markAsDisplayed("welcome");
        std::shared_ptr<WindowButton> welcomeWindow = std::make_shared<WindowButton>(*Locales::get()->get("welcome"), *Locales::get()->get("OK"), clickEvent);
        Events createWelcomeWindowEvent = clickEvent;
        createWelcomeWindowEvent.add(std::make_shared<CreateEEvent>(welcomeWindow));
        this->addEvents(createWelcomeWindowEvent);
    }
}
void Menu::drawEverything(sf::RenderWindow &window) {
    window.clear(sf::Color::Black);
    window.draw(this->background);
	for (const auto& b : this->buttons) {
        window.draw(b);
    }
    if (this->element != nullptr) {
        window.draw(*this->element);
    }
	window.display();
}
void Menu::removeFinishedElement() {
    if (this->element != nullptr and this->element->finished()) {
        this->element->restart();
        this->element = nullptr;
    }
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
void Menu::prepareToStartGame() {
    Music::get()->get("menu")->stop();
    this->buttons.clear();
}
void Menu::addButtonClickEventToQueue() {
    for (const auto& b : this->buttons) {
        Events event = b.click();
        if (!event.empty()) {
            this->addEvents(event);
            break;
        }
    }
}
void Menu::addEvents(Events &e) {
    for (uint32_t i = 0; i < e.size(); i = i + 1) {
        this->events.push(e.at(i));
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
    this->element->run(this->windowW, this->windowH);
}
void Menu::handleCloseMenuEvent(std::shared_ptr<CloseMenuEvent> e) {
    this->closeMenu = true;
}
void Menu::handleStartGameEvent(std::shared_ptr<StartGameEvent> e) {
    this->response = std::make_tuple(TYPE::START_LOCAL_GAME, e->getMapName());
}
void Menu::handleLoadGameEvent(std::shared_ptr<LoadGameEvent> e) {
    this->response = std::make_tuple(TYPE::LOAD_LOCAL_GAME, e->getSaveName());
}
void Menu::handleGenerateChooseSaveWindowEvent(std::shared_ptr<GenerateChooseSaveWindowEvent> e) { // It is impossible to create choose save window like other windows in menu, cuz it is content might be changed during run time
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    std::set<std::string> saves;
    if (std::filesystem::is_directory(USERDATA_ROOT + "/saves")) {
        for (const auto & entry : std::filesystem::directory_iterator(USERDATA_ROOT + "/saves")) {
            saves.insert(entry.path().filename());
        }
    }

    std::shared_ptr<PopUpElement> window;
    if (saves.empty()) {
        window = std::make_shared<WindowButton>(*Locales::get()->get("you_do_not_have_any_saves_yet"), *Locales::get()->get("OK"), clickEvent);
    }
    else {
        std::vector<HorizontalSelectionWindowComponent> components;
        components.emplace_back(
                "exit_icon",
                *Locales::get()->get("cancel"),
                true,
                clickEvent
        );
        for (const auto &save : saves) {

            Events loadSaveEvent = clickEvent;
            loadSaveEvent.add(std::make_shared<LoadGameEvent>(save));
            std::shared_ptr<WindowTwoButtons> loadSaveVerifyWindow = std::make_shared<WindowTwoButtons>(*Locales::get()->get("load") + UTFEncoder::get()->utf8ToUtf16(save) + L"?", *Locales::get()->get("yes"), *Locales::get()->get("no"), loadSaveEvent, clickEvent);
            Events createLoadSaveVerifyWindowEvent = clickEvent;
            createLoadSaveVerifyWindowEvent.add(std::make_shared<CreateEEvent>(loadSaveVerifyWindow));

            Events deleteSaveEvent = clickEvent;
            deleteSaveEvent.add(std::make_shared<DeleteSaveEvent>(save));
            std::shared_ptr<WindowTwoButtons> deleteSaveVerifyWindow = std::make_shared<WindowTwoButtons>(*Locales::get()->get("delete") + UTFEncoder::get()->utf8ToUtf16(save) + L"?", *Locales::get()->get("yes"), *Locales::get()->get("no"), deleteSaveEvent, clickEvent);
            Events createDeleteSaveVerifyWindowEvent = clickEvent;
            createDeleteSaveVerifyWindowEvent.add(std::make_shared<CreateEEvent>(deleteSaveVerifyWindow));

            std::shared_ptr<WindowTwoButtons> chooseActionWindow = std::make_shared<WindowTwoButtons>(*Locales::get()->get("save") + UTFEncoder::get()->utf8ToUtf16(save), *Locales::get()->get("load"), *Locales::get()->get("delete"), createLoadSaveVerifyWindowEvent, createDeleteSaveVerifyWindowEvent);
            Events createChooseActionWindowEvent = clickEvent;
            createChooseActionWindowEvent.add(std::make_shared<CreateEEvent>(chooseActionWindow));

            components.emplace_back(
                    "save_icon",
                    *Locales::get()->get("save") + UTFEncoder::get()->utf8ToUtf16(save),
                    true,
                    createChooseActionWindowEvent
            );
        }
        window = std::make_shared<HorizontalSelectionWindow>(components);
    }

    Events result = clickEvent;
    result.add(std::make_shared<CreateEEvent>(window));
    this->addEvents(result);
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
        w = std::make_shared<WindowButton>(*Locales::get()->get("language_was_changed"), *Locales::get()->get("OK"), clickEvent);
    }
    catch (LanguageAlreadyInUse&) {
        w = std::make_shared<WindowButton>(*Locales::get()->get("language_already_in_use"), *Locales::get()->get("OK"), clickEvent);
    }
    Events createWindowEvent = clickEvent;
    createWindowEvent.add(std::make_shared<CreateEEvent>(w));
    this->addEvents(createWindowEvent);
}