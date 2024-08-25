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
#include "ClientNetSpecs.hpp"
#include "PlaySoundEvent.hpp"
#include "Button.hpp"
#include "SaveGameEvent.hpp"
#include "ReturnToMenuEvent.hpp"
#include "CreateEEvent.hpp"
#include "ClosePopUpElementEvent.hpp"


#pragma once


class MainScreen {
public:
	typedef enum Type {
		CreateFromMap,
		CreateFromSave,
		Connect
	} Type;

	MainScreen(sf::RenderWindow& window, sf::IpAddress serverIp, uint16_t serverPort, Type type, const std::string &data, uint32_t playersAtThisHost, const RoomID& roomID);
	MainScreen(const MainScreen& copy) = delete;

	void run(sf::RenderWindow& window);

	static constexpr uint32_t EVERYONE = (1 << 30);
private:
	bool alreadyFinished;

	sf::IpAddress serverIP;
    uint16_t serverPort;

	sf::TcpSocket socket;
	std::queue<sf::Packet> toSend;
	std::tuple<bool, sf::Packet> received;
    bool error;

	Type type;
	std::string data;
	uint32_t playersAtThisHost;
	RoomID roomID;

	bool readyPackageGotten;
	Map map;
	std::shared_ptr<PopUpElement> element;
	ISelectable* selected;
	HighlightTable highlightTable;
	std::vector<std::shared_ptr<const RectangularUiElement>> buttonBases;
	ResourceBar resourceBar;
    bool cursorVisibility;

	std::shared_ptr<PopUpElement> localElement;
    std::vector<Button> localButtons;
    std::queue<std::shared_ptr<Event>> localEventQueue;

	bool returnToMenu;
	sf::View view;
	IlluminanceTable illiminanceTable;

	void processSending();
	void processReceiving();

    sf::Packet makeBasePacket() const;
    void send(sf::Packet &what);

	void sendInit();
    void sendClick(sf::RenderWindow &window, uint8_t button);
    void sendNeedSave();


	void receive(sf::RenderWindow &window);
	void receiveError(sf::Packet& remPacket);
	void receiveMap(sf::Packet& remPacket);
    void receiveElement(sf::Packet& remPacket);
    void receiveSelected(sf::Packet& remPacket);
    void receiveHighlightTable(sf::Packet& remPacket);
    void receiveButtonBases(sf::Packet& remPacket);
    void receiveResourceBar(sf::Packet& remPacket);
    void receiveCursorVisibility(sf::Packet& remPacket);
    void receiveReady();
	void receiveSound(sf::Packet& remPacket);
	void receiveFocus(sf::Packet& remPacket, sf::RenderWindow& window);
	void receiveReturnToMenu();
	void receiveSave(sf::Packet& remPacket);
    void receiveNotTimeToSave();
	void receiveNotYourMove();


	void drawEverything(sf::RenderWindow& window);
	void drawMap(sf::RenderWindow& window);
	void drawResourceBar(sf::RenderWindow& window);
	void drawCells(sf::RenderWindow& window);
	void drawHighlightion(sf::RenderWindow& window);
    void drawDarkness(sf::RenderWindow& window);


    void drawWaitingScreen(sf::RenderWindow &window);


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

    Events handleLocalButtonsClick();

	void addLocalEvents(Events& events);
    void processLocalEvents();

	void handleEvent(std::shared_ptr<Event> e);

	void handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e);
    void handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e);
    void handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e);
    void handleCreateEEvent(std::shared_ptr<CreateEEvent> e);
    void handleClosePopUpElementEvent(std::shared_ptr<ClosePopUpElementEvent> e);
};