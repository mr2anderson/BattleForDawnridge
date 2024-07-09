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


#include "LoadingScreen.hpp"


LoadingScreen* LoadingScreen::singletone = nullptr;


bool LoadingScreen::run(sf::RenderWindow &window) {
	this->setBaseScreen(window);
	if (!this->loadBase(window)) {
        return false;
    }

	this->setNormalScreen(window);
	if (!this->loadAll(window)) {
        return false;
    }

    return true;
}
void LoadingScreen::setBaseScreen(sf::RenderWindow &window) {
	window.clear(COLOR_THEME::UI_COLOR);
	window.display();
}
bool LoadingScreen::loadBase(sf::RenderWindow &window) {
    try {
        Fonts::get()->add("1", "1.ttf");
        Sounds::get()->add("click", "click.ogg");
    }
	catch (CouldntOpenFont &e) {
        return false;
    }
    catch (CouldntOpenSound &e) {
        return false;
    }

    try {
        Texts::get()->load("en.txt");
    }
	catch (CouldntOpenText &e) {
        this->loadingError(&e, window);
        return false;
    }
    catch (TextRedefinition &e) {
        this->loadingError(&e, window);
        return false;
    }

    return true;
}
void LoadingScreen::setNormalScreen(sf::RenderWindow& window) {
	sf::Text t;
	t.setFont(*Fonts::get()->get("1"));
	t.setString(*Texts::get()->get("please_wait"));
	t.setCharacterSize(32);
	t.setFillColor(sf::Color::White);
	t.setOutlineColor(sf::Color::Black);
	t.setOutlineThickness(2);
	t.setPosition((window.getSize().x - t.getLocalBounds().width) / 2, window.getSize().y - t.getLocalBounds().height - 50);

	window.clear(COLOR_THEME::UI_COLOR);
	window.draw(t);
	window.display();
}
bool LoadingScreen::loadAll(sf::RenderWindow &window) {
    try {
        for (const std::string& a : {
                "castle", "exit_icon", "food_icon", "forest", "gold_icon", "iron",
                "market", "mine", "quarry", "sawmill", "stone", "stone_icon", "upgrade_icon",
                "wood_icon", "iron_icon", "shield_icon", "hammer_icon", "trade_icon",
                "wall1", "wall2", "road", "mountains",
                "windmill", "arable", "black", "blue", "green", "grey",
                "purple", "yellow", "warehouse_food", "warehouse_wood", "warehouse_stone",
                "warehouse_gold", "warehouse_iron", "resources_icon", "up_icon",
                "down_icon"}) {
            Textures::get()->add(a, a + ".png");
        }
        for (uint32_t i = 1; i <= PlainsGeneration::TOTAL_PLAINS; i = i + 1) {
            Textures::get()->add(std::to_string(i), std::to_string(i) + ".png");
        }
        for (uint32_t i = 0; i < Water::TOTAL_ANIMATIONS; i = i + 1) {
            std::string s = std::to_string(i);
            while (s.size() < 4) {
                s = ('0' + s);
            }
            Textures::get()->add("water" + std::to_string(i), "water/" + s + ".png");
        }
    }
	catch (CouldntOpenTexture &e) {
        loadingError(&e, window);
        return false;
    }

    try {
        for (const std::string& a : { "click", "food", "gold", "hooray", "iron",
                                      "regeneration", "stone", "wood", "road", "wind", "water"}) {
            Sounds::get()->add(a, a + ".ogg");
        }
    }
    catch (CouldntOpenSound &e) {
        loadingError(&e, window);
        return false;
    }

    try {
        Music::get()->add("intro", "intro.ogg");
        Music::get()->add("menu", "menu.ogg");
        for (uint32_t i = 0; i < 10; i = i + 1) {
            Music::get()->add(std::to_string(i), "ingame_0" + std::to_string(i) + ".ogg");
        }
    }
	catch (CouldntOpenMusic &e) {
        loadingError(&e, window);
        return false;
    }

    try {
        for (const std::string& a : { "ridge" }) {
            Maps::get()->add(a, a + ".tmx");
        }
    }
    catch (CouldntOpenMap& e) {
        loadingError(&e, window);
        return false;
    }

    return true;
}
void LoadingScreen::loadingError(LoadingError *e, sf::RenderWindow &window) {
    WindowButton element = WindowButton("click", "click", UTFEncoder::get()->utf8ToUtf16(e->msg()), L"OK");
    element.run(window.getSize().x, window.getSize().y);
    sf::Event event;
    while (!element.finished()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    return;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                element.click();
            }
        }
        window.clear(COLOR_THEME::UI_COLOR);
        window.draw(element);
        window.display();
    }
}