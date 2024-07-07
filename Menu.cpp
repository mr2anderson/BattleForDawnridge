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


Map* Menu::run(sf::RenderWindow& window) {
    this->init(window.getSize().x, window.getSize().y);
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed) {
                if (this->elements.empty()) {
                    this->handleButtonsClick();
                }
                else {
                    this->handleEvent(this->elements.front()->click());
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
        if (this->startGame) {
            try {
                Map *map = Maps::get()->load("ridge");
                this->prepareToStartGame();
                return map;
            }
            catch (CouldntOpenMap &e) {
                this->addElement(std::make_shared<WindowButton>("click", "click", *Texts::get()->get("failed_to_load_map"), *Texts::get()->get("OK")));
            }
        }
	}
}
void Menu::init(uint32_t windowW, uint32_t windowH) {
    this->closeMenu = false;
    this->startGame = false;
    this->windowW = windowW;
    this->windowH = windowH;

    Event startGameEvent;
    startGameEvent.addStartGameEvent();
    startGameEvent.addPlaySoundEvent("click");
	this->buttons.emplace_back(std::make_shared<Label>(10, 10, 400, 60, *Texts::get()->get("start_game_2p_1pc")), startGameEvent);

    std::shared_ptr<WindowButton> supportWindow = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("support"), *Texts::get()->get("close"));
    Event supportEvent;
    supportEvent.addCreateEEvent(supportWindow);
    this->buttons.emplace_back(std::make_shared<Label>(10, 80, 400, 60,  *Texts::get()->get("show_support")), supportEvent);

    std::shared_ptr<WindowButton> creditsWindow = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("credits"), *Texts::get()->get("close"));
    Event creditsEvent;
    creditsEvent.addCreateEEvent(creditsWindow);
    this->buttons.emplace_back(std::make_shared<Label>(10, 150, 400, 60, *Texts::get()->get("show_credits")), creditsEvent);

    std::shared_ptr<WindowButton> licenseWindow = std::make_shared<WindowButton>("click", "click", *Texts::get()->get("license"), *Texts::get()->get("close"), 500, 400);
    Event licenseEvent;
    licenseEvent.addCreateEEvent(licenseWindow);
    this->buttons.emplace_back(std::make_shared<Label>(10, 220, 400, 60, *Texts::get()->get("show_license")), licenseEvent);

    Event exitEvent;
    exitEvent.addCloseMenuEvent();
	this->buttons.emplace_back(std::make_shared<Label>(10, 290, 400, 60, *Texts::get()->get("returnToMenu")), exitEvent);

	this->title.setFont(*Fonts::get()->get("1"));
	this->title.setString(*Texts::get()->get("title"));
	this->title.setCharacterSize(60);
	this->title.setFillColor(sf::Color::White);
	this->title.setPosition(windowW - 40 - title.getLocalBounds().width, windowH - 40 - title.getLocalBounds().height);
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
        this->handleEvent(this->elements.front()->run(this->windowW, this->windowH));
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
        this->handleEvent(this->elements.front()->run(this->windowW, this->windowH));
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
        Event event = b.click();
        if (!event.empty()) {
            this->handleEvent(event);
            return true;
        }
    }
    return false;
}
void Menu::handleEvent(const Event &e) {
    this->handleSoundEvent(e);
    this->handleCreateEEvent(e);
    this->handleExitEvent(e);
    this->handleStartGameEvent(e);
}
void Menu::handleSoundEvent(const Event &e) {
    const std::vector<std::string>* playSound = e.getPlaySoundEvent();
    for (uint32_t i = 0; i < playSound->size(); i = i + 1) {
        SoundQueue::get()->push(Sounds::get()->get(playSound->at(i)));
    }
}
void Menu::handleCreateEEvent(const Event &e) {
    const std::vector<std::shared_ptr<PopUpElement>>* toCreate = e.getCreateEEvent();
    for (uint32_t i = 0; i < toCreate->size(); i = i + 1) {
        this->addElement(toCreate->at(i));
    }
}
void Menu::handleExitEvent(const Event &e) {
    if (e.getCloseMenuEvent()) {
        this->closeMenu = true;
    }
}
void Menu::handleStartGameEvent(const Event &e) {
    if (e.getStartGameEvent()) {
        this->startGame = true;
    }
}