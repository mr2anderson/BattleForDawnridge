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


#include <sstream>
#include <iostream>
#include <boost/serialization/shared_ptr.hpp>
#include "MainScreen.hpp"
#include "ScreenAlreadyFinished.hpp"
#include "Building.hpp"
#include "Textures.hpp"
#include "Playlist.hpp"
#include "ResourceBar.hpp"
#include "SoundQueue.hpp"
#include "Sounds.hpp"
#include "NoServerResponse.hpp"
#include "Ports.hpp"
#include "ServerNetSpecs.hpp"
#include "ClientNetSpecs.hpp"
#include "PortIsBusy.hpp"
#include "NoServerConnection.hpp"







MainScreen::MainScreen(sf::RenderWindow& window, sf::IpAddress serverIP, uint16_t serverSendPort, uint16_t serverReceivePort, RoomID roomID) {
	this->alreadyFinished = false;

	this->serverIP = serverIP;
	this->serverSendPort = serverSendPort;
	this->serverReceivePort = serverReceivePort;
	this->sendSocket.setBlocking(false);
	if (this->sendSocket.bind(Ports::get()->getClientSendPort()) != sf::Socket::Done) {
		throw PortIsBusy(Ports::get()->getClientSendPort());
	}
	this->receiveSocket.setBlocking(false);
	if (this->receiveSocket.bind(Ports::get()->getClientReceivePort()) != sf::Socket::Done) {
		throw PortIsBusy(Ports::get()->getClientReceivePort());
	}
	this->roomID = roomID;
	this->sendOKTimer = Timer(1000, Timer::TYPE::FIRST_INSTANTLY);
	this->noOKReceivedTimer = Timer(20 * 1000, Timer::TYPE::FIRST_DEFAULT);

	this->uiPackageGotten = false;

	this->returnToMenu = false;
	this->view = sf::View(window.getDefaultView());
	this->illiminanceTable.createRender(window.getSize().x, window.getSize().y, window.getSettings());
}








MainScreenResponse MainScreen::run(sf::RenderWindow& window) {
	if (this->alreadyFinished) {
		throw ScreenAlreadyFinished();
	}
	this->alreadyFinished = true;

	sf::Event event{};
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed) {
				sf::Packet packet;
				packet << CLIENT_NET_SPECS::ROOM;
				packet << this->roomID.value();
				packet << CLIENT_NET_SPECS::ROOM_CODES::CLICK;
				packet << (uint8_t)event.mouseButton.button;
				packet << (uint32_t)sf::Mouse::getPosition().x;
				packet << (uint32_t)sf::Mouse::getPosition().y;
				packet << (uint32_t)std::get<0>(this->getMousePositionBasedOnView(window));
				packet << (uint32_t)std::get<1>(this->getMousePositionBasedOnView(window));
				packet << (uint32_t)window.getSize().x;
				packet << (uint32_t)window.getSize().y;
				this->sendSocket.send(packet, this->serverIP, this->serverReceivePort);
			}
		}
		this->sendOK();
		this->receive();
		if (!this->uiPackageGotten) {
			continue;
		}
		Playlist::get()->update();
		window.setMouseCursorVisible(this->cursorVisibility);
		this->drawEverything(window);
		this->moveView(window);
		if (this->returnToMenu) {
			Playlist::get()->stop();
			return MainScreenResponse(MainScreenResponse::TYPE::RETURN_TO_MENU);
		}
		if (this->noOKReceivedTimer.ready()) {
			Playlist::get()->stop();
			throw NoServerConnection();
		}
	}
}




void MainScreen::sendOK() {
	if (this->sendOKTimer.ready()) {
		this->sendOKTimer.reset();
		sf::Packet packet;
		packet << CLIENT_NET_SPECS::ROOM;
		packet << this->roomID.value();
		packet << CLIENT_NET_SPECS::ROOM_CODES::OK;
		this->sendSocket.send(packet, this->serverIP, this->serverReceivePort);
	}
}







void MainScreen::receive() {
	sf::Packet receivedPacket;
	sf::IpAddress senderIP;
	uint16_t senderPort;
	if (this->receiveSocket.receive(receivedPacket, senderIP, senderPort) == sf::Socket::Status::Done and senderIP == this->serverIP and senderPort == this->serverSendPort) {
		uint8_t code;
		receivedPacket >> code;

		if (code == SERVER_NET_SPECS::OK) {
			this->noOKReceivedTimer.reset();
		}
		else if (code == SERVER_NET_SPECS::WORLD_UI_STATE) {
			std::string data;
			receivedPacket >> data;
			std::stringstream stream(data);
			iarchive ar(stream);
			ar >> this->map >> this->element >> this->selected >> this->highlightTable >> this->cursorVisibility >> this->buttonBases >> this->resourceBar;
			this->uiPackageGotten = true;
		}
		else if (code == SERVER_NET_SPECS::SOUND) {
			std::string soundName;
			receivedPacket >> soundName;
			SoundQueue::get()->push(Sounds::get()->get(soundName));
		}
		else if (code == SERVER_NET_SPECS::FOCUS) {
			uint32_t x, y, sx, sy;
			receivedPacket >> x >> y >> sx >> sy;
			this->viewMovingQueue.emplace(64 * x + 64 / 2 * sx, 64 * y + 64 / 2 * sy);
		}
		else if (code == SERVER_NET_SPECS::RETURN_TO_MENU) {
			this->returnToMenu = true;
		}
		else {
			std::cerr << "MainScreen: warning: unknown code received from server: " << (uint32_t)code << std::endl;
		}
	}
}














void MainScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.setView(this->view);
	this->drawCells(window);
	this->drawMap(window);
	this->drawHighlightion(window);
    this->drawDarkness(window);
    if (this->selected != nullptr) {
        window.draw(*this->selected->getSelectablePointer(std::get<0>(this->getMousePositionBasedOnView(window)), std::get<1>(this->getMousePositionBasedOnView(window))));
    }
    if (this->element != nullptr and this->element->isCameraDependent()) {
        window.draw(*this->element);
    }
	window.setView(window.getDefaultView());
    if (this->element != nullptr and !this->element->isCameraDependent()) {
        window.draw(*this->element);
    }
	this->drawResourceBar(window);
	for (const auto& b : this->buttonBases) {
		window.draw(*b);
	}
	window.display();
}
void MainScreen::drawMap(sf::RenderWindow& window) {
	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		GO* go = this->map.getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DRAW_PRIORITY);
		if (go->exist() and go->inView(this->view)) {
			window.draw(*go);
		}
	}
}
void MainScreen::drawResourceBar(sf::RenderWindow& window) {
	window.draw(resourceBar);
}
void MainScreen::drawCells(sf::RenderWindow& window) {
    uint32_t sx = Textures::get()->get("plain")->getSize().x / 64;
    uint32_t sy = Textures::get()->get("plain")->getSize().y / 64;
	for (uint32_t i = 0; i < this->map.getStatePtr()->getMapSizePtr()->getWidth(); i = i + sx) {
		for (uint32_t j = 0; j < this->map.getStatePtr()->getMapSizePtr()->getHeight(); j = j + sy) {
			sf::Sprite s;
			s.setTexture(*Textures::get()->get("plain"));
			s.setPosition(64 * i, 64 * j);
			window.draw(s);
		}
	}
}
void MainScreen::drawHighlightion(sf::RenderWindow& window) {
	std::vector<sf::RectangleShape> rects = this->highlightTable.getRects();
	for (const auto& rect : rects) {
		window.draw(rect);
	}
}
void MainScreen::drawDarkness(sf::RenderWindow &window) {
	this->illiminanceTable.newFrame(this->view);

	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		const GO* go = this->map.getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
		if (go->exist()) {
			this->illiminanceTable.add(go);
		}
	}

	window.draw(this->illiminanceTable);
}











std::tuple<uint32_t, uint32_t> MainScreen::getMousePositionBasedOnView(sf::RenderWindow &window) const {
	uint32_t mouseX = sf::Mouse::getPosition().x + this->view.getCenter().x - window.getSize().x / 2;
	uint32_t mouseY = sf::Mouse::getPosition().y + this->view.getCenter().y - window.getSize().y / 2;
	return std::make_tuple(mouseX, mouseY);
}
void MainScreen::moveView(sf::RenderWindow &window) {
	if (this->viewMovingQueue.empty()) {
		auto p = sf::Mouse::getPosition();
		if (p.x < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::A) or sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			this->moveViewToWest(window);
		}
		else if (p.x > window.getSize().x - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::D) or sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			this->moveViewToEast(window);
		}
		if (p.y < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::W) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			this->moveViewToNorth(window);
		}
		else if (p.y > window.getSize().y - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			this->moveViewToSouth(window);
		}
	}
	else {
		uint32_t currentX, currentY;
		std::tie(currentX, currentY) = this->viewMovingQueue.front();

		uint32_t viewX = this->view.getCenter().x;
		uint32_t viewY = this->view.getCenter().y;

		bool horizontalOk = false;
		bool verticalOk = false;

		if (currentX < viewX) {
			horizontalOk = horizontalOk or !this->moveViewToWest(currentX, window);
		}
		else if (currentX > viewX) {
			horizontalOk = horizontalOk or !this->moveViewToEast(currentX, window);
		}
		else {
			horizontalOk = true;
		}

		if (currentY < viewY) {
			verticalOk = verticalOk or !this->moveViewToNorth(currentY, window);
		}
		else if (currentY > viewY) {
			verticalOk = verticalOk or !this->moveViewToSouth(currentY, window);
		}
		else {
			verticalOk = true;
		}

		if (horizontalOk and verticalOk) {
			this->viewMovingQueue.pop();
		}
	}
}








static float VIEW_MOVING_DELTA = 10;
static float VIEW_MOVING_BIG_DELTA = 1e+10;
bool MainScreen::moveViewToNorth(uint32_t border, sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(0, VIEW_MOVING_BIG_DELTA));
	return this->verifyViewNorth(window) and this->verifyViewNorth(border);
}
bool MainScreen::moveViewToNorth(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(0, VIEW_MOVING_DELTA));
	return this->verifyViewNorth(window);
}
bool MainScreen::moveViewToSouth(uint32_t border, sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(0, VIEW_MOVING_BIG_DELTA));
	return this->verifyViewSouth(window) and this->verifyViewSouth(border);
}
bool MainScreen::moveViewToSouth(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(0, VIEW_MOVING_DELTA));
	return this->verifyViewSouth(window);
}
bool MainScreen::moveViewToWest(uint32_t border, sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(VIEW_MOVING_BIG_DELTA, 0));
	return this->verifyViewWest(window) and this->verifyViewWest(border);
}
bool MainScreen::moveViewToWest(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(VIEW_MOVING_DELTA, 0));
	return this->verifyViewWest(window);
}
bool MainScreen::moveViewToEast(uint32_t border, sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(VIEW_MOVING_BIG_DELTA, 0));
	return this->verifyViewEast(window) and this->verifyViewEast(border);
}
bool MainScreen::moveViewToEast(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(VIEW_MOVING_DELTA, 0));
	return this->verifyViewEast(window);
}
bool MainScreen::verifyViewNorth(uint32_t border) {
	if (this->view.getCenter().y < border) {
		this->view.setCenter(sf::Vector2f(this->view.getCenter().x, border));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewNorth(sf::RenderWindow& window) {
	return this->verifyViewNorth(window.getSize().y / 2);
}
bool MainScreen::verifyViewSouth(uint32_t border) {
	if (this->view.getCenter().y > border) {
		this->view.setCenter(sf::Vector2f(this->view.getCenter().x, border));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewSouth(sf::RenderWindow& window) {
	return this->verifyViewSouth(64 * this->map.getStatePtr()->getMapSizePtr()->getHeight() - window.getSize().y / 2);
}
bool MainScreen::verifyViewWest(uint32_t border) {
	if (this->view.getCenter().x < border) {
		this->view.setCenter(sf::Vector2f(border, this->view.getCenter().y));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewWest(sf::RenderWindow& window) {
	return this->verifyViewWest(window.getSize().x / 2);
}
bool MainScreen::verifyViewEast(uint32_t border) {
	if (this->view.getCenter().x > border) {
		this->view.setCenter(sf::Vector2f(border, this->view.getCenter().y));
		return false;
	}
	return true;
}
bool MainScreen::verifyViewEast(sf::RenderWindow& window) {
	return this->verifyViewEast(64 * this->map.getStatePtr()->getMapSizePtr()->getWidth() - window.getSize().x / 2);
}