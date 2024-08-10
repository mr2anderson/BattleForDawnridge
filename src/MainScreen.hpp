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


#include <SFML/Graphics.hpp>
#include <queue>
#include "MainScreenResponse.hpp"
#include "MenuResponse.hpp"
#include "IlluminanceTable.hpp"


#pragma once


class MainScreen {
public:
	MainScreen(sf::RenderWindow &window, const MenuResponse &response);
	MainScreen(const MainScreen& copy) = delete;
	MainScreenResponse run(sf::RenderWindow& window);
private:
	bool alreadyFinished;
	bool returnToMenu;
	std::queue<std::tuple<uint32_t, uint32_t>> viewMovingQueue;
	sf::View view;
	IlluminanceTable illiminanceTable;


	void drawEverything(sf::RenderWindow& window);
	void drawMap(sf::RenderWindow& window);
	void drawResourceBar(sf::RenderWindow& window);
	void drawCells(sf::RenderWindow& window);
	void drawHighlightion(sf::RenderWindow& window);
    void drawDarkness(sf::RenderWindow& window);


	std::tuple<uint32_t, uint32_t> getMousePositionBasedOnView(sf::RenderWindow &window) const;
	void moveView(sf::RenderWindow &window);


	bool moveViewToNorth(uint32_t border, sf::RenderWindow& window);
	bool moveViewToNorth(sf::RenderWindow& window);
	bool moveViewToSouth(uint32_t border, sf::RenderWindow& window);
	bool moveViewToSouth(sf::RenderWindow& window);
	bool moveViewToWest(uint32_t borde, sf::RenderWindow& window);
	bool moveViewToWest(sf::RenderWindow& window);
	bool moveViewToEast(uint32_t border, sf::RenderWindow& window);
	bool moveViewToEast(sf::RenderWindow& window);


	bool verifyViewNorth(uint32_t border);
	bool verifyViewNorth(sf::RenderWindow& window);
	bool verifyViewSouth(uint32_t border);
	bool verifyViewSouth(sf::RenderWindow& window);
	bool verifyViewWest(uint32_t border);
	bool verifyViewWest(sf::RenderWindow& window);
	bool verifyViewEast(uint32_t border);
	bool verifyViewEast(sf::RenderWindow& window);
};