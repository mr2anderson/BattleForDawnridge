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
#include <SFML/Network.hpp>
#include "IlluminanceTable.hpp"
#include "RoomID.hpp"
#include "Map.hpp"
#include "Timer.hpp"
#include "RectangularUiElement.hpp"
#include "PopUpElement.hpp"
#include "ISelectable.hpp"
#include "HighlightTable.hpp"
#include "ResourceBar.hpp"


#pragma once


class MainScreen {
public:
	MainScreen(sf::RenderWindow& window, sf::IpAddress serverIp, uint16_t serverSendPort, uint16_t serverReceivePort, RoomID roomID);
	MainScreen(const MainScreen& copy) = delete;
	void run(sf::RenderWindow& window);
private:
	bool alreadyFinished;

	sf::IpAddress serverIP;
	uint16_t serverSendPort;
	uint16_t serverReceivePort;
	sf::UdpSocket sendSocket;
	sf::UdpSocket receiveSocket;
	RoomID roomID;
	Timer sendOKTimer;
	Timer noOKReceivedTimer;

	bool uiPackageGotten;
	Map map;
	std::shared_ptr<PopUpElement> element;
	ISelectable* selected;
	HighlightTable highlightTable;
	bool cursorVisibility;
	std::vector<std::shared_ptr<const RectangularUiElement>> buttonBases;
	ResourceBar resourceBar;

	bool returnToMenu;
	std::queue<std::tuple<uint32_t, uint32_t>> viewMovingQueue;
	sf::View view;
	IlluminanceTable illiminanceTable;


	void sendOK();


	void receive();


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