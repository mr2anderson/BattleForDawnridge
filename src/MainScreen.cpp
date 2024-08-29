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
#include "NoServerConnection.hpp"
#include "MenuBg.hpp"
#include "Root.hpp"
#include "Maps.hpp"
#include "WindowButton.hpp"
#include "GlobalRandomGenerator64.hpp"
#include "SaveGameButtonSpec.hpp"
#include "ReturnToMenuButtonSpec.hpp"
#include "ServerInitError.hpp"
#include "string_compression.hpp"






#define PRINT_LOGS





static void LOGS(const std::string &val) {
    #ifdef PRINT_LOGS
    std::cout << "MainScreen: " << val << std::endl;
    #endif
}







MainScreen::MainScreen(sf::RenderWindow& window, sf::IpAddress serverIP, uint16_t serverPort, Type type, const std::string &data, uint32_t playersAtThisHost, const RoomID& roomID) {
	this->alreadyFinished = false;

	this->serverIP = serverIP;
    this->serverPort = serverPort;

	this->error = false;

	this->type = type;
	this->data = data;
	this->playersAtThisHost = playersAtThisHost;
	this->roomID = roomID;

	this->readyPackageGotten = false;

    this->localElement = nullptr;
    this->localButtons.emplace_back(ReturnToMenuButtonSpec());
    this->localButtons.emplace_back(SaveGameButtonSpec(1));

	this->returnToMenu = false;
	this->view = sf::View(window.getDefaultView());
	this->illiminanceTable.createRender(window.getSize().x, window.getSize().y, window.getSettings());
}














void MainScreen::run(sf::RenderWindow& window) {
	if (this->alreadyFinished) {
		throw ScreenAlreadyFinished();
	}
	this->alreadyFinished = true;

    sf::Event event;

    LOGS("Connecting to server...");
    Timer timer(5 * 1000, Timer::TYPE::FIRST_DEFAULT);
	for (; ;) {
        while (window.pollEvent(event)) {}
        if (timer.ready()) {
            throw NoServerConnection();
        }
		this->drawWaitingScreen(window);
        if (this->socket.connect(this->serverIP, this->serverPort, sf::milliseconds(2500)) == sf::Socket::Status::Done) {
			this->socket.setBlocking(false);
			this->received = std::make_tuple(false, sf::Packet());
			break;
        }
    }
    LOGS("Connected to server.");

    this->sendInit();
    timer.reset();
    while (!this->readyPackageGotten) {
        while (window.pollEvent(event)) {}
        if (timer.ready()) {
            throw NoServerConnection();
        }
		this->processSending();
		this->processReceiving();
        this->receive();
        this->drawWaitingScreen(window);
    }
    LOGS("Got init pkg.");

    LOGS("Processing...");
	this->verifyZoom();
	while (!this->returnToMenu) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed) {
				if (this->localElement == nullptr) {
                    Events events = this->handleLocalButtonsClick();
                    if (events.empty()) {
                        this->sendClick(window, event.mouseButton.button);
                    }
					else {
                        this->addLocalEvents(events);
                    }
				}
				else {
					Events events = this->localElement->click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y, window.getSize().x, window.getSize().y);
					this->addLocalEvents(events);
				}
			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    this->zoomView(window, event.mouseWheelScroll.delta > 0);
                }
			}
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::PageUp) {
                    this->zoomView(window, true);
                }
                else if (event.key.code == sf::Keyboard::Key::PageDown) {
                    this->zoomView(window, false);
                }
            }
		}
        if (this->error) {
            throw NoServerConnection();
        }
		if (this->localElement != nullptr) {
            Events localElementEvent = this->localElement->update();
            this->addLocalEvents(localElementEvent);
		}
        this->processLocalEvents();
		this->processSending();
		this->processReceiving();
		this->receive();
		Playlist::get().update();
		this->drawEverything(window);
		this->moveView(window);
        window.setMouseCursorVisible(this->cursorVisibility);
	}
	LOGS("Processing was finished");
}






void MainScreen::processSending() {
	if (this->toSend.empty()) {
		return;
	}
	sf::Socket::Status status = this->socket.send(this->toSend.front());
	if (status == sf::Socket::Error or status == sf::Socket::Disconnected) {
		this->error = true;
	}
	else if (status == sf::Socket::Done) {
		this->toSend.pop();
	}
}
void MainScreen::processReceiving() {
	if (std::get<0>(this->received)) {
		return;
	}
	sf::Socket::Status status = this->socket.receive(std::get<sf::Packet>(this->received));
	if (status == sf::Socket::Error or status == sf::Socket::Disconnected) {
		this->error = true;
	}
	else if (status == sf::Socket::Done) {
		std::get<0>(this->received) = true;
	}
}





sf::Packet MainScreen::makeBasePacket() const {
    sf::Packet packet;
    packet << this->roomID.value();
    return packet;
}
void MainScreen::send(sf::Packet &what) {
    this->toSend.push(what);
}




std::string GENERATE_SAVE_FROM_MAP(const std::string& name) {
	Map tmpMap;
	Maps::get().load(name, &tmpMap);

	std::string serialStr;

	boost::iostreams::back_insert_device<std::string> inserter(serialStr);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string>> s(inserter);

	oarchive ar(s);
	ar << tmpMap << std::vector<bool>(tmpMap.getStatePtr()->getPlayersPtr()->total(), true) << (uint32_t)1 << (uint32_t)1;

	s.flush();

	return bfdlib::string_compression::get_compressed(serialStr);
}
std::string READ_SAVE(const std::string& name) {
	std::ifstream f(Root::get().getUserdataRoot() + "/saves/" + name, std::ios::binary);
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
void MainScreen::sendNeedSave() {
    sf::Packet packet = this->makeBasePacket();
    packet << CLIENT_NET_SPECS::CODES::NEED_SAVE;
    this->send(packet);
}







void MainScreen::receive() {
	if (!std::get<bool>(this->received)) {
		return;
	}
	std::get<bool>(this->received) = false;
	sf::Packet receivedPacket = std::get<sf::Packet>(this->received);
    std::string id;
    receivedPacket >> id;
    if (this->roomID.value() == id) {
        uint8_t code;
        receivedPacket >> code;

		if (code == SERVER_NET_SPECS::CODES::ERROR) {
			this->receiveError(receivedPacket);
		}
        else if (code == SERVER_NET_SPECS::CODES::WORLD_UI_STATE) {
            uint8_t worldUiStateCode;
            receivedPacket >> worldUiStateCode;
            if (worldUiStateCode == SERVER_NET_SPECS::CODES::WORLD_UI_STATE_CODES::MAP) {
                this->receiveMap(receivedPacket);
            }
            else if (worldUiStateCode == SERVER_NET_SPECS::CODES::WORLD_UI_STATE_CODES::ELEMENT) {
                this->receiveElement(receivedPacket);
            }
            else if (worldUiStateCode == SERVER_NET_SPECS::CODES::WORLD_UI_STATE_CODES::HIGHLIGHT_TABLE) {
                this->receiveHighlightTable(receivedPacket);
            }
            else if (worldUiStateCode == SERVER_NET_SPECS::CODES::WORLD_UI_STATE_CODES::SELECTED) {
                this->receiveSelected(receivedPacket);
            }
            else if (worldUiStateCode == SERVER_NET_SPECS::CODES::WORLD_UI_STATE_CODES::BUTTON_BASES) {
                this->receiveButtonBases(receivedPacket);
            }
            else if (worldUiStateCode == SERVER_NET_SPECS::CODES::WORLD_UI_STATE_CODES::RESOURCE_BAR) {
                this->receiveResourceBar(receivedPacket);
            }
            else if (worldUiStateCode == SERVER_NET_SPECS::CODES::WORLD_UI_STATE_CODES::CURSOR_VISIBILITY) {
                this->receiveCursorVisibility(receivedPacket);
            }
            else {
                LOGS("Warning: unknown world ui state code received from server: " + std::to_string((uint32_t)worldUiStateCode));
            }
        }
        else if (code == SERVER_NET_SPECS::CODES::READY) {
            this->receiveReady();
        }
        else if (code == SERVER_NET_SPECS::CODES::SOUND) {
            this->receiveSound(receivedPacket);
        }
        else if (code == SERVER_NET_SPECS::CODES::FOCUS) {
            this->receiveFocus(receivedPacket);
        }
        else if (code == SERVER_NET_SPECS::CODES::RETURN_TO_MENU) {
            this->receiveReturnToMenu();
        }
        else if (code == SERVER_NET_SPECS::CODES::SAVE) {
            this->receiveSave(receivedPacket);
        }
        else if (code == SERVER_NET_SPECS::CODES::NOT_TIME_TO_SAVE) {
            this->receiveNotTimeToSave();
        }
		else if (code == SERVER_NET_SPECS::CODES::NOT_YOUR_MOVE) {
			this->receiveNotYourMove();
		}
        else {
            LOGS("Warning: unknown code received from server: " + std::to_string((uint32_t) code));
        }
    }
}
void MainScreen::receiveError(sf::Packet& remPacket) {
    LOGS("Receiving error code");
	uint8_t errorCode;
	remPacket >> errorCode;
	throw ServerInitError(errorCode);
}
void MainScreen::receiveMap(sf::Packet& remPacket) {
    LOGS("Receiving map");
    std::string string;
    remPacket >> string;
    string = bfdlib::string_compression::get_decompressed(string);
    std::stringstream stream(string);
    iarchive a1(stream);
    a1 >> this->map;
}
void MainScreen::receiveElement(sf::Packet& remPacket) {
    LOGS("Receiving element");
    std::string string;
    remPacket >> string;
    string = bfdlib::string_compression::get_decompressed(string);
    std::stringstream stream(string);
    iarchive a1(stream);
    a1 >> this->element;
}
void MainScreen::receiveSelected(sf::Packet& remPacket) {
    LOGS("Receiving selected");
    std::string string;
    remPacket >> string;
    string = bfdlib::string_compression::get_decompressed(string);
    std::stringstream stream(string);
    iarchive a1(stream);
    a1 >> this->selected;
}
void MainScreen::receiveHighlightTable(sf::Packet& remPacket) {
    LOGS("Receiving highlight table");
    std::string string;
    remPacket >> string;
    string = bfdlib::string_compression::get_decompressed(string);
    std::stringstream stream(string);
    iarchive a1(stream);
    a1 >> this->highlightTable;
}
void MainScreen::receiveButtonBases(sf::Packet& remPacket) {
    LOGS("Receiving button bases");
    std::string string;
    remPacket >> string;
    string = bfdlib::string_compression::get_decompressed(string);
    std::stringstream stream(string);
    iarchive a1(stream);
    a1 >> this->buttonBases;
}
void MainScreen::receiveResourceBar(sf::Packet& remPacket) {
    LOGS("Receiving resource bar");
    std::string string;
    remPacket >> string;
    string = bfdlib::string_compression::get_decompressed(string);
    std::stringstream stream(string);
    iarchive a1(stream);
    a1 >> this->resourceBar;
}
void MainScreen::receiveCursorVisibility(sf::Packet &remPacket) {
    LOGS("Receiving cursor visibility");
    bool v;
    remPacket >> v;
    this->cursorVisibility = v;
}
void MainScreen::receiveReady() {
    LOGS("Receiving ready");
    this->readyPackageGotten = true;
}
void MainScreen::receiveSound(sf::Packet& remPacket) {
    LOGS("Receiving sound");
	std::string soundName;
	remPacket >> soundName;
	SoundQueue::get().push(Sounds::get().get(soundName));
}
void MainScreen::receiveFocus(sf::Packet& remPacket) {
    LOGS("Receiving focus");
	uint32_t x, y, sx, sy;
	remPacket >> x >> y >> sx >> sy;
	uint32_t centerX = 64 * x + 64 / 2 * sx;
	uint32_t centerY = 64 * y + 64 / 2 * sy;
	if (centerX >= view.getCenter().x - this->view.getSize().x / 2 and
		centerY >= view.getCenter().y - this->view.getSize().y / 2 and
		centerX < view.getCenter().x + this->view.getSize().x / 2 and
		centerY < view.getCenter().y + this->view.getSize().y / 2) {

	}
	else {
		this->view.setCenter(64 * x + 64 / 2 * sx, 64 * y + 64 / 2 * sy);
		this->verifyView();
	}
}
void MainScreen::receiveReturnToMenu() {
    LOGS("Receiving return tu menu");
	this->returnToMenu = true;
}
void MainScreen::receiveSave(sf::Packet& remPacket) {
    LOGS("Receiving save");
	std::string data;
	remPacket >> data;
	if (!std::filesystem::is_directory(Root::get().getUserdataRoot() + "/saves")) {
		std::filesystem::create_directories(Root::get().getUserdataRoot() + "/saves");
	}
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::stringstream ss;
	ss << std::put_time(&tm, "%m-%d-%H-%M");
	std::ofstream ofs(Root::get().getUserdataRoot() + "/saves/" + ss.str() + ".bfdsave", std::ios::binary);
	ofs.write(data.c_str(), data.size());
	ofs.close();

    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));
    Events createWindowEvent;
    createWindowEvent.add(std::make_shared<CreateEEvent>(std::make_shared<WindowButton>(StringLcl("{game_saved}"), StringLcl("{OK}"), clickEvent)));
    this->addLocalEvents(createWindowEvent);
}
void MainScreen::receiveNotTimeToSave() {
    LOGS("Receiving not time to save");

    Events clickEvent;
    clickEvent.add(std::make_shared<PlaySoundEvent>("click"));
    Events createWindowEvent;
    createWindowEvent.add(std::make_shared<CreateEEvent>(std::make_shared<WindowButton>(StringLcl("{not_time_to_save}"), StringLcl("{OK}"), clickEvent)));
    this->addLocalEvents(createWindowEvent);
}
void MainScreen::receiveNotYourMove() {
    LOGS("Receiving not your move");

	Events clickEvent;
	clickEvent.add(std::make_shared<PlaySoundEvent>("click"));
    Events createWindowEvent;
    createWindowEvent.add(std::make_shared<PlaySoundEvent>("wind"));
	createWindowEvent.add(std::make_shared<CreateEEvent>(std::make_shared<WindowButton>(StringLcl("{you_are_in_spot_mode}"), StringLcl("{OK}"), clickEvent)));
    this->addLocalEvents(createWindowEvent);
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
    if (this->element != nullptr) {
		if (this->element->isCameraDependent()) {
			window.setView(this->view);
		}
		else {
			window.setView(window.getDefaultView());
		}
        window.draw(*this->element);
    }
	if (this->localElement != nullptr) {
		if (this->localElement->isCameraDependent()) {
			window.setView(this->view);
		}
		else {
			window.setView(window.getDefaultView());
		}
		window.draw(*this->localElement);
	}
	window.setView(window.getDefaultView());
	this->drawResourceBar(window);
	for (const auto& b : this->buttonBases) {
		window.draw(*b);
	}
    for (const auto& b : this->localButtons) {
        window.draw(b);
    }
	window.display();
}
void MainScreen::drawMap(sf::RenderWindow& window) {
	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		std::shared_ptr<GO> go = this->map.getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DRAW_PRIORITY);
		if (go->exist() and go->inView(this->view)) {
			window.draw(*go);
		}
	}
}
void MainScreen::drawResourceBar(sf::RenderWindow& window) {
	window.draw(resourceBar);
}
void MainScreen::drawCells(sf::RenderWindow& window) {
    uint32_t sx = Textures::get().get("plain")->getSize().x / 64;
    uint32_t sy = Textures::get().get("plain")->getSize().y / 64;
	for (uint32_t i = 0; i < this->map.getStatePtr()->getMapSizePtr()->getWidth(); i = i + sx) {
		for (uint32_t j = 0; j < this->map.getStatePtr()->getMapSizePtr()->getHeight(); j = j + sy) {
			sf::Sprite s;
			s.setTexture(*Textures::get().get("plain"));
			s.setPosition(64 * i, 64 * j);
			s.setTextureRect(sf::IntRect(0, 0, 64 * std::min(sx, this->map.getStatePtr()->getMapSizePtr()->getWidth() - i), 64 * std::min(sy, this->map.getStatePtr()->getMapSizePtr()->getHeight() - j)));
			window.draw(s);
		}
	}
}
void MainScreen::drawHighlightion(sf::RenderWindow& window) {
	std::vector<std::shared_ptr<sf::Drawable>> d = this->highlightTable.getDrawable();
	for (const auto& e : d) {
		window.draw(*e);
	}
}
void MainScreen::drawDarkness(sf::RenderWindow &window) {
	this->illiminanceTable.newFrame(this->view);

	for (uint32_t i = 0; i < this->map.getStatePtr()->getCollectionsPtr()->totalGOs(); i = i + 1) {
		std::shared_ptr<const GO> go = this->map.getStatePtr()->getCollectionsPtr()->getGO(i, FILTER::DEFAULT_PRIORITY);
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









void MainScreen::zoomView(sf::RenderWindow& window, bool plus) {
	if (plus) {
		this->view.zoom(0.95f);
	}
	else {
		this->view.zoom(1.05f);
	}
	this->verifyZoom();
	this->verifyView();
}
void MainScreen::verifyZoom() {
	uint32_t mapW = 64 * this->map.getStatePtr()->getMapSizePtr()->getWidth();
	uint32_t mapH = 64 * this->map.getStatePtr()->getMapSizePtr()->getHeight();
	if (this->view.getSize().x < 1280) { // Order is important here
		float k = (float)1280 / (float)this->view.getSize().x;
		this->view.zoom(k);
	}
	if (this->view.getSize().x > mapW or this->view.getSize().y > mapH) {
		float k = std::min((float)mapW / (float)this->view.getSize().x, (float)mapH / (float)this->view.getSize().y);
		this->view.zoom(k);
	}
}






std::tuple<uint32_t, uint32_t> MainScreen::getMousePositionBasedOnView(sf::RenderWindow &window) const {
	auto pos = sf::Mouse::getPosition();
	auto worldPos = window.mapPixelToCoords(pos, this->view);
	return std::make_tuple(worldPos.x, worldPos.y);
}
void MainScreen::moveView(sf::RenderWindow &window) {
	auto p = sf::Mouse::getPosition();
	if (p.x < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::A) or sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->moveViewToWest();
	}
	else if (p.x > window.getSize().x - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::D) or sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->moveViewToEast();
	}
	if (p.y < 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::W) or sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		this->moveViewToNorth();
	}
	else if (p.y > window.getSize().y - 10 or sf::Keyboard::isKeyPressed(sf::Keyboard::S) or sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		this->moveViewToSouth();
	}
}








static float VIEW_MOVING_DELTA = 10;
void MainScreen::moveViewToNorth() {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(0, VIEW_MOVING_DELTA));
	this->verifyView();
}
void MainScreen::moveViewToSouth() {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(0, VIEW_MOVING_DELTA));
	this->verifyView();
}
void MainScreen::moveViewToWest() {
	this->view.setCenter(this->view.getCenter() - sf::Vector2f(VIEW_MOVING_DELTA, 0));
	this->verifyView();
}
void MainScreen::moveViewToEast() {
	this->view.setCenter(this->view.getCenter() + sf::Vector2f(VIEW_MOVING_DELTA, 0));
	this->verifyView();
}






void MainScreen::verifyView() {
	this->verifyViewNorth(); // Order is important here
	this->verifyViewSouth();
	this->verifyViewEast();
	this->verifyViewWest();
}
void MainScreen::verifyViewNorth() {
	uint32_t border = this->view.getSize().y / 2;
	if (this->view.getCenter().y < border) {
		this->view.setCenter(sf::Vector2f(this->view.getCenter().x, border));
	}
}
void MainScreen::verifyViewSouth() {
	uint32_t border = 64 * this->map.getStatePtr()->getMapSizePtr()->getHeight() - this->view.getSize().y / 2;
	if (this->view.getCenter().y > border) {
		this->view.setCenter(sf::Vector2f(this->view.getCenter().x, border));
	}
}
void MainScreen::verifyViewWest() {
	uint32_t border = this->view.getSize().x / 2;
	if (this->view.getCenter().x < border) {
		this->view.setCenter(sf::Vector2f(border, this->view.getCenter().y));
	}
}
void MainScreen::verifyViewEast() {
	uint32_t border = 64 * this->map.getStatePtr()->getMapSizePtr()->getWidth() - this->view.getSize().x / 2;
	if (this->view.getCenter().x > border) {
		this->view.setCenter(sf::Vector2f(border, this->view.getCenter().y));
	}
}




Events MainScreen::handleLocalButtonsClick() {
    Events events;
    for (auto &b : this->localButtons) {
        events = b.click(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
        if (!events.empty()) {
            break;
        }
    }
    return events;
}





void MainScreen::addLocalEvents(Events& events) {
	for (uint32_t i = 0; i < events.size(); i = i + 1) {
        if (events.at(i)->isUrgent()) {
            this->handleEvent(events.at(i));
        }
        else {
            this->localEventQueue.push(events.at(i));
        }
	}
}
void MainScreen::processLocalEvents() {
    while (!this->localEventQueue.empty()) {
        if (this->localElement != nullptr) {
            return;
        }
        this->handleEvent(this->localEventQueue.front());
        this->localEventQueue.pop();
    }
}
void MainScreen::handleEvent(std::shared_ptr<Event> e) {
	if (std::shared_ptr<PlaySoundEvent> playSoundEvent = std::dynamic_pointer_cast<PlaySoundEvent>(e)) {
		this->handlePlaySoundEvent(playSoundEvent);
	}
    else if (std::shared_ptr<ReturnToMenuEvent> returnToMenuEvent = std::dynamic_pointer_cast<ReturnToMenuEvent>(e)) {
        this->handleReturnToMenuEvent(returnToMenuEvent);
    }
    else if (std::shared_ptr<SaveGameEvent> saveGameEvent = std::dynamic_pointer_cast<SaveGameEvent>(e)) {
        this->handleSaveGameEvent(saveGameEvent);
    }
    else if (std::shared_ptr<CreateEEvent> createEEvent = std::dynamic_pointer_cast<CreateEEvent>(e)) {
        this->handleCreateEEvent(createEEvent);
    }
    else if (std::shared_ptr<ClosePopUpElementEvent> closePopUpElementEvent = std::dynamic_pointer_cast<ClosePopUpElementEvent>(e)) {
        this->handleClosePopUpElementEvent(closePopUpElementEvent);
    }
	else {
		LOGS("Warning: unknown event");
	}
}





void MainScreen::handlePlaySoundEvent(std::shared_ptr<PlaySoundEvent> e) {
	SoundQueue::get().push(Sounds::get().get(e->getSoundName()));
}
void MainScreen::handleReturnToMenuEvent(std::shared_ptr<ReturnToMenuEvent> e) {
    this->returnToMenu = true;
}
void MainScreen::handleSaveGameEvent(std::shared_ptr<SaveGameEvent> e) {
    this->sendNeedSave();
}
void MainScreen::handleCreateEEvent(std::shared_ptr<CreateEEvent> e) {
    this->localElement = e->getElement();
    this->localElement->restart();
}
void MainScreen::handleClosePopUpElementEvent(std::shared_ptr<ClosePopUpElementEvent> e) {
    this->localElement = nullptr;
}