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
#include <iostream>
#include "Menu.hpp"
#include "Maps.hpp"
#include "SoundQueue.hpp"
#include "ColorTheme.hpp"
#include "WindowButton.hpp"
#include "MenuBg.hpp"
#include "Sounds.hpp"
#include "Locales.hpp"
#include "Textures.hpp"
#include "Music.hpp"
#include "FirstTimeTipsTable.hpp"
#include "LanguageAlreadyInUse.hpp"
#include "Root.hpp"
#include "IsServerTable.hpp"
#include "ScreenAlreadyFinished.hpp"
#include "LocalGameButtonSpec.hpp"
#include "NetworkGameButtonSpec.hpp"
#include "GuideButtonSpec.hpp"
#include "LanguageButtonSpec.hpp"
#include "SupportButtonSpec.hpp"
#include "CreditsButtonSpec.hpp"
#include "LicenceButtonSpec.hpp"
#include "ExitButtonSpec.hpp"


#define USE_TEST_MAP


Menu::Menu(sf::RenderWindow& window, const boost::optional<StringLcl>& additionalWelcomeMsg) {
    this->alreadyFinished = false;
    this->closeMenu = false;
    this->element = nullptr;
    this->generateButtons();

    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));
    this->addEvents(clickEvent);
    if (!FirstTimeTipsTable::get()->wasDisplayed("welcome")) {
        FirstTimeTipsTable::get()->markAsDisplayed("welcome");
        std::shared_ptr<WindowButton> welcomeWindow = std::make_shared<WindowButton>(StringLcl("{welcome_first_time}"), StringLcl("{OK}"), clickEvent);
        Events createWelcomeWindowEvent;
        createWelcomeWindowEvent.add(std::make_shared<CreateEEvent>(welcomeWindow));
        this->addEvents(createWelcomeWindowEvent);
    }
    if (additionalWelcomeMsg.has_value()) {
        std::shared_ptr<WindowButton> msgWindow = std::make_shared<WindowButton>(additionalWelcomeMsg.value(), StringLcl("{OK}"), clickEvent);
        Events createMsgWindowEvent;
        createMsgWindowEvent.add(std::make_shared<CreateEEvent>(msgWindow));
        this->addEvents(createMsgWindowEvent);
    }
}
MenuResponse Menu::run(sf::RenderWindow& window) {
    if (this->alreadyFinished) {
        throw ScreenAlreadyFinished();
    }
    this->alreadyFinished = true;

    window.setMouseCursorVisible(true);
    Music::get()->play("menu");

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

        this->regenerateButtons();

		this->drawEverything(window);

        if (this->element != nullptr) {
            this->element->update();
            if (this->element->finished()) {
                this->element = nullptr;
            }
        }
        this->processEvents();

        if (this->closeMenu) {
            return MenuResponse(MenuResponse::TYPE::EXIT, "");
        }
        if (this->response.has_value()) {
            return this->response.value();
        }
	}
}
void Menu::generateButtons() {
    this->buttons = {
        Button(LocalGameButtonSpec(0)),
        Button(NetworkGameButtonSpec(1)),
        Button(GuideButtonSpec(2)),
        Button(LanguageButtonSpec(3)),
        Button(SupportButtonSpec(4)),
        Button(CreditsButtonSpec(5)),
        Button(LanguageButtonSpec(6)),
        Button(ExitButtonSpec(7))
    };
}
void Menu::regenerateButtons() {
    std::set<std::string> saveNames;
    for (const auto& entry : std::filesystem::directory_iterator(USERDATA_ROOT + "/saves")) {
        if (entry.is_regular_file() and entry.path().extension() == ".save") {
            saveNames.insert(entry.path().filename().string());
        }
    }

    if (!this->prevSavesNumber.has_value() or this->prevSavesNumber.value() != saveNames.size()) {

        std::vector<std::string> mapNames;
#if defined(USE_TEST_MAP)
        mapNames.push_back("test");
#endif
        mapNames.push_back("ridge");

        this->buttons.at(0) = LocalGameButtonSpec(0, mapNames, saveNames);
        this->buttons.at(1) = NetworkGameButtonSpec(1, mapNames, saveNames);
    }
}
void Menu::drawEverything(sf::RenderWindow &window) {
    window.clear();
    window.draw(MenuBg());
    this->drawButtons(window);
    if (this->element != nullptr) {
        window.draw(*this->element);
    }
	window.display();
}
void Menu::drawButtons(sf::RenderWindow& window) {
    for (const auto& b : this->buttons) {
        window.draw(b);
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
    else if (std::shared_ptr<StartLocalGameEvent> startGameEvent = std::dynamic_pointer_cast<StartLocalGameEvent>(e)) {
        this->handleStartLocalGameEvent(startGameEvent);
    }
    else if (std::shared_ptr<LoadLocalGameEvent> loadGameEvent = std::dynamic_pointer_cast<LoadLocalGameEvent>(e)) {
        this->handleLoadLocalGameEvent(loadGameEvent);
    }
    else if (std::shared_ptr<StartNetworkGameEvent> startGameEvent = std::dynamic_pointer_cast<StartNetworkGameEvent>(e)) {
        this->handleStartNetworkGameEvent(startGameEvent);
    }
    else if (std::shared_ptr<LoadNetworkGameEvent> loadGameEvent = std::dynamic_pointer_cast<LoadNetworkGameEvent>(e)) {
        this->handleLoadNetworkGameEvent(loadGameEvent);
    }
    else if (std::shared_ptr<DeleteSaveEvent> deleteSaveEvent = std::dynamic_pointer_cast<DeleteSaveEvent>(e)) {
        this->handleDeleteSaveEvent(deleteSaveEvent);
    }
    else if (std::shared_ptr<ChooseLanguageEvent> chooseLanguageEvent = std::dynamic_pointer_cast<ChooseLanguageEvent>(e)) {
        this->handleChooseLanguageEvent(chooseLanguageEvent);
    }
    else {
        std::cerr << "Menu: warning: unknown event handled" << std::endl;
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
void Menu::handleStartLocalGameEvent(std::shared_ptr<StartLocalGameEvent> e) {
    this->response = MenuResponse(MenuResponse::TYPE::START_LOCAL_GAME, e->getMapName());
}
void Menu::handleLoadLocalGameEvent(std::shared_ptr<LoadLocalGameEvent> e) {
    this->response = MenuResponse(MenuResponse::TYPE::LOAD_LOCAL_GAME, e->getSaveName());
}
void Menu::handleStartNetworkGameEvent(std::shared_ptr<StartNetworkGameEvent> e) {
    this->response = MenuResponse(MenuResponse::TYPE::START_NETWORK_GAME, e->getMapName());
}
void Menu::handleLoadNetworkGameEvent(std::shared_ptr<LoadNetworkGameEvent> e) {
    this->response = MenuResponse(MenuResponse::TYPE::LOAD_NETWORK_GAME, e->getSaveName());
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