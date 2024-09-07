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
#include "LoadingBar.hpp"
#include "Killer.hpp"
#include "Barbar.hpp"
#include "Princess.hpp"
#include "Hunter.hpp"
#include "HunterArrow.hpp"
#include "PoisonerArrow.hpp"
#include "Poisoner.hpp"


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

	if (!this->loadBase(window)) {
        return LoadingScreenResponse(LoadingScreenResponse::TYPE::LOADING_ERROR);
    }

    window.setMouseCursorVisible(false);
    SoundQueue::get().push(Sounds::get().get("wardrums"));

    sf::Event event;

    uint32_t percent = 0;
    while (percent != 100) {
        this->setNormalScreen(window, percent);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape) {
                return LoadingScreenResponse(LoadingScreenResponse::TYPE::EXIT);
            }
        }
        if (!this->loadAll(window, percent)) {
            return LoadingScreenResponse(LoadingScreenResponse::TYPE::LOADING_ERROR);
        }
    }

    sf::Texture* iconTexture = Textures::get().get("icon");
    sf::Image iconImage = iconTexture->copyToImage();
    const sf::Uint8* iconPixels = iconImage.getPixelsPtr();
    window.setIcon(iconImage.getSize().x, iconImage.getSize().y, iconPixels);

    return LoadingScreenResponse(LoadingScreenResponse::TYPE::OK);
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
        Textures::get().add("loading_bar0", "images/loading_bar/loading_bar0.png");
        Textures::get().add("loading_bar1", "images/loading_bar/loading_bar1.png");
        SoundQueue::get().loadVolume();
        Sounds::get().add("wardrums", "sounds/wardrums.ogg");
        return true;
    }
	catch (CouldntOpen &e) {
        this->loadingError(&e, window);
        return false;
    }
}
void LoadingScreen::setNormalScreen(sf::RenderWindow& window, uint32_t percent) {
    LoadingScreenBg bg;

	sf::Text t;
	t.setFont(*Fonts::get().get("1"));
	t.setString(StringLcl("{please_wait}").getWithoutStyles());
	t.setCharacterSize(31);
	t.setFillColor(sf::Color::White);
	t.setOutlineColor(sf::Color::Black);
	t.setOutlineThickness(2);
	t.setPosition((window.getSize().x - t.getGlobalBounds().width) / 2, window.getSize().y - t.getGlobalBounds().height - 125);

    LoadingBar b;
    b.setCenter(t.getPosition().x + t.getGlobalBounds().width / 2, t.getPosition().y + 100);
    b.setValue(percent);

	window.clear();
    window.draw(bg);
    window.draw(ClueManager::get().getClueLabel(window.getSize().x, window.getSize().y));
    window.draw(b);
	window.draw(t);
	window.display();
}
template<typename T> static void LOAD_WARRIOR_DATA() {
    for (const std::string& a : { "talking", "running", "attack", "been hit", "tipping over", "inspire" }) {
        for (const std::string& d : { "n", "s", "w", "e", "nw", "ne", "sw", "se" }) {
                for (uint32_t i = 0; i < T().getAnimationNumber(a, d); i = i + 1) {
                    std::string s = std::to_string(i);
                    while (s.size() < 4) {
                        s = ('0' + s);
                    }
                    Textures::get().add(T().getBaseTextureName() + " " + a + " " + d + std::to_string(i), "images/warriors/main/" + T().getBaseTextureName() + "/" + a + " " + d + s + ".png");
                }
            }
        }
}
bool LoadingScreen::loadAll(sf::RenderWindow &window, uint32_t &percent) {
    try {
        if (percent == 0) {
            percent = 5;
            IsServerTable::get().load();
            Parameters::get().load();
            IlluminationTableSettings::get().load();
            MainServerPosition::get().load();
            Textures::get().add("bg", "images/backgrounds/bg.jpg");
        }
        else if (percent == 5) {
            percent = 10;
            Music::get().loadVolume();
            Music::get().add("intro", "music/intro.ogg");
            for (uint32_t i = 0; i < Playlist::SOUNDTRACKS_N; i = i + 1) {
                Music::get().add(std::to_string(i), "music/ingame_0" + std::to_string(i) + ".ogg");
            }
            Music::get().add("menu", "music/menu.ogg");
        }
        else if (percent == 10) {
            percent = 15;
            for (const std::string& a :
                { "axe", "battle", "bell", "christianity", "cross", "destroy", "gear", "hammer", "healer", "heart", "illumination_settings",
                "infantryman", "kill", "lord", "magick", "music", "new_turn", "priest", "resources", "room_id", "save", "shield", "slow_movement", "sockerer",
                "sound", "to_menu", "trade", "poison", "poison_effect", "inspired_effect", "princess", "archer", "bow" }) {
                Textures::get().add(a + "_icon", "images/icons/" + a + "_icon.png");
            }
            for (const std::string& a : { "boost", "sfml" }) {
                Textures::get().add(a + "_icon", "images/icons/libs/" + a + "_icon.png");
            }
            for (const std::string& a : { "english", "russian" }) {
                Textures::get().add(a + "_icon", "images/icons/national/" + a + "_icon.png");
            }
            for (const std::string& a : { "food", "wood", "stone", "iron", "crystal", "gold", "helmet" }) {
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
        }
        else if (percent == 15) {
            percent = 20;
            for (const std::string& a :
                { "arable", "barracks", "castle", "gates1", "infirmary", "market", "mine", "quarry", "sawmill",
                "sockerer_house", "spell_factory", "tower1", "wall1", "warehouse_crystal", "warehouse_food", "warehouse_gold",
                "warehouse_iron", "warehouse_stone", "warehouse_wood", "workshop", "church", "range" }) {
                Textures::get().add(a, "images/buildings/main/" + a + ".png");
            }
        }
        else if (percent == 20) {
            percent = 25;
            for (uint32_t i = 1; i <= House::TOTAL_TYPES; i = i + 1) {
                Textures::get().add("house" + std::to_string(i), "images/buildings/main/house/" + std::to_string(i) + ".png");
            }
            for (uint32_t i = 1; i <= Well::TOTAL_TYPES; i = i + 1) {
                Textures::get().add("well" + std::to_string(i), "images/buildings/main/well/" + std::to_string(i) + ".png");
            }
            for (const std::string& a : { "none", "horizontal", "vertical", "all" }) {
                Textures::get().add("road_" + a, "images/buildings/main/road/" + a + ".png");
            }
        }
        else if (percent == 25) {
            percent = 30;
            for (uint32_t i = 1; i <= Fire::TOTAL_FRAMES; i = i + 1) {
                Textures::get().add("fire" + std::to_string(i), "images/buildings/fire/" + std::to_string(i) + ".png");
            }
            for (uint32_t i = 0; i < BuildingStatePointer::TOTAL_HP_POINTERS; i = i + 1) {
                Textures::get().add("building_hp_pointer" + std::to_string(i), "images/buildings/hp_pointers/" + std::to_string(i) + ".png");
            }
            for (const std::string& a : { "blue", "green", "purple" }) {
                Textures::get().add(a, "images/buildings/side_pointers/" + a + ".png");
            }
        }
        else if (percent == 30) {
            percent = 35;
            for (const std::string& a : { "forest", "iron", "mountains", "stone", "treasure", "water" }) {
                Textures::get().add(a, "images/landscape/" + a + ".png");
            }
            for (uint32_t i = 1; i <= Plant::TOTAL_TYPES; i = i + 1) {
                for (uint32_t j = 1; j <= Plant::ANIMATION_NUMBER[i - 1]; j = j + 1) {
                    Textures::get().add("plant" + std::to_string(i) + "_" + std::to_string(j), "images/landscape/plant/" + std::to_string(i) + "/" + std::to_string(j) + ".png");
                }
            }
        }
        else if (percent == 35) {
            percent = 40;
            for (uint32_t i = 1; i <= BigArrow::TOTAL_TYPES; i = i + 1) {
                Textures::get().add("big_arrow" + std::to_string(i), "images/projectiles/big_arrow/" + std::to_string(i) + ".png");
            }
            for (uint32_t i = 1; i <= HunterArrow::TOTAL_TYPES; i = i + 1) {
                Textures::get().add("hunter_arrow" + std::to_string(i), "images/projectiles/hunter_arrow/" + std::to_string(i) + ".png");
            }
            for (uint32_t i = 1; i <= PoisonerArrow::TOTAL_TYPES; i = i + 1) {
                Textures::get().add("poisoner_arrow" + std::to_string(i), "images/projectiles/poisoner_arrow/" + std::to_string(i) + ".png");
            }
        }
        else if (percent == 40) {
            percent = 45;
            for (uint32_t i = 1; i <= HealerProjectile::TOTAL_TYPES; i = i + 1) {
                Textures::get().add("healer_projectile" + std::to_string(i), "images/projectiles/healer_projectile/" + std::to_string(i) + ".png");
            }
        }
        else if (percent == 45) {
            percent = 50;
            for (uint32_t i = 1; i <= PatriarchProjectile::TOTAL_TYPES; i = i + 1) {
                Textures::get().add("patriarch_projectile" + std::to_string(i), "images/projectiles/patriarch_projectile/" + std::to_string(i) + ".png");
            }
        }
        else if (percent == 50) {
            percent = 55;
            for (const std::string& a : { "rage", "poison", "heal" }) {
                Textures::get().add(a + "_spell", "images/spells/" + a + "_spell.png");
            }
        }
        else if (percent == 55) {
            percent = 60;
            for (const std::string& a : { "dawn", "morning", "day", "sunset", "night1", "night2" }) {
                Textures::get().add(a, "images/times_of_day/" + a + ".png");
            }
        }
        else if (percent == 60) {
            percent = 65;
            for (const std::string& a : { "blue", "green", "purple" }) {
                for (const std::string& b : { "", "0", "1" }) {
                    Textures::get().add("warrior_" + a + b, "images/warriors/side_pointers/warrior_" + a + b + ".png");
                }
            }
        }
        else if (percent == 65) {
            percent = 70;
            LOAD_WARRIOR_DATA<Infantryman>();
            LOAD_WARRIOR_DATA<Legioner>();
            LOAD_WARRIOR_DATA<Hunter>();
        }
        else if (percent == 70) {
            percent = 75;
            LOAD_WARRIOR_DATA<Knight>();
            LOAD_WARRIOR_DATA<BlackKnight>();
            LOAD_WARRIOR_DATA<Princess>();
        }
        else if (percent == 75) {
            percent = 80;
            LOAD_WARRIOR_DATA<Healer>();
            LOAD_WARRIOR_DATA<Patriarch>();
            LOAD_WARRIOR_DATA<Barbar>();
        }
        else if (percent == 80) {
            percent = 85;
            LOAD_WARRIOR_DATA<Ram>();
            LOAD_WARRIOR_DATA<Golem>();
            LOAD_WARRIOR_DATA<Killer>();
            LOAD_WARRIOR_DATA<Poisoner>();
        }
        else if (percent == 85) {
            percent = 90;
            for (const std::string& a : { "btc", "hand", "plain", "icon" }) {
                Textures::get().add(a, "images/" + a + ".png");
            }
            for (const std::string& a : { "tmx_tileset" }) {
                Textures::get().add(a, "images/" + a + ".jpg");
            }
        }
        else if (percent == 90) {
            percent = 95;
            for (const std::string& a : { "click", "food", "gold", "hooray", "iron",
                                          "regeneration", "stone", "wood", "road", "wind", "water",
                                          "destroy", "sword", "breath", "knight", "fire",
                                          "ouch", "spell_factory", "rage_spell",
                                          "infantryman", "heal", "building_hit", "bell", "crystal", "swing",
                                          "ouch_woman_cute", "ouch_woman_cute2", "healer", "vehicle_hit",
                                          "vehicle", "sockerer_house", "roar", "dawn",
                                          "day", "night", "sunset", "ouch_old", "old", "patriarch_heal",
                                          "poison", "knife", "heal_spell", "barbar", "church", "princess", "bow"
            }) {
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
        }
        else if (percent == 95) {
            percent = 100;
            for (const std::string& a : { "ridge", "lake" }) {
                Maps::get().add(a, "levels/" + a + ".tmx");
            }
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