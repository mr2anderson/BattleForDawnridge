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


#include <iostream>
#include <sstream>
#include <fstream>
#include "CouldntOpenPorts.hpp"
#include "Ports.hpp"
#include "Root.hpp"


Ports* Ports::singletone = nullptr;


uint16_t Ports::getClientSendPort() const {
	return this->clientSendPort.value();
}
uint16_t Ports::getClientReceivePort() const {
	return this->clientReceivePort.value();
}

uint16_t Ports::getLocalServerSendPort() const {
	return this->localServerSendPort.value();
}
uint16_t Ports::getLocalServerReceivePort() const {
	return this->localServerReceivePort.value();
}

uint16_t Ports::getServerSendPort() const {
	return this->serverSendPort.value();
}
uint16_t Ports::getServerReceivePort() const {
	return this->serverReceivePort.value();
}


void Ports::load() {
	std::string path = DATA_ROOT + "/configs/ports.cfg";

	std::ifstream file(path);

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);

		std::string word;
		ss >> word;
		if (word == "client_send_port") {
			ss >> word;
			this->clientSendPort = std::stoi(word);
		}
		else if (word == "client_receive_port") {
			ss >> word;
			this->clientReceivePort = std::stoi(word);
		}
		else if (word == "local_server_send_port") {
			ss >> word;
			this->localServerSendPort = std::stoi(word);
		}
		else if (word == "local_server_receive_port") {
			ss >> word;
			this->localServerReceivePort = std::stoi(word);
		}
		else if (word == "server_send_port") {
			ss >> word;
			this->serverSendPort = std::stoi(word);
		}
		else if (word == "server_receive_port") {
			ss >> word;
			this->serverReceivePort = std::stoi(word);
		}
	}

	file.close();

	if (!this->clientSendPort.has_value() or !this->clientReceivePort.has_value() or !this->localServerSendPort.has_value() or !this->localServerReceivePort.has_value() or !this->serverSendPort.has_value() or !this->serverReceivePort.has_value()) {
		throw CouldntOpenPorts(path);
	}
}