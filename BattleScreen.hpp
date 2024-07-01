/*
 *  Conquesta
 *  Copyright (C) 2024 mr2anderson
 *
 *  Conquesta is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Conquesta is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Conquesta.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <iostream>
#include "Playlist.hpp"
#include "SoundQueue.hpp"
#include "SoundStorage.hpp"
#include "MessageWindow.hpp"
#include "Plant.hpp"
#include "Tree.hpp"
#include "Mountain.hpp"
#include "RedMountain.hpp"
#include "Fort.hpp"
#include "Caravan.hpp"


#pragma once


class BattleScreen {
public:
	static BattleScreen* get() {
		if (BattleScreen::singletone == nullptr) {
			BattleScreen::singletone = new BattleScreen();
		}
		return BattleScreen::singletone;
	}
	int32_t run(sf::RenderWindow& window);
private:
	BattleScreen() = default;
	BattleScreen(const BattleScreen& copy) = delete;
	static BattleScreen* singletone;

	uint32_t mapWidth, mapHeight;
	std::queue<PopUpWindow*> popUpWindows;
	Player players[2];
	uint32_t move = 1;
	std::vector<GameObject*> gameObjects;
	uint32_t windowW, windowH;
	sf::View view;
	Button endMove;

	void init(sf::RenderWindow &window);
	void initLandscape();
	void removeOldPopUpWindows();
	void initPlayers();
	void initMoveCtr();
	void initGraphics(sf::RenderWindow &window);

	int32_t start(sf::RenderWindow &window);
	void handleGameEvent(GameEvent event);
	void handlePopUpWindowEvent(PopUpWindowEvent even);
	void newMove();
	Player* getCurrentPlayer();
	void drawCells(sf::RenderWindow &window);
	void viewToNorth();
	void viewToSouth();
	void viewToWest();
	void viewToEast();
};