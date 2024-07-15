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


#include "Menu.hpp"
#include "WindowTwoButtons.hpp"
#include "WindowButtonImage.hpp"
#include "Maps.hpp"
#include "CouldntOpenMap.hpp"
#include "SoundQueue.hpp"
#include "Playlist.hpp"
#include "ColorTheme.hpp"
#include "WindowButton.hpp"
#include "Sounds.hpp"
#include "Texts.hpp"
#include "Textures.hpp"
#include "Music.hpp"
#include "UTFEncoder.hpp"


Menu* Menu::singletone = nullptr;


std::shared_ptr<Map> Menu::run(sf::RenderWindow& window) {
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
            return nullptr;
        }
        if (!this->startGameMap.empty()) {
            try {
                std::shared_ptr<Map> map = Maps::get()->load(startGameMap);
                this->prepareToStartGame();
                return map;
            }
            catch (CouldntOpenMap &e) {
                this->startGameMap = "";

                Events clickEvent;
                clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

                Events createWindowEvent = clickEvent;
                std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("failed_to_load_map"), *Texts::get()->get("OK"), clickEvent);
                createWindowEvent.add(std::make_shared<CreateEEvent>(w));
                this->addEvents(createWindowEvent);
            }
        }
	}
}
void Menu::init(uint32_t windowW, uint32_t windowH) {
    Music::get()->get("menu")->play();

    this->closeMenu = false;
    this->startGameMap = "";
    this->windowW = windowW;
    this->windowH = windowH;
    this->element = nullptr;



    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));




    Events startGameOnRidgeEvent = clickEvent;
    startGameOnRidgeEvent.add(std::make_shared<StartGameEvent>("ridge"));

    std::shared_ptr<WindowTwoButtons> startGameOnRidgeVerifyWindow = std::make_shared<WindowTwoButtons>(*Texts::get()->get("local_ridge_verify"), *Texts::get()->get("yes"), *Texts::get()->get("no"), startGameOnRidgeEvent, clickEvent);
    Events createStartGameOnRidgeVerifyWindow = clickEvent;
    createStartGameOnRidgeVerifyWindow.add(std::make_shared<CreateEEvent>(startGameOnRidgeVerifyWindow));

    std::vector<HorizontalSelectionWindowComponent> chooseLevelWindowComponents;
    chooseLevelWindowComponents.emplace_back(
        "exit_icon",
        *Texts::get()->get("cancel"),
        true,
        clickEvent
    );
    chooseLevelWindowComponents.emplace_back(
        "ridge",
        *Texts::get()->get("ridge_description"),
        true,
        createStartGameOnRidgeVerifyWindow
    );

    std::shared_ptr<HorizontalSelectionWindow> chooseLevelWindow = std::make_shared<HorizontalSelectionWindow>(chooseLevelWindowComponents, Maps::THUMBNAIL_SIZE);
    Events createChooseLevelWindowEvent = clickEvent;
    createChooseLevelWindowEvent.add(std::make_shared<CreateEEvent>(chooseLevelWindow));
	this->buttons.emplace_back(std::make_shared<Label>(10, 10, 400, 60, *Texts::get()->get("start_game_local")), createChooseLevelWindowEvent);




    std::vector<HorizontalSelectionWindowComponent> educationWindowComponents;
    educationWindowComponents.emplace_back(
        "exit_icon",
        *Texts::get()->get("leave"),
        true,
        clickEvent
    );
    educationWindowComponents.emplace_back(
        "food_icon",
        *Texts::get()->get("guide_food_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "wood_icon",
        *Texts::get()->get("guide_wood_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "stone_icon",
        *Texts::get()->get("guide_stone_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "iron_icon",
        *Texts::get()->get("guide_iron_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "gold_icon",
        *Texts::get()->get("guide_gold_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "road",
        *Texts::get()->get("guide_road_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "shield_icon",
        *Texts::get()->get("guide_damage_mods_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "fire1",
        *Texts::get()->get("guide_fire_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "warehouse_food",
        *Texts::get()->get("guide_storage_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "wall2",
        *Texts::get()->get("guide_wall_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "forest_icon",
        *Texts::get()->get("guide_movement_cost_system_description"),
        false,
        Events()
    );
    educationWindowComponents.emplace_back(
        "water_icon",
        *Texts::get()->get("guide_obstacle_system_description"),
        false,
        Events()
    );

    std::shared_ptr<HorizontalSelectionWindow> educationWindow = std::make_shared<HorizontalSelectionWindow>(educationWindowComponents);
    Events createEducationWindowEvent = clickEvent;
    createEducationWindowEvent.add(std::make_shared<CreateEEvent>(educationWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 80, 400, 60, *Texts::get()->get("education")), createEducationWindowEvent);




    Events chooseLanguageEnglishEvent;
    chooseLanguageEnglishEvent.add(std::make_shared<ChooseLanguageEvent>("en.txt"));

    Events chooseLanguageRussianEvent;
    chooseLanguageRussianEvent.add(std::make_shared<ChooseLanguageEvent>("ru.txt"));

    std::vector<HorizontalSelectionWindowComponent> chooseLanguageWindowComponents;
    chooseLanguageWindowComponents.emplace_back(
        "exit_icon",
        *Texts::get()->get("cancel"),
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
    this->buttons.emplace_back(std::make_shared<Label>(10, 150, 400, 60, *Texts::get()->get("language")), createChooseLanguageWindowEvent);




    std::shared_ptr<WindowButtonImage> supportWindow = std::make_shared<WindowButtonImage>(*Texts::get()->get("support"), *Texts::get()->get("close"), "btc", clickEvent, 400, 300);
    Events supportEvent = clickEvent;
    supportEvent.add(std::make_shared<CreateEEvent>(supportWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 220, 400, 60,  *Texts::get()->get("show_support")), supportEvent);




    std::shared_ptr<WindowButton> creditsWindow = std::make_shared<WindowButton>(*Texts::get()->get("credits"), *Texts::get()->get("close"), clickEvent);
    Events creditsEvent = clickEvent;
    creditsEvent.add(std::make_shared<CreateEEvent>(creditsWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 290, 400, 60, *Texts::get()->get("show_credits")), creditsEvent);




    std::shared_ptr<WindowButton> licenseWindow = std::make_shared<WindowButton>(*Texts::get()->get("license"), *Texts::get()->get("close"), clickEvent, 600, 400);
    Events licenseEvent = clickEvent;
    licenseEvent.add(std::make_shared<CreateEEvent>(licenseWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 360, 400, 60, *Texts::get()->get("show_license")), licenseEvent);




    Events exitEvent;
    exitEvent.add(std::make_shared<CloseMenuEvent>());

    std::shared_ptr<WindowTwoButtons> confirmExitWindow = std::make_shared<WindowTwoButtons>(*Texts::get()->get("confirm_exit"), *Texts::get()->get("yes"), *Texts::get()->get("no"), exitEvent, clickEvent);
    Events createConfirmExitWindowEvent = clickEvent;
    createConfirmExitWindowEvent.add(std::make_shared<CreateEEvent>(confirmExitWindow));
	this->buttons.emplace_back(std::make_shared<Label>(10, 430, 400, 60, *Texts::get()->get("exit")), createConfirmExitWindowEvent);

    this->background.setTexture(*Textures::get()->get("menu"));
    this->background.setPosition(windowW - this->background.getLocalBounds().width, windowH - this->background.getLocalBounds().height);
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
    this->startGameMap = e->getMapName();
}
void Menu::handleChooseLanguageEvent(std::shared_ptr<ChooseLanguageEvent> e) {
    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));

    Texts::get()->setDefaultPath(e->getLocaleFile());
    std::shared_ptr<WindowButton> w = std::make_shared<WindowButton>(*Texts::get()->get("language_was_changed"), *Texts::get()->get("OK"), clickEvent);
    Events createWindowEvent = clickEvent;
    createWindowEvent.add(std::make_shared<CreateEEvent>(w));
    this->addEvents(createWindowEvent);
}