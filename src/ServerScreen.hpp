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


#include "Label.hpp"


class ServerScreen {
public:
	static ServerScreen* get() {
		if (ServerScreen::singletone == nullptr) {
			ServerScreen::singletone = new ServerScreen();
		}
		return ServerScreen::singletone;
	}

	bool run(sf::RenderWindow& window);
private:
	ServerScreen() = default;
	ServerScreen(const ServerScreen& copy) = delete;
	static ServerScreen* singletone;

	sf::Sprite background;
	Label label;
	std::wstring log;

	void init(sf::RenderWindow& window);
	void prepareToExit();

	void addToLog(const std::wstring& content, sf::RenderWindow &window);

	void drawEverything(sf::RenderWindow& window);
};