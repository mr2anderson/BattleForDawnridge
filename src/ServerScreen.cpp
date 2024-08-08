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


ServerScreen* ServerScreen::singletone = nullptr;


bool ServerScreen::run(sf::RenderWindow& window) {
	this->init(window);
	sf::Event event;
	for (; ;) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					return false;
				}
				if (event.key.code == sf::Keyboard::Delete) {
					this->prepareToExit();
					return true;
				}
			}
		}
		Playlist::get()->update();
		this->drawEverything(window);
	}
}
void ServerScreen::init(sf::RenderWindow& window) {
	window.setMouseCursorVisible(false);

	this->ip = sf::IpAddress::getPublicAddress();

	this->background.setTexture(*Textures::get()->get("menu"));
	this->background.setPosition(window.getSize().x - this->background.getLocalBounds().width, window.getSize().y - this->background.getLocalBounds().height);

	this->addToLog(*Locales::get()->get("server_mode_welcome"), window);
	if (this->ip == sf::IpAddress::Any) {
		this->addToLog(*Locales::get()->get("couldnt_get_public_ip"), window);
	}
	else {
		this->addToLog(*Locales::get()->get("public_ip") + UTFEncoder::get()->utf8ToUtf16(this->ip.toString()), window);
	}
}
void ServerScreen::prepareToExit() {
	Playlist::get()->restartMusic();
}
void ServerScreen::addToLog(const std::wstring &content, sf::RenderWindow &window) {
	this->log = this->log + content + L"\n";
	this->label = Label(10, 10, window.getSize().x - 20, window.getSize().y - 20, this->log);
}
void ServerScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.draw(this->background);
	window.draw(this->label);
	window.display();
}