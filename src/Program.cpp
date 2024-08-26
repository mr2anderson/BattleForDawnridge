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
#include "NoServerConnection.hpp"
#include "CouldntOpenMap.hpp"
#include "MainServerPosition.hpp"
#include "InvalidRoomIDFormat.hpp"
#include "ServerInitError.hpp"


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



    if (IsServerTable::get().isServer()) {
        ServerScreen serverScreen(this->window);
        serverScreen.run(this->window);
    }
    else {
        boost::optional<StringLcl> error;
        
        for (; ;) {
            Menu menu(this->window, error);
            MenuResponse menuResponse = menu.run(this->window);
            if (menuResponse.getType() == MenuResponse::TYPE::EXIT) {
                return;
            }

            error = boost::none;

            if (menuResponse.getType() == MenuResponse::TYPE::START_LOCAL_GAME or menuResponse.getType() == MenuResponse::TYPE::LOAD_LOCAL_GAME) {
                this->localGame(menuResponse, error);
            }
            else if (menuResponse.getType() == MenuResponse::TYPE::START_NETWORK_GAME or menuResponse.getType() == MenuResponse::TYPE::LOAD_NETWORK_GAME or menuResponse.getType() == MenuResponse::TYPE::CONNECT_TO_NETWORK_GAME) {
                this->networkGame(menuResponse, error);
            }
        }
    }
}
void Program::localGame(const MenuResponse &response, boost::optional<StringLcl> &error) {
    LocalServer localServer;
    uint16_t port = localServer.launch();

    MainScreen::Type mainScreenType;
    if (response.getType() == MenuResponse::TYPE::START_LOCAL_GAME) {
        mainScreenType = MainScreen::Type::CreateFromMap;
    }
    else {
        mainScreenType = MainScreen::Type::CreateFromSave;
    }

    try {
        try {
            MainScreen mainScreen(this->window, sf::IpAddress::getLocalAddress(), port, mainScreenType, response.getData(), MainScreen::EVERYONE, RoomID());
            mainScreen.run(this->window);
        }
        catch (NoServerConnection&) {
            try {
                localServer.fine();
                std::rethrow_exception(std::current_exception());
            }
            catch (std::exception&) {
                std::rethrow_exception(std::current_exception());
            }
        }
    }
    catch (std::exception&) {
        this->handleException(std::current_exception(), error);
    }

    localServer.finish();
}
void Program::networkGame(const MenuResponse& response, boost::optional<StringLcl>& error) {
    try {
        if (response.getType() == MenuResponse::TYPE::START_NETWORK_GAME) {
            MainScreen mainScreen(this->window, MainServerPosition::get().getIP(), MainServerPosition::get().getPort(), MainScreen::Type::CreateFromMap, response.getData(), 1, RoomID());
            mainScreen.run(this->window);
        }
        else if (response.getType() == MenuResponse::TYPE::LOAD_NETWORK_GAME) {
            MainScreen mainScreen(this->window, MainServerPosition::get().getIP(), MainServerPosition::get().getPort(), MainScreen::Type::CreateFromSave, response.getData(), 1, RoomID());
            mainScreen.run(this->window);
        }
        else {
            MainScreen mainScreen(this->window, MainServerPosition::get().getIP(), MainServerPosition::get().getPort(), MainScreen::Type::Connect, "", 1, RoomID(response.getData()));
            mainScreen.run(this->window);
        }
    }
    catch (std::exception&) {
        this->handleException(std::current_exception(), error);
    }
}
void Program::handleException(std::exception_ptr exception, boost::optional<StringLcl> &error) {
    try {
        std::rethrow_exception(exception);
    }
    catch (CouldntOpenMap&) {
        error = StringLcl("{map_is_apcent_client}");
    }
    catch (boost::archive::archive_exception& e) {
        error = StringLcl("{boost_archive_exception_client} " + std::to_string(e.code) + "\n" + std::string(e.what()));
    }
    catch (NoServerConnection&) {
        error = StringLcl("{disconnect_client}");
    }
    catch (InvalidRoomIDFormat&) {
        error = StringLcl("{invalid_room_id_client}");
    }
    catch (ServerInitError& e) {
        uint8_t code = e.getCode();
        switch (code) {
        case SERVER_NET_SPECS::CODES::ERROR_CODES::FULL_ROOM:
            error = StringLcl("{full_room}");
            break;
        case SERVER_NET_SPECS::CODES::ERROR_CODES::ROOM_ALREADY_EXIST:
            error = StringLcl("{this_room_already_exist}");
            break;
        case SERVER_NET_SPECS::CODES::ERROR_CODES::UNKNOWN_ROOM_ID:
            error = StringLcl("{unknown_room_id}");
            break;
        case SERVER_NET_SPECS::CODES::ERROR_CODES::INVALID_DATA:
            error = StringLcl("{invalid_data}");
            break;
        default:
            error = StringLcl("{unknown_server_error}");
            break;
        }
    }
    catch (std::exception& e) {
        error = StringLcl("{unknown_error_client}\n" + std::string(e.what()));
    }
}