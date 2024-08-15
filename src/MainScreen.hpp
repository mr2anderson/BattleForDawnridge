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
	enum Type {
		CreateFromMap,
		CreateFromSave,
		Connect
	} typedef;

	MainScreen(sf::RenderWindow& window, sf::IpAddress serverIp, uint16_t serverSendPort, uint16_t serverReceivePort, Type type, const std::string &data, uint32_t playersAtThisHost, RoomID roomID);
	MainScreen(const MainScreen& copy) = delete;
	void run(sf::RenderWindow& window);

	static constexpr uint32_t EVERYONE = (1 << 30);
private:
	bool alreadyFinished;

	sf::IpAddress serverIP;
	uint16_t serverSendPort;
	uint16_t serverReceivePort;
	sf::UdpSocket sendSocket;
	sf::UdpSocket receiveSocket;
	Type type;
	std::string data;
	uint32_t playersAtThisHost;
	RoomID roomID;
	Timer sendInitTimer;
	Timer sendOKTimer;
	Timer noOKReceivedTimer;

	bool initPackageGotten;
	Map map;
	std::shared_ptr<PopUpElement> element;
	ISelectable* selected;
	HighlightTable highlightTable;
	bool cursorVisibility;
	std::vector<std::shared_ptr<const RectangularUiElement>> buttonBases;
	ResourceBar resourceBar;

	bool returnToMenu;
	sf::View view;
	IlluminanceTable illiminanceTable;


	void send();
	void sendInit();
	void sendOK();


	void receive(sf::RenderWindow &window);
	void receiveOK();
	void receiveWorldUIState(sf::Packet& remPacket);
	void receiveSound(sf::Packet& remPacket);
	void receiveFocus(sf::Packet& remPacket, sf::RenderWindow& window);
	void receiveReturnToMenu();
	void receiveSave(sf::Packet& remPacket);


	void drawEverything(sf::RenderWindow& window);
	void drawMap(sf::RenderWindow& window);
	void drawResourceBar(sf::RenderWindow& window);
	void drawCells(sf::RenderWindow& window);
	void drawHighlightion(sf::RenderWindow& window);
    void drawDarkness(sf::RenderWindow& window);


	std::tuple<uint32_t, uint32_t> getMousePositionBasedOnView(sf::RenderWindow &window) const;
	void moveView(sf::RenderWindow &window);


	void moveViewToNorth(sf::RenderWindow& window);
	void moveViewToSouth(sf::RenderWindow& window);
	void moveViewToWest(sf::RenderWindow& window);
	void moveViewToEast(sf::RenderWindow& window);


	void verifyView(sf::RenderWindow& window);
	void verifyViewNorth(sf::RenderWindow& window);
	void verifyViewSouth(sf::RenderWindow& window);
	void verifyViewWest(sf::RenderWindow& window);
	void verifyViewEast(sf::RenderWindow& window);
};