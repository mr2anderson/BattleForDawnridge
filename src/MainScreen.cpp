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
#include "NoServerResponse.hpp"
#include "Ports.hpp"
#include "ServerPacketCodes.hpp"
#include "ClientPacketCodes.hpp"







MainScreen::MainScreen(sf::RenderWindow& window, sf::IpAddress serverIP, RoomID roomID) {
	this->alreadyFinished = false;
	this->serverIP = serverIP;
	this->port = Ports::get()->getClientPort();
	this->socket.bind(this->port);
	this->socket.setBlocking(false);
	this->roomID = roomID;

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
				// TODO sending clicks to room
			}
		}
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
	}
}







void MainScreen::receive() {
	sf::Packet packet;

	if (this->socket.receive(packet, this->serverIP, this->port) == sf::Socket::Status::Done) {
		uint8_t code;
		packet >> code;

		if (code == ServerPacketCodes::OK) {
			
		}
		else if (code == ServerPacketCodes::GAME_UI_STATE) {
			std::string data;
			packet >> data;
			std::stringstream stream(data);
			iarchive ar(stream);
			ar >> this->map >> this->element >> this->selected >> this->highlightTable >> this->cursorVisibility >> this->buttonBases >> this->resourceBar;
			this->uiPackageGotten = true;
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
static float VIEW_MOVING_BIG_DELTA = 1.25f * VIEW_MOVING_DELTA;
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