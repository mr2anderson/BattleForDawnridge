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


#include "ServerScreen.hpp"
#include "IsServerTable.hpp"
#include "Textures.hpp"
#include "Playlist.hpp"
#include "Fonts.hpp"
#include "Locales.hpp"
#include "SoundQueue.hpp"
#include "Sounds.hpp"
#include "Label.hpp"
#include "UTFEncoder.hpp"
#include "ScreenAlreadyFinished.hpp"


ServerScreen::ServerScreen(sf::RenderWindow& window) {
	this->alreadyFinished = false;

	this->ip = sf::IpAddress::getPublicAddress();

	this->background.setTexture(*Textures::get()->get("bg"));
	this->background.setPosition(window.getSize().x - this->background.getLocalBounds().width, window.getSize().y - this->background.getLocalBounds().height);

	this->linesNumber = 0;
	this->linesLimit = (window.getSize().y - 20) / 15;
	this->addToLog(*Locales::get()->get("server_mode_welcome"), window);
	if (this->ip == sf::IpAddress::Any) {
		this->addToLog(*Locales::get()->get("couldnt_get_public_ip"), window);
	}
	else {
		this->addToLog(*Locales::get()->get("public_ip") + UTFEncoder::get()->utf8ToUtf16(this->ip.toString()), window);
	}
}
ServerScreenResponse ServerScreen::run(sf::RenderWindow& window) {
	if (this->alreadyFinished) {
		throw ScreenAlreadyFinished();
	}
	this->alreadyFinished = true;
	window.setMouseCursorVisible(false);

	sf::Event event;
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					Playlist::get()->stop();
					return ServerScreenResponse(ServerScreenResponse::TYPE::EXIT);
				}
				if (event.key.code == sf::Keyboard::Delete) {
					IsServerTable::get()->invert();
					if (IsServerTable::get()->isServer()) {
						this->addToLog(*Locales::get()->get("switched_to_server_mode"), window);
					}
					else {
						this->addToLog(*Locales::get()->get("switched_to_client_mode"), window);
					}
				}
			}
		}
		Playlist::get()->update();
		this->drawEverything(window);
	}
}
void ServerScreen::addToLog(const std::wstring &content, sf::RenderWindow &window) {
	this->linesNumber = this->linesNumber + std::count(content.begin(), content.end(), L'\n') + 1;
	if (this->linesNumber > this->linesLimit) {
		uint32_t toDelete = this->linesNumber - this->linesLimit;
		this->linesNumber = this->linesLimit;
		std::wstring newLog;
		bool writeToNewLog = false;
		for (uint32_t i = 0; i < this->log.size(); i = i + 1) {
			if (writeToNewLog) {
				newLog = newLog + this->log[i];
			}
			else {
				if (this->log[i] == L'\n') {
					toDelete = toDelete - 1;
					if (toDelete == 0) {
						writeToNewLog = true;
					}
				}
			}
		}
		this->log = newLog;
	}
	this->log = this->log + content + L"\n";
}
void ServerScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.draw(this->background);
	window.draw(Label(10, 10, window.getSize().x - 20, window.getSize().y - 20, this->log));
	window.display();
}