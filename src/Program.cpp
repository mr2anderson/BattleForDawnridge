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


#include "Program.hpp"
#include "IsServerTable.hpp"
#include "LoadingScreen.hpp"
#include "PAKScreen.hpp"
#include "Menu.hpp"
#include "MainScreen.hpp"
#include "ServerScreen.hpp"
#include "LocalServer.hpp"
#include "Ports.hpp"
#include "PortIsBusy.hpp"
#include "NoServerConnection.hpp"


Program* Program::singletone = nullptr;


Program::Program() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    this->window.create(sf::VideoMode::getDesktopMode(), "Battle for Dawnridge", sf::Style::Fullscreen, settings);
    this->window.setVerticalSyncEnabled(true);
    this->window.setFramerateLimit(60);
}
void Program::run() {
    LoadingScreen loadingScreen(this->window);
    LoadingScreenResponse loadingScreenResponse = loadingScreen.run(this->window);
    switch (loadingScreenResponse.getType()) {
    case LoadingScreenResponse::TYPE::OK: {
        break;
    }
    case LoadingScreenResponse::TYPE::LOADING_ERROR: {
        return;
    }
    }


    PAKScreen pakScreen(this->window);
    PAKScreenResponse pakScreenResponse = pakScreen.run(this->window);
    switch (pakScreenResponse.getType()) {
    case PAKScreenResponse::TYPE::CONTINUE: {
        break;
    }
    case PAKScreenResponse::TYPE::EXIT: {
        return;
    }
    }



    if (IsServerTable::get()->isServer()) {
        ServerScreen serverScreen(this->window);
        ServerScreenResponse serverScreenResponse = serverScreen.run(this->window);
        switch (serverScreenResponse.getType()) {
        case ServerScreenResponse::TYPE::EXIT: {
            return;
        }
        }
    }
    else {
        boost::optional<StringLcl> error;
        for (; ;) {
            Menu menu(this->window, error);
            MenuResponse menuResponse = menu.run(this->window);
            if (menuResponse.getType() == MenuResponse::TYPE::EXIT) {
                return;
            }
            // TODO: Network game


            error = boost::none;
            try {
                std::shared_ptr<Room> room = std::make_shared<Room>(menuResponse.getData());
                LocalServer LocalServer;
                LocalServer.launch(room);
                MainScreen mainScreen(this->window, sf::IpAddress::getLocalAddress(), Ports::get()->getLocalServerSendPort(), Ports::get()->getLocalServerReceivePort(), room->getID());
                MainScreenResponse mainScreenResponse = mainScreen.run(this->window);
                switch (mainScreenResponse.getType()) {
                case MainScreenResponse::TYPE::RETURN_TO_MENU: {
                    break;
                }
                }
            }
            catch (PortIsBusy& e) {
                error = StringLcl("{port_is_busy}" + std::to_string(e.getPort()));
            }
            catch (NoServerConnection&) {
                error = StringLcl("{disconnect_error}");
            }
        }
    }
}