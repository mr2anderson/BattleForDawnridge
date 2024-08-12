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
#include "SoundQueue.hpp"
#include "Label.hpp"
#include "ScreenAlreadyFinished.hpp"
#include "PublicIP.hpp"


ServerScreen::ServerScreen(sf::RenderWindow& window) {
	this->alreadyFinished = false;

	this->logs.setEntryLimit(10);
	this->logs.add(StringLcl("{server_mode_welcome}"));
	if (PublicIP::get()->getIp().toString() == "0.0.0.0") {
		this->logs.add(StringLcl("{couldnt_get_public_ip}"));
	}
	else {
		this->logs.add(StringLcl("{public_ip}") + StringLcl(PublicIP::get()->getIp().toString()));
	}
	this->logs.add(StringLcl("{entry_limit_set}" + std::to_string(this->logs.getEntryLimit())));
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
						this->logs.add(StringLcl("{switched_to_server_mode}"));
					}
					else {
						this->logs.add(StringLcl("{switched_to_client_mode}"));
					}
				}
			}
		}
		Playlist::get()->update();
		this->drawEverything(window);
	}
}
void ServerScreen::drawEverything(sf::RenderWindow& window) {
	window.clear();
	window.draw(this->bg);
	window.draw(Label(10, 10, window.getSize().x - 20, window.getSize().y - 20, this->logs.get()));
	window.display();
}