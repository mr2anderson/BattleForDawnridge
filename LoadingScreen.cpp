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


#include <cstdio>
#include "LoadingScreen.hpp"
#include "Textures.hpp"
#include "Fonts.hpp"
#include "Sounds.hpp"
#include "Music.hpp"
#include "Locales.hpp"
#include "Maps.hpp"
#include "Infantryman.hpp"
#include "Legioner.hpp"
#include "Knight.hpp"
#include "BlackKnight.hpp"
#include "PlainsGeneration.hpp"
#include "WindowButton.hpp"
#include "CouldntOpenTexture.hpp"
#include "CouldntOpenFont.hpp"
#include "CouldntOpenSound.hpp"
#include "CouldntOpenMusic.hpp"
#include "CouldntOpenLocales.hpp"
#include "CouldntOpenMap.hpp"
#include "UTFEncoder.hpp"
#include "ClueManager.hpp"
#include "Fire.hpp"
#include "House.hpp"
#include "PoisonFog.hpp"
#include "Parameters.hpp"
#include "CouldntOpenParameters.hpp"
#include "BigArrow.hpp"
#include "Playlist.hpp"
#include "Well.hpp"


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

    #if defined(_WIN32) // Unix does not support coloured cursors
        this->setCursor(window);
    #endif

    return true;
}
void LoadingScreen::setBaseScreen(sf::RenderWindow &window) {
	window.clear(sf::Color::Black);
	window.display();
}
bool LoadingScreen::loadBase(sf::RenderWindow &window) {
    try {
        Fonts::get()->add("1", "fonts/1.ttf");
        Textures::get()->add("loading_screen", "images/loading_screen.jpg");
    }
	catch (CouldntOpen&) {
        return false;
    }

    try {
        Locales::get()->load();
    }
	catch (CouldntOpen &e) {
        this->loadingError(&e, window);
        return false;
    }

    return true;
}
void LoadingScreen::setNormalScreen(sf::RenderWindow& window) {
    sf::Sprite s;
    s.setTexture(*Textures::get()->get("loading_screen"));
    s.setPosition(0, window.getSize().y - s.getLocalBounds().height);

	sf::Text t;
	t.setFont(*Fonts::get()->get("1"));
	t.setString(*Locales::get()->get("please_wait"));
	t.setCharacterSize(31);
	t.setFillColor(sf::Color::White);
	t.setOutlineColor(sf::Color::Black);
	t.setOutlineThickness(2);
	t.setPosition((window.getSize().x - t.getLocalBounds().width) / 2, window.getSize().y - t.getLocalBounds().height - 125);

	window.clear(sf::Color::Black);
    window.draw(s);
    window.draw(ClueManager::get()->getClueLabel(window.getSize().x, window.getSize().y));
	window.draw(t);
	window.display();
}
bool LoadingScreen::loadAll(sf::RenderWindow &window) {
    try {
        for (const std::string& a : {
                "castle", "exit_icon", "food_icon", "forest", "gold_icon", "iron",
                "market", "mine", "quarry", "sawmill", "stone", "stone_icon", "upgrade_icon",
                "wood_icon", "iron_icon", "shield_icon", "hammer_icon", "trade_icon",
                "wall1", "wall2", "mountains",
                "arable", "blue", "green",
                "purple", "warehouse_food", "warehouse_wood", "warehouse_stone",
                "warehouse_gold", "warehouse_iron", "resources_icon", "up_icon",
                "down_icon", "russian_icon", "english_icon", "bell", "destroy_icon",
                "btc", "producing_icon", "barracks", "treasure", "hand", "gates1", "gates2", "water",
                "forest_icon", "water_icon", "warrior_purple", "warrior_green", "warrior_blue",
                "cursor", "helmet", "skull", "resin", "spell_factory", "rage_spell", "poison_spell",
                "infirmary", "christianity", "tower1", "tower2", "big_arrow_icon", "crystal_icon", "warehouse_crystal"}) {
            Textures::get()->add(a, "images/" + a + ".png");
        }
        for (const std::string &a : {"none", "horizontal", "vertical", "all"}) {
            Textures::get()->add("road_" + a, "images/road/" + a + ".png");
        }
        Textures::get()->add("menu", "images/menu.jpg");
        for (const std::string& a : { "talking", "running", "attack", "been hit", "tipping over"}) {
            for (const std::string& d : { "n", "s", "w", "e", "nw", "ne", "sw", "se" }) {
                for (const std::tuple<std::string, uint32_t>& w : { 
                    std::make_tuple("infantryman", Infantryman().getAnimationNumber(a, d)), 
                    std::make_tuple("legioner", Legioner().getAnimationNumber(a, d)),
                    std::make_tuple("knight", Knight().getAnimationNumber(a, d)),
                    std::make_tuple("black_knight", BlackKnight().getAnimationNumber(a, d)) }) {
                    for (uint32_t i = 0; i < std::get<uint32_t>(w); i = i + 1) {
                        std::string s = std::to_string(i);
                        while (s.size() < 4) {
                            s = ('0' + s);
                        }
                        Textures::get()->add(std::get<std::string>(w) + " " + a + " " + d + std::to_string(i), "images/" + std::get<std::string>(w) + "/" + a + " " + d + s + ".png");
                    }
                }
            }
        }
        for (uint32_t i = 1; i <= PlainsGeneration::TOTAL_PLAINS; i = i + 1) {
            Textures::get()->add("plain" + std::to_string(i), "images/plains/" + std::to_string(i) + ".png");
        }
        for (uint32_t i = 1; i <= House::TOTAL_TYPES; i = i + 1) {
            Textures::get()->add("house" + std::to_string(i), "images/house/" + std::to_string(i) + ".png");
        }
        for (uint32_t i = 1; i <= Well::TOTAL_TYPES; i = i + 1) {
            Textures::get()->add("well" + std::to_string(i), "images/well/" + std::to_string(i) + ".png");
        }
        for (uint32_t i = 1; i <= PoisonFog::TOTAL_TYPES; i = i + 1) {
            Textures::get()->add("poison_fog" + std::to_string(i), "images/poison_fog/" + std::to_string(i) + ".png");
        }
        for (uint32_t i = 1; i <= BigArrow::TOTAL_TYPES; i = i + 1) {
            Textures::get()->add("big_arrow" + std::to_string(i), "images/big_arrow/" + std::to_string(i) + ".png");
        }
        for (uint32_t i = 1; i <= Fire::TOTAL_FRAMES; i = i + 1) {
            Textures::get()->add("fire" + std::to_string(i), "images/fire/" + std::to_string(i) + ".png");
        }


        for (const std::string& a : { "click", "food", "gold", "hooray", "iron",
                                      "regeneration", "stone", "wood", "road", "wind", "water",
                                      "destroy", "sword", "breath", "knight", "fire",
                                      "ouch", "bottles", "gurgle", "spell_factory", "rage_spell", "poison_fog",
                                      "infantryman", "heal", "building_hit", "bell", "crystal"}) {
            Sounds::get()->add(a, "sounds/" + a + ".ogg");
        }
        for (uint32_t i = 1; i <= Warrior::TOTAL_FOOTSTEPS; i = i + 1) {
            Sounds::get()->add("footsteps" + std::to_string(i), "sounds/footsteps/" + std::to_string(i) + ".ogg");
        }
        for (uint32_t i = 1; i <= BigArrow::TOTAL_SOUNDS; i = i + 1) {
            Sounds::get()->add("big_arrow" + std::to_string(i), "sounds/big_arrow/" + std::to_string(i) + ".ogg");
        }


        Music::get()->add("intro", "music/intro.ogg");
        Music::get()->add("menu", "music/menu.ogg");
        for (uint32_t i = 0; i < Playlist::SOUNDTRACKS_N; i = i + 1) {
            Music::get()->add(std::to_string(i), "music/ingame_0" + std::to_string(i) + ".ogg");
        }


        Parameters::get()->load();


        for (const std::string& a : { "ridge" }) {
            Maps::get()->add(a, "levels/" + a + ".tmx");
        }
    }
	catch (CouldntOpen &e) {
        loadingError(&e, window);
        return false;
    }


    return true;
}
void LoadingScreen::loadingError(LoadingError *e, sf::RenderWindow &window) {
    sf::Sprite s;
    s.setTexture(*Textures::get()->get("loading_screen"));
    s.setPosition(0, window.getSize().y - s.getLocalBounds().height);

    WindowButton element = WindowButton(UTFEncoder::get()->utf8ToUtf16(e->msg()), L"OK");
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
        window.clear(sf::Color::Black);
        window.draw(s);
        window.draw(element);
        window.display();
    }
}
void LoadingScreen::setCursor(sf::RenderWindow& window) {
    sf::Texture* cursorTexture = Textures::get()->get("cursor");
    sf::Image image = cursorTexture->copyToImage();
    const sf::Uint8* pixels = image.getPixelsPtr();

    sf::Cursor cursor;
    if (cursor.loadFromPixels(pixels, image.getSize(), sf::Vector2u(0, 0))) {
        window.setMouseCursor(cursor);
    }
}