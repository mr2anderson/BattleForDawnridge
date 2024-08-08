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


#include <SFML/Network.hpp>
#include "Label.hpp"
#include "ServerScreenResponse.hpp"


class ServerScreen {
public:
	ServerScreen(sf::RenderWindow& window);
	ServerScreen(const ServerScreen& copy) = delete;
	ServerScreenResponse run(sf::RenderWindow& window);
private:
	bool alreadyFinished;
	sf::IpAddress ip;
	sf::Sprite background;
	std::wstring log;
	uint32_t linesNumber;
	uint32_t linesLimit;

	void addToLog(const std::wstring& content, sf::RenderWindow &window);
	void drawEverything(sf::RenderWindow& window);
};