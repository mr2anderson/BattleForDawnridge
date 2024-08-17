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
#include <filesystem>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <fstream>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include "MainScreen.hpp"
#include "ScreenAlreadyFinished.hpp"
#include "Building.hpp"
#include "Textures.hpp"
#include "Playlist.hpp"
#include "ResourceBar.hpp"
#include "SoundQueue.hpp"
#include "Sounds.hpp"
#include "ServerNetSpecs.hpp"
#include "ClientNetSpecs.hpp"
#include "PortIsBusy.hpp"
#include "NoServerConnection.hpp"
#include "MenuBg.hpp"
#include "Root.hpp"
#include "Maps.hpp"
#include "GlobalRandomGenerator64.hpp"







MainScreen::MainScreen(sf::RenderWindow& window, sf::IpAddress serverIP, Type type, const std::string &data, uint32_t playersAtThisHost, RoomID roomID) {
	this->alreadyFinished = false;

	this->serverIP = serverIP;
	this->socketInited = false;
	this->type = type;
	this->data = data;
	this->playersAtThisHost = playersAtThisHost;
	this->roomID = roomID;

	this->initPackageGotten = false;

	this->returnToMenu = false;
	this->view = sf::View(window.getDefaultView());
	this->illiminanceTable.createRender(window.getSize().x, window.getSize().y, window.getSettings());
}





MainScreen::~MainScreen() {
    this->stop = true;
}








void MainScreen::run(sf::RenderWindow& window) {
	if (this->alreadyFinished) {
		throw ScreenAlreadyFinished();
	}
	this->alreadyFinished = true;

    sf::Event event{};

    Timer timer(5000, Timer::TYPE::FIRST_DEFAULT);
    while (!this->socketInited) {
        while (window.pollEvent(event)) {}
        if (timer.ready()) {
            throw NoServerConnection();
        }
        if (this->socket.connect(this->serverIP, SERVER_NET_SPECS::PORTS::TCP, sf::milliseconds(1000)) == sf::Socket::Status::Done) {
            this->stop = false;
            this->socket.setBlocking(false);
            this->sendingThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_help::process_sending, &this->socket, &this->toSend, &this->stop, &this->sentBytes));
            this->receivingThread = std::make_unique<sf::Thread>(std::bind(&bfdlib::tcp_help::process_receiving, &this->socket, &this->received, &this->stop, &this->receivedBytes));
            this->sendingThread->launch();
            this->receivingThread->launch();
            this->socketInited = true;
        }
        this->drawWaitingScreen(window);
    }

    this->sendInit();
    timer.reset();
    while (!this->initPackageGotten) {
        while (window.pollEvent(event)) {}
        if (timer.ready()) {
            throw NoServerConnection();
        }
        this->receive(window);
        this->drawWaitingScreen(window);
    }

	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed) {
                this->sendClick(window, event.mouseButton.button);
			}
		}
		this->receive(window);
		Playlist::get()->update();
		window.setMouseCursorVisible(this->cursorVisibility);
		this->drawEverything(window);
		this->moveView(window);
		if (this->returnToMenu) {
			return;
		}
	}
}




sf::Packet MainScreen::makeBasePacket() const {
    sf::Packet packet;
    packet << (sf::Uint64)this->roomID.value();
    return packet;
}
void MainScreen::send(sf::Packet &what) {
    this->toSend.push(what);
}




std::string GENERATE_SAVE_FROM_MAP(const std::string& name) {
	Map tmpMap;
	Maps::get()->load(name, &tmpMap);

	std::string serialStr;

	boost::iostreams::back_insert_device<std::string> inserter(serialStr);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string>> s(inserter);

	oarchive ar(s);
	ar << tmpMap << std::vector<bool>(tmpMap.getStatePtr()->getPlayersPtr()->total(), true) << (uint32_t)1 << (uint32_t)1;

	s.flush();

	return serialStr;
}
std::string READ_SAVE(const std::string& name) {
	std::ifstream f(USERDATA_ROOT + "/saves/" + name, std::ios::binary);
	std::stringstream buffer;
	buffer << f.rdbuf();
	f.close();
	return buffer.str();
}
void MainScreen::sendInit() {
	sf::Packet packet = this->makeBasePacket();
	if (this->type == MainScreen::Type::CreateFromMap) {
		packet << CLIENT_NET_SPECS::CODES::CREATE;
		packet << GENERATE_SAVE_FROM_MAP(this->data);
		packet << this->playersAtThisHost;
	}
	else if (this->type == MainScreen::Type::CreateFromSave) {
		packet << CLIENT_NET_SPECS::CODES::CREATE;
		packet << READ_SAVE(this->data);
		packet << this->playersAtThisHost;
	}
	else if (this->type == MainScreen::Type::Connect) {
		packet << CLIENT_NET_SPECS::CODES::CONNECT;
		packet << this->playersAtThisHost;
	}
	this->send(packet);
}
void MainScreen::sendClick(sf::RenderWindow &window, uint8_t button) {
    sf::Packet packet = this->makeBasePacket();
    packet << CLIENT_NET_SPECS::CODES::CLICK;
    packet << button;
    packet << (uint32_t)sf::Mouse::getPosition().x;
    packet << (uint32_t)sf::Mouse::getPosition().y;
    packet << (uint32_t)std::get<0>(this->getMousePositionBasedOnView(window));
    packet << (uint32_t)std::get<1>(this->getMousePositionBasedOnView(window));
    packet << (uint32_t)window.getSize().x;
    packet << (uint32_t)window.getSize().y;
    this->send(packet);
}







void MainScreen::receive(sf::RenderWindow &window) {
    if (this->received.empty()) {
        return;
    }
	sf::Packet receivedPacket = this->received.pop();
    sf::Uint64 id;
    receivedPacket >> id;
    if (this->roomID.value() == id) {
        uint8_t code;
        receivedPacket >> code;

        if (code == SERVER_NET_SPECS::CODES::WORLD_UI_STATE) {
            this->receiveWorldUIState(receivedPacket);
        }
        else if (code == SERVER_NET_SPECS::CODES::SOUND) {
            this->receiveSound(receivedPacket);
        }
        else if (code == SERVER_NET_SPECS::CODES::FOCUS) {
            this->receiveFocus(receivedPacket, window);
        }
        else if (code == SERVER_NET_SPECS::CODES::RETURN_TO_MENU) {
            this->receiveReturnToMenu();
        }
        else if (code == SERVER_NET_SPECS::CODES::SAVE) {
            this->receiveSave(receivedPacket);
        }
        else {
            std::cerr << "MainScreen: warning: unknown code received from server: " << (uint32_t)code << std::endl;
        }
    }
}
void MainScreen::receiveWorldUIState(sf::Packet& remPacket) {
	std::string data;
	remPacket >> data;
	std::stringstream stream(data);
	iarchive ar(stream);
	ar >> this->map >> this->element >> this->selected >> this->highlightTable >> this->cursorVisibility >> this->buttonBases >> this->resourceBar;
	this->initPackageGotten = true;
}
void MainScreen::receiveSound(sf::Packet& remPacket) {
	std::string soundName;
	remPacket >> soundName;
	SoundQueue::get()->push(Sounds::get()->get(soundName));
}
void MainScreen::receiveFocus(sf::Packet& remPacket, sf::RenderWindow& window) {
	uint32_t x, y, sx, sy;
	remPacket >> x >> y >> sx >> sy;
	uint32_t centerX = 64 * x + 64 / 2 * sx;
	uint32_t centerY = 64 * y + 64 / 2 * sy;
	if (centerX >= view.getCenter().x - window.getSize().x / 2 and
		centerY >= view.getCenter().y - window.getSize().y / 2 and
		centerX < view.getCenter().x + window.getSize().x / 2 and
		centerY < view.getCenter().y + window.getSize().y / 2) {

	}
	else {
		this->view.setCenter(64 * x + 64 / 2 * sx, 64 * y + 64 / 2 * sy);
		this->verifyView(window);
	}
}
void MainScreen::receiveReturnToMenu() {
	this->returnToMenu = true;
}
void MainScreen::receiveSave(sf::Packet& remPacket) {
	std::string data;
	remPacket >> data;
	if (!std::filesystem::is_directory(USERDATA_ROOT + "/saves")) {
		std::filesystem::create_directories(USERDATA_ROOT + "/saves");
	}
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::stringstream ss;
	ss << std::put_time(&tm, "%Y-%m-%d %H-%M-%S");
	std::ofstream ofs(USERDATA_ROOT + "/saves/" + ss.str() + ".save", std::ios::binary);
	ofs.write(data.c_str(), data.size());
	ofs.close();
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










void MainScreen::drawWaitingScreen(sf::RenderWindow &window) {
    window.draw(MenuBg());
    window.display();
}











std::tuple<uint32_t, uint32_t> MainScreen::getMousePositionBasedOnView(sf::RenderWindow &window) const {
	uint32_t mouseX = sf::Mouse::getPosition().x + this->view.getCenter().x - window.getSize().x / 2;
	uint32_t mouseY = sf::Mouse::getPosition().y + this->view.getCenter().y - window.getSize().y / 2;
	return std::make_tuple(mouseX, mouseY);
}
void MainScreen::moveView(sf::RenderWindow &window) {
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








static float VIEW_MOVING_DELTA = 10;
void MainScreen::moveViewToNorth(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(0, VIEW_MOVING_DELTA));
	this->verifyViewNorth(window);
}
void MainScreen::moveViewToSouth(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(0, VIEW_MOVING_DELTA));
	this->verifyViewSouth(window);
}
void MainScreen::moveViewToWest(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(VIEW_MOVING_DELTA, 0));
	this->verifyViewWest(window);
}
void MainScreen::moveViewToEast(sf::RenderWindow& window) {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(VIEW_MOVING_DELTA, 0));
	this->verifyViewEast(window);
}






void MainScreen::verifyView(sf::RenderWindow& window) {
	this->verifyViewNorth(window);
	this->verifyViewSouth(window);
	this->verifyViewWest(window);
	this->verifyViewEast(window);
}
void MainScreen::verifyViewNorth(sf::RenderWindow& window) {
	uint32_t border = window.getSize().y / 2;
	if (this->view.getCenter().y < border) {
		this->view.setCenter(sf::Vector2f(this->view.getCenter().x, border));
	}
}
void MainScreen::verifyViewSouth(sf::RenderWindow& window) {
	uint32_t border = 64 * this->map.getStatePtr()->getMapSizePtr()->getHeight() - window.getSize().y / 2;
	if (this->view.getCenter().y > border) {
		this->view.setCenter(sf::Vector2f(this->view.getCenter().x, border));
	}
}
void MainScreen::verifyViewWest(sf::RenderWindow& window) {
	uint32_t border = window.getSize().x / 2;
	if (this->view.getCenter().x < border) {
		this->view.setCenter(sf::Vector2f(border, this->view.getCenter().y));
	}
}
void MainScreen::verifyViewEast(sf::RenderWindow& window) {
	uint32_t border = 64 * this->map.getStatePtr()->getMapSizePtr()->getWidth() - window.getSize().x / 2;
	if (this->view.getCenter().x > border) {
		this->view.setCenter(sf::Vector2f(border, this->view.getCenter().y));
	}
}