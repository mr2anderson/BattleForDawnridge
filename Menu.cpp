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


std::string Menu::run(sf::RenderWindow& window) {
	Music::get()->get("menu")->play();
	if (!this->graphicsInited) {
		this->initGraphics(window.getSize().x, window.getSize().y);
	}
	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				return "";
			}
			if (event.type == sf::Event::KeyPressed) {
				auto code = event.key.code;
				if (code == sf::Keyboard::Escape) {
					return "";
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				auto pos = sf::Mouse::getPosition();
                if (this->element == nullptr) {
                    if (this->start2on1pcButton.click(pos.x, pos.y)) {
                        Music::get()->get("menu")->stop();
                        return "ridge";
                    }
                    if (this->supportButton.click(pos.x, pos.y)) {
                        this->element = new SimpleWindow("click", "click", *Texts::get()->get("support"), *Texts::get()->get("close"));
                    }
                    if (this->creditsButton.click(pos.x, pos.y)) {
                        this->element = new SimpleWindow("click", "click", *Texts::get()->get("credits"), *Texts::get()->get("close"));
                    }
                    if (this->licenseButton.click(pos.x, pos.y)) {
                        this->element = new SimpleWindow("click", "click", *Texts::get()->get("license"), *Texts::get()->get("close"), 500, 375);
                    }
                    if (this->exitButton.click(pos.x, pos.y)) {
                        return "";
                    }
                    if (this->element != nullptr) {
                        this->handleEvent(this->element->run(window.getSize().x, window.getSize().y));
                    }
                }
                else {
                    this->handleEvent(this->element->click());
                    if (this->element->finished()) {
                        delete this->element;
                        this->element = nullptr;
                    }
                }
			}
		}
		this->drawEverything(window);
        if (this->element != nullptr) {
            this->element->update();
        }
	}
}
void Menu::initGraphics(uint32_t windowW, uint32_t windowH) {
	this->graphicsInited = true;
    this->element = nullptr;
	this->start2on1pcButton = Button(10, 10, 400, 60, std::nullopt, *Texts::get()->get("start_game_2p_1pc"));
    this->supportButton = Button(10, 80, 400, 60, std::nullopt, *Texts::get()->get("show_support"));
    this->creditsButton = Button(10, 150, 400, 60, std::nullopt, *Texts::get()->get("show_credits"));
    this->licenseButton = Button(10, 220, 400, 60, std::nullopt, *Texts::get()->get("show_license"));
	this->exitButton = Button(10, 290, 400, 60, std::nullopt, *Texts::get()->get("exit"));

	this->title.setFont(*Fonts::get()->get("1"));
	this->title.setString(*Texts::get()->get("title"));
	this->title.setCharacterSize(60);
	this->title.setFillColor(sf::Color::White);
	this->title.setPosition(windowW - 40 - title.getLocalBounds().width, windowH - 40 - title.getLocalBounds().height);
}
void Menu::drawEverything(sf::RenderWindow &window) {
	window.clear(UI_COLOR);
	window.draw(this->start2on1pcButton);
    window.draw(this->supportButton);
    window.draw(this->creditsButton);
    window.draw(this->licenseButton);
	window.draw(this->exitButton);
	window.draw(this->title);
    if (this->element != nullptr) {
        window.draw(*this->element);
    }
	window.display();
}
void Menu::handleEvent(const Event &e) {
    this->handleSoundEvent(e);
}
void Menu::handleSoundEvent(const Event &e) {
    for (const auto& a : e.playSound) {
        SoundQueue::get()->push(Sounds::get()->get(a));
    }
}