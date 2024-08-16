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
#include "ServerIP.hpp"
#include "Root.hpp"
#include "CouldntOpenServerIP.hpp"


ServerIP* ServerIP::singletone = nullptr;


static std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}
void ServerIP::load() {
	std::string path(DATA_ROOT + "/configs/server_ip.cfg");

	std::ifstream file(path);

	std::string buff;
	std::getline(file, buff);

	std::vector<std::string> splitted = split(buff, '.');
	if (splitted.size() != 4) {
		throw CouldntOpenServerIP(path);
	}
	for (const auto& word : splitted) {
		int32_t number;
		try {
			number = std::stoi(word);
		}
		catch (std::exception&) {
			throw CouldntOpenServerIP(path);
		}
		if (number < 0 or number > 255) {
			throw CouldntOpenServerIP(path);
		}
	}

	this->ip = sf::IpAddress(buff);
}
sf::IpAddress ServerIP::getIP() const {
	return this->ip.value();
}