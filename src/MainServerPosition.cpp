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


#include <fstream>
#include <sstream>
#include <limits>
#include "MainServerPosition.hpp"
#include "Root.hpp"
#include "CouldntOpenMainServerPosition.hpp"


static std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}
void MainServerPosition::load() {
	std::string path(Root::get().getDataRoot() + "/configs/main_server_position.cfg");

	std::ifstream file(path);

	std::string buff;
	std::getline(file, buff);
	std::vector<std::string> splitted = split(buff, '.');
	if (splitted.size() != 4) {
		throw CouldntOpenMainServerPosition(path);
	}
	for (const auto& word : splitted) {
		int32_t number;
		try {
			number = std::stoi(word);
		}
		catch (std::exception&) {
			throw CouldntOpenMainServerPosition(path);
		}
		if (number < 0 or number > 255) {
			throw CouldntOpenMainServerPosition(path);
		}
	}
	this->ip = sf::IpAddress(buff);

    std::getline(file, buff);
    int32_t number;
    try {
        number = std::stoi(buff);
    }
    catch (std::exception&) {
        throw CouldntOpenMainServerPosition(path);
    }
    if (number < 0 or number > std::numeric_limits<uint16_t>::max()) {
        throw CouldntOpenMainServerPosition(path);
    }
    this->port = number;
}
sf::IpAddress MainServerPosition::getIP() const {
	return this->ip.value();
}
uint16_t MainServerPosition::getPort() const {
    return this->port.value();
}