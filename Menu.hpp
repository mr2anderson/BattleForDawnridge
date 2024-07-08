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


#include <queue>
#include "Maps.hpp"
#include "SoundQueue.hpp"
#include "Playlist.hpp"
#include "ColorTheme.hpp"
#include "WindowButton.hpp"
#include "Sounds.hpp"
#include "CloseMenuEvent.hpp"
#include "StartGameEvent.hpp"


#pragma once


class Menu {
public:
	static Menu* get() {
		if (Menu::singletone == nullptr) {
			Menu::singletone = new Menu();
		}
		return Menu::singletone;
	}

	Map* run(sf::RenderWindow& window);
private:
	Menu() = default;
	Menu(const Menu& copy) = delete;
	static Menu* singletone;

    bool closeMenu;
    bool startGame;
    uint32_t windowW, windowH;
    std::queue<std::shared_ptr<PopUpElement>> elements;
	std::vector<Button> buttons;
	sf::Text title;

	void init(uint32_t windowW, uint32_t windowH);
	void drawEverything(sf::RenderWindow& window);
    void addElement(std::shared_ptr<PopUpElement> e);
    void removeFinishedElements();
    void prepareToStartGame();
    bool handleButtonsClick();

    void handleEvent(Events &e);

    void handleSoundEvent(std::shared_ptr<PlaySoundEvent> e);
    void handleCreateEEvent(std::shared_ptr<CreateEEvent> e);
    void handleCloseMenuEvent(std::shared_ptr<CloseMenuEvent> e);
    void handleStartGameEvent(std::shared_ptr<StartGameEvent> e);
};