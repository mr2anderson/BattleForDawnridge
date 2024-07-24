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


#include "Game.hpp"
#include "LoadingScreen.hpp"
#include "PAKScreen.hpp"
#include "Menu.hpp"
#include "MainScreen.hpp"


Game* Game::singletone = nullptr;


void Game::run() {
	this->initWindow();
	if (!LoadingScreen::get()->run(this->window)) {
        return;
    }
	if (!PAKScreen::get()->run(this->window)) {
		return;
	}
	for (; ;) {
        std::shared_ptr<Map> map = Menu::get()->run(this->window);
		if (map == nullptr) {
            return;
        }
		if (!MainScreen::get()->run(map, this->window)) {
			return;
		}
	}
}
void Game::initWindow() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	this->window.create(sf::VideoMode::getDesktopMode(), "Battle for Dawnridge", sf::Style::Fullscreen, settings);
	this->window.setVerticalSyncEnabled(true);
	this->window.setFramerateLimit(60);
}