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


uint16_t Ports::getClientPort() const {
	return this->clientPort.value();
}
uint16_t Ports::getLocalServerPort() const {
	return this->localServerPort.value();
}
uint16_t Ports::getServerPort() const {
	return this->serverPort.value();
}
void Ports::load() {
	std::string path = DATA_ROOT + "/configs/ports.cfg";

	std::ifstream file(path);

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);

		std::string word;
		ss >> word;
		if (word == "client") {
			ss >> word;
			this->clientPort = std::stoi(word);
		}
		else if (word == "local_server") {
			ss >> word;
			this->localServerPort = std::stoi(word);
		}
		else if (word == "server") {
			ss >> word;
			this->serverPort = std::stoi(word);
		}
	}

	file.close();

	if (!this->clientPort.has_value() or !this->localServerPort.has_value() or !this->serverPort.has_value()) {
		throw CouldntOpenPorts(path);
	}
}