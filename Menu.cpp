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


Menu* Menu::singletone = nullptr;


std::shared_ptr<Map> Menu::run(sf::RenderWindow& window) {
    this->init(window.getSize().x, window.getSize().y);
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left) {
                if (this->elements.empty()) {
                    this->handleButtonsClick();
                }
                else if (!this->elements.empty()) {
                    Events events = this->elements.front()->click();
                    this->handleEvent(events);
                }
			}
		}
		this->drawEverything(window);
        this->removeFinishedElements();
        if (!this->elements.empty()) {
            this->elements.front()->update();
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
                this->addElement(std::make_shared<WindowButton>("click", "click", *Texts::get()->get("failed_to_load_map"), *Texts::get()->get("OK")));
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

    Events startGameOnRidgeEvent;
    startGameOnRidgeEvent.add(std::make_shared<StartGameEvent>("ridge"));

    std::vector<HorizontalSelectionWindowComponent> chooseLevelWindowComponents;
    chooseLevelWindowComponents.emplace_back(
        "exit_icon",
        *Texts::get()->get("cancel"),
        true,
        true,
        Events()
    );
    chooseLevelWindowComponents.emplace_back(
        "ridge",
        *Texts::get()->get("ridge_description"),
        true,
        false,
        startGameOnRidgeEvent
    );
    std::shared_ptr<HorizontalSelectionWindow> chooseLevelWindow = std::make_shared<HorizontalSelectionWindow>("click", "click", chooseLevelWindowComponents, Maps::THUMBNAIL_SIZE);
    Events createChooseLevelWindowEvent;
    createChooseLevelWindowEvent.add(std::make_shared<CreateEEvent>(chooseLevelWindow));

	this->buttons.emplace_back(std::make_shared<Label>(10, 10, 400, 60, *Texts::get()->get("start_game_local")), createChooseLevelWindowEvent);

    std::shared_ptr<WindowButton> supportWindow = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("support"), *Texts::get()->get("close"));
    Events supportEvent;
    supportEvent.add(std::make_shared<CreateEEvent>(supportWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 80, 400, 60,  *Texts::get()->get("show_support")), supportEvent);

    std::shared_ptr<WindowButton> creditsWindow = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("credits"), *Texts::get()->get("close"));
    Events creditsEvent;
    creditsEvent.add(std::make_shared<CreateEEvent>(creditsWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 150, 400, 60, *Texts::get()->get("show_credits")), creditsEvent);

    std::shared_ptr<WindowButton> licenseWindow = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("license"), *Texts::get()->get("close"), 500, 400);
    Events licenseEvent;
    licenseEvent.add(std::make_shared<CreateEEvent>(licenseWindow));
    this->buttons.emplace_back(std::make_shared<Label>(10, 220, 400, 60, *Texts::get()->get("show_license")), licenseEvent);

    Events exitEvent;
    exitEvent.add(std::make_shared<CloseMenuEvent>());

    std::shared_ptr<WindowTwoButtons> confirmExitWindow = std::make_shared<WindowTwoButtons>("click", "click", *Texts::get()->get("confirm_exit"), *Texts::get()->get("yes"), *Texts::get()->get("no"), exitEvent, Events());
    Events createConfirmExitWindowEvent;
    createConfirmExitWindowEvent.add(std::make_shared<CreateEEvent>(confirmExitWindow));

	this->buttons.emplace_back(std::make_shared<Label>(10, 290, 400, 60, *Texts::get()->get("returnToMenu")), createConfirmExitWindowEvent);

	this->title.setFont(*Fonts::get()->get("1"));
	this->title.setString(*Texts::get()->get("title"));
	this->title.setCharacterSize(60);
	this->title.setFillColor(sf::Color::White);
	this->title.setPosition(windowW - 40 - title.getLocalBounds().width, 40);
}
void Menu::drawEverything(sf::RenderWindow &window) {
	window.clear(COLOR_THEME::UI_COLOR);
	for (const auto& b : this->buttons) {
        window.draw(b);
    }
	window.draw(this->title);
    if (!this->elements.empty()) {
        window.draw(*this->elements.front());
    }
	window.display();
}
void Menu::addElement(std::shared_ptr<PopUpElement> e) {
    this->elements.push(e);
    if (this->elements.size() == 1) {
        Events events = this->elements.front()->run(this->windowW, this->windowH);
        this->handleEvent(events);
    }
}
void Menu::removeFinishedElements() {
    bool removed = false;
    while (!this->elements.empty()) {
        if (!this->elements.front()->finished()) {
            break;
        }
        this->elements.pop();
        removed = true;
    }
    if (removed and !this->elements.empty()) {
        Events events = this->elements.front()->run(this->windowW, this->windowH);
        this->handleEvent(events);
    }
}
void Menu::prepareToStartGame() {
    Music::get()->get("menu")->stop();
    while (!this->elements.empty()) {
        this->elements.pop();
    }
    this->buttons.clear();
}
bool Menu::handleButtonsClick() {
    for (const auto& b : this->buttons) {
        Events event = b.click();
        if (!event.empty()) {
            this->handleEvent(event);
            return true;
        }
    }
    return false;
}
void Menu::handleEvent(Events &e) {
    for (uint32_t i = 0; i < e.size(); i = i + 1) {
        if (std::shared_ptr<PlaySoundEvent> playSoundEvent = std::dynamic_pointer_cast<PlaySoundEvent>(e.at(i))) {
            this->handleSoundEvent(playSoundEvent);
        }
        else if (std::shared_ptr<CreateEEvent> createEEvent = std::dynamic_pointer_cast<CreateEEvent>(e.at(i))) {
            this->handleCreateEEvent(createEEvent);
        }
        else if (std::shared_ptr<CloseMenuEvent> closeMenuEvent = std::dynamic_pointer_cast<CloseMenuEvent>(e.at(i))) {
            this->handleCloseMenuEvent(closeMenuEvent);
        }
        else if (std::shared_ptr<StartGameEvent> startGameEvent = std::dynamic_pointer_cast<StartGameEvent>(e.at(i))) {
            this->handleStartGameEvent(startGameEvent);
        }
    }
}
void Menu::handleSoundEvent(std::shared_ptr<PlaySoundEvent> e) {
    SoundQueue::get()->push(Sounds::get()->get(e->getSoundName()));
}
void Menu::handleCreateEEvent(std::shared_ptr<CreateEEvent> e) {
    this->addElement(e->getElement());
}
void Menu::handleCloseMenuEvent(std::shared_ptr<CloseMenuEvent> e) {
    this->closeMenu = true;
}
void Menu::handleStartGameEvent(std::shared_ptr<StartGameEvent> e) {
    this->startGameMap = e->getMapName();
}