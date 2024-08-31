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
#include "WindowButton.hpp"
#include "ClueManager.hpp"
#include "Fire.hpp"
#include "House.hpp"
#include "Parameters.hpp"
#include "CouldntOpenParameters.hpp"
#include "BigArrow.hpp"
#include "Playlist.hpp"
#include "Well.hpp"
#include "Plant.hpp"
#include "Healer.hpp"
#include "HealerProjectile.hpp"
#include "Ram.hpp"
#include "BuildingStatePointer.hpp"
#include "ScreenAlreadyFinished.hpp"
#include "LoadingScreenBg.hpp"
#include "IsServerTable.hpp"
#include "MainServerPosition.hpp"
#include "Root.hpp"
#include "SoundQueue.hpp"
#include "Golem.hpp"
#include "IlluminationTableSettings.hpp"
#include "Patriarch.hpp"
#include "PatriarchProjectile.hpp"


#if defined(_WIN32) // Based on compilation system
#define DATA_ROOT "resources"
#define USERDATA_ROOT "userdata"
#else
#define DATA_ROOT "../resources"
#define USERDATA_ROOT "../userdata"
#endif


LoadingScreen::LoadingScreen(sf::RenderWindow &window) {
    this->alreadyFinished = false;
}
LoadingScreenResponse LoadingScreen::run(sf::RenderWindow &window) {
    if (this->alreadyFinished) {
        throw ScreenAlreadyFinished();
    }
    this->alreadyFinished = true;

    window.setMouseCursorVisible(false);

	this->setBaseScreen(window);
	if (!this->loadBase(window)) {
        return LoadingScreenResponse(LoadingScreenResponse::TYPE::LOADING_ERROR);
    }

	this->setNormalScreen(window);
	if (!this->loadAll(window)) {
        return LoadingScreenResponse(LoadingScreenResponse::TYPE::LOADING_ERROR);
    }

    sf::Texture* iconTexture = Textures::get().get("icon");
    sf::Image iconImage = iconTexture->copyToImage();
    const sf::Uint8* iconPixels = iconImage.getPixelsPtr();
    window.setIcon(iconImage.getSize().x, iconImage.getSize().y, iconPixels);

    return LoadingScreenResponse(LoadingScreenResponse::TYPE::OK);
}
void LoadingScreen::setBaseScreen(sf::RenderWindow &window) {
	window.clear(sf::Color::Black);
	window.display();
}
bool LoadingScreen::loadBase(sf::RenderWindow &window) {
    Root::get().setDataRoot(DATA_ROOT);
    Root::get().setUserdataRoot(USERDATA_ROOT);
    try {
        Fonts::get().add("1", "fonts/1.ttf");
        Textures::get().add("loading_screen", "images/backgrounds/loading_screen.jpg");
    }
	catch (CouldntOpen&) {
        return false;
    }
    try {
        Locales::get().load();
        return true;
    }
	catch (CouldntOpen &e) {
        this->loadingError(&e, window);
        return false;
    }
}
void LoadingScreen::setNormalScreen(sf::RenderWindow& window) {
    LoadingScreenBg bg;

	sf::Text t;
	t.setFont(*Fonts::get().get("1"));
	t.setString(StringLcl("{please_wait}").get());
	t.setCharacterSize(31);
	t.setFillColor(sf::Color::White);
	t.setOutlineColor(sf::Color::Black);
	t.setOutlineThickness(2);
	t.setPosition((window.getSize().x - t.getLocalBounds().width) / 2, window.getSize().y - t.getLocalBounds().height - 125);

	window.clear();
    window.draw(bg);
    window.draw(ClueManager::get().getClueLabel(window.getSize().x, window.getSize().y));
	window.draw(t);
	window.display();
}
bool LoadingScreen::loadAll(sf::RenderWindow &window) {
    try {
        IsServerTable::get().load();
        SoundQueue::get().loadVolume();
        Parameters::get().load();
        IlluminationTableSettings::get().load();
        MainServerPosition::get().load();
        Music::get().loadVolume();
        Music::get().add("intro", "music/intro.ogg");
        for (uint32_t i = 0; i < Playlist::SOUNDTRACKS_N; i = i + 1) {
            Music::get().add(std::to_string(i), "music/ingame_0" + std::to_string(i) + ".ogg");
        }
        Textures::get().add("bg", "images/backgrounds/bg.jpg");
        Music::get().add("menu", "music/menu.ogg");
        Textures::get().add("tmx_tileset", "images/tmx_tileset.jpg");
        for (const std::string& a : 
            {"axe", "battle", "bell", "christianity", "cross", "destroy", "gear", "hammer", "healer", "heart", "illumination_settings",
            "infantryman", "kill", "lord", "magick", "music", "new_turn", "priest", "resources", "room_id", "save", "shield", "slow_movement", "sockerer",
            "sound", "to_menu", "trade" }) {
            Textures::get().add(a + "_icon", "images/icons/" + a + "_icon.png");
        }
        for (const std::string& a : { "boost", "sfml" }) {
            Textures::get().add(a + "_icon", "images/icons/libs/" + a + "_icon.png");
        }
        for (const std::string& a : { "english", "russian" }) {
            Textures::get().add(a + "_icon", "images/icons/national/" + a + "_icon.png");
        }
        for (const std::string& a : { "food", "wood", "stone", "iron", "crystal", "gold", "helmet"}) {
            Textures::get().add(a + "_icon", "images/icons/resources/" + a + "_icon.png");
        }
        for (const std::string& a : { "positive", "negative" }) {
            for (const std::string& b : { "good", "bad", "neutral" }) {
                Textures::get().add(a + "_" + b + "_icon", "images/icons/times_of_day/" + a + "_" + b + "_icon.png");
            }
        }
        for (const std::string& a : { "down", "up", "exit" }) {
            Textures::get().add(a + "_icon", "images/icons/ui/" + a + "_icon.png");
        }
        for (const std::string& a :
            { "arable", "barracks", "castle", "gates1", "infirmary", "market", "mine", "quarry", "sawmill",
            "sockerer_house", "spell_factory", "tower1", "wall1", "warehouse_crystal", "warehouse_food", "warehouse_gold",
            "warehouse_iron", "warehouse_stone", "warehouse_wood", "workshop" }) {
            Textures::get().add(a, "images/buildings/main/" + a + ".png");
        }
        for (uint32_t i = 1; i <= House::TOTAL_TYPES; i = i + 1) {
            Textures::get().add("house" + std::to_string(i), "images/buildings/main/house/" + std::to_string(i) + ".png");
        }
        for (uint32_t i = 1; i <= Well::TOTAL_TYPES; i = i + 1) {
            Textures::get().add("well" + std::to_string(i), "images/buildings/main/well/" + std::to_string(i) + ".png");
        }
        for (const std::string& a : { "none", "horizontal", "vertical", "all" }) {
            Textures::get().add("road_" + a, "images/buildings/main/road/" + a + ".png");
        }
        for (uint32_t i = 1; i <= Fire::TOTAL_FRAMES; i = i + 1) {
            Textures::get().add("fire" + std::to_string(i), "images/buildings/fire/" + std::to_string(i) + ".png");
        }
        for (uint32_t i = 0; i < BuildingStatePointer::TOTAL_HP_POINTERS; i = i + 1) {
            Textures::get().add("building_hp_pointer" + std::to_string(i), "images/buildings/hp_pointers/" + std::to_string(i) + ".png");
        }
        for (const std::string& a : { "blue", "green", "purple" }) {
            Textures::get().add(a, "images/buildings/side_pointers/" + a + ".png");
        }
        for (const std::string& a : { "forest", "iron", "mountains", "stone", "treasure", "water" }) {
            Textures::get().add(a, "images/landscape/" + a + ".png");
        }
        for (uint32_t i = 1; i <= Plant::TOTAL_TYPES; i = i + 1) {
            for (uint32_t j = 1; j <= Plant::ANIMATION_NUMBER[i - 1]; j = j + 1) {
                Textures::get().add("plant" + std::to_string(i) + "_" + std::to_string(j), "images/landscape/plant/" + std::to_string(i) + "/" + std::to_string(j) + ".png");
            }
        }
        for (uint32_t i = 1; i <= BigArrow::TOTAL_TYPES; i = i + 1) {
            Textures::get().add("big_arrow" + std::to_string(i), "images/projectiles/big_arrow/" + std::to_string(i) + ".png");
        }
        for (uint32_t i = 1; i <= HealerProjectile::TOTAL_TYPES; i = i + 1) {
            Textures::get().add("healer_projectile" + std::to_string(i), "images/projectiles/healer_projectile/" + std::to_string(i) + ".png");
        }
        for (uint32_t i = 1; i <= PatriarchProjectile::TOTAL_TYPES; i = i + 1) {
            Textures::get().add("patriarch_projectile" + std::to_string(i), "images/projectiles/patriarch_projectile/" + std::to_string(i) + ".png");
        }
        for (const std::string& a : { "heal", "rage" }) {
            Textures::get().add(a + "_spell", "images/spells/" + a + "_spell.png");
        }
        for (const std::string& a : { "dawn", "morning", "day", "sunset", "night1", "night2" }) {
            Textures::get().add(a, "images/times_of_day/" + a + ".png");
        }
        for (const std::string& a : { "blue", "green", "purple" }) {
            for (const std::string& b : { "", "0", "1" }) {
                Textures::get().add("warrior_" + a + b, "images/warriors/side_pointers/warrior_" + a + b + ".png");
            }
        }
        for (const std::string& a : { "talking", "running", "attack", "been hit", "tipping over" }) {
            for (const std::string& d : { "n", "s", "w", "e", "nw", "ne", "sw", "se" }) {
                for (std::tuple<std::string, uint32_t> w : {
                    std::make_tuple("infantryman", Infantryman().getAnimationNumber(a, d)),
                    std::make_tuple("legioner", Legioner().getAnimationNumber(a, d)),
                    std::make_tuple("knight", Knight().getAnimationNumber(a, d)),
                    std::make_tuple("black_knight", BlackKnight().getAnimationNumber(a, d)),
                    std::make_tuple("healer", Healer().getAnimationNumber(a, d)),
                    std::make_tuple("patriarch", Patriarch().getAnimationNumber(a, d)),
                    std::make_tuple("ram", Ram().getAnimationNumber(a, d)),
                    std::make_tuple("golem", Golem().getAnimationNumber(a, d)) }) {
                    for (uint32_t i = 0; i < std::get<uint32_t>(w); i = i + 1) {
                        std::string s = std::to_string(i);
                        while (s.size() < 4) {
                            s = ('0' + s);
                        }
                        Textures::get().add(std::get<std::string>(w) + " " + a + " " + d + std::to_string(i), "images/warriors/main/" + std::get<std::string>(w) + "/" + a + " " + d + s + ".png");
                    }
                }
            }
        }
        for (const std::string& a : { "btc", "hand", "plain", "icon"}) {
            Textures::get().add(a, "images/" + a + ".png");
        }
        for (const std::string& a : { "tmx_tileset" }) {
            Textures::get().add(a, "images/" + a + ".jpg");
        }
        for (const std::string& a : { "click", "food", "gold", "hooray", "iron",
                                      "regeneration", "stone", "wood", "road", "wind", "water",
                                      "destroy", "sword", "breath", "knight", "fire",
                                      "ouch", "spell_factory", "rage_spell",
                                      "infantryman", "heal", "building_hit", "bell", "crystal", "swing",
                                      "ouch_woman_cute", "healer", "vehicle_hit",
                                      "vehicle", "sockerer_house", "roar", "dawn",
                                      "day", "night", "sunset", "ouch_old", "old", "patriarch_heal",
                                      "wardrums"}) {
            Sounds::get().add(a, "sounds/" + a + ".ogg");
        }
        for (uint32_t i = 1; i <= Warrior::TOTAL_FOOTSTEPS; i = i + 1) {
            Sounds::get().add("footsteps" + std::to_string(i), "sounds/footsteps/" + std::to_string(i) + ".ogg");
        }
        for (uint32_t i = 1; i <= Warrior::TOTAL_WINGS; i = i + 1) {
            Sounds::get().add("wings" + std::to_string(i), "sounds/wings/" + std::to_string(i) + ".ogg");
        }
        for (uint32_t i = 1; i <= BigArrow::TOTAL_SOUNDS; i = i + 1) {
            Sounds::get().add("big_arrow" + std::to_string(i), "sounds/big_arrow/" + std::to_string(i) + ".ogg");
        }
        for (const std::string& a : { "ridge", "lake", "river" }) {
            Maps::get().add(a, "levels/" + a + ".tmx");
        }
        return true;
    }
	catch (CouldntOpen &e) {
        loadingError(&e, window);
        return false;
    }
}
void LoadingScreen::loadingError(CouldntOpen *e, sf::RenderWindow &window) {
    window.setMouseCursorVisible(true);

    sf::Sprite s;
    s.setTexture(*Textures::get().get("loading_screen"));
    s.setPosition(0, window.getSize().y - s.getLocalBounds().height);

    WindowButton element = WindowButton(StringLcl("Couldn't open " + e->getResourceName() + "\nPath: " + e->getPath()), StringLcl("OK")); // Avoid using locales cuz they can be not loaded yet
    sf::Event event;
    for (; ;) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    return;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                Events events = element.click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y, window.getSize().x, window.getSize().y);
                if (!events.empty()) {
                    return;
                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(s);
        window.draw(element);
        window.display();
    }
}